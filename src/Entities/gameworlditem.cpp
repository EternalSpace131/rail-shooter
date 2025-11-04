#include "gameworlditem.h"
#include "Manager/EntityManager.h"
#include "Gamelogic/GameMap.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Bullet.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>
#include "EnemySpec.h"
#include "AuraEnemies.h"

GameWorldItem::GameWorldItem(qreal mapWidth, qreal mapHeight, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    m_player(nullptr),
    m_moveLeft(false),
    m_moveRight(false),
    m_enemySpawnCounter(0),
    m_shootCooldownCounter(0),
    m_fireRate(500),
    m_currentCooldown(0)
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
    qreal playerY = m_gameMap->goalLineY()+30; // 与你 Player::tick 中的逻辑一致

    auto player = std::make_unique<Player>(playerX, playerY, 120, 120, m_gameMap.get(), DEFAULT_DELTA_TIME_MS);

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

    // 1. 绘制所有游戏实体（敌人、子弹、玩家等）
    m_entityManager->drawAll(painter);

    // 2. 绘制顶部分数
    painter->save(); // 保存画笔状态

    // 初始化分数字体
    QFont scoreFont;
    scoreFont.setFamily("Microsoft YaHei"); // 字体
    scoreFont.setPointSize(16); // 字号
    scoreFont.setBold(true); // 加粗
    painter->setFont(scoreFont);

    // 分数文本（从 EntityManager 获取总分）
    QString scoreText = QString("分数: %1").arg(m_entityManager->getTotalScore());

    // 绘制文字阴影
    painter->setPen(QColor(0, 0, 0)); // 阴影颜色：黑色
    painter->drawText(
        QPointF(m_gameMap->bounds().width()/2 - 100 + 1, 30 + 1), // 阴影位置（右移1，下移1）
        scoreText
        );

    // 绘制主分数文本（白色)
    painter->setPen(QColor(255, 255, 255)); // 文字颜色：白色
    painter->drawText(
        QPointF(m_gameMap->bounds().width()/2 - 100, 30), // 主文字位置
        scoreText
        );

    painter->restore(); // 恢复画笔状态
}

// *** 关键：逻辑 ***
// QTimer 每 16ms 调用一次此函数
void GameWorldItem::gameTick()
{

    if (!m_player) return;  // 玩家为空时不处理

    // 1. 冷却时间递减（基于玩家的帧间隔时间同步）
    m_currentCooldown -= m_player->getDeltaTime();
    if (m_currentCooldown < 0) m_currentCooldown = 0;

    // 2. 冷却结束时自动发射子弹，并重置冷却
    if (m_currentCooldown == 0) {
        playerShoot();
        m_currentCooldown = m_fireRate;  // 重置为设定的发射间隔
    }

    // 更新敌人生成
    m_enemySpawnCounter += DEFAULT_DELTA_TIME_MS;
    if (m_enemySpawnCounter >= ENEMY_SPAWN_INTERVAL_MS) {
        spawnEnemy();
        m_enemySpawnCounter = 0;
    }

    // --- 3. 运行所有游戏逻辑 ---
    // (tick, 碰撞检测, 清理)
    m_entityManager->tickAll();

    // --- 3. 请求重绘 ---
    update();
}

void GameWorldItem::spawnEnemy()
{
    auto enemyBook = getEnemyBook();
    if (enemyBook.empty()) return;

    // 1. 计算总权重和权重累积列表
    int totalWeight = 0;
    QVector<int> weightCumulative; // 存储累积权重，用于区间匹配
    for (const auto& spec : enemyBook) {
        totalWeight += spec.wight; // 累加所有敌人权重（注意：原字段名是wight，非weight）
        weightCumulative.append(totalWeight);
    }

    // 2. 生成随机数
    int randomValue = QRandomGenerator::global()->bounded(totalWeight);

    // 3. 匹配随机数对应的敌人
    int targetIndex = 0;
    for (int i = 0; i < weightCumulative.size(); ++i) {
        if (randomValue < weightCumulative[i]) {
            targetIndex = i;
            break;
        }
    }
    const EnemySpec& spec = enemyBook[targetIndex];

    // 4. 生成位置、创建敌人实例逻辑
    const QRectF& bounds = m_gameMap->bounds();
    qreal enemyWidth = spec.body.width();
    qreal enemyHeight = spec.body.height();

    qreal minX = bounds.left() + enemyWidth / 2.0;
    qreal maxX = bounds.left() + bounds.width();
    qreal rangeX = maxX - minX;
    qreal randomFactorX = QRandomGenerator::global()->generateDouble();
    qreal spawnX = minX + (randomFactorX * rangeX);

    qreal spawnY = bounds.top() - enemyHeight / 2.0;

    // 根据敌人类型创建实例
    std::unique_ptr<Enemy> enemy;
    if (spec.kind == EnemyKind::Penguin) {
        enemy = std::make_unique<DefenseAuraEnemy>(
            spawnX, spawnY, enemyWidth, enemyHeight,
            m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
            spec.speed, spec.hp,
            spec.auraRadius, spec.auraDefenceAdd,
            spec.kind, // 传递敌人类型
            spec.animationFrames
            );
    } else if (spec.kind == EnemyKind::Xiaoyan) {
        enemy = std::make_unique<SpeedAuraEnemy>(
            spawnX, spawnY, enemyWidth, enemyHeight,
            m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
            spec.speed, spec.hp,
            spec.auraRadius, spec.auraSpeedMult,
            spec.kind, // 传递敌人类型
            spec.animationFrames
            );
    } else {
        enemy = std::make_unique<Enemy>(
            spawnX, spawnY, enemyWidth, enemyHeight,
            m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
            spec.speed, spec.hp,
            spec.kind, // 传递敌人类型
            spec.animationFrames
            );
    }

    // 设置精英、BOSS属性
    enemy->setElite(spec.elite);
    enemy->setBoss(spec.boss);

    // 添加到管理器
    m_entityManager->addEnemy(std::move(enemy));
}

// --- 键盘输入处理 ---
void GameWorldItem::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_moveLeft = true;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_moveRight = true;
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
        return;
    }

    const QPointF playerPos = m_player->getPosition();  // 玩家中心位置
    const qreal playerHalfHeight = m_player->getBoundingBox().height() / 2;  // 玩家半高

    const qreal bulletSize = 80;
    const qreal bulletSpeed = 600;  // 像素/秒
    const QPointF bulletDir(0, -1);  // 固定向上
    const int bulletDamage = 1;

    // 计算子弹初始位置（玩家顶部正上方)
    qreal bulletX = playerPos.x();
    qreal bulletY = playerPos.y();

    // 创建子弹并添加到实体管理器
    auto bullet = std::make_unique<Bullet>(
        bulletX, bulletY,
        bulletSize, bulletSize,
        m_entityManager->map(),
        m_player->getDeltaTime(),  // 与玩家同帧率
        bulletSpeed,
        bulletDir,
        bulletDamage
        );

    m_entityManager->addBullet(std::move(bullet));
}
