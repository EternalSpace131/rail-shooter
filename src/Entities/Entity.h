#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QPainter>

class GameMap;

class Entity : public QObject {
Q_OBJECT
public:
    Entity(qreal startX, qreal startY, qreal width, qreal height, GameMap* gameMap, int deltaTime);
    virtual ~Entity() = default;

    virtual void tick() = 0;
    virtual void draw(QPainter* painter);
    virtual void move();

    void setPosition(qreal x, qreal y);
    void updateBoundingBox();

    // —— 命中盒：返回“独立命中框”在世界坐标下的位置 —— //
    virtual QRectF hitBox() const;

    // —— 提供设置命中框的接口（以中心为基准的局部坐标） —— //
    // 例如：setHitRect(QRectF(-w/2, -h/2, w, h))
    void setHitRect(const QRectF& localCenteredRect) { m_hitRect = localCenteredRect; }

    // 基本访问器（保持你们原有接口不变）
    [[nodiscard]] const QRectF& getBoundingBox() const { return m_boundingBox; }
    [[nodiscard]] const QPointF& getPosition()   const { return m_position; }
    [[nodiscard]] qreal          getSpeed()      const { return m_speed; }
    [[nodiscard]] QPointF        getDirection()  const { return m_direction; }
    [[nodiscard]] bool           isAlive()       const { return m_isAlive; }
    [[nodiscard]] int            getDeltaTime()  const { return m_deltaTime; }

protected:
    static QRectF makeCenteredRect(const QPointF& c, qreal w, qreal h) {
        return QRectF(c.x() - w/2.0, c.y() - h/2.0, w, h);
    }

protected:
    // —— 位置与外观（外接矩形：可含留白，仅用于显示/越界清理） —— //
    QPointF m_position;   // 中心点
    qreal   m_width;
    qreal   m_height;
    QRectF  m_boundingBox;

    // —— 运动与状态 —— //
    QPointF m_direction;
    qreal   m_speed = 0;
    bool    m_isAlive = true;
    GameMap* m_gameMapPtr = nullptr;
    int     m_deltaTime = 16; // ms

    // —— 真实命中框（局部坐标、以中心为基准），只用于碰撞 —— //
    QRectF  m_hitRect;
};

#endif // ENTITY_H
