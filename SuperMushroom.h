#pragma once

#include "Item.h"
#include <QPixmap>

class SuperMushroom : public Item {
public:
    SuperMushroom(int x, int y);

    void draw(QPainter& painter, int scrollX) override;
    bool checkCollision(const Mario& mario) const override;
    void checkPipeCollision(const QRect& pipeRect);

    void update() override;

    bool isCollected() const { return collected; }
    void setCollected(bool x) { collected = x; }

    // 狀態機
    enum State { Emerging, Walking, Falling };
    State getState() const { return state; }
    void setState(State s) { state = s; }

    // 位置、速度操作
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setX(int val) { x = val; }
    void setY(int val) { y = val; }
    double getVy() const { return vy; }
    void setVy(double v) { vy = v; }

    QRect getRect() const { return QRect(x, y, width, height); }

private:
    QPixmap pix;
    bool collected = false;
    State state = Emerging;
    int emergeCounter = 0;
    int vx = 2;
    double vy = 0;

    void reverse();  // 尚未使用，可後續加入碰撞轉向邏輯
};
