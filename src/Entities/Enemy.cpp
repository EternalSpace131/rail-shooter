//
// Created by Morgan on 25-10-13.
//
#include"Enemy.h"

#include <GameLogic/GameMap.h>

Enemy::Enemy(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, int hp)
    :Entity(x, y, width, height, map, deltaTime),m_hp(hp){
    m_speed = speed;
    m_direction=QPointF(0,1);
}

void Enemy::tick() {
    move();
    //抵线处理
    if(getPosition().y()+m_height/2.0 >= m_gameMapPtr->goalLineY()) {
        m_isAlive = false;//抵线后消失
    }
    //触碰边界处理
    if(!m_gameMapPtr->bounds().intersects(getBoundingBox())) {
        m_isAlive = false;
    }
}

void Enemy::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    painter->fillRect(getBoundingBox(),Qt::red);
}

void Enemy::takeDamage(int damage) {
    m_hp -= damage;
    if(m_hp<=0) {
        m_isAlive = false;
    }
}



