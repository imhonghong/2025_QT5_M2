#include "FinishStageWidget.h"

FinishStageWidget::FinishStageWidget(QWidget *parent)
    : QWidget(parent)
{
    resultLabel = new QLabel("You Win!", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: green;");

    restartButton = new QPushButton("Restart", this);
    restartButton->setFixedSize(150, 40);

    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(150, 40);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(resultLabel);
    layout->addSpacing(30);
    layout->addWidget(restartButton, 0, Qt::AlignCenter);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch();
    setLayout(layout);

    connect(restartButton, &QPushButton::clicked, this, &FinishStageWidget::onRestartButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &FinishStageWidget::onExitButtonClicked);
}

void FinishStageWidget::setResult(bool win)
{
    if (win) {
        resultLabel->setText("You Win!");
        resultLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: green;");
    } else {
        resultLabel->setText("You Lose!");
        resultLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: red;");
    }
}

void FinishStageWidget::onRestartButtonClicked()
{
    emit restartGame();
}

void FinishStageWidget::onExitButtonClicked()
{
    emit exitGame();
}
