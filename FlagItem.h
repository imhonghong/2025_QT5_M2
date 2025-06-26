#pragma once
#include "Item.h"
#include <QPixmap>

class FlagItem : public Item {
public:
    FlagItem(int x, int y);

    void draw(QPainter& painter, int scrollX) override;
    bool checkCollision(const Mario& mario) const override;

    void activate();       // Mario 碰到後觸發動畫
    void update() override;        // 每次動畫執行旗子滑下
    bool isActivated() const;
    bool isDone() const;

    void setX(int x_pos) { x = x_pos ;}
    void setY(int y_pos) { y = y_pos ;}
    int getX() const { return x ;}
    int getY() const { return y ;}


private:
    QPixmap flagPixmap;
    QPixmap polePixmap;

    int poleHeight = 100;
    int flagHeight = 25;
    int flagYOffset = 0;
    int flagSpeed = 5;

    bool activated = false;
    bool done = false;
};
