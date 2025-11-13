#include "Enemy.h"
#include <QPainter>
#include <QtMath>
#include <GameLogic/GameMap.h>
#include <Manager/EntityManager.h>

Enemy::Enemy(qreal x, qreal y, qreal width, qreal height, GameMap* map, int deltaTime,
             qreal speed, int hp, EnemyKind kind, const QStringList& anims)
        : Entity(x, y, width, height, map, deltaTime),
          m_kind(kind),
          m_hp(hp)
{
    m_speed = speed;
    loadAnimationFrames(anims);

    // 敌人“真实命中框”（以中心为基准，略小于外观；具体角色可在生成处覆盖 setHitRect）
    setHitRect(QRectF(-m_width*0.41, -m_height*0.41, m_width*0.82, m_height*0.82));

    // 动画：每 100ms 切帧
    QObject::connect(&m_animationTimer, &QTimer::timeout, [this](){
        if (!m_animationFrames.isEmpty()) {
            m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animationFrames.size();
        }
    });
    m_animationTimer.start(100);
}

void Enemy::loadAnimationFrames(const QStringList& fpaths) {
    m_animationFrames.clear();
    for (const auto& p : fpaths) {
        QPixmap frame;
        if (frame.load(p)) {
            m_animationFrames.push_back(frame);
        }
    }
}

void Enemy::resetFrameBuffs() {
    m_speedFrameMult = 1.0;
    m_defenseFrameAdd = 0;
}

void Enemy::addSpeedMult(qreal m) { m_speedFrameMult *= m; }
void Enemy::addDefense(int v)     { m_defenseFrameAdd += v; }

void Enemy::applyAura(const std::vector<Enemy*>&) {
    // 缺省：无光环（光环怪在子类或外部逻辑里实现）
}

void Enemy::tick() {
    // 基础下落
    const qreal dt = m_deltaTime / 1000.0;
    m_position += QPointF(0, m_speed * m_speedFrameMult * dt);
    updateBoundingBox();

    // —— 过线消失（方案B）：到达保卫线就标记死亡，交由管理器统一清理 —— //
    // 注意：这里拿不到 EntityManager 指针，所以不在此处调用 onLeak(...)。
    // 如果你们需要在漏怪时触发扣分/音效，请把那部分留在 EntityManager 里做。
    // 删掉这段逻辑，避免敌人自主标记死亡
    // if (m_gameMapPtr && getBoundingBox().bottom() >= m_gameMapPtr->goalLineY()) {
    //     m_isAlive = false;
    // }
}

void Enemy::draw(QPainter* painter) {
    if (!painter) return;

    // 按“逻辑尺寸”绘制，避免素材原始像素把显示拉大
    const QRectF target(m_position.x() - m_width  / 2.0,
                        m_position.y() - m_height / 2.0,
                        m_width, m_height);

    if (!m_animationFrames.isEmpty()) {
        const QPixmap& pm = m_animationFrames[m_currentFrameIndex];
        painter->drawPixmap(target, pm, pm.rect());  // 将整张帧图映射到目标矩形
    } else {
        painter->setBrush(QColor(200, 60, 60));
        painter->setPen(Qt::NoPen);
        painter->drawRect(target);
    }

    // 如需绘制光环，仅在此处绘制，不影响 hitRect
    // 例如：
    // painter->setPen(QPen(QColor(0,255,0,80), 2));
    // painter->setBrush(Qt::NoBrush);
    // painter->drawEllipse(m_position, haloR, haloR);
}

void Enemy::takeDamage(int damage) {
    const int real = std::max(1, damage - (m_defenseBase + m_defenseFrameAdd));
    m_hp -= real;
    if (m_hp <= 0) m_isAlive = false;
}
