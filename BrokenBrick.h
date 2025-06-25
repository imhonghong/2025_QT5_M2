#pragma once
#include "Brick.h"
#include <QPixmap>

class BrokenBrick : public Brick {
public:
    BrokenBrick(int x, int y);

    void draw(QPainter& painter, int scrollX) override;
    int onHitFromBelow() override;

    bool isBroken() const { return broken; }

private:
    QPixmap brickPixmap;
    bool broken = false;
};
