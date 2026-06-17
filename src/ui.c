#include <stdio.h>
#include <stdlib.h>  /* system() */
#include <conio.h>   /* _getch() */
#include "ui.h"

/* ANSI 颜色 */
#define COLOR_GOLD  "\033[1;33m"
#define COLOR_PURPLE "\033[1;35m"
#define COLOR_BLUE  "\033[1;34m"
#define COLOR_RESET "\033[0m"

int ui_menu(void) {
    system("cls");
    int choice = 0;
    do {
        printf("\n");
        printf("========================\n");
        printf("   ✦ Gacha 抽卡模拟器 ✦\n");
        printf("========================\n");
        printf("  1. 单抽一次\n");
        printf("  2. 十连\n");
        printf("  3. 查看历史记录\n");
        printf("  4. 退出\n");
        printf("========================\n");
        printf("  请输入选项：");

        int result = scanf("%d", &choice);
        /* 清空输入缓冲区 */
        while (getchar() != '\n');

        if (result != 1 || choice < 1 || choice > 4) {
            printf("  输入无效，请输入 1-4 之间的数字。\n");
            choice = 0;
        }
    } while (choice == 0);

    return choice;
}

void ui_show_result(int rarity, const char* name) {
    switch (rarity) {
        case 5:
            printf("\n" COLOR_GOLD "  ★★★★★ %s ★★★★★" COLOR_RESET "\n", name);
            printf(COLOR_GOLD "    ✨ 恭喜出货！✨" COLOR_RESET "\n");
            break;
        case 4:
            printf("\n" COLOR_PURPLE "  ★★★★ %s" COLOR_RESET "\n", name);
            break;
        default:
            printf("\n  ★★★ %s\n", name);
            break;
    }
}

void ui_show_ten_result(int rarities[], const char* names[], int count) {
    int i;

    /* 先统计各类数量 */
    int count_5 = 0, count_4 = 0, count_3 = 0;
    for (i = 0; i < count; i++) {
        if (rarities[i] == 5) count_5++;
        else if (rarities[i] == 4) count_4++;
        else count_3++;
    }

    printf("\n========== 十连结果 ==========\n\n");

    /* 5★ 优先展示 */
    if (count_5 > 0) {
        for (i = 0; i < count; i++) {
            if (rarities[i] == 5) {
                printf(COLOR_GOLD "  ★★★★★ %s" COLOR_RESET "\n", names[i]);
            }
        }
        printf("\n");
    }

    /* 4★ */
    if (count_4 > 0) {
        for (i = 0; i < count; i++) {
            if (rarities[i] == 4) {
                printf(COLOR_PURPLE "  ★★★★ %s" COLOR_RESET "\n", names[i]);
            }
        }
        printf("\n");
    }

    /* 3★ */
    if (count_3 > 0) {
        for (i = 0; i < count; i++) {
            if (rarities[i] == 3) {
                printf("  ★★★ %s\n", names[i]);
            }
        }
    }

    printf("\n==============================\n");
    printf("按任意键继续...");
    _getch();
    printf("\n");
}
