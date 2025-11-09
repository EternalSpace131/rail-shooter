//
// Created by wangs on 2025/10/12.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <QPointF>
#include <QRectF>
#include <QPixmap>
#include <QPainter>
#include <QObject>

class GameMap; // 前向声明，避免循环引用

class Entity : public QObject{
Q_OBJECT
protected:
    // --- 核心属性 ---
    QPointF m_position;        // 位置（中心点）
    qreal   m_width;           // 逻辑宽（用于碰撞/布局）
    qreal   m_height;          // 逻辑高
    QRectF  m_boundingBox;     // 外观/逻辑外接矩形（以中心为基准）

    // --- 物理与状态 ---
    QPointF m_direction;       // 方向（建议单位向量）
    qreal   m_speed;           // 像素/秒
    bool    m_isAlive;

    // --- 游戏世界交互 ---
    GameMap* m_gameMapPtr;
    int      m_deltaTime;      // ms

    // --- 命中盒缩放（独立于外观/光环）---
    // 命中盒 = 以中心为基准的矩形，尺寸 = (m_width * m_hitScaleX, m_height * m_hitScaleY)
    // 默认四周各收 20%（= 0.8 缩放）；子类可覆写或在构造时调整
    qreal m_hitScaleX {0.80};
    qreal m_hitScaleY {0.80};

public:
    Entity(qreal startX, qreal startY, qreal width, qreal height, GameMap* gameMap, int deltaTime);
    virtual ~Entity() = default;

    // --- 每帧更新 ---
    virtual void tick() = 0;
    virtual void draw(QPainter* painter);

    // --- 移动与位置 ---
    virtual void move();
    void updateBoundingBox();                 // 以中心为基准更新 m_boundingBox
    void setPosition(qreal x, qreal y);

    // --- 命中盒（用于碰撞判定）---
    // 缩放后的“更小矩形”，**不包含光环/贴图留白**，默认在基类按缩放生成
    virtual QRectF hitBox() const;

    // —— 调参接口（可按对象/种类微调）——
    void setHitboxScale(qreal sx, qreal sy) { m_hitScaleX = sx; m_hitScaleY = sy; }

    // --- 状态获取 ---
    [[nodiscard]] const QRectF& getBoundingBox() const; // 外观/逻辑外接矩形
    [[nodiscard]] const QPointF& getPosition()   const;
    [[nodiscard]] qreal          getSpeed()      const;
    [[nodiscard]] QPointF        getDirection()  const;
    [[nodiscard]] bool           isAlive()       const;
    [[nodiscard]] int            getDeltaTime()  const;

protected:
    // 以中心为基准生成矩形工具：给定中心与尺寸（w,h）
    static QRectF makeCenteredRect(const QPointF& c, qreal w, qreal h) {
        return QRectF(c.x() - w/2.0, c.y() - h/2.0, w, h);
    }
};

#endif
