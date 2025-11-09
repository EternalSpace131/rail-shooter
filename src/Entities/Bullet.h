//
// Created by Morgan on 25-10-13.
//

#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>
#include "Entity.h"

class Bullet : public Entity{
public:
    Bullet(qreal x, qreal y, qreal width, qreal height, GameMap* map, int deltaTime,
           qreal speed, const QPointF& dir, int damage);

    void tick() override;
    void draw(QPainter* painter) override;

    // 命中盒：子弹更紧（体验更好）
    QRectF hitBox() const override {
        // 宽仅 35%，高 55%（可按手感微调）
        const qreal hw = m_width  * 0.35;
        const qreal hh = m_height * 0.55;
        return makeCenteredRect(m_position, hw, hh);
    }

    int  damage() const; // 获取伤害
    void kill();         // 消失处理

private:
    int     m_damage{1};
    void    normalizeDirection(); // 将方向向量归一化
    QPixmap m_bulletPixmap;       // 子弹贴图
};

#endif // BULLET_H
