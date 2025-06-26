#pragma once

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton> //only for debugging

#include "Brick.h"
#include "FloorBrick.h"
#include <QVector>
#include "Mario.h"
#include "Item.h"
#include "FlagItem.h"
#include "FloatingCoin.h"
#include "SuperMushroom.h"
#include "Fireball.h"
#include "ToxicMushroom.h"
#include "Brick.h"

class GameStageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameStageWidget(QWidget* parent = nullptr);
    void reset(); // 重新初始化遊戲

    void addItem(Item* item);
    void addScore(int delta);
    void addFloatingCoin(int x, int y);

    void updateOtherItem();


protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

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
    void initStage();

    QPixmap fullBackground;     // 拼好的 7000x620 大圖
    int scrollX = 0;            // 捲動的水平偏移（例如 Mario 的位置）

    QVector<Brick*> bricks;  // 加入這個成員變數
    QPushButton* coinButton;
    Mario mario;

    bool isDeathHandled = false; // 防止重複 emit

    QVector<Item*> items;       // 所有 item（目前包含旗子）
    QTimer* flagTimer;          // 控制旗子滑落動畫

    void checkGameState(); // 判斷是否勝利或失敗

    QVector<FloatingCoin*> floatingCoins;
    QVector<Fireball*> fireballs;

    QVector<ToxicMushroom*> toxicMushrooms;

    bool invincible = false;
    QTimer* invincibleTimer;

    bool marioVisible = true;          // 控制是否繪出 Mario
    QTimer* flickerTimer = nullptr;    // 閃爍用

};
