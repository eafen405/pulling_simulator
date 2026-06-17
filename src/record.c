#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "record.h"

#define RECORD_FILE "record.txt"

void record_append(int rarity, const char* name) {
    FILE* fh = fopen(RECORD_FILE, "a");
    if (fh == NULL) {
        printf("警告：无法写入抽卡记录到 %s\n", RECORD_FILE);
        return;
    }
    fprintf(fh, "%ld|%d|%s\n", (long)time(NULL), rarity, name);
    fclose(fh);
}

void record_show_history(const struct GachaState* state) {
    printf("\n===== 当前保底状态 =====\n");
    printf("  距上次 5★：%d 抽\n", state->pity_5star);
    printf("  距上次 4★：%d 抽\n", state->pity_4star);
    printf("  状态：%s\n", state->guarantee_5star ? "大保底" : "小保底");
    printf("  累计抽数：%d\n\n", state->total_pulls);

    FILE* fh = fopen(RECORD_FILE, "r");
    if (fh == NULL) {
        printf("暂无抽卡记录。\n");
        return;
    }

    printf("===== 历史抽卡记录 =====\n");
    char line[128];
    int count = 0;
    while (fgets(line, sizeof(line), fh)) {
        long timestamp;
        int r;
        char name[64];

        if (sscanf(line, "%ld|%d|%63[^\n]", &timestamp, &r, name) == 3) {
            count++;
            printf("  [%s] ★%d %s\n", name, r, (r == 5) ? "★" : "");
        }
    }
    fclose(fh);

    if (count == 0)
        printf("暂无抽卡记录。\n");
    else
        printf("\n共 %d 条记录\n", count);
}
