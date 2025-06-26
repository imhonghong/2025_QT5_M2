#include "SuperMushroom.h"
#include "Mario.h"

#include <QDebug>
SuperMushroom::SuperMushroom(int x, int y)
    : Item(x, y, 50, 50), pix(":/item/data/item/super mushroom.png") {}

void SuperMushroom::draw(QPainter& painter, int scrollX) {
    if (!collected) {
        painter.drawPixmap(x - scrollX, y, width, height, pix);
    }
}

bool SuperMushroom::checkCollision(const Mario& mario) const {
    if (collected) return false;

    QRect mushroomRect(x, y, width, height);
    QRect marioRect = mario.getRect();  // 你 Mario 類別已有 getRect()
    return mushroomRect.intersects(marioRect);
}

void SuperMushroom::checkPipeCollision(const QRect& pipeRect) {
    QRect selfRect(x, y, width, height);

    if (vx > 0 && selfRect.right() >= pipeRect.left() && selfRect.left() < pipeRect.left()
        && selfRect.bottom() > pipeRect.top() && selfRect.top() < pipeRect.bottom()) {
        qDebug() << "[SM] checking pipe:" << pipeRect << " vs " << selfRect;
        x = pipeRect.left() - width;  // 推回左邊
        vx = -vx;
    }
    else if (vx < 0 && selfRect.left() <= pipeRect.right() && selfRect.right() > pipeRect.right()
             && selfRect.bottom() > pipeRect.top() && selfRect.top() < pipeRect.bottom()) {
        qDebug() << "[SM] checking pipe:" << pipeRect << " vs " << selfRect;
        x = pipeRect.right();         // 推回右邊
        vx = -vx;
    }
}

void SuperMushroom::update() {
    if (collected) return;

    switch (state) {
    case Emerging:
        if (emergeCounter < 10) {
            y -= 2;
            emergeCounter++;
        } else {
            state = Walking;
        }
        break;

    case Walking:
        x += vx;
        // 可呼叫外部 checkPipeCollision 或 checkBlockCollision 進行反向
        break;

    case Falling:
        vy += 1;
        y += static_cast<int>(vy);
        break;
    }
}
