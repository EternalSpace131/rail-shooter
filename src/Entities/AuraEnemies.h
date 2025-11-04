//
// Created by Morgan on 25-10-23.
//

#ifndef AURAENEMIES_H
#define AURAENEMIES_H
#include"Enemy.h"
#include <QPainter>

class DefenseAuraEnemy: public Enemy {
public:
    DefenseAuraEnemy(qreal x,qreal y,qreal w,qreal h,GameMap* map,int dt,qreal speed,int hp,
                     qreal radius,int defenseAdd,EnemyKind kind,const QStringList& animFrames);

    void applyAura(const std::vector<Enemy *> & all) override;
    void draw(QPainter* painter) override; // 重写draw方法绘制光环
    void tick() override; // 呼吸动画

private:
    qreal m_radius;
    int m_defAdd;

    qreal m_haloScale = 1.0;    // 光环缩放比例
    bool m_haloGrow = true;     // 缩放方向（true=放大，false=缩小）
};

class SpeedAuraEnemy : public Enemy {
public:
    SpeedAuraEnemy(qreal x,qreal y,qreal w,qreal h,GameMap* map,int dt,qreal speed,int hp,
                   qreal radius,qreal speedMult,EnemyKind kind,const QStringList& animFrames);

    void applyAura(const std::vector<Enemy *> & all) override;
    void draw(QPainter* painter) override; // 重写draw方法绘制光环
    void tick() override; // 呼吸动画

private:
    qreal m_radius;
    qreal m_Mult;

    qreal m_haloScale = 1.0;    // 光环缩放比例
    bool m_haloGrow = true;     // 缩放方向（true=放大，false=缩小）
};

#endif //AURAENEMIES_H
