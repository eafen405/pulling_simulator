# 项目：pulling_simulator（抽卡模拟器）

## 一、模块清单

| 模块 | 文件 | 职责 |
|------|------|------|
| 主控 | main.c | 调度中心，串联所有模块，不处理具体逻辑 |
| 卡池模块 | pool.h / pool.c | 静态数据库，存储角色信息、概率配置，提供只读查询 |
| 算法模块 | algorithm.h / algorithm.c | 纯函数，接收状态输入，先决定稀有度，再决定具体角色 |
| 状态管理 | state.h / state.c | 维护运行时状态，负责文件读写，暴露 get/set 接口 |
| 记录模块 | record.h / record.c | 追加写入抽卡日志（JSON格式） |
| UI模块 | ui.h / ui.c | 命令行交互，显示菜单、结果，不碰文件 |

---

## 二、核心设计决策

- **算法是纯函数**：不存状态，不碰文件，同样输入→同样输出
- **状态封装在模块内部**：用 static 隐藏变量，外部只能通过 get/set 接口访问
- **两步抽卡**：先按概率+保底决定稀有度，再在指定稀有度内随机角色
- **状态文件格式**：键值对文本（`pity=73`），人眼可读
- **记录文件格式**：JSON，与状态文件分开存储

---

## 三、模块详细设计

### 1. 卡池模块（pool.h / pool.c）

#### 功能描述
- 作为静态数据库，硬编码存储所有可抽角色信息（名称、稀有度、ID）
- 存储各稀有度（如 ★3 / ★4 / ★5）的基础概率和保底机制参数
- 提供只读查询：按稀有度筛选角色列表、按 ID 查询角色信息、获取概率配置
- 模块内数据全部为 `const`，编译后不可变

#### 对外接口

```c
// 角色信息结构
typedef struct {
    int id;
    const char* name;
    int rarity;        // 3 / 4 / 5
} Character;

// 概率配置结构
typedef struct {
    double base_rate;       // 基础概率（如 ★5 = 0.006）
    int hard_pity;          // 硬保底（累计此数量必定出）
    int soft_pity_start;    // 软保底起始（从此数量开始概率递增）
} RarityConfig;

// 获取指定稀有度的概率配置
const RarityConfig* pool_get_rarity_config(int rarity);

// 获取指定稀有度的角色列表（返回数组指针 + 数量）
const Character* const* pool_get_characters_by_rarity(int rarity, int* out_count);

// 按 ID 查找角色，找不到返回 NULL
const Character* pool_get_character_by_id(int id);

// 获取卡池中角色总数
int pool_get_total_count(void);

// 获取稀有度种类数（比如 3）
int pool_get_rarity_count(void);
```

#### 依赖关系
- **无依赖**：纯静态数据模块，不依赖任何其他模块

---

### 2. 状态管理模块（state.h / state.c）

#### 功能描述
- 维护玩家抽卡运行时状态：各稀有度的保底计数（pity counter）、总抽数、拥有的角色列表
- 所有状态用 `static` 变量隐藏在 `.c` 文件内，外部只能通过 `get` / `set` 接口读写
- 程序启动时从状态文件（键值对文本）读取状态到内存
- 提供 `state_save()` 将当前内存状态写回文件
- 支持重置状态（新档 / 删档重来）

#### 对外接口

```c
// 初始化：从文件加载状态，文件不存在则使用默认值
void state_init(void);

// 保存当前状态到文件
void state_save(void);

// 重置所有状态为默认值
void state_reset(void);

// --- pity 计数 ---
int  state_get_pity(int rarity);
void state_set_pity(int rarity, int count);
void state_increment_pity(int rarity);

// --- 总抽数 ---
int  state_get_total_pulls(void);
void state_increment_total_pulls(void);

// --- 已拥有角色 ---
bool state_has_character(int char_id);
void state_add_character(int char_id);
int  state_get_owned_count(void);

// 关闭服务专用：告知客户端状态模块即将销毁
void state_shutdown(void);
```

#### 依赖关系
- **无模块依赖**：仅依赖 C 标准库（stdio, stdlib, string）
- 被 `main.c`、`algorithm.h`（通过 get 读取 pity）、`ui.c`、`record.c` 依赖

---

### 3. 算法模块（algorithm.h / algorithm.c）

#### 功能描述
- 核心抽卡逻辑，**纯函数**：不持有任何内部状态、不访问文件、不调用外部副作用函数
- 两步抽卡流程：
  1. **决定稀有度**：根据各稀有度基础概率 + 当前保底计数 + 软保底递增，随机决定本次抽到的稀有度
  2. **决定具体角色**：在选定稀有度的角色池中随机抽取一名角色
- 软保底逻辑：当 pity 计数进入 `soft_pity_start` 区间后，概率逐步线性递增，直到 `hard_pity` 时概率为 100%
- 返回抽卡结果结构体，由调用方决定如何使用（展示、记录、更新状态）

#### 对外接口

