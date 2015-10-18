#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QMap>
#include <QString>

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
    // TODO: create class that handles translation of chat JSONs to readable format.
    static QString assembleTextExtra(const QJsonObject & textObject);

private:
    Ui::MainWindow *ui;

    // TODO: move to class that handles chat text
    static const QMap<QString, QString> m_minecraftColorsAsHtml;
};

#endif // MAINWINDOW_HPP
