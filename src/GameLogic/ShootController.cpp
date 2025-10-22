//
// Created by ROG on 25-10-19.
//

#include "ShootController.h"
#include "../Entities/Bullet.h"
#include "../Gamelogic/GameMap.h"

// 构造函数：可指定发射间隔（fireRate越小，发射越快）
ShootController::ShootController(EntityManager& entityManager, Player* player, int fireRate)
    : m_entityManager(entityManager), m_player(player), m_fireRate(fireRate), m_currentCooldown(0) {}

// 每帧更新：自动处理冷却和发射
void ShootController::tick() {
    if (!m_player) return;  // 玩家为空时不处理

    // 1. 冷却时间递减（基于玩家的帧间隔时间同步）
    m_currentCooldown -= m_player->getDeltaTime();
    if (m_currentCooldown < 0) m_currentCooldown = 0;

    // 2. 冷却结束时自动发射子弹，并重置冷却
    if (m_currentCooldown == 0) {
        spawnBullet();
        m_currentCooldown = m_fireRate;  // 重置为设定的发射间隔
    }
}

void ShootController::spawnBullet() {
    const QPointF playerPos = m_player->getPosition();  // 玩家中心位置
    const qreal playerHalfHeight = m_player->getBoundingBox().height() / 2;  // 玩家半高

    // 子弹参数（10x10方框，向上发射）
    const qreal bulletSize = 10;
    const qreal bulletSpeed = 600;  // 像素/秒（与Entity的move逻辑匹配）
    const QPointF bulletDir(0, -1);  // 固定向上
    const int bulletDamage = 1;

    // 计算子弹初始位置（玩家顶部正上方，避免重叠）
    qreal bulletX = playerPos.x();
    qreal bulletY = playerPos.y() - playerHalfHeight - bulletSize / 2;

    // 创建子弹并添加到实体管理器
    auto bullet = std::make_unique<Bullet>(
        bulletX, bulletY,
        bulletSize, bulletSize,
        m_entityManager.map(),
        m_player->getDeltaTime(),  // 与玩家同帧率
        bulletSpeed,
        bulletDir,
        bulletDamage
    );

    m_entityManager.addBullet(std::move(bullet));
}