```c
// 单次抽卡结果
typedef struct {
    int rarity;         // 最终稀有度
    int char_id;        // 抽到的角色 ID
    bool is_pity;       // 是否由保底触发
} PullResult;

// 执行一次抽卡（纯函数）
//   pity_map: 各稀有度当前的保底计数（如 {5: 73, 4: 8, 3: 0}）
//   调用方负责更新状态，本函数不修改任何外部数据
PullResult algorithm_pull(const int* pity_map, int pity_map_len);

// 便捷接口：根据单个稀有度的 pity 值，计算该稀有度本次的命中概率（含软保底递增）
double algorithm_calc_rate(int rarity, int current_pity);
```

#### 依赖关系
- **依赖 `pool.h`**：读取概率配置（`RarityConfig`）和角色列表
- **不依赖** `state.h`（算法不直接读状态，由调用方传入 pity_map）
- 被 `main.c` 调用

---

### 4. 记录模块（record.h / record.c）

#### 功能描述
- 每次抽卡完成后，将结果以 JSON 格式**追加写入**日志文件
- 日志记录包含：时间戳、稀有度、角色名称、是否保底触发、当前累计抽数
- 文件不存在时自动创建
- 不读取或解析已有日志（纯写入语义，保持简单）

#### 对外接口

```c
// 追加一条抽卡记录
//   内部获取当前时间戳
void record_log_pull(int char_id, int rarity, bool is_pity, int total_pulls);

// 获取日志文件路径（只读，供 UI 展示用）
const char* record_get_filepath(void);
```

#### 依赖关系
- **依赖 `pool.h`**：通过 `char_id` 查询角色名称以写入 JSON
- **依赖 `state.h`**：读取当前状态信息（可选，total_pulls 等已由调用方传入）
- 被 `main.c` 调用（每次抽卡后调用 `record_log_pull`）

---

### 5. UI 模块（ui.h / ui.c）

#### 功能描述
- 提供命令行交互界面：主菜单、抽卡操作、查看记录、查看状态、退出
- **不碰文件**：所有数据通过其他模块的接口获取，UI 只负责显示和收集用户输入
- 显示内容：菜单选项、单次 / 十连抽结果、保底进度条、已拥有角色图鉴
- 用户输入校验在本模块内完成（如非法选项提示重输）

#### 对外接口

```c
// 初始化 UI（如设置控制台编码等）
void ui_init(void);

// 显示主菜单，返回用户选择的操作码
//   返回值：1=单抽, 2=十连, 3=查看图鉴, 4=查看状态, 0=退出
int ui_show_menu(void);

// 显示单次抽卡结果
void ui_show_pull_result(const PullResult* result);

// 显示十连结果列表
void ui_show_multi_pull_results(const PullResult* results, int count);

// 显示当前保底状态
void ui_show_pity_status(const int* pity_map, int map_len, int total_pulls);

// 显示已拥有角色图鉴
void ui_show_collection(const Character* const* owned, int count);

// 显示提示信息（如"状态已保存"、错误提示等）
void ui_show_message(const char* msg);
```

#### 依赖关系
- **依赖 `pool.h`**：显示角色名称、稀有度信息
- **依赖 `state.h`**：读取保底计数、总抽数、已拥有角色
- **不依赖** `algorithm.h`、`record.h`
- 被 `main.c` 调用

---

### 6. 主控模块（main.c）

#### 功能描述
- 程序入口，**调度中心**：按正确顺序初始化各模块，然后进入主循环
- 主循环流程：显示菜单 → 获取用户选择 → 调用算法 → 更新状态 → 显示结果 → 记录日志
- 不包含具体业务逻辑（不计算概率、不格式化输出、不直接操作文件）
- 退出前确保状态保存和资源清理

#### 对外接口
- `int main(void)` — 唯一的对外接口，程序入口

#### 依赖关系
- **依赖所有模块**：`pool.h`、`state.h`、`algorithm.h`、`record.h`、`ui.h`
- 不被任何模块依赖

---

## 四、模块依赖图

```
main.c
  ├── pool.h        (只读查询)
  ├── state.h       (get/set/save)
  ├── algorithm.h   (抽卡计算)
  │     └── pool.h  (概率配置 + 角色池)
  ├── record.h      (写日志)
  │     └── pool.h  (角色信息)
  └── ui.h          (交互显示)
        ├── pool.h  (角色展示)
        └── state.h (状态展示)
```

## 五、数据流（一次抽卡的完整流程）

```
用户输入 → ui_show_menu()
         → main 调用 algorithm_pull(pity_map)
                → algorithm 读取 pool_get_rarity_config() 获得概率
                → algorithm 计算稀有度 → 在 pool 中随机角色
                → 返回 PullResult
         → main 调用 state_increment_pity() / state_add_character()
         → main 调用 ui_show_pull_result()
         → main 调用 record_log_pull()
         → 循环回 ui_show_menu()
```

## 六、文件结构

```
pulling_simulator/
├── src/
│   ├── main.c
│   ├── pool.c
│   ├── state.c
│   ├── algorithm.c
│   ├── record.c
│   └── ui.c
├── include/
│   ├── pool.h
│   ├── state.h
│   ├── algorithm.h
│   ├── record.h
│   └── ui.h
├── data/                  ← 运行时生成
│   ├── state.txt          ← 键值对格式
│   └── record.json        ← JSON 日志
├── docs/
│   └── plan.md
├── pulling_simulator.sln
├── pulling_simulator.vcxproj
└── README.md
```
