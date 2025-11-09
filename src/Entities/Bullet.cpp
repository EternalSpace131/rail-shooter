#include "Bullet.h"
#include <QDebug>
#include <QtMath>
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
        // 注意：这是显示用，不影响命中框
        m_bulletPixmap = m_bulletPixmap.scaled(m_width, m_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // ✅ 子弹的真实命中框（更紧）：以中心为基准
    // （宽 35%，高 55%）可按手感微调
    setHitRect(QRectF(-m_width*0.175, -m_height*0.275, m_width*0.35, m_height*0.55));
}

void Bullet::tick() {
    move();
    if(!m_gameMapPtr->bounds().intersects(getBoundingBox())) {
        m_isAlive = false; // 越界回收
    }
}

void Bullet::draw(QPainter *painter) {
    if(!painter) return;
    if (!m_bulletPixmap.isNull()) {
        const QPointF tl = m_position - QPointF(m_bulletPixmap.width()/2.0, m_bulletPixmap.height()/2.0);
        painter->drawPixmap(tl, m_bulletPixmap);
    } else {
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QRectF(m_position.x()-m_width/2.0, m_position.y()-m_height/2.0, m_width, m_height));
    }
}

void Bullet::normalizeDirection() {
    const qreal len = std::hypot(m_direction.x(), m_direction.y());
    if(len > 0) m_direction /= len;
}
