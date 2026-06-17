#pragma once

#include "state.h"

/* 追加一条抽卡记录到 record.txt (格式: timestamp|rarity|name) */
void record_append(int rarity, const char* name);

/* 显示历史记录，顶部显示当前保底状态 */
void record_show_history(const struct GachaState* state);
