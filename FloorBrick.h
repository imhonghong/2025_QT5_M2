#pragma once

#include "Brick.h"

class FloorBrick : public Brick {
public:
    FloorBrick(int x, int y);
    void draw(QPainter& painter, int scrollX) override;
    int onHitFromBelow() override;

private:
    QPixmap pixmap;
};
