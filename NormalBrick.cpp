#include "NormalBrick.h"
#include "GameStageWidget.h"
#include <QDebug>

NormalBrick::NormalBrick(int x, int y, int coinLimit)
    : Brick(x, y, 50, 50), coinsLeft(coinLimit), isDepleted(false),
      normalPixmap(":/brick/data/brick/normal brick.png"),
      stonePixmap(":/brick/data/brick/stone brick.png") {}

void NormalBrick::draw(QPainter& painter, int scrollX) {
    if (isDepleted) {
        painter.drawPixmap(x - scrollX, y, width, height, stonePixmap);
    } else {
        painter.drawPixmap(x - scrollX, y, width, height, normalPixmap);
    }
}

void NormalBrick::onHitFromBelow() {
    if (isDepleted) return;

    if (coinsLeft > 0) {
        coinsLeft--;
        qDebug() << "Coin from normal brick! Remaining:" << coinsLeft;
    }

    if (coinsLeft == 0) {
        isDepleted = true;
        qDebug() << "Normal brick is now depleted!";
    }
}
