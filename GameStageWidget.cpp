#include "GameStageWidget.h"
#include "FloorBrick.h"
#include "Item.h"
#include "Mario.h"
#include "InteractiveBrick.h"
#include "Coin.h"
#include "NormalBrick.h"
#include "BrokenBrick.h"
#include "SuperMushroom.h"
#include "FireFlower.h"
#include "Fireball.h"
#include "Pipe.h"

#include <cmath>
#include <QVBoxLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QPushButton>
#include <QPainter>

GameStageWidget::GameStageWidget(QWidget* parent)
    : QWidget(parent), hp(3), score(0)
{

    hpLabel = new QLabel("HP: 3", this);
    hpLabel->move(5,5);
    hpLabel->setStyleSheet("color: red; font-weight: bold;");
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setFixedWidth(100);
    scoreLabel->move(100,5);
    scoreLabel->setStyleSheet("color: red; font-weight: bold;");

    marioPosLabel = new QLabel("X:1000", this);
    marioPosLabel->setFixedWidth(400);
    marioPosLabel->move(200, 5);
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
                    if(score >= 20) emit gameWin();
                    else            emit gameLose();
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
        // qDebug() << "Init floor at x =" << i * tileWidth;
    }
    bricks.append(new InteractiveBrick(500, 400, BrickContent::None, this));

    Brick* nb = new NormalBrick(600, 370, 3);  // 擺在某個你看得到的位置
    bricks.push_back(nb);
    bricks.push_back(new NormalBrick(1000, 400));  // 不含金幣
    bricks.push_back(new NormalBrick(1100, 410));  // 不含金幣

    bricks.push_back(new BrokenBrick(900, 370));

    bricks.push_back(new Pipe(1000, 420));
    bricks.push_back(new Pipe(1400, 420));

    qDebug() << "Init InteractiveBrick at pos = (450, 370)";

    // 旗子
    items.push_back(new FlagItem(6975, 520));

    // 金幣
    Coin* c = new Coin(400, 400);
    items.push_back(c);

    // superMushroom
    items.append(new FireFlower(300, 400));

    // 毒蘑菇
    toxicMushrooms.append(new ToxicMushroom(1200, 470, this));


    // 瑪利歐無敵時間計時器
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);

    connect(invincibleTimer, &QTimer::timeout, this, [=]() {
        invincible = false;
        marioVisible = true;
        flickerTimer->stop();
        qDebug() << "Mario is no longer invincible.";
    });
    flickerTimer = new QTimer(this);
    connect(flickerTimer, &QTimer::timeout, this, [=]() {
        marioVisible = !marioVisible;
        update();
    });

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

void GameStageWidget::addItem(Item* item) {
    items.append(item);
}

