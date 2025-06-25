#pragma once

#include "Item.h"
#include <QPixmap>

class SuperMushroom : public Item {
public:
    SuperMushroom(int x, int y);

    void draw(QPainter& painter, int scrollX) override;
    bool checkCollision(const Mario& mario) const override;

    bool isCollected() const { return collected; }
    void setCollected(bool x) { collected = x; };

private:
    QPixmap pix;
    bool collected = false;
};
