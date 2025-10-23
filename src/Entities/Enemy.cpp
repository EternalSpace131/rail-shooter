//
// Created by Morgan on 25-10-13.
//
#include"Enemy.h"

#include <GameLogic/GameMap.h>

Enemy::Enemy(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, int hp)
    :Entity(x, y, width, height, map, deltaTime),m_hp(hp){
    m_speed = speed;
    m_direction=QPointF(0,1);
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
    m_speed+=oldSpeed;// 还原基础速度（加成只影响当帧）

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
    QColor c = Qt::red;
    if (m_isBoss) c = QColor(180,30,180);
    else if (m_isElite) c = QColor(255,100,100);
    painter->fillRect(getBoundingBox(), c);
}

void Enemy::takeDamage(int damage) {
    m_hp -= damage;
    if(m_hp<=0) {
        m_isAlive = false;
    }
}



