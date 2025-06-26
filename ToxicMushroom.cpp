#include "ToxicMushroom.h"
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QObject>

ToxicMushroom::ToxicMushroom(int x, int y, QObject* parent)
    : QObject(parent), x(x), y(y), vx(1.5), alive(true), walkFrame(0), walkCounter(0)
{
    currentImgPath = ":/item/data/mushroom/toxic mushroom1.png";
    deathTimer = new QTimer();
    deathTimer->setSingleShot(true);
    connect(deathTimer, &QTimer::timeout, [=]() {
        currentImgPath = "";  // 停止畫圖
    });
}

void ToxicMushroom::update() {
    if (!alive) return;
    x += vx;
    walkCounter++;
    if (walkCounter >= 10) {
        walkCounter = 0;
        walkFrame = (walkFrame + 1) % 2;
        currentImgPath = (walkFrame == 0)
            ? ":/item/data/mushroom/toxic mushroom1.png"
            : ":/item/data/mushroom/toxic mushroom2.png";
    }
}

void ToxicMushroom::draw(QPainter& painter, int scrollX) {

    if (currentImgPath.isEmpty()) return;
    qDebug() << "[TM] draw currentImgPath=" << currentImgPath;
    painter.drawPixmap(x - scrollX, y, width, height,
                       QPixmap(currentImgPath));
}

QRect ToxicMushroom::getRect() const {
    return QRect(x, y, width, height);
}

bool ToxicMushroom::checkMarioCollision( Mario& mario) {
    if (!alive) return false;

    QRect marioRect = mario.getRect();
    QRect selfRect = getRect();

    qDebug() << "[TM] 進入 checkMarioCollision, marioBottom=" << marioRect << " y+10=" << y+10;

    if (marioRect.intersects(selfRect)) {
        int marioBottom = mario.getY() + mario.getHeight();
        if (marioBottom <= y + 10 && mario.getVy() > 0) {
            alive = false;
            currentImgPath = ":/item/data/mushroom/toxic mushroom3.png";
            vx = 0;
            mario.setVy(-8);
            deathTimer->start(500);
            qDebug() << "[TM] 踩死毒蘑菇! ";
            return false;  // 踩死 → 不扣血
        } else {
            return true;   // 撞擊 → 要扣血
        }
    }
    return false;
}

void ToxicMushroom::checkFireballCollision(const QRect& fireballRect, bool& hit) {
    if (!alive) return;
    if (fireballRect.intersects(getRect())) {
        alive = false;
        hit = true;
        qDebug() << "Toxic mushroom killed by fireball!";
    }
}

void ToxicMushroom::checkBlockCollision(const QRect& blockRect) {
    if (!alive) return;
    if (getRect().intersects(blockRect)) {
        vx = -vx;  // 反彈
    }
}

bool ToxicMushroom::isAlive() const {
    return alive;
}
