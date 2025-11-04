//
// Created by Morgan on 25-10-14.
//
#include "EntityManager.h"
#include<Entities/Player.h>
#include<Entities/Enemy.h>
#include<Entities/Bullet.h>
#include<GameLogic/GameMap.h>
#include <Entities/EnemySpec.h>

EntityManager::EntityManager(GameMap* map) : m_map(map) {}
EntityManager::~EntityManager() = default;

void EntityManager::setOnReward(std::function<void(int)> cb) {
    m_onReward=std::move(cb);
}

void EntityManager::grantReward(int amount) {
    if(m_onReward) {
        m_onReward(amount);
    }
}

Player *EntityManager::addPlayer(std::unique_ptr<Player> p) {
    Player* raw = p.get();
    players.emplace_back(std::move(p));
    return raw;
}

Enemy *EntityManager::addEnemy(std::unique_ptr<Enemy> e) {
    Enemy* raw = e.get();
    enemies.emplace_back(std::move(e));
    return raw;
}

Bullet *EntityManager::addBullet(std::unique_ptr<Bullet> b) {
    Bullet* raw = b.get();
    bullets.emplace_back(std::move(b));
    return raw;
}

// 加分
void EntityManager::addScore(int score) {
    if (score > 0) {
        m_totalScore += score;
    }
}

// 获取总分
int EntityManager::getTotalScore() const {
    return m_totalScore;
}

void EntityManager::tickAll() {
    // 推进所有实体各自的内部逻辑
    for (auto& p : players) p->tick();

     //敌人：先重置临时加成 → 施放光环 → 再逐个 tick()
    //收集裸指针列表，便于传给 applyAura
    std::vector<Enemy*> raw;
    raw.reserve(enemies.size());
    for (auto& e : enemies) raw.push_back(e.get());

    //  重置临时加成
    for (auto* e : raw) if (e->isAlive()) e->resetFrameBuffs();

    // 施放光环（由子类覆写）
    for (auto* e : raw) if (e->isAlive()) e->applyAura(raw);

    // 逐个推进
    for (auto& e : enemies) e->tick();

    for (auto& b : bullets) b->tick();

    // 极简碰撞：Bullet × Enemy 的 AABB 相交

    for (auto& b : bullets) if (b->isAlive()) {
            for (auto& e : enemies) if (e->isAlive()) {
                    if (b->getBoundingBox().intersects(e->getBoundingBox())) {
                        e->takeDamage(b->damage());// 伤害结算
                        if(!e->isAlive()) {
                            e->onKilled(this);//让具体敌人在死亡时执行自定义行为（比如奖励）
                            // 根据敌人类型匹配分数并添加
                            std::vector<EnemySpec> enemyBook = getEnemyBook();
                            for (const auto& spec : enemyBook) {
                                if (spec.kind == e->getKind()) { // 依赖 Enemy::getKind() 方法
                                    addScore(spec.score);
                                    break;
                                }
                            }
                        }
                        b->kill(); // 命中即销毁该子弹（防止穿透）
                        break;// 一颗子弹只命中一个敌人
                    }
                }
        }



    // 3) 清理：
    // 我们定义“保留条件”为：实体仍然存活 且 仍在地图可见范围内（与边界相交）。
    auto keep_alive_and_inside = [this](auto& up) {
        return up->isAlive() && m_map->bounds().intersects(up->getBoundingBox());
    };
    erase_if(players, keep_alive_and_inside);
    erase_if(enemies, keep_alive_and_inside);
    erase_if(bullets, keep_alive_and_inside);
}

template<typename T, typename Pred>
void EntityManager::erase_if(std::vector<std::unique_ptr<T> > &v, Pred keep) {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const std::unique_ptr<T>& x){
    return !keep(x);
        }), v.end());
}

void EntityManager::drawAll(QPainter*painter) {
    for (auto& p : players) p->draw(painter);
    for (auto& e : enemies) e->draw(painter);
    for (auto& b : bullets) b->draw(painter);
    // 画出保卫线用于调试
    painter->setPen(QPen(Qt::darkGreen, 2));
    painter->drawLine(QPointF(m_map->bounds().left(), m_map->goalLineY()),
    QPointF(m_map->bounds().right(), m_map->goalLineY()));
}

GameMap *EntityManager::map() const {
    return m_map;
}





