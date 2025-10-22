//
// Created by ROG on 25-10-19.
//

#ifndef SHOOTCONTROLLER_H
#define SHOOTCONTROLLER_H

#include "../Manager/EntityManager.h"
#include "../Entities/Player.h"

class ShootController {
public:

    ShootController(EntityManager& entityManager, Player* player, int fireRate = 500);

    void tick();

private:
    EntityManager& m_entityManager;  // 实体管理器（用于添加子弹）
    Player* m_player;

    const int m_fireRate;            // 发射间隔（毫秒，控制自动发射频率，默认500ms/发）
    int m_currentCooldown;           // 当前剩余冷却时间
    void spawnBullet();
};

#endif // SHOOTCONTROLLER_H