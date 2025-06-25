#pragma once
#include <QPixmap>
#include <QElapsedTimer>

class FloatingCoin {
public:
    FloatingCoin(int x, int y);

    void update();  // 更新位置（往上飄）
    void draw(QPainter& painter, int scrollX);
    bool isDone() const;

private:
    int x, y;
    int elapsedMs; // 經過時間（ms）
    QPixmap coinPixmap;
    QElapsedTimer timer;

    static const int animTime = 200;
};
