# 抽卡模拟器

C 语言课程设计项目 — 命令行抽卡模拟器。

## 项目结构

```
├── include/         .h 头文件
│   ├── pool.h         卡池数据接口
│   ├── state.h        状态管理接口
│   ├── algorithm.h    抽卡算法接口
│   ├── record.h       抽卡记录接口
│   └── ui.h          命令行界面接口
├── src/             .c 源文件
│   ├── main.c         主循环调度器
│   ├── pool.c         卡池数据（角色/武器/概率）
│   ├── state.c        状态读写（state.txt）
│   ├── algorithm.c    抽卡核心逻辑（软保底/硬保底）
│   ├── record.c       记录读写（record.txt）
│   └── ui.c           菜单与结果展示
└── docs/             设计文档
```

## 功能

- **单抽 / 十连** — 支持 3★/4★/5★ 稀有度判定
- **保底机制** — 5★ 软保底（74 抽起概率提升）+ 硬保底（90 抽必出）
- **4★ 硬保底** — 每 10 抽必出 4★ 或以上
- **大/小保底** — 5★ 角色 50/50 机制，大保底必出当期 UP
- **4★ UP** — 重云、北斗、迪奥娜，大小保底机制
- **数据持久化** — 保底状态存 `state.txt`，抽卡记录存 `record.txt`
- **历史记录** — 查看抽卡历史与当前保底状态

## 编译运行

### VS Code

按 `F5` 或 `Ctrl+Shift+B` 编译运行。

### 命令行

```bash
gcc -Wall -Wextra -I include -o pulls src/main.c src/pool.c src/state.c src/algorithm.c src/record.c src/ui.c
./pulls
```

## 卡池数据

- **5★ UP**: 桑多涅
- **5★ 常驻**: 刻晴、七七、迪卢克、琴、莫娜、提纳里、迪希雅、梦见月瑞希
- **4★ UP**: 重云、北斗、迪奥娜
- **4★**: 18 件武器 + 50 名角色
- **3★**: 13 件武器

详细数据见 `docs/pool_ref.md`。
