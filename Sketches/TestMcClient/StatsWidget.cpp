#include "StatsWidget.hpp"
#include "ui_StatsWidget.h"
#include <QString>

StatsWidget::StatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatsWidget)
{
    ui->setupUi(this);
}

StatsWidget::~StatsWidget()
{
    delete ui;
}

void StatsWidget::onTimeChanged(qint64 time)
{
    qint64 dayTime = time % 24000;

    int day = time / 24000;
    int mins = dayTime / 20 / 60;
    int secs = dayTime / 20 % 60;

    ui->labelTime->setText(QString("Day %1; %2:%3").arg(day).arg(mins).arg(secs));
    ui->labelTime->adjustSize();
}

void StatsWidget::onHealthChanged(float health)
{
    ui->labelHealth->setText(QString::number(health));
}

void StatsWidget::onFoodChanged(int food, float saturation)
{
    ui->labelFood->setText(QString("%1+%2").arg(food).arg(saturation));
}
