#ifndef STATSWIDGET_HPP
#define STATSWIDGET_HPP

#include <QWidget>

namespace Ui {
class StatsWidget;
}

class StatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatsWidget(QWidget *parent = nullptr);
    ~StatsWidget();

public slots:
    void onTimeChanged(qint64 time);
    void onHealthChanged(float health);

private:
    Ui::StatsWidget *ui;
};

#endif // STATSWIDGET_HPP
