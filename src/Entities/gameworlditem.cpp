#include "gameworlditem.h"
#include "Manager/EntityManager.h"       // 你的实体管理器
#include "Gamelogic/GameMap.h"             // 你的地图
#include "Entities/Player.h"   // 你的玩家类 (确保路径正确)
#include "Entities/Enemy.h"    // (未来你可能在这里生成敌人)
#include "Entities/Bullet.h"
#include <QPainter>
#include <QTimer>
#include <QDebug> // 用于调试
#include <QRandomGenerator>

GameWorldItem::GameWorldItem(qreal mapWidth, qreal mapHeight, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    m_player(nullptr),
    m_moveLeft(false),
    m_moveRight(false),
    m_enemySpawnCounter(0),
    m_shootCooldownCounter(0)
{
    // --- 1. 创建你的 GameMap ---
    // 定义地图的边界和终点线
    QRectF mapBounds(0, 0, mapWidth, mapHeight);
    // 假设“终点线”在底部向上 80 像素处 (你需要根据你的设计调整这个 80)
    qreal goalLine = mapHeight - 80.0;

    // 使用你提供的构造函数
    m_gameMap = std::make_unique<GameMap>(mapBounds, goalLine);

    // --- 2. 创建你的 EntityManager ---
    m_entityManager = std::make_unique<EntityManager>(m_gameMap.get());

    // --- 3. 创建并添加玩家 ---
    qreal playerX = m_gameMap->bounds().center().x();
    qreal playerY = m_gameMap->goalLineY() - 60; // 与你 Player::tick 中的逻辑一致

    // 假设玩家大小为 30x30 (你需要根据你的设计调整)
    auto player = std::make_unique<Player>(playerX, playerY, 30, 30, m_gameMap.get(), DEFAULT_DELTA_TIME_MS);

    // 将玩家添加到管理器，并保留一个原始指针用于输入
    m_player = m_entityManager->addPlayer(std::move(player));

    // --- 4. 启动游戏循环 ---
    m_gameTimer = new QTimer(this);
    // 关键：连接 QTimer 的 timeout 信号到我们的 gameTick() 槽
    connect(m_gameTimer, &QTimer::timeout, this, &GameWorldItem::gameTick);
    m_gameTimer->start(DEFAULT_DELTA_TIME_MS);

    // --- 5. 允许此 Item 接收键盘焦点 ---
    // 这样 keyPressEvent 才能工作
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

GameWorldItem::~GameWorldItem()
{
    // unique_ptr 会在这里自动释放 m_gameMap 和 m_entityManager
    // m_player 会被 m_entityManager 的析构函数自动清理
}

// 告诉 QGraphicsScene 这个 Item 占据多大区域
QRectF GameWorldItem::boundingRect() const
{
    // 我们的 Item 和地图一样大
    return m_gameMap->bounds();
}

// *** 关键：绘制 ***
// QGraphicsScene 会自动调用此函数来 "打印"
void GameWorldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); // 避免编译器警告
    Q_UNUSED(widget); // 避免编译器警告

    // 我们唯一要做的就是命令 EntityManager 绘制所有实体
    // 这会调用你所有的 Player::draw(), Enemy::draw(), Bullet::draw()
    m_entityManager->drawAll(painter);
}

// *** 关键：逻辑 ***
// QTimer 每 16ms 调用一次此函数
void GameWorldItem::gameTick()
{
    // --- 1. 更新计时器 ---

    // 更新射击冷却
    if (m_shootCooldownCounter > 0) {
        m_shootCooldownCounter -= DEFAULT_DELTA_TIME_MS;
    }

    // 更新敌人生成
    m_enemySpawnCounter += DEFAULT_DELTA_TIME_MS;
    if (m_enemySpawnCounter >= ENEMY_SPAWN_INTERVAL_MS) {
        spawnEnemy();
        m_enemySpawnCounter = 0;
    }

    // --- 2. 运行所有游戏逻辑 ---
    // (tick, 碰撞检测, 清理)
    m_entityManager->tickAll();

    // --- 3. 请求重绘 ---
    update();
}

