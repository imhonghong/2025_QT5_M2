#pragma once

#include <QWidget>
#include <QTimer>
#include <QLabel>

#include "Brick.h"
#include "FloorBrick.h"
#include <QVector>
#include "Mario.h"


class GameStageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameStageWidget(QWidget* parent = nullptr);
    void reset(); // 重新初始化遊戲

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void gameWin();
    void gameLose();

private slots:
    void updateGame(); // 遊戲進行邏輯更新

private:
    QTimer* gameTimer;
    QLabel* hpLabel;
    QLabel* scoreLabel;
    QLabel* marioPosLabel;

    int hp;
    int score;

    QPixmap fullBackground;     // 拼好的 7000x620 大圖
    int scrollX = 0;            // 捲動的水平偏移（例如 Mario 的位置）

    QVector<Brick*> bricks;  // 加入這個成員變數

    Mario mario;

    bool isDeathHandled = false; // 防止重複 emit

    void checkGameState(); // 判斷是否勝利或失敗
};
