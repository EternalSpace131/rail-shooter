//
// Created by Morgan on 25-10-14.
//

#ifndef GAMEMAP_H
#define GAMEMAP_H

#include<QRectF>

class GameMap{
public:
  explicit GameMap(const QRectF& bounds, qreal goalLineY):m_bounds(bounds),m_goalLineY(goalLineY){};

  [[nodiscard]]const QRectF& bounds() const;//获取边界

  [[nodiscard]]qreal goalLineY() const;//获取线的坐标

  [[nodiscard]]bool contains(const QRectF& r) const;//简单的包容关系判断
  // 新增：判断实体的世界坐标命中框是否越过底线
  [[nodiscard]]bool isBelowGoalLine(const QRectF& worldHitBox) const {
    return worldHitBox.bottom() >= m_goalLineY;
  }

private:
  QRectF m_bounds;//边界
  qreal m_goalLineY;//保卫线的坐标
};


#endif //GAMEMAP_H
