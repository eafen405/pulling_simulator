#pragma once

#define MAX_NAME_LEN 32

/* 卡池概率配置 */
struct PoolRates {
    double rate_5star;       /* 基础 5★ 概率 (0.006) */
    double rate_4star;       /* 4★ 概率 (0.051) */
    int soft_pity_start;     /* 软保底起始抽数 (74) */
    int hard_pity_5;         /* 5★ 硬保底 (90) */
    int hard_pity_4;         /* 4★ 硬保底 (10) */
};

/* 获取概率配置 */
const struct PoolRates* pool_get_rates(void);

/* 根据稀有度和下标获取名称 */
const char* pool_get_name(int rarity, int index);

/* 获取某个稀有度的物品总数 */
int pool_get_count(int rarity);
