/* ========== 概率配置表 ========== */
static const RateConfig rate_table[] = {
    [3] = { .base_rate = 0.940, .hard_pity = 10, .soft_pity_start = 10 },  /* ★3：每次10抽保底，实际大部分都是三星 */
    [4] = { .base_rate = 0.051, .hard_pity = 10, .soft_pity_start = 10 },  /* ★4：每10抽保底 */
    [5] = { .base_rate = 0.006, .hard_pity = 90, .soft_pity_start = 74 },  /* ★5：90抽硬保底，74抽开始软保底 */
};

/* ========== 武器名称类型名 ========== */
static const char* weapon_type_names[] = {
    [WEAPON_SWORD]    = "单手剑",
    [WEAPON_CLAYMORE] = "双手剑",
    [WEAPON_BOW]      = "弓",
    [WEAPON_CATALYST] = "法器",
    [WEAPON_POLEARM]  = "长柄武器",
};

/* ========== ★4 武器 ========== */
static const Weapon weapons_4star[] = {
    /* 单手剑 */
    { .id = 401, .name = "西风剑",     .weapon_type = WEAPON_SWORD,    .rarity = RARITY_4, .is_up = 0 },
    { .id = 402, .name = "祭礼剑",     .weapon_type = WEAPON_SWORD,    .rarity = RARITY_4, .is_up = 0 },
    { .id = 403, .name = "匣里龙吟",   .weapon_type = WEAPON_SWORD,    .rarity = RARITY_4, .is_up = 0 },
    { .id = 404, .name = "笛剑",       .weapon_type = WEAPON_SWORD,    .rarity = RARITY_4, .is_up = 0 },
    /* 双手剑 */
    { .id = 405, .name = "祭礼大剑",   .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_4, .is_up = 0 },
    { .id = 406, .name = "西风大剑",   .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_4, .is_up = 0 },
    { .id = 407, .name = "钟剑",       .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_4, .is_up = 0 },
    { .id = 408, .name = "雨裁",       .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_4, .is_up = 0 },
    /* 弓 */
    { .id = 409, .name = "祭礼弓",     .weapon_type = WEAPON_BOW,      .rarity = RARITY_4, .is_up = 0 },
    { .id = 410, .name = "西风猎弓",   .weapon_type = WEAPON_BOW,      .rarity = RARITY_4, .is_up = 0 },
    { .id = 411, .name = "弓藏",       .weapon_type = WEAPON_BOW,      .rarity = RARITY_4, .is_up = 0 },
    { .id = 412, .name = "绝弦",       .weapon_type = WEAPON_BOW,      .rarity = RARITY_4, .is_up = 0 },
    /* 法器 */
    { .id = 413, .name = "祭礼残章",   .weapon_type = WEAPON_CATALYST, .rarity = RARITY_4, .is_up = 0 },
    { .id = 414, .name = "西风秘典",   .weapon_type = WEAPON_CATALYST, .rarity = RARITY_4, .is_up = 0 },
    { .id = 415, .name = "昭心",       .weapon_type = WEAPON_CATALYST, .rarity = RARITY_4, .is_up = 0 },
    { .id = 416, .name = "流浪乐章",   .weapon_type = WEAPON_CATALYST, .rarity = RARITY_4, .is_up = 0 },
    /* 长柄武器 */
    { .id = 417, .name = "西风长枪",   .weapon_type = WEAPON_POLEARM,  .rarity = RARITY_4, .is_up = 0 },
    { .id = 418, .name = "匣里灭辰",   .weapon_type = WEAPON_POLEARM,  .rarity = RARITY_4, .is_up = 0 },
};

/* ========== ★3 武器 ========== */
static const Weapon weapons_3star[] = {
    /* 单手剑 */
    { .id = 301, .name = "飞天御剑",     .weapon_type = WEAPON_SWORD,    .rarity = RARITY_3, .is_up = 0 },
    { .id = 302, .name = "黎明神剑",     .weapon_type = WEAPON_SWORD,    .rarity = RARITY_3, .is_up = 0 },
    { .id = 303, .name = "旅行剑",       .weapon_type = WEAPON_SWORD,    .rarity = RARITY_3, .is_up = 0 },
    /* 双手剑 */
    { .id = 304, .name = "佣兵重剑",     .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_3, .is_up = 0 },
    { .id = 305, .name = "沐浴龙血的剑", .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_3, .is_up = 0 },
    { .id = 306, .name = "白铁大剑",     .weapon_type = WEAPON_CLAYMORE, .rarity = RARITY_3, .is_up = 0 },
    /* 弓 */
    { .id = 307, .name = "弹弓",         .weapon_type = WEAPON_BOW,      .rarity = RARITY_3, .is_up = 0 },
    { .id = 308, .name = "神射手之誓",   .weapon_type = WEAPON_BOW,      .rarity = RARITY_3, .is_up = 0 },
    { .id = 309, .name = "鸦羽弓",       .weapon_type = WEAPON_BOW,      .rarity = RARITY_3, .is_up = 0 },
    /* 法器 */
    { .id = 310, .name = "甲级宝珏",     .weapon_type = WEAPON_CATALYST, .rarity = RARITY_3, .is_up = 0 },
    { .id = 311, .name = "讨龙英杰谭",   .weapon_type = WEAPON_CATALYST, .rarity = RARITY_3, .is_up = 0 },
    { .id = 312, .name = "翡玉法球",     .weapon_type = WEAPON_CATALYST, .rarity = RARITY_3, .is_up = 0 },
    /* 长柄武器 */
    { .id = 313, .name = "黑缨枪",       .weapon_type = WEAPON_POLEARM,  .rarity = RARITY_3, .is_up = 0 },
};
四星角色：
布伦妮
叶洛亚
雅珂达
爱诺
塔利雅
伊法
伊安珊
蓝砚
欧洛伦
卡齐娜
赛索斯
嘉明
夏沃蕾
夏洛蒂
菲米尼
琳妮特
绮良良
卡维
米卡
瑶瑶
珐露珊
莱依拉
坎蒂丝
多莉
柯莱
鹿野院平藏
久岐忍
云堇
五郎
托马
九条裟罗
早柚
烟绯
罗莎莉亚
辛焱
迪奥娜
砂糖
雷泽
菲谢尔
丽莎
北斗
香菱
安柏
班尼特
行秋
芭芭拉
凝光
诺艾尔
凯亚
重云
