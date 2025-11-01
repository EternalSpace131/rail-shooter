//
// Created by Morgan on 25-10-13.
//

#ifndef PLAYER_H
#define PLAYER_H
#include"Entity.h"
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QObject>

class Player : public Entity{
    Q_OBJECT
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
private:
    QVector<QPixmap> m_animationFrames; // 存储动画帧
    int m_currentFrameIndex = 0;        // 当前帧索引
    QTimer m_animationTimer;            // 控制帧切换的定时器
    void loadAnimationFrames();         // 加载动画帧的辅助函数
};


#endif //PLAYER_H
