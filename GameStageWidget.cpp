#include "GameStageWidget.h"
#include "FloorBrick.h"
#include "Item.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QPushButton>

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

    coinButton = new QPushButton("+1 Coin", this);
    coinButton->move(20, 20);
    connect(coinButton, &QPushButton::clicked, [this]() {
        score++;
        qDebug() << "Score: " << score;
    });

    setFocusPolicy(Qt::StrongFocus); // 確保可接收鍵盤
    setFocus();// 主動取得焦點

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

    // 終點旗子
    flagTimer = new QTimer(this);
    connect(flagTimer, &QTimer::timeout, [this]() {
        for (Item* item : items) {
            auto* flag = dynamic_cast<FlagItem*>(item);
            if (flag && flag->isActivated()) {
                flag->update();         // 滑落動畫
                if (flag->isDone()) {
                    flagTimer->stop();
                    emit gameWin();     // 勝利訊號
                }
            }
        }
        update(); // 重新繪製旗子
    });

    // 生成 brick
    initStage();
}

void GameStageWidget::initStage() {
    int tileWidth = 50;
    int screenWidth = 1400;
    int totalWidth = 7000;
    int groundY = 520;
    int tilesPerScreen = screenWidth / tileWidth;
    int totalTiles = totalWidth / tileWidth;

    for (int i = 0; i < totalTiles; ++i) { // floorBrick都挖掉畫面中間那塊
        if (i % tilesPerScreen == tilesPerScreen / 2) continue;
        Brick* b = new FloorBrick(i * tileWidth, groundY);
        bricks.push_back(b);
        qDebug() << "Init floor at x =" << i * tileWidth;
    }

    items.push_back(new FlagItem(6975, 520));
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

    for (Item* item : items) {
        auto* flag = dynamic_cast<FlagItem*>(item);
        if (flag && flag->checkCollision(mario)) {
            flag->activate();
            gameTimer->stop();
            flagTimer->start(30);
            return;
        }
    }

    checkGameState();
    update();
    marioPosLabel->setText(QString("X: %1, Y: %2").arg(mario.getX()).arg(mario.getY()));
    scoreLabel->setText(QString("Score: %1").arg(score));
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
        // 確保只有真的有碰到旗子才算勝利
        for (Item* item : items) {
            auto* flag = dynamic_cast<FlagItem*>(item);
            if (flag && flag->isActivated()) {
                gameTimer->stop();
                emit gameWin();
                return;
            }
        }
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

    for (Item* item : items) {
        if (item) item->draw(painter, scrollX);
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
