#include "Player.h"
#include <QDebug>
#include <GameLogic/GameMap.h>

Player::Player(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime)
        :Entity(x, y, width, height,map, deltaTime){
    m_speed=220;
    m_direction=QPointF(0,0);

    loadAnimationFrames(); // 加载动画帧

    // ✅ 独立小碰撞箱（不影响图片大小）
    qreal hitW = m_width * 0.55;
    qreal hitH = m_height * 0.70;
    setHitRect(QRectF(-hitW/2, -hitH/2, hitW, hitH));

    if (!m_animationFrames.isEmpty()) {
        connect(&m_animationTimer, &QTimer::timeout, this, [this]() {
            m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animationFrames.size();
        });
        m_animationTimer.start(100);
    } else {
        qWarning() << "Player 动画帧加载失败，未启动动画计时器。";
    }
}

void Player::loadAnimationFrames() {
    QStringList framePaths = {
            ":/assets/images/character/cat/cat1.png",
            ":/assets/images/character/cat/cat2.png",
            ":/assets/images/character/cat/cat3.png",
            ":/assets/images/character/cat/cat4.png"
    };

    for (const QString& path : framePaths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            qDebug() << "图片加载失败，路径：" << path;
            continue;
        }

        // ✅ 修复尺寸方向（宽，高）
        m_animationFrames.append(
                pixmap.scaled(
                        m_width, m_height,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                )
        );
    }
}

void Player::tick() {

    Entity::move();
    const QRectF& B = m_gameMapPtr->bounds();

    // ✅ 用命中箱宽度限制贴边，而不是贴图宽度
    qreal halfHitW = hitBox().width() / 2.0;

    if (m_position.x() - halfHitW < B.left())
        m_position.setX(B.left() + halfHitW);

    if (m_position.x() + halfHitW > B.right())
        m_position.setX(B.right() - halfHitW);

    // 固定 Y 轴（不允许上下移动）
    qreal laneY = m_gameMapPtr->goalLineY() + 30;
    setPosition(m_position.x(), laneY);
}

void Player::draw(QPainter *painter) {
    if (!painter || m_animationFrames.isEmpty()) return;

    const QPixmap &currentFrame = m_animationFrames[m_currentFrameIndex];
    QPointF topLeft(m_position.x() - m_width / 2, m_position.y() - m_height / 2);
    painter->drawPixmap(topLeft, currentFrame);
}
