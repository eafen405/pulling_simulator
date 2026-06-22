#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

#define STATE_FILE "state.txt"

void state_load(struct GachaState* state) {
    /* 先初始化为 0，旧格式缺少的字段也能保持默认值 */
    state->pity_5star = 0;
    state->pity_4star = 0;
    state->guarantee_5star = 0;
    state->guarantee_4star = 0;
    state->total_pulls = 0;

    FILE* fh = fopen(STATE_FILE, "r");
    if (fh == NULL) {
        return;
    }

    /* 解析 key=value 格式 */
    char line[64];
    while (fgets(line, sizeof(line), fh)) {
        /* 去掉末尾换行符 */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (sscanf(line, "pity_5=%d", &state->pity_5star) == 1) continue;
        if (sscanf(line, "pity_4=%d", &state->pity_4star) == 1) continue;
        if (sscanf(line, "guarantee_5=%d", &state->guarantee_5star) == 1) continue;
        if (sscanf(line, "guarantee_4=%d", &state->guarantee_4star) == 1) continue;
        if (sscanf(line, "guarantee=%d", &state->guarantee_5star) == 1) continue; /* 旧格式兼容 */
        if (sscanf(line, "total=%d", &state->total_pulls) == 1) continue;
    }

    fclose(fh);
}

void state_update(struct GachaState* state, int rarity, int is_featured_5, int is_featured_4) {
    if (rarity == 5) {
        state->pity_5star = 0;
        state->pity_4star = 0;  /* 5★ 同时重置 4★ 保底（5★ ≥ 4★） */
        /* 大保底状态：出了 UP 角色则下一发为小保底，否则下一发为大保底 */
        state->guarantee_5star = !is_featured_5;
    } else if (rarity == 4) {
        state->pity_4star = 0;
        state->pity_5star++;
        state->guarantee_4star = !is_featured_4;
    } else {
        /* rarity == 3 */
        state->pity_4star++;
        state->pity_5star++;
    }
    state->total_pulls++;
}

void state_save(const struct GachaState* state) {
    FILE* fh = fopen(STATE_FILE, "w");
    if (fh == NULL) {
        printf("警告：无法保存状态到 %s\n", STATE_FILE);
        return;
    }
    fprintf(fh, "pity_5=%d\n", state->pity_5star);
    fprintf(fh, "pity_4=%d\n", state->pity_4star);
    fprintf(fh, "guarantee_5=%d\n", state->guarantee_5star);
    fprintf(fh, "guarantee_4=%d\n", state->guarantee_4star);
    fprintf(fh, "total=%d\n", state->total_pulls);
    fclose(fh);
}
