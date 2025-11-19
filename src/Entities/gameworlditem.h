#ifndef GAMEWORLDITEM_H
#define GAMEWORLDITEM_H

#include <QGraphicsObject> // 继承 QGraphicsObject
#include <QKeyEvent>
#include <memory> // for std::unique_ptr
#include <QFont>
#include "Manager/EntityManager.h"
#include<random>
#include <QSoundEffect>

// 前向声明 (Forward declare)
// 我们不需要在这里 #include 它们的 .h 文件，可以加快编译速度
class GameMap;
class Player;

// 游戏逻辑的更新频率 (DeltaTime)
const int DEFAULT_DELTA_TIME_MS = 16; // 约 60 FPS
const int ENEMY_SPAWN_INTERVAL_MS = 2000;
const int SHOOT_COOLDOWN_MS = 200;

class GameWorldItem : public QGraphicsObject
{
    Q_OBJECT // 必须的宏

public:
    // 构造函数需要知道地图的大小
    GameWorldItem(qreal mapWidth, qreal mapHeight, QGraphicsItem* parent = nullptr);
    ~GameWorldItem(); // 析构函数会自动清理 std::unique_ptr

    // --- Qt 必须重写的虚函数 ---

    // 1. 告诉 Qt 这个 Item 占多大区域
    QRectF boundingRect() const override;

    // 2. Qt 在这里调用 "draw"
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    EntityManager* getEntityManager() const { return m_entityManager.get(); }

    void onGameOver();
protected:
    // --- 键盘输入事件 ---
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // --- 游戏循环 ---
    // QTimer 会调用这个槽函数
    void gameTick();

private:
    // 更新玩家的 m_direction
    void updatePlayerDirection();
    void spawnEnemy();
    void playerShoot();

    // ===== 强化系统：状态 & 行为 =====
    void bindRewardCallback();  //绑定EntityManager::setOnReward
    void grantRandomUpgrade();  //发放一项随机强化
    void showUpgradeToast(const QString& text); // 可选：小提示（先占位）

    int   m_bulletDamage = 1;        // 子弹基础伤害，默认1
    int   m_spreadLevel  = 0;// 散射等级（0=直射；1=3发；2=5发…）
    int   m_spreadTimeLeftMs=0;

    std::mt19937 m_rng{std::random_device{}()}; // 用于随机选强化

    void updateTimedUpgrades(int deltaMs);

    int m_fireRate;            // 发射间隔（毫秒，控制自动发射频率，默认500ms/发）
    int m_currentCooldown;           // 当前剩余冷却时间

    // --- 你的游戏逻辑对象 ---
    // 使用智能指针自动管理内存
    std::unique_ptr<GameMap> m_gameMap;
    std::unique_ptr<EntityManager> m_entityManager;

    // 一个指向玩家的原始指针，用于快速传递输入
    // 注意：它的生命周期由 m_entityManager 里的 vector<unique_ptr> 管理
    Player* m_player;

    // --- Qt 组件 ---
    QTimer* m_gameTimer; // 游戏循环计时器

    // --- 玩家输入状态 ---
    bool m_moveLeft;
    bool m_moveRight;
    int m_enemySpawnCounter;
    int m_shootCooldownCounter;

    //播放音效
    void playSpawnSound(const QString& soundPath);


    //boss状态
    int m_lastBossScore = 0; // 上次生成 BOSS 时的分数
    bool m_isBossOnScreen = false; // BOSS 是否在场上
};

#endif // GAMEWORLDITEM_H
