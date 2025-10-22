//
// Created by Morgan on 25-10-22.
//

#ifndef REWARDCRATE_H
#define REWARDCRATE_H
#include"Entity.h"
class EntityManager;

class RewardCrate : public Enemy{
public:
  RewardCrate(qreal x, qreal y, qreal width, qreal height,GameMap* map,int deltaTime,qreal speed,int hp,int reward);

  void draw(QPainter *painter) override;

  //被打掉：发放奖励，不走普通击杀逻辑
  void onKilled(EntityManager* mgr) override;

  void onLeak(EntityManager* /*mgr*/) override;

private:
  int m_reward{20};
};

#endif //REWARDCRATE_H
