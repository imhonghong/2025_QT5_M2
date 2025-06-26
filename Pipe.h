#pragma once
#include "Brick.h"
#include <QPixmap>

class Pipe : public Brick {
public:
    Pipe(int x, int y, int w = 50, int h = 100);  // 預設高兩格
    void draw(QPainter& painter, int scrollX) override;
    int onHitFromBelow() override;  // 無效

private:
    QPixmap pipePixmap;
};
