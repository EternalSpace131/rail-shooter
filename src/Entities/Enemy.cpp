//
// Created by Morgan on 25-10-13.
//
#include"Enemy.h"

#include <GameLogic/GameMap.h>

Enemy::Enemy(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, int hp,EnemyKind kind,const QStringList& animFramePaths)
    :Entity(x, y, width, height, map, deltaTime),m_hp(hp),m_kind(kind){
    m_speed = speed;
    m_direction=QPointF(0,1);

    // 加载当前敌人的动画帧
    loadAnimationFrames(animFramePaths);

    // 配置动画定时器（每100ms切换一帧，可调整速度）
    m_animationTimer.setInterval(100);
    connect(&m_animationTimer, &QTimer::timeout, this, [this]() {
        if (m_animationFrames.isEmpty()) return;
        // 循环切换帧索引（0 → 1 → ... → 最后一帧 → 0）
        m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animationFrames.size();
    });
    m_animationTimer.start(); // 启动定时器
}

// 加载动画帧的辅助函数
void Enemy::loadAnimationFrames(const QStringList& paths) {
    m_animationFrames.clear(); // 清空原有帧
    for (const QString& path : paths) {
        QPixmap frame(path);
        if (!frame.isNull()) {
            // 缩放帧尺寸与敌人尺寸一致
            QPixmap scaledFrame = frame.scaled(
                m_width, m_height,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );
            m_animationFrames.append(scaledFrame);
        }
    }
}

void Enemy::addDefense(int v) {
    m_defenseFrameAdd+=v;
}

void Enemy::addSpeedMult(qreal m) {
    m_speedFrameMult*=m;
}

void Enemy::resetFrameBuffs() {
    m_speedFrameMult=1.0;
    m_defenseFrameAdd=0;
}

void Enemy::applyAura(const std::vector<Enemy *> &) {}

void Enemy::tick() {
    // 应用本帧速度加成后移动
    const qreal oldSpeed = m_speed;
    m_speed = oldSpeed * m_speedFrameMult;
    move();
    m_speed=oldSpeed;// 还原基础速度（加成只影响当帧）

    //抵线处理
    if(getPosition().y()+m_height/2.0 >= m_gameMapPtr->goalLineY()) {
        m_isAlive = false;//抵线后消失
        onLeak(nullptr);// 当前默认无操作；奖励箱会覆写它
    }
    //触碰边界处理
    if(!m_gameMapPtr->bounds().intersects(getBoundingBox())) {
        m_isAlive = false;
    }
}

// 可选：绘制时根据精英/彩蛋/BOSS换颜色
void Enemy::draw(QPainter *painter) {
    if(!painter) {
        return;
    }

    if (!m_animationFrames.isEmpty()) {
        // 绘制当前帧（位置与敌人坐标对齐）
        painter->drawPixmap(
            getPosition().x() - m_width/2,
            getPosition().y() - m_height/2,
            m_animationFrames[m_currentFrameIndex]
            );
    } else {
        // 原颜色块绘制逻辑
        QColor c = Qt::red;
        if (m_isBoss) c = QColor(180, 30, 180);
        else if (m_isElite) c = QColor(255, 100, 100);
        painter->fillRect(getBoundingBox(), c);
    }
}


void Enemy::takeDamage(int damage) {
    int totalDefense = m_defenseBase + m_defenseFrameAdd;
    int finalDamage = qMax(1, damage - totalDefense); // 最小伤害1，防止无敌
    m_hp -= finalDamage;
    if(m_hp <= 0) {
        m_isAlive = false;
    }
}



