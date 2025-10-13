//
// Created by Morgan on 25-10-13.
//
#include "Bullet.h"

Bullet::Bullet(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, const QPointF &dir, int damage)
    :Entity(x, y, width, height, map,deltaTime),m_damage(damage){
    m_speed = speed;
    m_direction = dir;
    normalizeDirection();
}
void Bullet::tick() {
    move();
    //处理边界情况
    //todo
}
void Bullet::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    painter->fillRect(getBoundingBox(),Qt::yellow);
}
int Bullet::damage() const {
    return m_damage;
}

void Bullet::kill() {
    m_isAlive = false;
}

void Bullet::normalizeDirection() {
    qreal len=std::hypot(m_direction.x(),m_direction.y());
    if(len>0) {
        m_direction/=len;
    }
}




