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
    Enemy(qreal x, qreal y, qreal width, qreal height, GameMap* map, int deltaTime,
          qreal speed, int hp, EnemyKind kind, const QStringList& animFramePaths);

    EnemyKind getKind() const { return m_kind; }

    // 每帧前清零（光环是“本帧生效”的暂态）
    void resetFrameBuffs();
    // 供光环调用
    void addSpeedMult(qreal m);
    void addDefense(int v);
    // 子类可覆写（施放光环）
    virtual void applyAura(const std::vector<Enemy*>&/*allEnemies*/);

    void tick() override;
    void draw(QPainter* painter) override;
    void takeDamage(int damage);

    // —— 命中盒：只覆盖“本体”，不包含光环 —— //
    QRectF hitBox() const override {
        // 敌人略紧：左右/上下各收约 18%
        const qreal hw = m_width  * 0.82;
        const qreal hh = m_height * 0.82;
        return makeCenteredRect(m_position, hw, hh);
    }

    // 钩子：死亡/触线
    virtual void onKilled(EntityManager* /*mgr*/) { /* 默认无操作 */ }
    virtual void onLeak  (EntityManager* /*mgr*/) { /* 默认无操作 */ }

    bool isElite() const { return m_isElite; }
    bool isBoss()  const { return m_isBoss;  }
    void setElite(bool elite) { m_isElite = elite; }
    void setBoss (bool boss ) { m_isBoss  = boss;  }

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
