//
// Created by Morgan on 25-10-14.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <Entities\Entity.h>
#include<vector>
#include<algorithm>
#include<memory>

class Player;
class Enemy;
class Bullet;

class EntityManager{
public:
    explicit EntityManager(GameMap* gameMap);
    ~EntityManager();

    // —— 实体创建 ——
    // 传入 unique_ptr，容器接管所有权；返回裸指针供调用方临时使用（例如绑定输入）。
    Player* addPlayer(std::unique_ptr<Player> p);
    Enemy*  addEnemy (std::unique_ptr<Enemy>  e);
    Bullet* addBullet(std::unique_ptr<Bullet> b);

    // —— 帧更新主流程 ——
    // 顺序：
    // 1) 逐类 tick()（推进位置/AI/冷却等）；
    // 2) 子弹-敌人碰撞（命中立即结算伤害，并标记子弹死亡，避免一弹多杀）；
    // 3) 清理：移除死亡或越界（不与地图相交）的实体，回收内存。
    void tickAll();

    // —— 统一绘制 ——
    void drawAll(QPainter* painter);

    GameMap* map() const;

private:
    // —— 小工具：条件删除 ——
    // 将“should keep”的谓词传进来；若返回 false 就删除该元素。
    template <typename T, typename Pred>
    static void erase_if(std::vector<std::unique_ptr<T>>& v, Pred keep);

    GameMap* m_map;//非拥有指针：由更高层管理其生命周期

    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Enemy>> enemies;
};

#endif //ENTITYMANAGER_H

