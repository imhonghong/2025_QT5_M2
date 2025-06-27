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
        if (    i % tilesPerScreen == tilesPerScreen / 2
            ||  i % tilesPerScreen == (tilesPerScreen / 2)+1
            /*||  i % tilesPerScreen == (tilesPerScreen / 2)-1*/ ) continue;
        Brick* b = new FloorBrick(i * tileWidth, groundY);
        bricks.push_back(b);
        // qDebug() << "Init floor at x =" << i * tileWidth;
    }

    addFrame1();
    addFrame2();
    addFrame3();
    addFrame4();
    addFrame5();

    mario.setBricks(bricks);

    // 旗子
    items.push_back(new FlagItem(6975, 520));

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
    // 停止遊戲暫停計時
    gameTimer->stop();

    // 回收記憶體並清空容器
    qDeleteAll(bricks);
    bricks.clear();
    qDeleteAll(items);
    items.clear();
    qDeleteAll(floatingCoins);
    floatingCoins.clear();
    qDeleteAll(fireballs);
    fireballs.clear();
    qDeleteAll(toxicMushrooms);
    toxicMushrooms.clear();

    // 重置分數與血量
    score = 0;
    hp = 3;
    scrollX = 0;
    // 重置 Mario
    mario = Mario(); // 呼叫 Mario 預設建構子
    mario.setBricks(bricks);

    // 重置標籤
    hpLabel->setText("HP: 3");
    scoreLabel->setText("Score: 0");
    marioPosLabel->setText("X:0, Y:420");

    // 清除狀態標記
    isDeathHandled = false;
    invincible = false;
    marioVisible = true;

    // 重新初始化場景
    initStage();

    // 重啟遊戲計時器
    gameTimer->start();

    qDebug() << "[GameStage] Game reset and restarted!";
}

void GameStageWidget::addItem(Item* item) {
    items.append(item);
}

void GameStageWidget::addScore(int delta) {
    score += delta;
    qDebug() << "[Game] Score += " << delta << ", now:" << score;
}

void GameStageWidget::addFloatingCoin(int x, int y) {
    floatingCoins.append(new FloatingCoin(x, y));
}

void GameStageWidget::handleDeath() {
    emit gameLose();
}

