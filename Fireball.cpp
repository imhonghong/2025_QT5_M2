#include "Fireball.h"

Fireball::Fireball(int startX, int startY, float vx, float vy)
    : x(startX), y(startY), vx(vx), vy(vy), pix(":/item/data/item/bullet.png") {}

void Fireball::update() {
    if (!alive) return;
    x += vx;
    y += vy;

    // 超出畫面就消滅
    if (x < 0 || x > 7000 || y < 0 || y > 800)
        alive = false;
}

void Fireball::draw(QPainter& painter, int scrollX) {
    if (alive)
        painter.drawPixmap((int)x - scrollX, (int)y, size, size, pix);
}

QRect Fireball::getRect() const {
    return QRect((int)x, (int)y, size, size);
}
