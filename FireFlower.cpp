#include "FireFlower.h"
#include "Mario.h"

FireFlower::FireFlower(int x, int y)
    : Item(x, y, 50, 50), pix(":/item/data/item/Fire Flower.png") {}

void FireFlower::draw(QPainter& painter, int scrollX) {
    if (!collected) {
        painter.drawPixmap(x - scrollX, y, width, height, pix);
    }
}

bool FireFlower::checkCollision(const Mario& mario) const {
    if (collected) return false;

    QRect flowerRect(x, y, width, height);
    QRect marioRect = mario.getRect();
    return flowerRect.intersects(marioRect);
}
