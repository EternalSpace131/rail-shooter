//
// Created by Morgan on 25-10-13.
//

#ifndef PLAYER_H
#define PLAYER_H
#include"Entity.h"

class Player : public Entity{
public:
  Player(qreal x, qreal y, qreal width, qreal height, GameMap* map,int deltaTime);

  void tick() override;//玩家移动
  void draw(QPainter* painter) override;//玩家渲染

  //帮助处理输入
  void moveLeft(){ m_direction=QPointF(-1,0); }
  void moveRight(){ m_direction=QPointF(1,0); }
  void stop(){ m_direction=QPointF(0,0); }
  void setHorizontalDirection(qreal xDir) {
      // Player::tick() 会处理Y轴，我们只设置X轴方向
      m_direction.setX(xDir);
  }
protected:
  //todo
};


#endif //PLAYER_H
