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
    QRectF mapBounds(0, 0, mapWidth, mapHeight);
    qreal goalLine = mapHeight - 80.0;
    m_gameMap = std::make_unique<GameMap>(mapBounds, goalLine);

    // --- 2. 创建你的 EntityManager ---
    m_entityManager = std::make_unique<EntityManager>(m_gameMap.get());

    // --- 3. 创建并添加玩家 ---
    qreal playerX = m_gameMap->bounds().center().x();
    qreal playerY = m_gameMap->goalLineY() + 30; // 与 Player::tick 中的逻辑一致
    auto player = std::make_unique<Player>(playerX, playerY, 120, 120, m_gameMap.get(), DEFAULT_DELTA_TIME_MS);
    m_player = m_entityManager->addPlayer(std::move(player));

    // --- 4. 启动游戏循环 ---
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &GameWorldItem::gameTick);
    m_gameTimer->start(DEFAULT_DELTA_TIME_MS);

    // --- 5. 允许此 Item 接收键盘焦点 ---
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

GameWorldItem::~GameWorldItem()
{
    // unique_ptr 自动清理
}

QRectF GameWorldItem::boundingRect() const
{
    return m_gameMap->bounds();
}

void GameWorldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // 1. 绘制所有实体
    m_entityManager->drawAll(painter);

    // 2. 绘制顶部“分数”
    painter->save();
    QFont scoreFont;
    scoreFont.setFamily("Microsoft YaHei");
    scoreFont.setPointSize(16);
    scoreFont.setBold(true);
    painter->setFont(scoreFont);

    QString scoreText = QString("分数: %1").arg(m_entityManager->getTotalScore());

    painter->setPen(QColor(0, 0, 0));
    painter->drawText(QPointF(m_gameMap->bounds().width()/2 - 100 + 1, 30 + 1), scoreText);

    painter->setPen(QColor(255, 255, 255));
    painter->drawText(QPointF(m_gameMap->bounds().width()/2 - 100, 30), scoreText);
    painter->restore();
}

void GameWorldItem::gameTick()
{
    if (!m_player) return;

    // 1) 自动冷却与射击
    m_currentCooldown -= m_player->getDeltaTime();
    if (m_currentCooldown < 0) m_currentCooldown = 0;
    if (m_currentCooldown == 0) {
        playerShoot();
        m_currentCooldown = m_fireRate;
    }

    // 2) 刷怪计时
    m_enemySpawnCounter += DEFAULT_DELTA_TIME_MS;
    if (m_enemySpawnCounter >= ENEMY_SPAWN_INTERVAL_MS) {
        spawnEnemy();
        m_enemySpawnCounter = 0;
    }

    // 3) 运行所有游戏逻辑并重绘
    m_entityManager->tickAll();
    update();
}

