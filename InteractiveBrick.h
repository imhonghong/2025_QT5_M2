#pragma once

#include "Brick.h"
#include <QPixmap>
#include "GameStageWidget.h"

enum class BrickContent {
    None,
    Coin,
    Mushroom,
    FireFlower
};

class InteractiveBrick : public Brick {
protected:
    BrickContent content;
    QPixmap activePixmap;
    QPixmap usedPixmap;

public:
    InteractiveBrick(int x, int y, BrickContent content, GameStageWidget* game);

    void draw(QPainter& painter, int scrollX) override;
    int onHitFromBelow() override;

private:
    GameStageWidget* game;
    bool used = false;

};