void GameStageWidget::updateGame() {
    // ✅ 如果 Mario 掉下畫面底下，直接結束遊戲
    /*
    if (mario.getY() > 620 && !isDeathHandled) {
        isDeathHandled = true;
        gameTimer->stop();
        mario.die();
        update();
        QTimer::singleShot(1000, this, &GameStageWidget::handleDeath);
        return;
    }
    */

    if (mario.getState() == Mario::DYING) {
        mario.update();
        update();
        if (mario.getY() >= 620) {  // 到達畫面底部時觸發 gameLose
            emit gameLose();
        }
        return;  // 死亡動畫時不進行其他碰撞偵測
    }

    if (mario.getY() >= 620 - mario.getHeight() && !isDeathHandled && mario.getState() != Mario::DYING) {
        isDeathHandled = true;
        gameTimer->stop();
        mario.die();
        update();
        QTimer::singleShot(100, this, [this]() {
            mario.setVy(-15);
            gameTimer->start();
        });
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

    // === 第一步：處理地板磚塊的落地檢測 ===
    for (Brick* brick : bricks) {
        if (!brick) continue;

        FloorBrick* floorBrick = dynamic_cast<FloorBrick*>(brick);
        if (!floorBrick) continue; // 只處理地板磚塊

        int mx = mario.getX();
        int my = mario.getY();
        int mw = mario.getWidth();
        int mh = mario.getHeight();

        int bx = brick->getX();
        int by = brick->getY();
        int bw = brick->getWidth();
        int bh = brick->getHeight();

        // 地板只檢測落地，不檢測水平碰撞
        if (mario.getVy() >= 0 && // 瑪利歐正在下落或靜止
            my + mh <= by + 3 && // 瑪利歐底部接近地板頂部
            my + mh + mario.getVy() >= by && // 下一幀會接觸到地板頂部
            mx + mw > bx + 2 && mx < bx + bw - 2) { // 水平範圍重疊

            mario.setY(by - mh);
            mario.setVy(0);
            mario.setOnGround(true);
            mario.setIsJumping(false);
            mario.land();
            landed = true;
            qDebug() << "地板落地碰撞";
            break;
        }
    }

    // === 第二步：處理其他磚塊的完整碰撞檢測 ===
    for (Brick* brick : bricks) {
        if (!brick) continue;

        // 跳過地板磚塊，地板已經在上面處理過了
        if (dynamic_cast<FloorBrick*>(brick)) continue;

        int mx = mario.getX();
        int my = mario.getY();
        int mw = mario.getWidth();
        int mh = mario.getHeight();

        int bx = brick->getX();
        int by = brick->getY();
        int bw = brick->getWidth();
        int bh = brick->getHeight();

        QRect marioRect(mx, my, mw, mh);
        QRect brickRect(bx, by, bw, bh);

        // === 2.1 優先檢測往上撞到磚塊（頭撞天花板）===
        QRect marioHead(mx, my, mw, 1);
        if (!mario.getOnGround() && mario.getVy() < 0 && marioHead.intersects(brickRect)) {
            mario.setVy(0);
            mario.setY(by + bh);

            // 處理敲擊效果
            NormalBrick* nb = dynamic_cast<NormalBrick*>(brick);
            if (nb) {
                int coinsGained = nb->onHitFromBelow();
                score += coinsGained;
                if (coinsGained > 0) {
                    floatingCoins.append(new FloatingCoin(brick->getX(), brick->getY() - 10));
                    qDebug() << "Score += " << coinsGained << ", total score:" << score;
                }
            } else {
                brick->onHitFromBelow();
            }
            qDebug() << "上方碰撞";
            break;
        }

        // === 2.2 檢測往下落地（腳接觸磚塊頂部）===
        if (!landed && mario.getVy() >= 0 && // 如果還沒有在地板上落地
            my + mh <= by + 3 && // 瑪利歐底部接近磚塊頂部
            my + mh + mario.getVy() >= by && // 下一幀會接觸到磚塊頂部
            mx + mw > bx + 2 && mx < bx + bw - 2) { // 水平範圍重疊

            mario.setY(by - mh);
            mario.setVy(0);
            mario.setOnGround(true);
            mario.setIsJumping(false);
            mario.land();
            landed = true;
            qDebug() << "磚塊落地碰撞";
            break;
        }
    }
    updateOtherItem();
    checkGameState();
    update();
    marioPosLabel->setText(QString("X: %1, Y: %2").arg(mario.getX()).arg(mario.getY()));
    scoreLabel->setText(QString("Score: %1").arg(score));
    hpLabel->setText(QString("hp: %1").arg(hp));
    if (!landed) mario.setOnGround(false);
}


void GameStageWidget::checkGameState()
{
    if (hp <= 0 && !isDeathHandled) {
        isDeathHandled = true;
        gameTimer->stop();     // 暫停遊戲
        mario.die();           // 設定 DYING 狀態
        update();              // 重繪
        QTimer::singleShot(100, this, [this]() {
                mario.setVy(-15);   // 彈跳速度，可依需求微調
                gameTimer->start(); // 恢復動畫更新
            });
        return;
    }

    for (Item* item : items) {
        auto* flag = dynamic_cast<FlagItem*>(item);
        if (flag && flag->checkCollision(mario)) {
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
            if (mario.getIsBig())
                mario.setVy(-26); // 大瑪利歐跳 160px
            else
                mario.setVy(-22); // 小瑪利歐跳 100px

            mario.setOnGround(false);
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

void GameStageWidget::updateOtherItem()
{
    for (Item* item : items) {
        auto* coin = dynamic_cast<Coin*>(item);
        if (coin && coin->isVisible() && coin->checkCollision(mario)) {
            coin->setVisible(false);
            score++;
            qDebug() << "Coin collected! Score: " << score;
        }
    }
    for (Item* item : items) {
        auto* sm = dynamic_cast<SuperMushroom*>(item);
        if (sm && !sm->isCollected()) {
            sm->update();  // 內部控制 Emerging → Walking
            for (Brick* b : bricks) {
                Pipe* pipe = dynamic_cast<Pipe*>(b);
                if (pipe) {
                    sm->checkPipeCollision(pipe->getRect());  // ✅ 確保執行這段
                }
            }


            // 🔽 若目前為 walking，且沒有落在磚塊上，就改為 Falling
            if (sm->getState() == SuperMushroom::Walking) {
                bool onBlock = false;
                QRect mushroomRect(sm->getX(), sm->getY() + sm->getHeight(), sm->getWidth(), 1);  // 下方線

                for (Brick* b : bricks) {
                    if (mushroomRect.intersects(b->getRect())) {
                        onBlock = true;
                        break;
                    }
                }
                if (!onBlock) {
                    sm->setState(SuperMushroom::Falling);  // 進入掉落
                }
            }

            // 🔽 若目前為 falling，檢查是否落地
            if (sm->getState() == SuperMushroom::Falling) {
                QRect mushroomFeet(sm->getX(), sm->getY() + sm->getHeight(), sm->getWidth(), 1);
                for (Brick* b : bricks) {
                    if (mushroomFeet.intersects(b->getRect())) {
                        sm->setY(b->getY() - sm->getHeight());
                        sm->setVy(0);
                        sm->setState(SuperMushroom::Walking);
                        break;
                    }
                }
            }

            if (sm->checkCollision(mario)) {
                mario.setIsBig(true);
                sm->setCollected(true);
                hp ++;
                qDebug() << "Mario eat Super Mushroom!";
            }
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

        if (tm->getState() == ToxicMushroom::Walking) {
            bool onBlock = false;
            QRect mushroomRect(tm->getX(), tm->getY() + tm->getHeight(), tm->getWidth(), 1);
            for (Brick* b : bricks) {
                if (mushroomRect.intersects(b->getRect())) {
                    onBlock = true;
                    break;
                }
            }
            if (!onBlock) {
                tm->setState(ToxicMushroom::Falling);
            }
        }

        // 若正在 Falling，檢查是否落地
        if (tm->getState() == ToxicMushroom::Falling) {
            QRect mushroomFeet(tm->getX(), tm->getY() + tm->getHeight(), tm->getWidth(), 1);
            for (Brick* b : bricks) {
                if (mushroomFeet.intersects(b->getRect())) {
                    tm->setY(b->getY() - tm->getHeight());
                    tm->setVy(0);
                    tm->setState(ToxicMushroom::Walking);
                    break;
                }
            }
        }


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
}


void GameStageWidget::addFrame1()
{
    int offsetX = 0;
    int offsetY = 620-100 ;
    items.append(new Coin(          offsetX + 250,  offsetY - 100));
    items.append(new Coin(          offsetX + 300,  offsetY - 150));
    items.append(new Coin(          offsetX + 350,  offsetY - 100));
    bricks.append(new BrokenBrick(  offsetX + 500,  offsetY - 100));
    bricks.append(new BrokenBrick(  offsetX + 600,  offsetY - 200));
    items.append(new Coin(          offsetX + 600,  offsetY - 300));
    items.append(new Coin(          offsetX + 650,  offsetY - 350));
    items.append(new Coin(          offsetX + 700,  offsetY - 300));
    bricks.append(new InteractiveBrick(             offsetX + 850, offsetY - 150, BrickContent::None, this));
    bricks.append(new Pipe(         offsetX + 1000, offsetY - 100));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 1100, offsetY - 50, this));
    bricks.append(new Pipe(         offsetX + 1300, offsetY - 100));

}

void GameStageWidget::addFrame2()
{
    int offsetX = 1400;
    int offsetY = 620-100 ;

    bricks.append(new NormalBrick(  offsetX + 100,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX + 100,  offsetY - 100)); //no coin
    bricks.append(new NormalBrick(  offsetX + 100,  offsetY - 150));
    bricks.append(new BrokenBrick(  offsetX + 150,  offsetY - 150));
    bricks.append(new BrokenBrick(  offsetX + 200,  offsetY - 150));
    bricks.append(new BrokenBrick(  offsetX + 250,  offsetY - 150));
    items.append(new Coin(          offsetX + 150,  offsetY - 50));
    items.append(new Coin(          offsetX + 200,  offsetY - 50));
    items.append(new Coin(          offsetX + 250,  offsetY - 50));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 350, offsetY - 50, this));
    bricks.append(new Pipe(         offsetX + 450,  offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 850,  offsetY - 50));
    bricks.append(new BrokenBrick(  offsetX + 950,  offsetY - 100));
    items.append(new Coin(          offsetX + 1000, offsetY - 200));
    items.append(new Coin(          offsetX + 1050, offsetY - 250));
    items.append(new Coin(          offsetX + 1100, offsetY - 250));
    items.append(new Coin(          offsetX + 1150, offsetY - 250));
    items.append(new Coin(          offsetX + 1200, offsetY - 200));
    bricks.append(new BrokenBrick(  offsetX + 1100, offsetY - 300));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 1150, offsetY - 50, this));
    bricks.append(new Pipe(         offsetX + 1250, offsetY - 100));

}


