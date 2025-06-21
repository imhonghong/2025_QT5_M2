#include "PrepareStageWidget.h"
#include <QPixmap>

PrepareStageWidget::PrepareStageWidget(QWidget *parent)
    : QWidget(parent)
{

    // ✅ 背景圖片
    QLabel* backgroundLabel = new QLabel(this);
    QPixmap bgPixmap(":/bg/data/bg/start_screen.png");
    backgroundLabel->setPixmap(bgPixmap.scaled(1400, 620, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, 1400, 620);
    backgroundLabel->lower();

    double widRatio = 1400/1080;
    // ✅ Start 按鈕使用圖片
    startButton = new QPushButton(this);
    QPixmap buttonPixmap(":/bg/data/bg/start_btn.png");
    QPixmap scaledPixmap = buttonPixmap.scaled(196*widRatio*1.3, 46*1.05, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    startButton->setIcon(QIcon(scaledPixmap));
    startButton->setIconSize(scaledPixmap.size());
    startButton->setFixedSize(scaledPixmap.size());
    startButton->move(550, 450);
    startButton->setFlat(true);
    startButton->setStyleSheet("background: transparent;");

    connect(startButton, &QPushButton::clicked, this, &PrepareStageWidget::onStartButtonClicked);
}

void PrepareStageWidget::onStartButtonClicked()
{
    emit startGame();  // 通知 MainWindow 切換到 GameStage
}
