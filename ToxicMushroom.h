#pragma once
#include <QPainter>
#include <QTimer>
#include "Mario.h"

class ToxicMushroom : public QObject {
    Q_OBJECT
public:
    ToxicMushroom(int x, int y, QObject* parent = nullptr);

    void update();
    void draw(QPainter& painter, int scrollX);
    QRect getRect() const;

    bool checkMarioCollision( Mario& mario);
    void checkFireballCollision(const QRect& fireballRect, bool& hit);
    void checkBlockCollision(const QRect& blockRect);

    bool isAlive() const;

    enum State { Walking, Falling };
    int getX() const { return x; }
    int getY() const { return y; }
    void setVy (int x) { vy = x;}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    State getState() const {return state; }
    void setState(State x) {state = x ;}
    void setY(int newY) { y = newY; }


private:
    int x, y;
    int width = 50, height = 50;
    float vx;
    bool alive;

    int walkFrame = 0;          // 目前是第幾張走路圖
    int walkCounter = 0;        // 每 N 幀切換一次
    QString currentImgPath;     // 當前使用的圖片

    QTimer* deathTimer = nullptr;

    State state = Walking;
    double vy = 0;

};
