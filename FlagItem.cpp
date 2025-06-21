#include "FlagItem.h"
#include "Mario.h"

FlagItem::FlagItem(int x, int y)
    : Item(x, y, 50, 100)  // 寬高設為旗杆大小
{
    flagPixmap = QPixmap(":/item/data/item/flag.png").scaled(30, flagHeight);
    polePixmap = QPixmap(":/item/data/item/flag-pole.png").scaled(10, poleHeight);
}

void FlagItem::draw(QPainter& painter, int scrollX) {
    int drawX = x - scrollX;

    // 畫旗杆
    painter.drawPixmap(drawX + 20, y - poleHeight, polePixmap);

    // 畫旗子（旗子滑動時從旗杆頂部往下）
    painter.drawPixmap(drawX, y - poleHeight + flagYOffset, flagPixmap);
}

bool FlagItem::checkCollision(const Mario& mario) const {
    if (activated) return false;

    int mx = mario.getX();
    int my = mario.getY();
    int mw = mario.getWidth();
    int mh = mario.getHeight();

    // 簡單 AABB 判斷
    return !(mx + mw < x || mx > x + width || my + mh < y - poleHeight || my > y);
}

void FlagItem::activate() {
    activated = true;
}

void FlagItem::update() {
    if (!activated || done) return;

    flagYOffset += flagSpeed;
    if (flagYOffset >= poleHeight - flagHeight) {
        flagYOffset = poleHeight - flagHeight;
        done = true;
    }
}

bool FlagItem::isActivated() const {
    return activated;
}

bool FlagItem::isDone() const {
    return done;
}
