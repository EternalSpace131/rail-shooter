//
// Created by Morgan on 25-10-23.
//
#include "AuraEnemies.h"

DefenseAuraEnemy::DefenseAuraEnemy(qreal x, qreal y, qreal w, qreal h, GameMap *map, int dt, qreal speed, int hp,  qreal radius, int defenseAdd):
Enemy(x,y,w,h,map,dt,speed,hp) {
    m_radius = radius;
    m_defAdd=defenseAdd;
}

void DefenseAuraEnemy::applyAura(const std::vector<Enemy *> &all) {
    const QPointF c = getPosition();
    for (Enemy* e : all) {
        if (e==this || !e->isAlive()) continue;
        const QPointF d = e->getPosition() - c;
        if (std::hypot(d.x(), d.y()) <= m_radius) e->addDefense(m_defAdd);
    }
}

SpeedAuraEnemy::SpeedAuraEnemy(qreal x, qreal y, qreal w, qreal h, GameMap *map, int dt, qreal speed, int hp, qreal radius, qreal speedMult):
Enemy(x,y,w,h,map,dt,speed,hp){
    m_radius = radius;
    m_Mult=speedMult;
}

void SpeedAuraEnemy::applyAura(const std::vector<Enemy *> &all) {
    const QPointF c = getPosition();
    for (Enemy* e : all) {
        if (e==this || !e->isAlive()) continue;
        const QPointF d = e->getPosition() - c;
        if (std::hypot(d.x(), d.y()) <= m_radius) e->addSpeedMult(m_Mult);
    }
}