void GameStageWidget::addFrame3()
{
    int offsetX = 2800;
    int offsetY = 620-100 ;

    bricks.append(new NormalBrick(  offsetX + 150,  offsetY - 50));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 250, offsetY - 50, this));
    bricks.append(new NormalBrick(  offsetX + 300,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX + 350,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX + 400,  offsetY - 150));
    items.append(new Coin(          offsetX + 300,  offsetY - 200));
    items.append(new Coin(          offsetX + 350,  offsetY - 200));
    items.append(new Coin(          offsetX + 400,  offsetY - 200));
    bricks.append(new NormalBrick(  offsetX + 550,  offsetY - 50));
    bricks.append(new InteractiveBrick(             offsetX + 900, offsetY - 150, BrickContent::None, this));
    bricks.append(new Pipe(         offsetX + 1100, offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 1250,  offsetY - 200));
    items.append(new Coin(          offsetX + 1250,  offsetY - 250));

}

void GameStageWidget::addFrame4()
{
    int offsetX = 4200 ;
    int offsetY = 620-100 ;

    items.append(new Coin(          offsetX + 250,  offsetY - 150));
    items.append(new Coin(          offsetX + 300,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX + 450,  offsetY - 200));
    bricks.append(new BrokenBrick(  offsetX + 500,  offsetY - 150));
    bricks.append(new BrokenBrick(  offsetX + 550,  offsetY - 150));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 500, offsetY - 200, this));
    bricks.append(new NormalBrick(  offsetX + 600,  offsetY - 200));
    bricks.append(new Pipe(         offsetX + 1000, offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 1150,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX + 1150,  offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 1150,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX + 1150,  offsetY - 200));

}

