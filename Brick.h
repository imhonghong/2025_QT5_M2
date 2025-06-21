#pragma once

#include <QPainter>
#include <QPixmap>

class Brick {
public:
    int x, y;
    int width, height;
    bool visible = true;

    virtual ~Brick() = default;

    virtual void draw(QPainter& painter, int scrollX) = 0;
    virtual void onHitFromBelow() = 0;
};
