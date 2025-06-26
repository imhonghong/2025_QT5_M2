#include "Pipe.h"

Pipe::Pipe(int x, int y, int w, int h)
    : Brick(x, y) {
    width = w;
    height = h;
    pipePixmap = QPixmap(":/brick/data/brick/water pipe.png");  // 確保這張圖存在
}

void Pipe::draw(QPainter& painter, int scrollX) {
    painter.drawPixmap(x - scrollX, y, width, height, pipePixmap);
}

int Pipe::onHitFromBelow() {
    return 0;  // 無作用
}