void GameStageWidget::updateGame() {
    // ✅ 如果 Mario 掉下畫面底下，直接結束遊戲
    if (mario.getY() > 700 && !isDeathHandled) {
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

    // 跳躍後落地
    if (mario.getOnGround() && mario.getState() == Mario::JUMPING) {
        mario.setState(Mario::STANDING);

    }
    mario.update();

    bool landed = false;

    // 和磚塊碰撞
    for (Brick* brick : bricks) {
        if (!brick) continue;

        int mx = mario.getX();
        int my = mario.getY();
        int mw = mario.getWidth();
        int mh = mario.getHeight();

        int bx = brick->getX();
        int by = brick->getY();
        int bw = brick->getWidth();
        int bh = brick->getHeight();

        // 建立 QRect 幫助判斷
        QRect marioRect(mx, my, mw, mh);
        QRect brickRect(bx, by, bw, bh);

        // === 往上撞到磚塊 ===
        QRect marioHead(mx, my, mw, 1);
        if (!mario.getOnGround() && mario.getVy() < 0 && marioHead.intersects(brickRect)) {
            mario.setVy(0);
            mario.setY(by + bh);
        // 🔽 在這裡處理敲擊 & 加分
            NormalBrick* nb = dynamic_cast<NormalBrick*>(brick);
            if (nb) {
                int coinsGained = nb->onHitFromBelow();  // 回傳這次獲得的金幣數
                score += coinsGained;
                if (coinsGained > 0) {
                    floatingCoins.append(new FloatingCoin(brick->getX(), brick->getY() - 10));
                    qDebug() << "Score += " << coinsGained << ", total score:" << score;
                }
            } else {
                brick->onHitFromBelow();
            }
            qDebug() << "上方碰撞" ;
            break;
        }

        // === 往下撞到磚塊（腳落地）===
        if (my + mh <= by && my + mh + mario.getVy() >= by &&
            mx + mw > bx && mx < bx + bw) {
            mario.setY(by - mh);
            mario.setVy(0);
            mario.setOnGround(true);
            mario.setIsJumping(false);
            mario.land();
            landed = true;
            break;
        }

        if (marioRect.intersects(brickRect) && !(mario.getVy() <= 0 ) ) {
            bool isAbove = my + mh <= by + 5;
            bool isBelow = my >= by + bh - 5;

            if (!isAbove && !isBelow) {
                if (mario.getDirection() == Mario::LEFT) {
                    mario.setX(bx + bw);  // 撞到右側 → 擋住
                    qDebug() << "碰撞右側";
                }
                else if (mario.getDirection() == Mario::RIGHT) {
                    mario.setX(bx - mw);  // 撞到左側 → 擋住
                    qDebug() << "碰撞左側";
                }
            }
        }
    }

    for (Item* item : items) {
        auto* coin = dynamic_cast<Coin*>(item);
        if (coin && coin->isVisible() && coin->checkCollision(mario)) {
            coin->setVisible(false);
            score++;
            qDebug() << "Coin collected! Score: " << score;
        }
    }

    for (Item* item : items) {
        SuperMushroom* sm = dynamic_cast<SuperMushroom*>(item);
        if (sm && sm->checkCollision(mario)) {
            mario.setIsBig(true);
            sm->setCollected(true);  // 你可以新增這函式，或直接 sm->collected = true;
            qDebug() << "Mario eat Super Mushroom!";
        }
    }

    for (Item* item : items) {
        FireFlower* flower = dynamic_cast<FireFlower*>(item);
        if (flower && flower->checkCollision(mario)) {
            mario.enableFire();
            flower->setCollected(true);
            qDebug() << "Mario got Fire Flower!";
        }
    }

    for (int i = 0; i < fireballs.size(); ++i) {
        Fireball* fb = fireballs[i];
        fb->update();

        // 🔸 檢查撞磚塊
        for (Brick* brick : bricks) {
            if (fb->getRect().intersects(brick->getRect())) {
                fb->destroy();
                break;
            }
        }

        if (!fb->isAlive()) {
            delete fb;
            fireballs.remove(i);
            --i;
        }
    }

    // broken brick移除
    for (int i = 0; i < bricks.size(); ) {
        BrokenBrick* bb = dynamic_cast<BrokenBrick*>(bricks[i]);
        if (bb && bb->isBroken()) {
            delete bricks[i];
            bricks.remove(i);
            continue;  // 不用 ++i，因為 remove 已移位
        }
        ++i;
    }

    // 更新浮動金幣動畫
    for (int i = 0; i < floatingCoins.size(); ) {
        floatingCoins[i]->update();
        if (floatingCoins[i]->isDone()) {
            delete floatingCoins[i];
            floatingCoins.remove(i);
        } else {
            ++i;
        }
    }

    // 毒蘑菇偵測
    for (ToxicMushroom* tm : toxicMushrooms) {
        if (!tm->isAlive()) continue;

        tm->update();
        if (tm->checkMarioCollision(mario)) {
            if (!invincible) {
                hp--;
                invincible = true;
                marioVisible = true;
                invincibleTimer->start(2000);  // 無敵持續時間
                flickerTimer->start(100);      // 每 100ms 閃爍一次
                qDebug() << "Mario hit! HP:" << hp;
            }
        }

        for (Fireball* fb : fireballs) {
            bool hit = false;
            tm->checkFireballCollision(fb->getRect(), hit);
            if (hit) fb->destroy();
        }

        for (Brick* brick : bricks) {
            tm->checkBlockCollision(brick->getRect());
        }
    }

    checkGameState();
    update();
    marioPosLabel->setText(QString("X: %1, Y: %2").arg(mario.getX()).arg(mario.getY()));
    scoreLabel->setText(QString("Score: %1").arg(score));
    if (!landed) mario.setOnGround(false);


}


void GameStageWidget::checkGameState()
{
    // 假設遊戲結束條件
    if (hp <= 0) {
        gameTimer->stop();
        emit gameLose();
    }

    // 碰到旗杆
    for (Item* item : items) {
        auto* flag = dynamic_cast<FlagItem*>(item);
        if ( flag && flag->checkCollision(mario)) {
            flag->activate();
            gameTimer->stop();
            flagTimer->start(30);
            return;
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

    if (marioVisible) {
        mario.draw(painter, scrollX);
    }

    for (FloatingCoin* fc : floatingCoins) {
        fc->draw(painter, scrollX);
    }
    for (Fireball* f : fireballs){
        f->draw(painter, scrollX);
    }
    for (ToxicMushroom* tm : toxicMushrooms) {
        tm->draw(painter, scrollX);
    }

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
            mario.setVy(-25);             // 向上跳
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

void GameStageWidget::mousePressEvent(QMouseEvent* event) {
    if (mario.canShoot() && event->button() == Qt::LeftButton) {
        QPoint target = event->pos();
        int mx = mario.getX() + mario.getWidth() / 2;
        int my = mario.getY() + mario.getHeight() / 2;
        float dx = target.x() + scrollX - mx;
        float dy = target.y() - my;
        float len = sqrt(dx*dx + dy*dy);
        float speed = 10.0;
        float vx = speed * dx / len;
        float vy = speed * dy / len;

        if (mario.shootFireball()) {
            fireballs.append(new Fireball(mx, my, vx, vy));
        }
    }
}
