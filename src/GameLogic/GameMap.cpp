//
// Created by Morgan on 25-10-14.
//
#include "GameMap.h"

const QRectF &GameMap::bounds() const {
    return m_bounds;
}

qreal GameMap::goalLineY() const {
    return m_goalLineY;
}

bool GameMap::contains(const QRectF& r) const {
    return m_bounds.contains(r);
}


