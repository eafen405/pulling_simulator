#pragma once

#include "pool.h"
#include "state.h"

/* 根据保底状态 + 卡池概率 → 返回本次抽取的稀有度 (3/4/5) */
int algorithm_draw(const struct GachaState* state, const struct PoolRates* rates);

/* 根据稀有度和保底状态 → 返回物品下标 */
int algorithm_pick_character(int rarity, int guarantee_5star);
