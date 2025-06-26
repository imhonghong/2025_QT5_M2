#include "Coin.h"
#include "Mario.h"

Coin::Coin(int x, int y)
    : Item(x, y, 50, 50), visible(true),
      coinPixmap(":/item/data/item/coin.png") {}

void Coin::draw(QPainter& painter, int scrollX) {
    if (visible) {
        painter.drawPixmap(x - scrollX, y, width, height, coinPixmap);
    }
}

bool Coin::checkCollision(const Mario& mario) const {
    if (!visible) return false;

    QRect marioRect(mario.getX(), mario.getY(), mario.getWidth(), mario.getHeight());
    QRect coinRect(x, y, width, height);
    return marioRect.intersects(coinRect);
}

void Coin::setVisible(bool v) {
    visible = v;
}

bool Coin::isVisible() const {
    return visible;
}

void Coin::update() {
    // 不需要做任何事
}
