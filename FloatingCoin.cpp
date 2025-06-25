#include "FloatingCoin.h"
#include <QPainter>

FloatingCoin::FloatingCoin(int x, int y)
    : x(x), y(y), elapsedMs(0), coinPixmap(":/item/data/item/coin.png") {
    timer.start();
}

void FloatingCoin::update() {
    elapsedMs = timer.elapsed();
}

void FloatingCoin::draw(QPainter& painter, int scrollX) {
    if (elapsedMs < animTime) {
        int dy = -elapsedMs / 4;  // 每 5ms 上升 1 像素，最多約 -100
        painter.drawPixmap(x - scrollX, y + dy, 50, 50, coinPixmap);
    }
}

bool FloatingCoin::isDone() const {
    return elapsedMs >= animTime;  // 動畫時間結束（0.5 秒）
}
