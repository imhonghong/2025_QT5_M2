#include "GameStageWidget.h"
#include "FloorBrick.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>

GameStageWidget::GameStageWidget(QWidget* parent)
    : QWidget(parent), hp(3), score(0)
{
    //QVBoxLayout* layout = new QVBoxLayout(this);
    hpLabel = new QLabel("HP: 3", this);
    hpLabel->move(0,5);
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->move(100,5);

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

    qDebug() << "[GameStage] Game reset!";
    gameTimer->start();
}

void GameStageWidget::updateGame()
{
    // TODO: 加入瑪利歐、敵人、道具更新邏輯

    checkGameState();
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

}