void GameWorldItem::spawnEnemy()
{
    // 1. 获取地图边界
    const QRectF& bounds = m_gameMap->bounds();

    // 2. 定义敌人的属性
    qreal enemyWidth = 30.0;
    qreal enemyHeight = 30.0;
    qreal enemySpeed = 100.0;
    int enemyHp = 1;

    // 3. 计算生成位置

    // 3a. 计算 X 轴的最小、最大值和范围
    qreal minX = bounds.left() + enemyWidth / 2.0;
    qreal maxX = bounds.right() - enemyWidth / 2.0;
    qreal rangeX = 0.0; // X 轴的范围

    if (minX < maxX) {
        rangeX = maxX - minX;
    }

    // vvv 这是解决问题的关键 vvv

    // 3b. 使用 generateDouble() 来获取一个 [0, 1) 之间的随机因子
    //     这个函数没有重载，没有歧义！
    qreal randomFactor = QRandomGenerator::global()->generateDouble();

    // 3c. 手动计算 spawnX
    qreal spawnX = minX + (randomFactor * rangeX);

    // ^^^ 修改结束 ^^^

    // Y 轴
    qreal spawnY = bounds.top() - enemyHeight / 2.0;

    // 4. 创建敌人
    auto enemy = std::make_unique<Enemy>(
        spawnX,
        spawnY,
        enemyWidth,
        enemyHeight,
        m_gameMap.get(),
        DEFAULT_DELTA_TIME_MS,
        enemySpeed,
        enemyHp
        );

    // 5. 将敌人添加到管理器
    m_entityManager->addEnemy(std::move(enemy));
}

// --- 步骤 1.b：确保 Player 可以接收输入 ---
// 你需要在 Player.h 中添加一个 public 函数:
// void setHorizontalDirection(qreal xDir) { m_direction.setX(xDir); }

// --- 键盘输入处理 ---
void GameWorldItem::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_moveLeft = true;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_moveRight = true;

        // vvv 新增：处理空格键 vvv
    } else if (event->key() == Qt::Key_Space) {
        // 检查射击冷却是否已过
        if (m_shootCooldownCounter <= 0) {
            playerShoot(); // 调用射击函数
            m_shootCooldownCounter = SHOOT_COOLDOWN_MS; // 重置冷却时间
        }
        // ^^^ 新增结束 ^^^

    }

    updatePlayerDirection();
}

void GameWorldItem::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_moveLeft = false;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_moveRight = false;
    }

    updatePlayerDirection();
}

// 根据按键状态更新玩家的方向
void GameWorldItem::updatePlayerDirection()
{
    if (!m_player) return; // 确保玩家仍然存在

    if (m_moveLeft && !m_moveRight) {
        m_player->setHorizontalDirection(-1.0); // 向左
    } else if (m_moveRight && !m_moveLeft) {
        m_player->setHorizontalDirection(1.0);  // 向右
    } else {
        m_player->setHorizontalDirection(0.0);  // 停止
    }
}

void GameWorldItem::playerShoot()
{
    if (!m_player) {
        // 如果玩家不存在 (例如刚被摧毁)，则不执行任何操作
        return;
    }

    // --- 1. 定义子弹属性 (你可以随意调整) ---
    qreal bulletWidth = 5.0;   // 子弹宽度
    qreal bulletHeight = 15.0; // 子弹高度
    qreal bulletSpeed = 400.0; // 子弹速度 (像素/秒)
    int bulletDamage = 1;      // 子弹伤害
    QPointF bulletDir(0, -1);  // 方向 (0, -1) 表示正上方

    // --- 2. 计算子弹的起始位置 ---
    // 我们从玩家的“头顶”中心发射

    const QRectF& playerBox = m_player->getBoundingBox();
    QPointF playerPos = m_player->getPosition(); // 玩家的中心点

    // 玩家的 X 坐标
    qreal spawnX = playerPos.x();
    // 玩家的 Y 坐标 (中心) 减去半个玩家高度 (玩家头顶) 再减去半个子弹高度 (子弹中心)
    qreal spawnY = playerPos.y() - (playerBox.height() / 2.0) - (bulletHeight / 2.0);

    // --- 3. 创建子弹 ---
    // (使用你 Bullet.h 中定义的构造函数)
    auto bullet = std::make_unique<Bullet>(
        spawnX,
        spawnY,
        bulletWidth,
        bulletHeight,
        m_gameMap.get(),
        DEFAULT_DELTA_TIME_MS,
        bulletSpeed,
        bulletDir,
        bulletDamage
        );

    // --- 4. 将子弹添加到管理器 ---
    m_entityManager->addBullet(std::move(bullet));
}
