#include "gameworlditem.h"
#include "Manager/EntityManager.h"
#include "Gamelogic/GameMap.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Bullet.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QRandomGenerator>
#include "EnemySpec.h"
#include "AuraEnemies.h"
#include"RewardCrate.h"
#include<QtMath>
#include<QGraphicsTextItem>
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>
#include<QEasingCurve>
#include <QGraphicsScene>

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

    //绑定奖励回调（强化系统入口）
    bindRewardCallback();

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

// =====强化系统实现=====
void GameWorldItem::bindRewardCallback() {
    if(!m_entityManager) {
        return;
    }
    // 当奖励箱被击破，EntityManager::grantReward(amount) 会触发这里
    m_entityManager->setOnReward([this](int amount) {
        Q_UNUSED(amount);
        grantRandomUpgrade();
    });
}

void GameWorldItem::updateTimedUpgrades(int deltaMs) {
    if(m_spreadTimeLeftMs>0) {
        m_spreadTimeLeftMs -= deltaMs;
        if(m_spreadTimeLeftMs<=0) {
            m_spreadTimeLeftMs = 0;
            m_spreadLevel=0;
            showUpgradeToast(QStringLiteral("散射结束"));
        }
    }
}


void GameWorldItem::grantRandomUpgrade() {
    //0:射速;1:伤害;2:散射;
    std::uniform_int_distribution<int> dist(0,2);
    const int pick=dist(m_rng);
    switch (pick) {
    case 0: {
        const int old=m_fireRate;
        m_fireRate=std::max(120,int(m_fireRate*0.85));
        showUpgradeToast(QStringLiteral("强化：射速提升  (%1→%2 ms)").arg(old).arg(m_fireRate));
        break;
    }
    case 1: {
        ++m_bulletDamage;
        showUpgradeToast(QStringLiteral("强化：伤害 +1  (当前=%1)").arg(m_bulletDamage));
        break;
    }
    case 2: {
        constexpr int kSpreadDurationMs=8000;//8秒
        m_spreadLevel=std::min(m_spreadLevel+1,3);
        m_spreadTimeLeftMs=kSpreadDurationMs; //刷新倒计时
        showUpgradeToast(QStringLiteral("强化：散射 +1  (当前=%1，持续%2s)")
                             .arg(m_spreadLevel).arg(kSpreadDurationMs/1000));
        break;
    }
    default:break;
    }
}

