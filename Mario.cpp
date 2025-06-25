#include "Mario.h"
#include <QString>
#include <QDebug>

Mario::Mario()
{
    x = 0;
    y = 420;
    vy = 0;
    isOnGround = true;
    width = 50;
    height = 50;
    isBig = false;
    facing = RIGHT;
    state = STANDING;

    animFrame = 0;

    loadAllPixmaps();
}

void Mario::moveLeft() {
    if (x > 0) {
            facing = LEFT;
            state = RUNNING;
            x -= isBig ? 70 : 50;
        }
}

void Mario::moveRight() {
    int maxX = 7000 - width;
        if (x < maxX) {
            facing = RIGHT;
            state = RUNNING;
            x += isBig ? 70 : 50;
        }
}

void Mario::stopMoving() {
    state = STANDING;
}

void Mario::update() {

    animFrame = (animFrame + 1) % 3;

    if (!isOnGround) {
        state = JUMPING;
        vy += 2;
        if (vy > 20) vy = 20;
        y += vy;
    }
}

void Mario::draw(QPainter& painter, int scrollX) {
    QPixmap frame = getCurrentFrame();
    painter.drawPixmap(x - scrollX, y, width, height, frame);
}

QPixmap Mario::getCurrentFrame() {
    QString dir = (facing == LEFT) ? "L" : "R";
    QString act;
    int idx = 0;

    switch (state) {
        case STANDING: act = "S"; idx = 0; break;
        case RUNNING: act = "R"; idx = animFrame % 3; break;
        case JUMPING: act = "J"; idx = 1; break;
        case DYING:   return pixmapTable["DIE"];
    }

    QString sizeStr = isBig ? "B" : "S";
    QString key = QString("M%1_%2%3_%4").arg(sizeStr).arg(dir).arg(act).arg(idx);
    if (pixmapTable.contains(key)) {
        return pixmapTable[key];
    } else {
        qDebug() << "Missing pixmap:" << key;
        return QPixmap(); // fallback
    }
}

void Mario::loadAllPixmaps() {
    QStringList sizes = {"S", "B"};
    QStringList dirs = {"L", "R"};
    QStringList acts = {"S", "R", "J"};

    for (const QString& size : sizes) {
        for (const QString& dir : dirs) {
            for (const QString& act : acts) {
                int maxIdx = (act == "S") ? 1 : (act == "R" ? 3 : 4);
                for (int i = 0; i < maxIdx; ++i) {
                    QString key = QString("M%1_%2%3_%4").arg(size).arg(dir).arg(act).arg(i);
                    QString path = QString(":/Mario/data/M%1/%2.png").arg(size).arg(key);
                    pixmapTable[key] = QPixmap(path);
                }
            }
        }
    }

    pixmapTable["DIE"] = QPixmap(":/Mario/data/MS/MS_die.png");
}

void Mario::die() {
    state = DYING;
}

void Mario::land() {
    isOnGround = true;
    isJumping = false;
    vy = 0;

}

void Mario::setIsBig(bool x) {
    isBig = x;

    if (isBig) {
        width = 56;
        height = 80;  // 變大時，調整高度
    } else {
        width = 50;
        height = 50;   // 恢復小瑪利歐大小
    }
}

void Mario::enableFire() {
    fireballsLeft = 3;
}

bool Mario::canShoot() const {
    return fireballsLeft > 0;
}

bool Mario::shootFireball() {
    if (fireballsLeft > 0) {
        fireballsLeft--;
        return true;
    }
    return false;
}
