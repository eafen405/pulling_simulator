#pragma once

/* 显示主菜单，返回用户选择 (1/2/3/4) */
int ui_menu(void);

/* 显示单次抽卡结果 */
void ui_show_result(int rarity, const char* name);

/* 显示十连结果（按 5★ → 4★ → 3★ 排序展示） */
void ui_show_ten_result(int rarities[], const char* names[], int count);
