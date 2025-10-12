//
// Created by wangs on 2025/10/12.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <QPointF>
#include <QRectF>
#include <QPixmap>
#include <QPainter>

class GameMap; // 前向声明，避免循环引用

class Entity {
protected:
    // --- 核心属性 ---
    QPointF m_position;        // 位置：实体在地图上的精确坐标，是中心点
    qreal m_width;             // 宽度：碰撞箱的宽度
    qreal m_height;            // 高度：碰撞箱的高度
    QRectF m_boundingBox;      // 碰撞箱：用于碰撞检测的矩形区域

    // --- 物理与状态 ---
    QPointF m_direction;       // 移动方向，是个向量，最好设置为模长为1
    qreal m_speed;             // 移动速度：决定了实体每帧可以移动多远
    bool m_isAlive;            // 生存状态


    // --- 游戏世界交互 ---
    GameMap* m_gameMapPtr;     // 指向游戏地图的指针，用于获取地图信息（如边界、障碍物）
    int m_deltaTime;           // 帧间隔（毫秒）：用于确保游戏逻辑与帧率无关，这个要看他们窗口是怎么实现的

public:

    Entity(qreal startX, qreal startY, qreal width, qreal height, GameMap* gameMap, int deltaTime);


    virtual ~Entity() = default;

    // --- 每帧更新 ---

    //每个实体都需要实现自己的`tick`函数，用于处理该帧的所有逻辑，

    virtual void tick() = 0;


    virtual void draw(QPainter* painter);

    // --- 移动与位置 ---

    virtual void move();


    void updateBoundingBox();


    void setPosition(qreal x, qreal y);

    // --- 状态获取 ---
    [[nodiscard]] const QRectF& getBoundingBox() const;
    [[nodiscard]] const QPointF& getPosition() const;
    [[nodiscard]] qreal getSpeed() const;
    [[nodiscard]] QPointF getDirection() const;
    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] int getDeltaTime() const;

};

#endif