void GameStageWidget::addFrame5()
{
    int offsetX = 5600 ;
    int offsetY = 620-100 ;

    items.append(new Coin(          offsetX + 150,  offsetY - 150));
    items.append(new Coin(          offsetX + 200,  offsetY - 150));
    bricks.append(new Pipe(         offsetX + 450,  offsetY - 100));
    toxicMushrooms.append(new ToxicMushroom(        offsetX + 550, offsetY - 50, this));
    bricks.append(new NormalBrick(  offsetX + 600,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX + 600,  offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 600,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX + 600,  offsetY - 200));

    bricks.append(new NormalBrick(  offsetX + 800,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX + 800,  offsetY - 100));
    bricks.append(new NormalBrick(  offsetX + 800,  offsetY - 150));

    bricks.append(new BrokenBrick(  offsetX +1000,  offsetY - 150));
    bricks.append(new NormalBrick(  offsetX +1050,  offsetY - 150, 4));
    bricks.append(new BrokenBrick(  offsetX +1100,  offsetY - 150));
    items.append(new Coin(          offsetX +1000,  offsetY - 50));
    items.append(new Coin(          offsetX +1050,  offsetY - 200));
    items.append(new Coin(          offsetX +1100,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX +1200,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX +1250,  offsetY - 50));
    bricks.append(new NormalBrick(  offsetX +1250,  offsetY - 100));
    bricks.append(new NormalBrick(  offsetX +1250,  offsetY - 150));





}

