#pragma once

#include <QPainter>
#include <QPixmap>
#include "Mario.h"

class Brick {
public:
    Brick(int x, int y, int w = 50, int h = 50)
            : x(x), y(y), width(w), height(h), visible(true) {}

    virtual ~Brick() = default;

    virtual void draw(QPainter& painter, int scrollX) = 0;
    virtual int onHitFromBelow() = 0;

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    QRect getRect() const { return QRect(x, y, width, height); }

protected:
    int x, y;
    int width, height;
    bool visible = true;
};
