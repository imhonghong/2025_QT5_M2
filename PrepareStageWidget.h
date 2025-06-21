#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class PrepareStageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrepareStageWidget(QWidget *parent = nullptr);

signals:
    void startGame();  // 用來通知 MainWindow 開始遊戲

private slots:
    void onStartButtonClicked();

private:
    QLabel* backgroundLabel;
    QPushButton* startButton;
};

