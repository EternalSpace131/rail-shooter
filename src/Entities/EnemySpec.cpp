//
// Created by Morgan on 25-10-23.
//
#include"EnemySpec.h"
static const std::vector<EnemySpec> kEnemyBook = {
    {EnemyKind::Mowan,      "魔丸",        90,  3, 1, {28,24},  50, false,false, false,0,1.0,0},
    {EnemyKind::Kobe,      "牢大(冰红茶)", 80,  7, 0, {30,26},  20, true,false,  false,0,1.0,0}, // 精英，死亡可做烟雾特效
    {EnemyKind::Wujing,     "吴京(彩蛋)",  85,  5, 1, {28,24},  10, false,false, false,0,1.0,0}, // 彩蛋，无实际效果
    {EnemyKind::Penguin,    "马达加斯加企鹅", 70, 6, 1, {30,26}, 12, false,false, true,120,1.0, +1}, // 光环：+防御
    {EnemyKind::Xiaoyan, "刘晓燕",      70, 6, 1, {30,26}, 12, false,false, true,120,1.2,  0},  // 光环：+速度
    {EnemyKind::Songhao,    "宋浩(彩蛋)",  80,  6, 1, {30,26},  10, false,false, false,0,1.0,0},
    {EnemyKind::NJUboss,    "nju(BOSS)",   55, 40, 3, {44,40},   2, false,true,  false,0,1.0,0},
};