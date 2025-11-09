#include "Entity.h"
#include <QDebug>
#include <GameLogic/GameMap.h>

Entity::Entity(qreal startX, qreal startY, qreal width, qreal height, GameMap* gameMap, int deltaTime)
        : QObject(nullptr),
          m_position(startX, startY),
          m_width(width),
          m_height(height),
          m_gameMapPtr(gameMap),
          m_deltaTime(deltaTime)
{
    updateBoundingBox();
    // 默认把命中框设置为“略小于外观的矩形”（先给个保底值；具体角色会再覆盖/重置）
    m_hitRect = QRectF(-m_width*0.40, -m_height*0.40, m_width*0.80, m_height*0.80);
}

void Entity::updateBoundingBox() {
    m_boundingBox = makeCenteredRect(m_position, m_width, m_height);
}

void Entity::move() {
    if (!m_gameMapPtr) return;
    if (m_speed == 0) return;
    const qreal dt = m_deltaTime / 1000.0;
    m_position += QPointF(m_direction.x() * m_speed * dt,
                          m_direction.y() * m_speed * dt);
    updateBoundingBox();
}

void Entity::draw(QPainter* painter) {
    if (!painter) return;
    // 默认不画贴图：由子类决定；这里给个“灰框占位”以防空
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(120,120,120));
    painter->drawRect(m_boundingBox);
}

void Entity::setPosition(qreal x, qreal y) {
    m_position = QPointF(x,y);
    updateBoundingBox();
}

QRectF Entity::hitBox() const {
    QRectF r = m_hitRect;
    r.translate(m_position);
    return r;
}
