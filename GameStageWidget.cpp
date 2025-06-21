#include "GameStageWidget.h"
#include "FloorBrick.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

GameStageWidget::GameStageWidget(QWidget* parent)
    : QWidget(parent), hp(3), score(0)
{

    hpLabel = new QLabel("HP: 3", this);
    hpLabel->move(0,5);
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->move(100,5);

    marioPosLabel = new QLabel("X:1000", this);
    marioPosLabel->setFixedWidth(400);
    marioPosLabel->move(200, 5);  // 位置你可自訂
    marioPosLabel->setStyleSheet("color: white; font-weight: bold;");

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameStageWidget::updateGame);
    gameTimer->start(30); // 每 30ms 更新一次畫面與邏輯

    QPixmap tile(":/bg/data/bg/background.png");
    QPixmap scaledTile = tile.scaled(1400, 620, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fullBackground = QPixmap(7000, 620);
    fullBackground.fill(Qt::transparent);

    // 大圖背景
    QPainter painter(&fullBackground);
    for (int i = 0; i < 5; ++i) {
        painter.drawPixmap(i * 1400, 0, scaledTile);
    }
    painter.end();

    // 地板
    for (int i = 0; i < 28; ++i) {
        bricks.push_back(new FloorBrick(i * 50, 520)); // y=620-100
    }

}

void GameStageWidget::reset()
{
    hp = 3;
    score = 0;
    hpLabel->setText("HP: 3");
    scoreLabel->setText("Score: 0");
    isDeathHandled = false;
    qDebug() << "[GameStage] Game reset!";
    gameTimer->start();
}

void GameStageWidget::updateGame() {
    // ✅ 如果 Mario 掉下畫面底下，直接結束遊戲
    if (mario.getY() > 650 && !isDeathHandled) {
        isDeathHandled = true;
        gameTimer->stop();
        emit gameLose();
        return;
    }

    // 背景跟隨滾動
    int marioX = mario.getX();
    int marioRight = marioX + mario.getWidth();

    if (marioRight > 700 && marioRight < 7000) {
        scrollX = marioRight - 700;
        if (scrollX > 7000 - width()) {
            scrollX = 7000 - width();
        }
    }

    mario.update();
    checkGameState();
    update();
    marioPosLabel->setText(QString("X: %1, Y: %2").arg(mario.getX()).arg(mario.getY()));

    mario.setOnGround(false);

    for (Brick* brick : bricks) {
        if (!brick) continue;

        int mx = mario.getX();
        int my = mario.getY();
        int mw = mario.getWidth();
        int mh = mario.getHeight();

        int bx = brick->x;
        int by = brick->y;
        int bw = brick->width;
        int bh = brick->height;

        if (my + mh <= by && my + mh + mario.getVy() >= by &&
            mx + mw > bx && mx < bx + bw) {
            mario.setY(by - mh);
            mario.setVy(0);
            mario.setOnGround(true);
            mario.setIsJumping(false);
            mario.land();
        }
    }
}


void GameStageWidget::checkGameState()
{
    // 假設遊戲結束條件
    if (hp <= 0) {
        gameTimer->stop();
        emit gameLose();
    }
    else if (score >= 21) {
        gameTimer->stop();
        emit gameWin();
    }
}

void GameStageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, fullBackground.copy(scrollX, 0, width(), height()));



    // ✅ 畫出目前範圍內的磚塊
    for (Brick* brick : bricks) {
        if (brick) brick->draw(painter, scrollX);
    }

    mario.draw(painter, scrollX);

}

void GameStageWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left) {
        mario.moveLeft();
    } else if (event->key() == Qt::Key_Right) {
        mario.moveRight();
    }
    if (event->key() == Qt::Key_Space && !mario.getIsJumping()) {
        if (mario.getOnGround()) {
            mario.setVy(-20);             // 向上跳
            mario.setOnGround(false);     // 離地
        }
    }
    QWidget::keyPressEvent(event);
}

void GameStageWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        mario.stopMoving();
    }
    QWidget::keyReleaseEvent(event);
}
