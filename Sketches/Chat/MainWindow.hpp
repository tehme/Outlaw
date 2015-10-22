#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>
#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <NetworkClient/TcpClient.hpp>
#include "GameState.hpp"

//----------------------------------------------------------------------------//

namespace Ui {
class MainWindow;
}

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

enum class ConnectionState
{
    Disconnected,
    Connecting,
    Connected
};

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

private:
    void updateConnectButton();
    void appendHtml(const QString & htmlString);
    void cleanup();
    void handleDisconnection(const QString & reportString);

private slots:
    void on_sendButton_clicked();
    void on_connectButton_clicked();
    void on_hostLineEdit_textChanged(const QString &text);
    void on_nameLineEdit_textChanged(const QString &text);
    void onConnected();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onConnectTimeout();

private:
    Ui::MainWindow                 * ui;
    std::unique_ptr<nc::TcpClient>   m_tcpClient = nullptr;
    std::unique_ptr<GameState>       m_gameState = nullptr;
    ConnectionState                  m_connectionState = ConnectionState::Disconnected;
    QTimer                           m_connectionTimeout;
};

//----------------------------------------------------------------------------//

#endif // MAINWINDOW_HPP

//----------------------------------------------------------------------------//
