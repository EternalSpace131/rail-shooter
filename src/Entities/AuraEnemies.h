//
// Created by Morgan on 25-10-23.
//

#ifndef AURAENEMIES_H
#define AURAENEMIES_H
#include"Enemy.h"
class DefenseAuraEnemy: public Enemy {
public:
    DefenseAuraEnemy(qreal x,qreal y,qreal w,qreal h,GameMap* map,int dt,qreal speed,int hp,
                     qreal radius,int defenseAdd);

    void applyAura(const std::vector<Enemy *> & all) override;

private:
    qreal m_radius;
    int m_defAdd;
};

class SpeedAuraEnemy : public Enemy {
public:
    SpeedAuraEnemy(qreal x,qreal y,qreal w,qreal h,GameMap* map,int dt,qreal speed,int hp,
                   qreal radius,qreal speedMult);

    void applyAura(const std::vector<Enemy *> & all) override;

private:
    qreal m_radius;
    qreal m_Mult;
};

#endif //AURAENEMIES_H
