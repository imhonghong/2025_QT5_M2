#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "PrepareStageWidget.h"
#include "GameStageWidget.h"
#include "FinishStageWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void switchToGameStage();
    void switchToFinishStage(bool win);
    void restartGame();
    void exitGame();

private:
    QStackedWidget* stackedWidget;
    PrepareStageWidget* prepareStage;
    GameStageWidget* gameStage;
    FinishStageWidget* finishStage;
};


