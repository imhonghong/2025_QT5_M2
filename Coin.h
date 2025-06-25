#pragma once
#include "Item.h"
#include <QPixmap>

class Coin : public Item {
public:
    Coin(int x, int y);

    void draw(QPainter& painter, int scrollX) override;
    bool checkCollision(const Mario& mario) const override;
    void setVisible(bool visible);
    bool isVisible() const;

private:
    QPixmap coinPixmap;
    bool visible;
};
