#pragma once

struct PullState {
    int pity_5star;         /* 距上次 5★ 已抽次数 */
    int pity_4star;         /* 距上次 4★ 已抽次数 */
    int guarantee_5star;    /* 1 = 大保底（下次必出 UP）, 0 = 小保底 */
    int guarantee_4star;    /* 1 = 4★大保底（下次必出 4★ UP）, 0 = 小保底 */
    int total_pulls;        /* 累计抽数 */
};

/* 从 state.txt 加载状态，文件不存在则初始化为 0 */
void state_load(struct PullState* state);

/* 根据抽卡结果更新保底计数 */
void state_update(struct PullState* state, int rarity, int is_featured_5, int is_featured_4);

/* 保存状态到 state.txt */
void state_save(const struct PullState* state);
