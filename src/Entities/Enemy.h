#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <QTimer>
#include "EnemySpec.h"

class EntityManager;

class Enemy: public Entity{
public:
    Enemy(qreal x, qreal y, qreal width, qreal height, GameMap* map, int deltaTime,
          qreal speed, int hp, EnemyKind kind, const QStringList& animFramePaths);

    EnemyKind getKind() const { return m_kind; }

    void resetFrameBuffs();
    void addSpeedMult(qreal m);
    void addDefense(int v);
    virtual void applyAura(const std::vector<Enemy*>&);

    void tick() override;
    void draw(QPainter* painter) override;
    void takeDamage(int damage);

    // 钩子：死亡/触线
    virtual void onKilled(EntityManager*) {}
    virtual void onLeak  (EntityManager*) {}

    bool isElite() const { return m_isElite; }
    bool isBoss()  const { return m_isBoss;  }
    void setElite(bool v){ m_isElite = v; }
    void setBoss (bool v){ m_isBoss  = v; }

private:
    EnemyKind m_kind;
    int   m_hp{1};
    int   m_defenseBase{0};
    qreal m_speedFrameMult{1.0};
    int   m_defenseFrameAdd{0};
    bool  m_isElite{false};
    bool  m_isBoss{false};

    QVector<QPixmap> m_animationFrames;
    int   m_currentFrameIndex = 0;
    QTimer m_animationTimer;

    void  loadAnimationFrames(const QStringList&);
};

#endif //ENEMY_H
