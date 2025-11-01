//
// Created by wangs on 2025/10/12.
//
#include "Entity.h"

//
// Created by Wangs on 2025/10/12.
//

#include "Entity.h"
#include <QDebug>

/**
 * 构造函数的实现。
 * 初始化所有成员变量，并尝试加载纹理贴图。
 * 如果贴图加载失败，会输出一个警告。这在调试时非常有用！
 */
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
    // 初始化碰撞箱
    updateBoundingBox();
}

/**
 * 更新碰撞箱的位置和大小。
 * 每当实体的位置或尺寸变化时，都需要调用此函数。
 */
void Entity::updateBoundingBox() {
    qreal left = m_position.x() - m_width / 2.0;
    qreal top = m_position.y() - m_height / 2.0;
    m_boundingBox.setRect(left, top, m_width, m_height);
}


void Entity::move() {
    if (!m_gameMapPtr) {
        qWarning("Entity::move(): m_gameMapPtr is null!");
        return;
    }

    // 根据方向、速度和时间计算本帧的位移
    // (m_deltaTime / 1000.0) 是为了将毫秒转换为秒
    qreal distance = m_speed * (m_deltaTime / 1000.0);
    qreal dx = m_direction.x() * distance;
    qreal dy = m_direction.y() * distance;

    // 如果没有移动，直接返回
    if (m_speed == 0) {
        return;
    }
    // 这里没有实现边界检测，针对不同类型的实体应该分别实现，可能是有差别的，比如子弹碰到边界会自动摧毁，玩家则是不给移动
    // 在子类中建议重载函数或者直接修改基类，因为地图还未实现不好操作
    m_position.setX(m_position.x() + dx);
    m_position.setY(m_position.y() + dy);

    updateBoundingBox();

}


// 绘制实体。
void Entity::draw(QPainter* painter) {
    if (!painter) return;
    painter->fillRect(m_boundingBox, Qt::gray); // 使用灰色作为默认颜色
}


void Entity::setPosition(qreal x, qreal y) {
    m_position.setX(x);
    m_position.setY(y);
    updateBoundingBox();
}

// --- Getter函数的实现 ---

const QRectF& Entity::getBoundingBox() const {
    return m_boundingBox;
}

const QPointF& Entity::getPosition() const {
    return m_position;
}


qreal Entity::getSpeed() const {
    return m_speed;
}

QPointF Entity::getDirection() const{
    return m_direction;
}


bool Entity::isAlive() const {
    return m_isAlive;
}

int Entity::getDeltaTime() const {
    return m_deltaTime;
}
