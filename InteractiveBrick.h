#pragma once

#include "Brick.h"
#include <QPixmap>

enum class BrickContent {
    None,
    Coin,
    Mushroom,
    FireFlower
};

class InteractiveBrick : public Brick {
protected:
    bool used = false;
    BrickContent content;
    QPixmap activePixmap;
    QPixmap usedPixmap;

public:
    InteractiveBrick(int x, int y, BrickContent content);

    void draw(QPainter& painter, int scrollX) override;
    void onHitFromBelow() override;
};
