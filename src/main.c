#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "pool.h"
#include "state.h"
#include "algorithm.h"
#include "record.h"
#include "ui.h"

int main(void) {
    /* 初始化随机数种子 */
    srand((unsigned int)time(NULL));

    /* 切换终端到 UTF-8 编码，解决中文乱码 */
    system("chcp 65001 > nul");

    struct GachaState state;
    const struct PoolRates* rates = pool_get_rates();

    state_load(&state);

    printf("\n✦ 欢迎来到 Gacha 抽卡模拟器！ ✦\n");

    int running = 1;
    while (running) {
        int choice = ui_menu();

        switch (choice) {
            case 1: { /* 单抽 */
                int rarity = algorithm_draw(&state, rates);
                int idx = algorithm_pick_character(rarity, state.guarantee_5star);
                const char* name = pool_get_name(rarity, idx);
                int is_featured = (rarity == 5 && idx == 0);

                ui_show_result(rarity, name);
                state_update(&state, rarity, is_featured);
                state_save(&state);
                record_append(rarity, name);

                printf("按任意键继续...");
                _getch();
                break;
            }

            case 2: { /* 十连 */
                int rarities[10];
                const char* names[10];

                for (int i = 0; i < 10; i++) {
                    int r = algorithm_draw(&state, rates);
                    int idx = algorithm_pick_character(r, state.guarantee_5star);
                    const char* name = pool_get_name(r, idx);
                    int is_featured = (r == 5 && idx == 0);

                    rarities[i] = r;
                    names[i] = name;
                    state_update(&state, r, is_featured);
                }

                state_save(&state);

                for (int i = 0; i < 10; i++) {
                    record_append(rarities[i], names[i]);
                }

                ui_show_ten_result(rarities, names, 10);
                break;
            }

            case 3: /* 查看历史记录 */
                record_show_history(&state);
                printf("\n按任意键继续...");
                _getch();
                break;

            case 4: /* 退出 */
                state_save(&state);
                printf("\n✦ 下次再见！✦\n");
                running = 0;
                break;
        }
    }

    return 0;
}
