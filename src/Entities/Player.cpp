//
// Created by Morgan on 25-10-13.
//
#include "Player.h"

#include <GameLogic/GameMap.h>

Player::Player(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime)
    :Entity(x, y, width, height,map, deltaTime){
    m_speed=220;
    m_direction=QPointF(0,0);
}

void Player::tick() {
    //限制水平移动
    move();
    const QRectF& B=m_gameMapPtr->bounds();
    //保持玩家不出界
    if(m_position.x()-m_width/2<B.left()) {
        m_position.setX(B.left()+m_width/2);
    }
    if(m_position.x()+m_width/2>B.right()) {
        m_position.setX(B.right()-m_width/2);
    }
    //将玩家的Y值固定
    qreal laneY=m_gameMapPtr->goalLineY()-60;//数值暂定为60
    setPosition(m_position.x(),laneY);
}
void Player::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    painter->fillRect(getBoundingBox(),Qt::blue);
}

