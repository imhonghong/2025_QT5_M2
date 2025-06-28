#include "MainWindow.h"
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setFixedSize(1400, 620);

    srand(time(NULL));

    stackedWidget = new QStackedWidget(this);

    prepareStage = new PrepareStageWidget(this);
    gameStage = new GameStageWidget(this);
    finishStage = new FinishStageWidget(this);

    stackedWidget->addWidget(prepareStage); // index 0
    stackedWidget->addWidget(gameStage);    // index 1
    stackedWidget->addWidget(finishStage);  // index 2

    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(prepareStage); // 初始畫面

    // 連接 Prepare → Game
    connect(prepareStage, &PrepareStageWidget::startGame, this, &MainWindow::switchToGameStage);

    // 連接 Game → Finish
    connect(gameStage, &GameStageWidget::gameWin, [=]() { switchToFinishStage(true); });
    connect(gameStage, &GameStageWidget::gameLose, [=]() { switchToFinishStage(false); });

    // 連接 Finish → Restart / Exit
    connect(finishStage, &FinishStageWidget::restartGame, this, &MainWindow::restartGame);
    connect(finishStage, &FinishStageWidget::exitGame, this, &MainWindow::exitGame);
}

void MainWindow::switchToGameStage()
{
    gameStage->reset(); // 可選：重設遊戲狀態
    stackedWidget->setCurrentWidget(gameStage);
}

void MainWindow::switchToFinishStage(bool win)
{
    finishStage->setResult(win);
    stackedWidget->setCurrentWidget(finishStage);
}

void MainWindow::restartGame()
{
    switchToGameStage(); // 重新開始遊戲
}

void MainWindow::exitGame()
{
    close(); // 關閉視窗
}
