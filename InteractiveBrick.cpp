#include "InteractiveBrick.h"
#include "SuperMushroom.h"
#include "Coin.h"
#include "FireFlower.h"
#include "GameStageWidget.h"
#include <QDebug>

InteractiveBrick::InteractiveBrick(int x, int y, BrickContent content, GameStageWidget* game)
    : Brick(x, y), content(content), game(game)
{
    width = height = 50;
    activePixmap = QPixmap(":/brick/data/brick/box brick.png");
    usedPixmap = QPixmap(":/brick/data/brick/stone brick.png");
}

void InteractiveBrick::draw(QPainter& painter, int scrollX) {
    if (!visible) return;

    QPixmap& toDraw = used ? usedPixmap : activePixmap;
    painter.drawPixmap(x - scrollX, y, width, height, toDraw);
}

int InteractiveBrick::onHitFromBelow() {
    if (used) return 0;
    used = true;

    int r = rand() % 3;  // 0 = coin, 1 = mushroom, 2 = fireflower
    Item* item = nullptr;

    if (r == 0)
        item = new Coin(x, y - 50);
    else if (r == 1)
        item = new SuperMushroom(x, y - 50);
    else
        item = new FireFlower(x, y - 50);

    if (item)
        game->addItem(item);

    return 0;  // 🔺 補上 return 值
}
