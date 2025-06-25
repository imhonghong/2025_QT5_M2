#include "FloorBrick.h"
#include <QDebug>

FloorBrick::FloorBrick(int xPos, int yPos)
    : Brick(xPos, yPos){
    x = xPos;
    y = yPos;
    width = 50;
    height = 100;
    pixmap = QPixmap(":/brick/data/brick/floor brick.png");
}

void FloorBrick::draw(QPainter& painter, int scrollX) {
    if (visible)
        painter.drawPixmap(x - scrollX, y, width, height, pixmap);
}

int FloorBrick::onHitFromBelow() {
    return 0;
}

