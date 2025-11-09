//
// Created by wangs on 2025/10/12.
//
#include "Entity.h"
#include <QDebug>

Entity::Entity(qreal startX, qreal startY, qreal width, qreal height, GameMap* gameMap, int deltaTime)
        : QObject(nullptr),
          m_position(startX, startY),
          m_width(width),
          m_height(height),
          m_speed(0),
          m_isAlive(true),
          m_gameMapPtr(gameMap),
          m_deltaTime(deltaTime)
{
    updateBoundingBox();
}

void Entity::updateBoundingBox() {
    m_boundingBox = makeCenteredRect(m_position, m_width, m_height);
}

void Entity::move() {
    if (!m_gameMapPtr) {
        qWarning("Entity::move(): m_gameMapPtr is null!");
        return;
    }
    if (m_speed == 0) return;

    const qreal dt = m_deltaTime / 1000.0;
    m_position += QPointF(m_direction.x() * m_speed * dt,
                          m_direction.y() * m_speed * dt);
    updateBoundingBox();
}

void Entity::draw(QPainter* painter) {
    if (!painter) return;
    painter->fillRect(m_boundingBox, Qt::gray);
}

void Entity::setPosition(qreal x, qreal y) {
    m_position = QPointF(x,y);
    updateBoundingBox();
}

// —— 新增：命中盒 ——
// 默认：在外接矩形基础上做缩放（四周各收 20%），子类可覆写
QRectF Entity::hitBox() const {
    const qreal hw = m_width  * m_hitScaleX;
    const qreal hh = m_height * m_hitScaleY;
    return makeCenteredRect(m_position, hw, hh);
}

// --- Getters ---
const QRectF& Entity::getBoundingBox() const { return m_boundingBox; }
const QPointF& Entity::getPosition() const   { return m_position; }
qreal  Entity::getSpeed() const              { return m_speed; }
QPointF Entity::getDirection() const         { return m_direction; }
bool   Entity::isAlive() const               { return m_isAlive; }
int    Entity::getDeltaTime() const          { return m_deltaTime; }
