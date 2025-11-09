//
// Created by Morgan on 25-10-13.
//
#include "Bullet.h"
#include <QDebug>
#include <GameLogic/GameMap.h>

Bullet::Bullet(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime,
               qreal speed, const QPointF &dir, int damage)
        : Entity(x, y, width, height, map, deltaTime),
          m_damage(damage)
{
    m_speed = speed;
    m_direction = dir;
    normalizeDirection();

    if (!m_bulletPixmap.load(":/assets/images/spit.png")) {
        qDebug() << "子弹图片加载失败";
    } else {
        // 贴图缩放到与逻辑宽高接近（保持纵横比）
        m_bulletPixmap = m_bulletPixmap.scaled(
                m_width, m_height,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
        );
    }
}

void Bullet::tick() {
    move();
    // 越界：交给外层清理时也会被剔除，这里可按需提前置死
    if(!m_gameMapPtr->bounds().intersects(getBoundingBox())) {
        m_isAlive = false;
    }
}

void Bullet::draw(QPainter *painter) {
    if(!painter) return;

    if (!m_bulletPixmap.isNull()) {
        // 以中心为基准绘制
        const QPointF topLeft = m_position - QPointF(m_bulletPixmap.width()/2.0, m_bulletPixmap.height()/2.0);
        painter->drawPixmap(topLeft, m_bulletPixmap);
    } else {
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(makeCenteredRect(m_position, m_width, m_height));
    }
}

int Bullet::damage() const { return m_damage; }
void Bullet::kill()        { m_isAlive = false; }

void Bullet::normalizeDirection() {
    const qreal len = std::hypot(m_direction.x(), m_direction.y());
    if(len > 0) m_direction /= len;
}
