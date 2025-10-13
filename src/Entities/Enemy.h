//
// Created by Morgan on 25-10-13.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

class Enemy: public Entity{
public:
  Enemy(qreal x, qreal y, qreal width, qreal height,GameMap* map,int deltaTime,qreal speed,int hp);

  void tick() override;

  void draw(QPainter* painter) override;

  void takeDamage(int damage);//伤害处理

private:
  int m_hp{1};
};


#endif //ENEMY_H
