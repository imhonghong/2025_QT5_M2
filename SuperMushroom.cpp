#include "SuperMushroom.h"
#include "Mario.h"

SuperMushroom::SuperMushroom(int x, int y)
    : Item(x, y, 50, 50), pix(":/item/data/item/super mushroom.png") {}

void SuperMushroom::draw(QPainter& painter, int scrollX) {
    if (!collected) {
        painter.drawPixmap(x - scrollX, y, width, height, pix);
    }
}

bool SuperMushroom::checkCollision(const Mario& mario) const {
    if (collected) return false;

    QRect mushroomRect(x, y, width, height);
    QRect marioRect = mario.getRect();  // 你 Mario 類別已有 getRect()
    return mushroomRect.intersects(marioRect);
}
