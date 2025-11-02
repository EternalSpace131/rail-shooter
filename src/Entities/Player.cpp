//
// Created by Morgan on 25-10-13.
//
#include "Player.h"
#include <QDebug>
#include <GameLogic/GameMap.h>

Player::Player(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime)
    :Entity(x, y, width, height,map, deltaTime){
    m_speed=220;
    m_direction=QPointF(0,0);

    loadAnimationFrames(); // 加载动画帧

    // 初始化动画定时器：每100ms切换一帧
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
    // 加载四张动画图片
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

        m_animationFrames.append(pixmap.scaled(
            m_height, m_width,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            ));
    }
}

void Player::tick() {
    //限制水平移动
    Entity::move();
    const QRectF& B=m_gameMapPtr->bounds();
    //保持玩家不出界
    if(m_position.x()-m_width/2<B.left()) {
        m_position.setX(B.left()+m_width/2);
    }
    if(m_position.x()+m_width/2>B.right()) {
        m_position.setX(B.right()-m_width/2);
    }
    //将玩家的Y值固定
    qreal laneY=m_gameMapPtr->goalLineY()+30;
    setPosition(m_position.x(),laneY);
}

void Player::draw(QPainter *painter) {
    // if(!painter) {
    //     return;
    // }
    // painter->fillRect(getBoundingBox(),Qt::blue);
    if (!painter || m_animationFrames.isEmpty()) return;

    const QPixmap &currentFrame = m_animationFrames[m_currentFrameIndex];
    QPointF topLeft(m_position.x() - m_width / 2, m_position.y() - m_height / 2);
    painter->drawPixmap(topLeft, currentFrame);
}

