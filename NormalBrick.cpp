#include "NormalBrick.h"
#include "GameStageWidget.h"
#include <QDebug>

NormalBrick::NormalBrick(int x, int y, int coinLimit)
    : Brick(x, y, 50, 50),
      normalPixmap(":/brick/data/brick/normal brick.png"),
      stonePixmap(":/brick/data/brick/stone brick.png"),
      coinsLeft(coinLimit),
      isDepleted(false) {}


void NormalBrick::draw(QPainter& painter, int scrollX) {
    if (isDepleted) {
        painter.drawPixmap(x - scrollX, y, width, height, stonePixmap);
    } else {
        painter.drawPixmap(x - scrollX, y, width, height, normalPixmap);
    }
}

int NormalBrick::onHitFromBelow() {
    if (isDepleted) return 0;

    if (coinsLeft > 0) {
        coinsLeft--;
        if (coinsLeft == 0) isDepleted = true;
        qDebug() << "Coin from normal brick! Remaining:" << coinsLeft;
        return 1;  // 回傳 1 分
    }
    else if (coinsLeft == 0) {
        qDebug() << "hit normal brick" ;
        isDepleted = true;
    }
    return 0;
}
