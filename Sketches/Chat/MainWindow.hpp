#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

//----------------------------------------------------------------------------//

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void chatMessageSent(QString chatMessage);

public slots:
    void onChatMessageReceived(QString jsonMessage);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

//----------------------------------------------------------------------------//

#endif // MAINWINDOW_HPP

//----------------------------------------------------------------------------//
