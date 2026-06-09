#include "pool.h"

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

/* ========== ★5 武器 ========== */
static const Weapon weapons_5star[] = {
    /* TODO: 待补充 */
};

/* ========== ★5 角色 ========== */
static const Character characters_5star[] = {
    /* TODO: 待补充 */
};

/* ========== ★4 角色 ========== */
static const Character characters_4star[] = {
    /* TODO: 待补充 */
};

/* ========== ★3 角色 ========== */
static const Character characters_3star[] = {
    /* TODO: 待补充 */
};

/* ========== 接口实现 ========== */

const RateConfig* pool_get_rate(Rarity rarity) {
    if (rarity < 3 || rarity > 5) return NULL;
    return &rate_table[(int)rarity];
}

/* --- 角色接口 --- */
int pool_get_character_count(Rarity rarity) {
    switch (rarity) {
        case RARITY_5: return sizeof(characters_5star) / sizeof(Character);
        case RARITY_4: return sizeof(characters_4star) / sizeof(Character);
        case RARITY_3: return sizeof(characters_3star) / sizeof(Character);
        default:       return 0;
    }
}

const Character* pool_get_character(Rarity rarity, int index) {
    const Character* arr = NULL;
    int count = 0;
    switch (rarity) {
        case RARITY_5: arr = characters_5star; count = sizeof(characters_5star) / sizeof(Character); break;
        case RARITY_4: arr = characters_4star; count = sizeof(characters_4star) / sizeof(Character); break;
        case RARITY_3: arr = characters_3star; count = sizeof(characters_3star) / sizeof(Character); break;
    }
    if (!arr || index < 0 || index >= count) return NULL;
    return &arr[index];
}

const Character* pool_get_character_by_id(int id) {
    static const struct { const Character* arr; int count; } tables[] = {
        { characters_5star, sizeof(characters_5star) / sizeof(Character) },
        { characters_4star, sizeof(characters_4star) / sizeof(Character) },
        { characters_3star, sizeof(characters_3star) / sizeof(Character) },
    };
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < tables[t].count; i++) {
            if (tables[t].arr[i].id == id) return &tables[t].arr[i];
        }
    }
    return NULL;
}

/* --- 武器接口 --- */
int pool_get_weapon_count(Rarity rarity) {
    switch (rarity) {
        case RARITY_5: return sizeof(weapons_5star) / sizeof(Weapon);
        case RARITY_4: return sizeof(weapons_4star) / sizeof(Weapon);
        case RARITY_3: return sizeof(weapons_3star) / sizeof(Weapon);
        default:       return 0;
    }
}

const Weapon* pool_get_weapon(Rarity rarity, int index) {
    const Weapon* arr = NULL;
    int count = 0;
    switch (rarity) {
        case RARITY_5: arr = weapons_5star; count = sizeof(weapons_5star) / sizeof(Weapon); break;
        case RARITY_4: arr = weapons_4star; count = sizeof(weapons_4star) / sizeof(Weapon); break;
        case RARITY_3: arr = weapons_3star; count = sizeof(weapons_3star) / sizeof(Weapon); break;
    }
    if (!arr || index < 0 || index >= count) return NULL;
    return &arr[index];
}

const Weapon* pool_get_weapon_by_id(int id) {
    static const struct { const Weapon* arr; int count; } tables[] = {
        { weapons_5star, sizeof(weapons_5star) / sizeof(Weapon) },
        { weapons_4star, sizeof(weapons_4star) / sizeof(Weapon) },
        { weapons_3star, sizeof(weapons_3star) / sizeof(Weapon) },
    };
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < tables[t].count; i++) {
            if (tables[t].arr[i].id == id) return &tables[t].arr[i];
        }
    }
    return NULL;
}

const char* pool_weapon_type_name(WeaponType type) {
    if (type < 0 || type > WEAPON_POLEARM) return "未知";
    return weapon_type_names[type];
}
