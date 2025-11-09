//
// Created by Morgan on 25-10-14.
//
#include "EntityManager.h"
#include <Entities/Player.h>
#include <Entities/Enemy.h>
#include <Entities/Bullet.h>
#include <GameLogic/GameMap.h>
#include <Entities/EnemySpec.h>
#include <algorithm>   // for std::remove_if
#include <vector>

EntityManager::EntityManager(GameMap* map) : m_map(map) {}
EntityManager::~EntityManager() = default;

void EntityManager::setOnReward(std::function<void(int)> cb) {
    m_onReward = std::move(cb);
}

void EntityManager::grantReward(int amount) {
    if (m_onReward) {
        m_onReward(amount);
    }
}

Player* EntityManager::addPlayer(std::unique_ptr<Player> p) {
    Player* raw = p.get();
    players.emplace_back(std::move(p));
    return raw;
}

Enemy* EntityManager::addEnemy(std::unique_ptr<Enemy> e) {
    Enemy* raw = e.get();
    enemies.emplace_back(std::move(e));
    return raw;
}

Bullet* EntityManager::addBullet(std::unique_ptr<Bullet> b) {
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
    // 1) 推进玩家
    for (auto& p : players) p->tick();

    // 2) 敌人：先重置临时加成 → 施放光环 → 再逐个 tick()
    std::vector<Enemy*> raw;
    raw.reserve(enemies.size());
    for (auto& e : enemies) raw.push_back(e.get());

    for (auto* e : raw) if (e->isAlive()) e->resetFrameBuffs();   // 清空本帧 buff
    for (auto* e : raw) if (e->isAlive()) e->applyAura(raw);      // 施放光环

    for (auto& e : enemies) e->tick(); // 敌人推进
    for (auto& b : bullets) b->tick(); // 子弹推进

    // 3) 碰撞：Bullet × Enemy 使用命中盒（HitBox）
    for (auto& b : bullets) {
        if (!b->isAlive()) continue;
        const QRectF hb = b->hitBox(); // 子弹命中盒

        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            const QRectF he = e->hitBox(); // 敌人命中盒（不含光环/留白）

            if (hb.intersects(he)) {
                // 命中
                e->takeDamage(b->damage());
                if (!e->isAlive()) {
                    e->onKilled(this);
                    // 依类型加分
                    std::vector<EnemySpec> enemyBook = getEnemyBook();
                    for (const auto& spec : enemyBook) {
                        if (spec.kind == e->getKind()) {
                            addScore(spec.score);
                            break;
                        }
                    }
                }
                b->kill(); // 一颗子弹只命中一个敌人
                break;
            }
        }
    }

    // 4) 清理：保留 “仍然存活 且 仍在地图内” 的对象
    auto keep_alive_and_inside = [this](const auto& up) {
        return up->isAlive() && m_map->bounds().intersects(up->getBoundingBox());
    };
    erase_if(players, keep_alive_and_inside);
    erase_if(enemies, keep_alive_and_inside);
    erase_if(bullets, keep_alive_and_inside);
}

template<typename T, typename Pred>
void EntityManager::erase_if(std::vector<std::unique_ptr<T>>& v, Pred keep) {
    v.erase(std::remove_if(v.begin(), v.end(),
                           [&](const std::unique_ptr<T>& x) { return !keep(x); }),
            v.end());
}

void EntityManager::drawAll(QPainter* painter) {
    for (auto& p : players) p->draw(painter);
    for (auto& e : enemies) e->draw(painter);
    for (auto& b : bullets) b->draw(painter);

    // 画出保卫线用于调试
    painter->setPen(QPen(Qt::darkGreen, 2));
    painter->drawLine(QPointF(m_map->bounds().left(),  m_map->goalLineY()),
                      QPointF(m_map->bounds().right(), m_map->goalLineY()));

    // --- 命中盒可视化（调试时可开） ---
    // const bool DEBUG_HITBOX = true;
    // if (DEBUG_HITBOX) {
    //     QPen pen(Qt::red); pen.setWidth(1);
    //     QBrush brush(QColor(255,0,0,60));
    //     painter->setPen(pen); painter->setBrush(brush);
    //     for (auto& e : enemies) painter->drawRect(e->hitBox());
    //     for (auto& b : bullets) painter->drawRect(b->hitBox());
    // }
}

GameMap* EntityManager::map() const {
    return m_map;
}
//