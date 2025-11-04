//
// Created by Morgan on 25-10-22.
//
#include <QColor>
#include "Entity.h"
#include "Enemy.h"
#include "RewardCrate.h"
#include <Manager/EntityManager.h>

RewardCrate::RewardCrate(qreal x, qreal y, qreal width, qreal height, GameMap *map, int deltaTime, qreal speed, int hp, int reward,EnemyKind kind,const QStringList& animFrames)
:Enemy(x,y,width,height,map,deltaTime,speed,hp,kind,animFrames),m_reward(reward){}

void RewardCrate::draw(QPainter *painter) {
    if(!painter) {
        return;
    }
    painter->fillRect(getBoundingBox(),QColor(200,160,20));
}

void RewardCrate::onKilled(EntityManager *mgr) {
    if(mgr) {
        mgr->grantReward(m_reward);
    }
}
void RewardCrate::onLeak(EntityManager *) {}





