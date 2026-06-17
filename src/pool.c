#include "pool.h"

/* ========== 概率配置 ========== */
static const struct PoolRates rates = {
    .rate_5star = 0.006,
    .rate_4star = 0.051,
    .soft_pity_start = 74,
    .hard_pity_5 = 90,
    .hard_pity_4 = 10
};

const struct PoolRates* pool_get_rates(void) {
    return &rates;
}

/* ========== ★3 武器 ========== */
#define COUNT_3STAR 13
static const char* items_3star[COUNT_3STAR] = {
    "飞天御剑", "黎明神剑", "旅行剑",       /* 单手剑 */
    "佣兵重剑", "沐浴龙血的剑", "白铁大剑", /* 双手剑 */
    "弹弓", "神射手之誓", "鸦羽弓",         /* 弓 */
    "甲级宝珏", "讨龙英杰谭", "翡玉法球",   /* 法器 */
    "黑缨枪"                                 /* 长柄武器 */
};

/* ========== ★4 ========== */
#define COUNT_4STAR_WEAPONS 18
static const char* weapons_4star[COUNT_4STAR_WEAPONS] = {
    /* 单手剑 */
    "西风剑", "祭礼剑", "匣里龙吟", "笛剑",
    /* 双手剑 */
    "祭礼大剑", "西风大剑", "钟剑", "雨裁",
    /* 弓 */
    "祭礼弓", "西风猎弓", "弓藏", "绝弦",
    /* 法器 */
    "祭礼残章", "西风秘典", "昭心", "流浪乐章",
    /* 长柄武器 */
    "西风长枪", "匣里灭辰"
};

#define COUNT_4STAR_CHARS 50
static const char* chars_4star[COUNT_4STAR_CHARS] = {
    "布伦妮", "叶洛亚", "雅珂达", "爱诺", "塔利雅",
    "伊法", "伊安珊", "蓝砚", "欧洛伦", "卡齐娜",
    "赛索斯", "嘉明", "夏沃蕾", "夏洛蒂", "菲米尼",
    "琳妮特", "绮良良", "卡维", "米卡", "瑶瑶",
    "珐露珊", "莱依拉", "坎蒂丝", "多莉", "柯莱",
    "鹿野院平藏", "久岐忍", "云堇", "五郎", "托马",
    "九条裟罗", "早柚", "烟绯", "罗莎莉亚", "辛焱",
    "迪奥娜", "砂糖", "雷泽", "菲谢尔", "丽莎",
    "北斗", "香菱", "安柏", "班尼特", "行秋",
    "芭芭拉", "凝光", "诺艾尔", "凯亚", "重云"
};

#define COUNT_4STAR (COUNT_4STAR_WEAPONS + COUNT_4STAR_CHARS)

/* ========== ★5 ========== */
#define COUNT_5STAR 9
static const char* items_5star[COUNT_5STAR] = {
    /* index 0 = UP 角色, indices 1-8 = 常驻 */
    "桑多涅",
    "刻晴", "七七", "迪卢克", "琴", "莫娜",
    "提纳里", "迪希雅", "梦见月瑞希"
};

/* ========== 公开接口 ========== */

const char* pool_get_name(int rarity, int index) {
    switch (rarity) {
        case 3:
            if (index >= 0 && index < COUNT_3STAR)
                return items_3star[index];
            break;
        case 4:
            if (index >= 0 && index < COUNT_4STAR)
                /* 前 18 个是武器，后面是角色 */
                return (index < COUNT_4STAR_WEAPONS)
                    ? weapons_4star[index]
                    : chars_4star[index - COUNT_4STAR_WEAPONS];
            break;
        case 5:
            if (index >= 0 && index < COUNT_5STAR)
                return items_5star[index];
            break;
    }
    return "???";
}

int pool_get_count(int rarity) {
    switch (rarity) {
        case 3: return COUNT_3STAR;
        case 4: return COUNT_4STAR;
        case 5: return COUNT_5STAR;
        default: return 0;
    }
}
