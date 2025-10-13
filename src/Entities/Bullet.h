//
// Created by Morgan on 25-10-13.
//

#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
class Bullet : public Entity{
public:
  Bullet(qreal x, qreal y, qreal width, qreal height,GameMap* map,int deltaTime,qreal speed,const QPointF& dir,int damage);

  void tick() override;

  void draw(QPainter* painter) override;

  int damage() const;//获取伤害
  void kill();//消失处理

private:
  int m_damage{1};
  void normalizeDirection();//将方向向量归一化
};


#endif //BULLET_H
