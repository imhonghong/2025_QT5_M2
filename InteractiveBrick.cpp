#include "InteractiveBrick.h"
#include <QDebug>

InteractiveBrick::InteractiveBrick(int x, int y, BrickContent content)
    : Brick(x, y), content(content)
{
    width = height = 50;
    activePixmap = QPixmap(":/brick/data/brick/box brick.png");
    usedPixmap = QPixmap(":/brick/data/brick/stone brick.png");
}

void InteractiveBrick::draw(QPainter& painter, int scrollX) {
    if (!visible) return;

    QPixmap& toDraw = used ? usedPixmap : activePixmap;
    painter.drawPixmap(x - scrollX, y, width, height, toDraw);
}

void InteractiveBrick::onHitFromBelow() {
    if (used) return;
    used = true;

    // 這裡先用 qDebug 模擬掉落物件行為
    switch (content) {
    case BrickContent::Coin:
        qDebug() << "Coin appeared at (" << x << "," << y << ")";
        break;
    case BrickContent::Mushroom:
        qDebug() << "Mushroom appeared at (" << x << "," << y << ")";
        break;
    case BrickContent::FireFlower:
        qDebug() << "Fire Flower appeared at (" << x << "," << y << ")";
        break;
    default:
        break;
    }
}
