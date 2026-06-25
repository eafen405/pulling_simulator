#include <stdlib.h>
#include "algorithm.h"

/* 根据保底状态计算当期 5★ 概率 */
static double calc_rate_5(int pity_5star, const struct PoolRates* rates) {
    /* 硬保底：第 90 抽必出 */
    if (pity_5star >= rates->hard_pity_5 - 1)
        return 1.0;

    /* 软保底：第 74 抽开始概率提升 */
    if (pity_5star >= rates->soft_pity_start - 1) {
        int pulls_into_soft = pity_5star - (rates->soft_pity_start - 1) + 1;
        /* 每抽 +6% (即 base_rate × 10 per pull past 73) */
        return rates->rate_5star * (1.0 + 10.0 * pulls_into_soft);
    }

    /* 普通概率 */
    return rates->rate_5star;
}

static int roll_percent(double probability) {
    /* probability 是小数（如 0.006 表示 0.6%） */
    int threshold = (int)(probability * 10000);
    return (rand() % 10000) < threshold;
}

int algorithm_draw(const struct PullState* state, const struct PoolRates* rates) {
    double rate_5 = calc_rate_5(state->pity_5star, rates);
    int at_4star_pity = (state->pity_4star >= rates->hard_pity_4 - 1);
    /* 第 10 抽（pity_4star == 9）触发 4★ 硬保底 */

    if (at_4star_pity) {
        /* 4★ 硬保底触发：先判定 5★ 是否出货 */
        if (roll_percent(rate_5))
            return 5;
        /* 5★ 没中 → 强制出 4★ */
        return 4;
    }

    /* 正常流程 */
    if (roll_percent(rate_5))
        return 5;

    if (roll_percent(rates->rate_4star))
        return 4;

    return 3;
}

int algorithm_pick_character(int rarity, int guarantee_5star, int guarantee_4star) {
    switch (rarity) {
        case 5: {
            /* 5★ 池：index 0 = UP, 1~8 = 常驻 */
            if (guarantee_5star)
                return 0;     /* 大保底 → UP */
            /* 小保底：50% UP / 50% 常驻 */
            if (rand() % 2 == 0)
                return 0;     /* 中了 UP */
            /* 没中 UP，从 8 个常驻中随机 */
            return 1 + (rand() % 8);
        }
        case 4: {
            /* 4★ UP：重云、北斗、迪奥娜 → 对应池内 index 53, 58, 67 */
            static const int up4_idx[] = {53, 58, 67};
            int up4_count = 3;

            if (guarantee_4star)
                return up4_idx[rand() % up4_count];

            if (rand() % 2 == 0)    /* 50% 中 UP */
                return up4_idx[rand() % up4_count];

            /* 未中 UP → 从非 UP 4★ 中随机（68 个 - 3 个 UP） */
            int idx = rand() % (pool_get_count(4) - up4_count);
            for (int i = 0; i < up4_count; i++) {
                if (idx >= up4_idx[i])
                    idx++;
            }
            return idx;
        }
        case 3: {
            /* 3★ 池：13 个武器 */
            int count = pool_get_count(3);
            return rand() % count;
        }
        default:
            return 0;
    }
}
