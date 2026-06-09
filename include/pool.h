#ifndef POOL_H
#define POOL_H

/* ========== 枚举定义 ========== */

/* 稀有度 */
typedef enum {
    RARITY_3 = 3,
    RARITY_4 = 4,
    RARITY_5 = 5
} Rarity;

/* 武器类型 */
typedef enum {
    WEAPON_SWORD,       /* 单手剑 */
    WEAPON_CLAYMORE,    /* 双手剑 */
    WEAPON_BOW,         /* 弓 */
    WEAPON_CATALYST,    /* 法器 */
    WEAPON_POLEARM      /* 长柄武器 */
} WeaponType;

/* ========== 数据结构 ========== */

/* 角色 */
typedef struct {
    int id;
    char name[32];
    Rarity rarity;
    int is_up;          /* 1=UP角色，0=常驻 */
} Character;

/* 武器 */
typedef struct {
    int id;
    char name[32];
    WeaponType weapon_type;
    Rarity rarity;
    int is_up;          /* 1=UP武器，0=常驻 */
} Weapon;

/* 概率配置 */
typedef struct {
    double base_rate;       /* 基础概率，如 ★5 = 0.006 */
    int hard_pity;          /* 硬保底所需抽数 */
    int soft_pity_start;    /* 软保底起始抽数（概率开始递增） */
} RateConfig;

/* ========== 对外接口 ========== */

/* --- 概率配置 --- */
const RateConfig* pool_get_rate(Rarity rarity);

/* --- 角色查询 --- */
int               pool_get_character_count(Rarity rarity);
const Character*  pool_get_character(Rarity rarity, int index);
const Character*  pool_get_character_by_id(int id);

/* --- 武器查询 --- */
int               pool_get_weapon_count(Rarity rarity);
const Weapon*     pool_get_weapon(Rarity rarity, int index);
const Weapon*     pool_get_weapon_by_id(int id);

/* --- 武器类型名称 --- */
const char*       pool_weapon_type_name(WeaponType type);

#endif