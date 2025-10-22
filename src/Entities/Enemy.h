//
// Created by Morgan on 25-10-13.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

class EntityManager;// 前置声明，供 onKilled/onLeak 使用

class Enemy: public Entity{
public:
  Enemy(qreal x, qreal y, qreal width, qreal height,GameMap* map,int deltaTime,qreal speed,int hp);

  void tick() override;

  void draw(QPainter* painter) override;

  void takeDamage(int damage);//伤害处理

  // 新增：敌人“被击杀时”的钩子（由 EntityManager 在敌人死亡那刻调用）
  virtual void onKilled(EntityManager* /*mgr*/) { /* 默认无操作 */ }

  // 新增：敌人“触线到底部时”的钩子（在 tick 里触发）
  virtual void onLeak(EntityManager* /*mgr*/) { /* 默认无操作（基础敌人目前也只是自毁）*/ }

private:
  int m_hp{1};
};


#endif //ENEMY_H
