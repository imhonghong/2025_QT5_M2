#pragma once
#include <QPainter>

class Mario;  // 前向宣告，避免循環引用

class Item {
public:
    int x, y, width, height;

    Item(int x, int y, int w, int h)
        : x(x), y(y), width(w), height(h) {}

    virtual ~Item() {}

    virtual void draw(QPainter& painter, int scrollX) = 0;

    // 用來偵測是否與 Mario 發生接觸
    virtual bool checkCollision(const Mario& mario) const = 0;
};
