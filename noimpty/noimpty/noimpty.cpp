#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局状态
int gold = 1600;           // 金币
int pity = 0;              // 保底计数 (0~79)
int guaranteed = 0;        // 大保底标记：1 = 下一五星必为UP

// 角色名字
const char* four_stars[] = { "a","s","d","f","g","h","j","k","l","z" };
const char* five_star_standard = "r";  // 常驻五星
const char* five_star_up = "o";        // UP五星

/**
 * 执行一次抽卡，将抽到的角色名字写入 out_name（需保证缓冲区足够大）
 * 返回值: 0-四星, 1-五星常驻(r), 2-五星UP(o)
 * 内部会扣除金币、更新保底/大保底状态
 * 若金币不足，返回 -1，不扣金币，不改变状态
 */
int single_draw(char* out_name) {
    if (gold < 160) {
        return -1;
    }

    gold -= 160;
    pity++;

    int is_five_star = 0;
    // 检查保底
    if (pity >= 80) {
        is_five_star = 1;          // 第80抽强制五星
    }
    else {
        if ((rand() % 100) < 1) {  // 1%基础概率
            is_five_star = 1;
        }
    }

    if (is_five_star) {
        int is_up = 0;
        if (guaranteed) {
            is_up = 1;             // 大保底必定UP
        }
        else {
            is_up = (rand() % 2 == 0); // 50%概率UP
        }

        // 确定角色名字
        if (is_up) {
            sprintf(out_name, "%s", five_star_up);
            guaranteed = 0;        // 抽到UP，清除大保底
            pity = 0;              // 出五星重置保底
            return 2;
        }
        else {
            sprintf(out_name, "%s", five_star_standard);
            guaranteed = 1;        // 歪了，启动大保底
            pity = 0;
            return 1;
        }
    }
    else {
        // 未获得五星，随机选择一个四星角色
        int idx = rand() % 10;
        sprintf(out_name, "%s", four_stars[idx]);
        return 0;
    }
}

// 十连抽（必须拥有1600金币，否则不执行）
void ten_draw() {
    printf("\n===== 十连抽 =====\n");
    if (gold < 1600) {
        printf("金币不足！十连抽需要1600金币，当前仅有%d。\n", gold);
        return;
    }

    char name[16];
    for (int i = 1; i <= 10; i++) {
        printf("第%d抽: ", i);
        int res = single_draw(name);
        if (res == -1) {
            printf("金币不足，抽卡中断\n");
            break;
        }
        printf("%s", name);
        if (res == 1 || res == 2) {
            printf(" ★★★★★");
        }
        printf("\n");
    }
    printf("当前金币: %d, 保底计数: %d\n", gold, pity);
}

// 数字炸弹游戏，赚取金币
void play_number_bomb() {
    int target = rand() % 101;      // 0 ~ 100 的随机数
    int attempts = 1;               // 当前尝试次数（首次猜为第1次）
    int reward = 160;               // 当前奖励金币

    printf("\n===== 数字炸弹 =====\n");
    printf("系统已经生成了一个 0~100 之间的整数，猜中即可获得金币！\n");
    printf("首次猜中得 160 金币，每多猜一次奖励减少 10 金币，最低 0 金币。\n");

    while (1) {
        if (reward <= 0) {
            printf("奖励已减为 0，游戏结束。获得 0 金币。\n");
            break;
        }

        int guess;
        printf("当前奖励: %d 金币，请输入你猜的数字 (0~100): ", reward);
        scanf("%d", &guess);

        if (guess < 0 || guess > 100) {
            printf("输入无效，数字必须在 0~100 之间，请重试。\n");
            continue;
        }

        if (guess == target) {
            int final_reward = 160 - (attempts - 1) * 10;
            if (final_reward < 0) final_reward = 0;
            gold += final_reward;
            printf("恭喜你猜中了！目标数字就是 %d。获得 %d 金币。\n", target, final_reward);
            break;
        }
        else {
            if (guess > target) {
                printf("大了！\n");
            }
            else {
                printf("小了！\n");
            }
            attempts++;
            reward = 160 - (attempts - 1) * 10;
        }
    }
    printf("游戏结束，当前金币: %d\n", gold);
}

// 充值系统：1元 = 10金币
void recharge() {
    int yuan;
    printf("\n===== 充值系统 =====\n");
    printf("当前金币: %d，兑换比例：1元 = 10金币\n", gold);
    printf("请输入充值金额（元）：");
    scanf("%d", &yuan);

    if (yuan <= 0) {
        printf("金额必须大于0，充值失败。\n");
        return;
    }

    int add_gold = yuan * 10;
    gold += add_gold;
    printf("充值成功！获得 %d 金币，当前总金币: %d\n", add_gold, gold);
}

int main() {
    srand((unsigned int)time(NULL));

    int choice;
    printf("欢迎来到抽卡模拟！初始金币1600。\n");
    printf("四星角色: a s d f g h j k l z\n");
    printf("五星常驻: r   UP角色: o\n");

    do {
        printf("\n-------------------------------\n");
        printf("金币: %d   保底计数: %d/80\n", gold, pity);
        if (guaranteed) printf("【大保底生效中：下一五星必定为UP】\n");
        printf("1. 单抽 (160金币)\n");
        printf("2. 十连抽 (1600金币)\n");
        printf("3. 数字炸弹 (赚金币)\n");
        printf("4. 充值 (1元=10金币)\n");
        printf("0. 退出\n");
        printf("请选择: ");
        scanf("%d", &choice);

        char name[16];
        switch (choice) {
        case 1: {
            printf("\n--- 单抽 ---\n");
            int res = single_draw(name);
            if (res == -1) {
                printf("金币不足！\n");
            }
            else {
                printf("抽到角色: %s", name);
                if (res == 1 || res == 2) {
                    printf(" ★★★★★");
                }
                printf("\n");
            }
            printf("剩余金币: %d, 保底计数: %d\n", gold, pity);
            break;
        }
        case 2:
            ten_draw();
            break;
        case 3:
            play_number_bomb();
            break;
        case 4:
            recharge();
            break;
        case 0:
            printf("再见！\n");
            break;
        default:
            printf("无效选项，请重新输入。\n");
        }
    } while (choice != 0);

    return 0;
}