void GameWorldItem::spawnEnemy()
{
    // 敌人字典（含名字、速度、体型、是否光环等）
    auto enemyBook = getEnemyBook();
    if (enemyBook.empty()) return;

    // —— 权重抽样（保持原有逻辑）——
    int totalWeight = 0;
    QVector<int> weightCumulative;
    for (const auto& spec : enemyBook) {
        totalWeight += spec.wight;     // 注意：字段名是 wight
        weightCumulative.append(totalWeight);
    }
    int randomValue = QRandomGenerator::global()->bounded(totalWeight);
    int targetIndex = 0;
    for (int i = 0; i < weightCumulative.size(); ++i) {
        if (randomValue < weightCumulative[i]) { targetIndex = i; break; }
    }
    const EnemySpec& spec = enemyBook[targetIndex];

    // —— 位置与体型 ——
    const QRectF& bounds = m_gameMap->bounds();
    qreal enemyWidth  = spec.body.width();
    qreal enemyHeight = spec.body.height();

    qreal minX = bounds.left() + enemyWidth / 2.0;
    qreal maxX = bounds.left() + bounds.width();
    qreal rangeX = maxX - minX;
    qreal spawnX = minX + (QRandomGenerator::global()->generateDouble() * rangeX);
    qreal spawnY = bounds.top() - enemyHeight / 2.0;

    // —— 根据类型创建（光环怪用 Aura 子类）——
    std::unique_ptr<Enemy> enemy;
    if (spec.kind == EnemyKind::Penguin) {
        enemy = std::make_unique<DefenseAuraEnemy>(
                spawnX, spawnY, enemyWidth, enemyHeight,
                m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
                spec.speed, spec.hp,
                spec.auraRadius, spec.auraDefenceAdd,
                spec.kind,
                spec.animationFrames
        );
    } else if (spec.kind == EnemyKind::Xiaoyan) {
        enemy = std::make_unique<SpeedAuraEnemy>(
                spawnX, spawnY, enemyWidth, enemyHeight,
                m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
                spec.speed, spec.hp,
                spec.auraRadius, spec.auraSpeedMult,
                spec.kind,
                spec.animationFrames
        );
    } else {
        enemy = std::make_unique<Enemy>(
                spawnX, spawnY, enemyWidth, enemyHeight,
                m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
                spec.speed, spec.hp,
                spec.kind,
                spec.animationFrames
        );
    }

    // —— 精英/BOSS 属性 ——
    enemy->setElite(spec.elite);
    enemy->setBoss(spec.boss);

    // ====== ★ 核心改动：为不同“角色”设置专属命中框（显示矩形之外的真实判定） ======
    // 说明：
    // 1) 命中框以实体中心为基准（Entity 已提供 setHitRect & hitBox）
    // 2) 这里的系数是经验好用值：魔丸更紧、光环怪适中、Boss 略紧。
    auto setCenteredHitRect = [&](qreal scaleX, qreal scaleY){
        const qreal w = enemyWidth  * scaleX;
        const qreal h = enemyHeight * scaleY;
        enemy->setHitRect(QRectF(-w/2.0, -h/2.0, w, h));
    };

    switch (spec.kind) {
        case EnemyKind::Mowan:      // 魔丸（贴图留白相对多 → 命中框更紧）
            setCenteredHitRect(0.48, 0.48);
            break;
        case EnemyKind::Kobe:       // 牢大：头肩装饰多，适度收紧
            setCenteredHitRect(0.70, 0.70);
            break;
        case EnemyKind::Wujing:     // 吴京（彩蛋）：一般收紧即可
            setCenteredHitRect(0.50, 0.50);
            break;
        case EnemyKind::Penguin:    // 企鹅（防御光环）：留白适中
            setCenteredHitRect(0.40, 0.40);
            break;
        case EnemyKind::Xiaoyan:    // 小燕（速度光环）：留白适中
            setCenteredHitRect(0.40, 0.40);
            break;
        case EnemyKind::Songhao:    // 宋浩（彩蛋）：一般收紧
            setCenteredHitRect(0.40, 0.40);
            break;
        case EnemyKind::NJUboss:    // BOSS 体型大：略紧但不苛刻
            setCenteredHitRect(0.68, 0.68);
            break;
        default:
            setCenteredHitRect(0.75, 0.75); // 兜底
            break;
    }
    // ====== ★ 改动结束 ======

    // 加入管理器
    m_entityManager->addEnemy(std::move(enemy));
}

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

void GameWorldItem::updatePlayerDirection()
{
    if (!m_player) return;
    if (m_moveLeft && !m_moveRight) {
        m_player->setHorizontalDirection(-1.0);
    } else if (m_moveRight && !m_moveLeft) {
        m_player->setHorizontalDirection(1.0);
    } else {
        m_player->setHorizontalDirection(0.0);
    }
}

void GameWorldItem::playerShoot()
{
    if (!m_player) return;

    const QPointF playerPos = m_player->getPosition();
    const qreal bulletSize   = 80;
    const qreal bulletSpeed  = 600;
    const QPointF bulletDir(0, -1);
    const int bulletDamage   = 1;

    auto bullet = std::make_unique<Bullet>(
            playerPos.x(), playerPos.y(),
            bulletSize, bulletSize,
            m_entityManager->map(),
            m_player->getDeltaTime(),
            bulletSpeed,
            bulletDir,
            bulletDamage
    );
    m_entityManager->addBullet(std::move(bullet));
}
