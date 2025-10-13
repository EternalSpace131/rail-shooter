//
// Created by Morgan on 25-10-13.
//
#include "Player.h"

Player::Player(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime)
    :Entity(x, y, width, height,map, deltaTime){
    m_speed=220;
    m_direction=QPointF(0,0);
}

void Player::tick() {
    //限制水平移动
    move();
    //todo
    //将玩家的Y值固定
}
void Player::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    painter->fillRect(getBoundingBox(),Qt::blue);
}

