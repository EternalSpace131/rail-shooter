//
// Created by Morgan on 25-10-13.
//
#include "Bullet.h"
#include <QDebug>
#include <GameLogic/GameMap.h>

Bullet::Bullet(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, const QPointF &dir, int damage)
    :Entity(x, y, width, height, map,deltaTime),m_damage(damage){
    m_speed = speed;
    m_direction = dir;
    normalizeDirection();

    if (!m_bulletPixmap.load(":/assets/images/spit.png")) {
        qDebug() << "子弹图片加载失败";
    }else {
        // 缩放图片以匹配子弹宽高
        m_bulletPixmap = m_bulletPixmap.scaled(
            m_height, m_width,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
        // qDebug() << "子弹宽高:" << width << height;

    }
}
void Bullet::tick() {
    move();
    //处理边界情况
    if(!m_gameMapPtr->bounds().intersects(getBoundingBox())) {
        m_isAlive = false;
    }

}
void Bullet::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    // painter->fillRect(getBoundingBox(),Qt::yellow);
    if (!m_bulletPixmap.isNull()) {
        QPointF drawPos = m_position - QPointF(m_width/2, m_height/2);
        painter->drawPixmap(drawPos, m_bulletPixmap);
        // qDebug() << drawPos;
    }else{
        qDebug() << "子弹图片加载失败";
    }
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




