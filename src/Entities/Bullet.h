#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include <QPixmap>

class Bullet : public Entity {
public:
    Bullet(qreal x, qreal y, qreal width, qreal height, GameMap* map, int deltaTime,
           qreal speed, const QPointF& dir, int damage);

    void tick() override;
    void draw(QPainter* painter) override;

    int  damage() const { return m_damage; }
    void kill()         { m_isAlive = false; }

private:
    void normalizeDirection();
    QPixmap m_bulletPixmap;
    int     m_damage{1};
};

#endif // BULLET_H
