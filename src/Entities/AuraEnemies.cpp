//
// Created by Morgan on 25-10-23.
//
#include "AuraEnemies.h"

DefenseAuraEnemy::DefenseAuraEnemy(qreal x, qreal y, qreal w, qreal h, GameMap *map, int dt, qreal speed, int hp,  qreal radius, int defenseAdd,EnemyKind kind,const QStringList& animFrames):
Enemy(x,y,w,h,map,dt,speed,hp,kind,animFrames) {
    m_radius = radius;
    m_defAdd=defenseAdd;
}

void DefenseAuraEnemy::applyAura(const std::vector<Enemy *> &all) {
    const QPointF c = getPosition();
    for (Enemy* e : all) {
        if (e==this || !e->isAlive()) continue;
        const QPointF d = e->getPosition() - c;
        if (std::hypot(d.x(), d.y()) <= m_radius) e->addDefense(m_defAdd);
    }
}

// 绘制防御光环（绿色半透明）
void DefenseAuraEnemy::draw(QPainter *painter) {
    if (!painter) return;

    // 1. 先绘制敌人自身（动画帧）
    Enemy::draw(painter);

    // 2. 绘制光环
    QPointF center = getPosition(); // 敌人中心坐标
    qreal radius = m_radius;       // 光环实际生效半径

    // 保存画笔状态（避免影响其他绘制）
    painter->save();

    // 配置光环样式：绿色半透明圆形，带细边框
    QColor fillColor(0, 255, 0, 50);  // 填充色：绿色，透明度80（0-255）
    QColor borderColor(0, 200, 0, 150); // 边框色：深绿色，透明度150
    QBrush brush(fillColor);
    QPen pen(borderColor);
    pen.setWidth(2); // 边框宽度2像素
    painter->setBrush(brush);
    painter->setPen(pen);

    // 应用呼吸动画缩放（
    painter->translate(center); // 坐标原点移到敌人中心
    painter->scale(m_haloScale, m_haloScale); // 缩放光环

    // 绘制圆形光环（此时圆心在原点）
    painter->drawEllipse(QPointF(0, 0), radius, radius);

    // 恢复画笔状态
    painter->restore();
}

// 呼吸动画逻辑
void DefenseAuraEnemy::tick() {
    Enemy::tick(); // 先执行父类的移动、边界检测等逻辑

    // 光环在 0.9~1.1 之间缩放，每帧变化 0.01（可调整速度）
    if (m_haloGrow) {
        m_haloScale += 0.01;
        if (m_haloScale >= 1.1) m_haloGrow = false;
    } else {
        m_haloScale -= 0.01;
        if (m_haloScale <= 0.9) m_haloGrow = true;
    }
}

SpeedAuraEnemy::SpeedAuraEnemy(qreal x, qreal y, qreal w, qreal h, GameMap *map, int dt, qreal speed, int hp, qreal radius, qreal speedMult,EnemyKind kind,const QStringList& animFrames):
Enemy(x,y,w,h,map,dt,speed,hp,kind,animFrames){
    m_radius = radius;
    m_Mult=speedMult;
}

void SpeedAuraEnemy::applyAura(const std::vector<Enemy *> &all) {
    const QPointF c = getPosition();
    for (Enemy* e : all) {
        if (e==this || !e->isAlive()) continue;
        const QPointF d = e->getPosition() - c;
        if (std::hypot(d.x(), d.y()) <= m_radius) e->addSpeedMult(m_Mult);
    }
}

// 绘制速度光环（蓝色半透明）
void SpeedAuraEnemy::draw(QPainter *painter) {
    if (!painter) return;

    // 1. 先绘制敌人自身（动画帧）
    Enemy::draw(painter);

    // 2. 绘制光环
    QPointF center = getPosition();
    qreal radius = m_radius;

    painter->save();

    // 配置光环样式：蓝色半透明圆形，带细边框
    QColor fillColor(0, 150, 255, 50);  // 填充色：蓝色，透明度80
    QColor borderColor(0, 100, 200, 150); // 边框色：深蓝色，透明度150
    QBrush brush(fillColor);
    QPen pen(borderColor);
    pen.setWidth(2);
    painter->setBrush(brush);
    painter->setPen(pen);

    // 应用呼吸动画缩放
    painter->translate(center);
    painter->scale(m_haloScale, m_haloScale);

    // 绘制圆形光环
    painter->drawEllipse(QPointF(0, 0), radius, radius);

    painter->restore();
}

// 呼吸动画逻辑
void SpeedAuraEnemy::tick() {
    Enemy::tick();

    if (m_haloGrow) {
        m_haloScale += 0.01;
        if (m_haloScale >= 1.1) m_haloGrow = false;
    } else {
        m_haloScale -= 0.01;
        if (m_haloScale <= 0.9) m_haloGrow = true;
    }
}