void GameWorldItem::showUpgradeToast(const QString &text) {
    //创建文本项
    auto* label=new QGraphicsTextItem(text);
    QFont f(QStringLiteral("Microsoft YaHei"));
    f.setPointSize(14);
    f.setWeight(QFont::Bold);
    label->setFont(f);
    label->setDefaultTextColor(QColor(255,240,120));
    label->setZValue(1000);

    //放到玩家头顶
    const QPointF p=m_player->getPosition();
    const QRectF br=label->boundingRect();
    const qreal startX=p.x()-br.width()/2.0;
    const qreal startY=p.y()-100.0;
    label->setPos(startX,startY);

    scene()->addItem(label);

    // 做一个“向上漂 + 淡出”的并行动画
    // 动 y：上移 40px
    auto* animPos = new QPropertyAnimation(label,"y");
    animPos->setDuration(700);
    animPos->setStartValue(label->y());
    animPos->setEndValue(label->y()-40.0);
    animPos->setEasingCurve(QEasingCurve::OutQuad);

    //动透明度:1.0->0.0
    auto* animOpacity=new QPropertyAnimation(label,"opacity");
    animOpacity->setDuration(700);
    animOpacity->setStartValue(1.0);
    animOpacity->setEndValue(0.0);

    auto* group=new QParallelAnimationGroup(label);//以label为父，方便统一销毁
    group->addAnimation(animPos);
    group->addAnimation(animOpacity);

    //结束后自动删除
    QObject::connect(group, &QParallelAnimationGroup::finished, label,[label]() {
        if(label->scene()) {
            label->scene()->removeItem(label);
            label->deleteLater();
        }
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
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

    m_entityManager->tickAll();

    // 2) 立即检测是否有敌人越界 → 优先触发游戏结束
    if (m_entityManager->hasEnemyBelowGoalLine(m_gameMap.get())) {
        onGameOver();
        return;
    }

    // 1) 自动冷却与射击
    m_currentCooldown -= m_player->getDeltaTime();
    if (m_currentCooldown < 0) m_currentCooldown = 0;
    if (m_currentCooldown == 0) {
        playerShoot();
        m_currentCooldown = m_fireRate;
    }

    //限时强化倒计时
    updateTimedUpgrades(m_player->getDeltaTime());

    // --- 【新增或修改：检查 BOSS 状态】 ---

    // 更新 BOSS 是否在场的状态
    // 如果 EntityManager 中没有 BOSS 且我们之前认为 BOSS 在场，则 BOSS 被击败。
    if (m_isBossOnScreen && !m_entityManager->hasBoss()) {
        m_isBossOnScreen = false;
        showUpgradeToast(QStringLiteral("BOSS 击败！"));
    }

    // 2) 刷怪计时
    // 如果 BOSS 在场，或者当前分数是 BOSS 分数点（防止 BOSS 刷出前刷小怪），暂停小怪生成。
    const int currentScore = m_entityManager->getTotalScore();
    const bool isBossScorePoint = (currentScore > 0 && currentScore % 1000 <= 90);

    if (m_isBossOnScreen) {
        // 如果 BOSS 在场或即将生成 BOSS，则重置计时器，暂停小怪生成
        m_enemySpawnCounter = 0;
    } else {
        // 正常刷小怪计时
        m_enemySpawnCounter += DEFAULT_DELTA_TIME_MS;
        if (m_enemySpawnCounter >= ENEMY_SPAWN_INTERVAL_MS) {
            spawnEnemy();
            m_enemySpawnCounter = 0;
        }
    }
    // ------------------------------------

    // 3) 运行所有游戏逻辑并重绘
    update();
}

void GameWorldItem::playSpawnSound(const QString& soundPath)
{
    if (soundPath.isEmpty())
        return;

    QSoundEffect* effect = new QSoundEffect(this);
    effect->setSource(QUrl(soundPath));
    effect->setVolume(0.8f);
    effect->play();

    connect(effect, &QSoundEffect::playingChanged, effect, [effect]() {
        if (!effect->isPlaying())
            effect->deleteLater();
    });
}


void GameWorldItem::spawnEnemy()
{
    // 获取当前游戏分数
    const int currentScore = m_entityManager->getTotalScore();
    //播放出场音乐的概率
    double playMusic = 0.1;

    // --- 1. 根据分数计算当前的难度等级 ---
    // 每 300 分提升一个难度等级
    const int scoreLevel = currentScore / 300;

    // 敌人字典（含名字、速度、体型、是否光环等）
    auto enemyBook = getEnemyBook();
    if (enemyBook.empty()) return;

    const bool isBossScorePoint = (currentScore > 900 && currentScore % 1000 <= 90 );

    // 只有当：是BOSS分数点 AND BOSS当前不在场 AND 这个分数的BOSS还没刷过
    static const int kBossStep = 1000;
    if (!m_isBossOnScreen && currentScore >= m_lastBossScore+kBossStep) {

        // 1. 在字典中查找 NJUboss 的数据
        const EnemySpec* bossSpecPtr = nullptr;
        for (const auto& s : enemyBook) {
            if (s.kind == EnemyKind::NJUboss) { // 找到 BOSS 类型
                bossSpecPtr = &s;
                break;
            }
        }

        // 2. 如果找到了 BOSS 数据，开始生成
        if (bossSpecPtr) {
            const EnemySpec& bossSpec = *bossSpecPtr;

            // 计算居中位置
            const QRectF& bounds = m_gameMap->bounds();
            qreal enemyWidth  = bossSpec.body.width();
            qreal enemyHeight = bossSpec.body.height();
            qreal spawnX = bounds.center().x();
            qreal spawnY = bounds.top() - enemyHeight / 2.0;

            // 创建实体
            std::unique_ptr<Enemy> boss = std::make_unique<Enemy>(
                spawnX, spawnY, enemyWidth, enemyHeight,
                m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
                bossSpec.speed, bossSpec.hp,
                bossSpec.kind,
                bossSpec.animationFrames
                );

            boss->setElite(true);
            boss->setBoss(true);

            boss->setObjectName("nju(BOSS)");

            // 设置 BOSS 专属命中框 (这里直接用您之前代码里的参数 0.68)
            qreal w = enemyWidth * 0.68;
            qreal h = enemyHeight * 0.68;
            boss->setHitRect(QRectF(-w/2.0, -h/2.0, w, h));

            // 加入管理器
            m_entityManager->addEnemy(std::move(boss));

            // 更新状态：BOSS 已在场，记录本次分数
            m_isBossOnScreen = true;
            m_lastBossScore += kBossStep;

            return;
        }
    }
    // --- 2. 动态调整权重（核心改动） ---
    // 根据当前难度等级，只考虑“已解锁”的怪物，并提升它们的权重

    QVector<EnemySpec> availableEnemies;
    for (auto& spec : enemyBook) {
        // 【假设：minScoreToAppear 是 EnemySpec 中的一个字段】
        // 只有当怪物所需的最低分数等级低于或等于当前分数等级时，才加入候选列表
        if (spec.minScoreToAppear <= scoreLevel) {
            // 示例：动态增强权重
            // 让更高级的怪物有更高的出现几率（例如，每多一个等级，权重加倍）
            int adjustedWeight = spec.wight * (1 + (scoreLevel - spec.minScoreToAppear));

            // 创建一个新的 Spec 副本用于抽样，只修改权重
            EnemySpec weightedSpec = spec;
            weightedSpec.wight = adjustedWeight;
            availableEnemies.append(weightedSpec);
        }
    }

    if (availableEnemies.empty()) {
        // 如果没有可生成的怪物，直接返回（理论上不应该发生）
        return;
    }

    // —— 权重抽样（使用调整后的列表）——
    int totalWeight = 0;
    QVector<int> weightCumulative;
    for (const auto& spec : availableEnemies) {
        totalWeight += spec.wight;
        weightCumulative.append(totalWeight);
    }

    int randomValue = QRandomGenerator::global()->bounded(totalWeight);
    int targetIndex = 0;
    for (int i = 0; i < weightCumulative.size(); ++i) {
        if (randomValue < weightCumulative[i]) { targetIndex = i; break; }
    }
    // 获取最终抽中的怪物规格
    const EnemySpec& spec = availableEnemies[targetIndex];

    // 奖励箱作为“魔丸”变体的概率（可调）
    // 奖励箱的概率可以随分数等级降低，减少玩家在后期获得强化频率
    // 例如：等级 0 时 100%，等级 3 时 40%

    double kCrateAsMowanChance = 0.3 - std::min(0.15, scoreLevel * 0.05); // 20%
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
    if(spec.kind == EnemyKind::Mowan && QRandomGenerator::global()->generateDouble() < kCrateAsMowanChance) {
        const qreal crateSpeed=spec.speed;
        const int crateHp=1;
        const int rewardPts=20;

        enemy = std::make_unique<RewardCrate>(
            spawnX, spawnY,
            enemyWidth, enemyHeight,
            m_gameMap.get(), DEFAULT_DELTA_TIME_MS,
            crateSpeed, crateHp, rewardPts,
            spec.kind,
            spec.animationFrames
            );

    }else if (spec.kind == EnemyKind::Penguin) {
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
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::Kobe:       // 牢大：头肩装饰多，适度收紧
        setCenteredHitRect(0.70, 0.70);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::Wujing:     // 吴京（彩蛋）：一般收紧即可
        setCenteredHitRect(0.50, 0.50);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::Penguin:    // 企鹅（防御光环）：留白适中
        setCenteredHitRect(0.40, 0.40);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::Xiaoyan:    // 小燕（速度光环）：留白适中
        setCenteredHitRect(0.40, 0.40);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::Songhao:    // 宋浩（彩蛋）：一般收紧
        setCenteredHitRect(0.40, 0.40);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
        break;
    case EnemyKind::NJUboss:    // BOSS 体型大：略紧但不苛刻
        setCenteredHitRect(0.68, 0.68);
        if (!spec.spawnSound.isEmpty() && QRandomGenerator::global()->generateDouble() < playMusic){
            playSpawnSound(spec.spawnSound);
        }
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

void GameWorldItem::playerShoot(){
    if (!m_player) return;

    // const QPointF playerPos = m_player->getPosition();
    // const qreal bulletSize   = 80;
    // const qreal bulletSpeed  = 600;
    // const QPointF bulletDir(0, -1);
    // const int bulletDamage   = 1;
    //
    // auto bullet = std::make_unique<Bullet>(
    //         playerPos.x(), playerPos.y(),
    //         bulletSize, bulletSize,
    //         m_entityManager->map(),
    //         m_player->getDeltaTime(),
    //         bulletSpeed,
    //         bulletDir,
    //         bulletDamage
    // );
    // m_entityManager->addBullet(std::move(bullet));

    const QPointF playerPos = m_player->getPosition();
    const qreal bulletSize = 80;
    const qreal bulletSpeed  = 600;
    const int dmg=m_bulletDamage;

    //基础方向：正上
    auto makeDir = [](qreal deg) {
        const qreal rad = qDegreesToRadians(deg);
        return QPointF(qSin(rad)*1.0, -qCos(rad)*1.0);
    };

    // 按散射等级发射多枚子弹：level=0 → 0°；1 → 0/±6°；2 → 0/±6/±12°；3 → 0/±6/±12/±18°
    QVector<qreal> angles;
    angles.push_back(0.0);
    const qreal step = 6.0;
    for(int i=1; i<=m_spreadLevel;i++) {
        angles.push_back(+i*step);
        angles.push_back(-i*step);
    }

    for(qreal a:angles) {
        auto b = std::make_unique<Bullet>(
            playerPos.x(), playerPos.y(),
            bulletSize, bulletSize,
            m_entityManager->map(),
            m_player->getDeltaTime(),
            bulletSpeed,
            makeDir(a),
            dmg
            );
        m_entityManager->addBullet(std::move(b));
    }
}

void GameWorldItem::onGameOver() {
    // 停止游戏循环（安全检查）
    if (m_gameTimer && m_gameTimer->isActive()) {
        m_gameTimer->stop();
    }

    // 半透明背景（突出文字，不设层级，默认在底层）
    QGraphicsRectItem* background = new QGraphicsRectItem(boundingRect(), this);
    background->setBrush(QColor(0, 0, 0, 150)); // 黑色半透明
    background->setPen(Qt::NoPen); // 仅去掉边框，不涉及文字描边

    // 游戏结束标题（无描边，无层级设置）
    QGraphicsTextItem* titleText = new QGraphicsTextItem("游戏结束！", this);
    QFont titleFont("Microsoft YaHei", 30, QFont::Bold);
    titleText->setFont(titleFont);
    titleText->setDefaultTextColor(Qt::white); // 白色标题，无黑色描边

    // 最终分数（无描边，无层级设置）
    int finalScore = m_entityManager ? m_entityManager->getTotalScore() : 0;
    QGraphicsTextItem* scoreText = new QGraphicsTextItem(QString("最终分数：%1").arg(finalScore), this);
    QFont scoreFont("Microsoft YaHei", 22, QFont::Bold);
    scoreText->setFont(scoreFont);
    scoreText->setDefaultTextColor(QColor(50, 180, 255)); // 亮蓝色分数

    // 居中布局（仅保留核心位置计算）
    QRectF titleRect = titleText->boundingRect();
    QRectF scoreRect = scoreText->boundingRect();

    // 整体垂直居中，标题在上，分数在下，间距20px
    qreal totalHeight = titleRect.height() + 20 + scoreRect.height();
    qreal startY = boundingRect().center().y() - totalHeight / 2;

    // 水平居中
    titleText->setPos(
        boundingRect().center().x() - titleRect.width() / 2,
        startY
        );
    scoreText->setPos(
        boundingRect().center().x() - scoreRect.width() / 2,
        startY + titleRect.height() + 20
        );
}
