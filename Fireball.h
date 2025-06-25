#pragma once

#include <QPixmap>
#include <QPainter>
#include <QRect>

class Fireball {
public:
    Fireball(int startX, int startY, float vx, float vy);
    void update();
    void draw(QPainter& painter, int scrollX);
    QRect getRect() const;
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

private:
    float x, y;
    float vx, vy;
    int size = 20;
    bool alive = true;
    QPixmap pix;
};
