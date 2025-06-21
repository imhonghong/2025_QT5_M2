#include "FloorBrick.h"
#include <QDebug>

FloorBrick::FloorBrick(int xPos, int yPos) {
    x = xPos;
    y = yPos;
    width = 50;
    height = 100;
    pixmap = QPixmap(":/brick/data/brick/floor brick.png");
}

void FloorBrick::draw(QPainter& painter, int scrollX) {
    if (visible)
        painter.drawPixmap(x - scrollX, y, width, height, pixmap);
    qDebug() << "Draw floor at x=" << x << ", screenX=" << x - scrollX;
}

void FloorBrick::onHitFromBelow() {
    // Floor brick 沒有反應
}
