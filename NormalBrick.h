#pragma once
#include "Brick.h"
#include <QPixmap>

class NormalBrick : public Brick {
public:
    NormalBrick(int x, int y, int coinLimit = 0);  // coinLimit=0 表示沒有金幣

    void draw(QPainter& painter, int scrollX) override;
    void onHitFromBelow() override;

    int getCoinsLeft() const { return coinsLeft; }

private:
    QPixmap normalPixmap;
    QPixmap stonePixmap;

    int coinsLeft;      // 還剩幾個金幣
    bool isDepleted;    // true = 已變成石磚
};
