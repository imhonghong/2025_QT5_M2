#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class FinishStageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinishStageWidget(QWidget *parent = nullptr);
    void setResult(bool win); // true 表示勝利，false 表示失敗

signals:
    void restartGame();
    void exitGame();

private slots:
    void onRestartButtonClicked();
    void onExitButtonClicked();

private:
    QLabel* resultLabel;
    QPushButton* restartButton;
    QPushButton* exitButton;
};

