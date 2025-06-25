#pragma once

#include <QPixmap>
#include <QPainter>
#include <QMap>

class Mario {
public:
    enum Direction { LEFT, RIGHT };
    enum State { STANDING, RUNNING, JUMPING, DYING };

    Mario();

    void moveLeft();
    void moveRight();
    void stopMoving();

    void update(); // 處理動畫幀與狀態變化
    void draw(QPainter& painter, int scrollX); // 畫出角色

    int getX() const { return x; }
    int getY() const { return y; }

    State getState() const { return state; }
    void setState(State s) { state = s; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool getOnGround() const {return isOnGround; }

    int getVy() const { return vy; }
    void setY(int newY) { y = newY; }
    void setVy(int newVy) { vy = newVy; }
    void setOnGround(bool val) { isOnGround = val; }

    bool getIsJumping() const { return isJumping; }
    void setIsJumping(bool val) { isJumping = val; }

    void land();
    void die();

private:
    int x, y;
    int width, height;
    int vy;              // 垂直速度

    bool isBig;
    bool isOnGround;     // 是否落地
    bool isJumping = false;
    Direction facing;
    State state;
    int animFrame;

    QMap<QString, QPixmap> pixmapTable;

    QPixmap getCurrentFrame();
    void loadAllPixmaps(); // 載入所有圖片
};
