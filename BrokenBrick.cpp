#include "BrokenBrick.h"

BrokenBrick::BrokenBrick(int x, int y)
    : Brick(x, y, 50, 50),
      brickPixmap(":/brick/data/brick/broken brick.png") {}

void BrokenBrick::draw(QPainter& painter, int scrollX) {
    if (!broken) {
        painter.drawPixmap(x - scrollX, y, width, height, brickPixmap);
    }
}

int BrokenBrick::onHitFromBelow() {
    if (!broken) {
        broken = true;
        // 可擴充動畫或音效
    }
    return 0;  // 不加分
}
