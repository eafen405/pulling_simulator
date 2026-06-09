#ifndef POOL_H
#define POOL_H

//定义稀有度
typedef enum{
    _3STARS = 3,
    _4STARS = 4,
    _5STARS = 5

}Rarity;

typedef enum {
    WEAPON_SWORD,       // 单手剑
    WEAPON_BIGSWORD,    // 双手剑
    WEAPON_BOW,         // 弓
    WEAPON_CATALYST,    // 法器
    WEAPON_POLEARM      // 长柄武器
} Weapon_type;

//定义角色
typedef struct{
    int id;
    char name[32];
    Rarity rarity;
    int is_up;    //1=UP角色，0=常驻
}Character;

//定义武器
typedef struct{
    int id;
    char name[32];
    Weapon_type Weapon_type;
    Rarity rarity;
    int is_up;    //1=UP武器，0=常驻
}Weapon;

typedef struct{
    double base_rate;    //基础概率
    int baodi;          //保底抽数
    int riseup_point;   //开始概率提升的抽数
}Rate;

//对外接口

const Rate* get_rate(Rarity rarity);
int get_count(Rarity rarity);          //某星级的角色数
const Character* get_character(Rarity rarity, int index);
const Weapon* get_Weapon(Rarity rarity, int index);

#endif