//
// Created by Morgan on 25-10-23.
//
//用来储存敌人参数
#ifndef ENEMYSPEC_H
#define ENEMYSPEC_H
#include<QString>
#include<QSizeF>
enum class EnemyKind{ Mowan, Kobe, Wujing, Penguin, Xiaoyan, Songhao, NJUboss};

struct EnemySpec {
    EnemyKind kind;
    QString name;
    qreal speed; //基础速度 px/s
    int hp;
    int defence;  //基础防御（抵消伤害）
    QSizeF body; //宽高
    int weight;
    //可选效果
    bool elite=false,boss=false;
    //光环（给周围敌人加成）
    bool aura=false;
    qreal auraRadius=0; //像素
    qreal auraSpeedMult=1; //速度乘子
    int auraDefenceAdd=0;  //防御加值
};

#endif //ENEMYSPEC_H
