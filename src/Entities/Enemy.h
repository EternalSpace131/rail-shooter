//
// Created by Morgan on 25-10-13.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include <QTimer>
#include "EnemySpec.h"

class EntityManager;// 前置声明，供 onKilled/onLeak 使用

class Enemy: public Entity{
public:
  Enemy(qreal x, qreal y, qreal width, qreal height,GameMap* map,int deltaTime,qreal speed,int hp,EnemyKind kind,const QStringList& animFramePaths);

    // 获取敌人类型（用于匹配分数）
  EnemyKind getKind() const { return m_kind; }

  // 每帧开始前由管理器清零（光环是“本帧生效”的暂态）
  void resetFrameBuffs();

  // 供光环调用
  void addSpeedMult(qreal m);
  void addDefense(int v);

  // 给管理器调用的“光环施放”虚函数（默认空实现）
  virtual void applyAura(const std::vector<Enemy*>&/*allEnemies*/);

  void tick() override;

  void draw(QPainter* painter) override;

  void takeDamage(int damage);//伤害处理

  // 新增：敌人“被击杀时”的钩子（由 EntityManager 在敌人死亡那刻调用）
  virtual void onKilled(EntityManager* /*mgr*/) { /* 默认无操作 */ }

  // 新增：敌人“触线到底部时”的钩子（在 tick 里触发）
  virtual void onLeak(EntityManager* /*mgr*/) { /* 默认无操作（基础敌人目前也只是自毁）*/ }

  bool isElite() const { return m_isElite; }
  bool isBoss() const { return m_isBoss; }
  void setElite(bool elite) { m_isElite = elite; }
  void setBoss(bool boss) { m_isBoss = boss; }

private:
  EnemyKind m_kind; //存储敌人类型
  int m_hp{1};
  int m_defenseBase{0};
  // “本帧”临时加成（由管理器先重置、再由光环叠加）
  qreal m_speedFrameMult{1.0};
  int   m_defenseFrameAdd{0};
  bool  m_isElite{false};
  bool  m_isBoss{false};

  QVector<QPixmap> m_animationFrames; // 存储动画帧
  int m_currentFrameIndex = 0;        // 当前帧索引
  QTimer m_animationTimer;            // 控制帧切换的定时器
  void loadAnimationFrames(const QStringList&);         // 加载动画帧的辅助函数
};


#endif //ENEMY_H
