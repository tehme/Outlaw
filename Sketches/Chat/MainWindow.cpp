#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QKeyEvent>
#include <QDebug>
#include "ChatHtmlFormatter.hpp"

//----------------------------------------------------------------------------//

ChatInputSendFilter::ChatInputSendFilter(QObject * parent) :
    QObject(parent)
{
}

ChatInputSendFilter::~ChatInputSendFilter()
{
}

bool ChatInputSendFilter::eventFilter(QObject * object, QEvent * event)
{
    Q_UNUSED(object);

    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Return && !keyEvent->isAutoRepeat())
        {
            emit sendPressed();
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------//

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateConnectButton();
    updateSendButton();

    m_connectionTimeout.setInterval(5000);
    m_connectionTimeout.setSingleShot(true);
    connect(&m_connectionTimeout, SIGNAL(timeout()), this, SLOT(onConnectTimeout()));

    ChatInputSendFilter * sendFilter = new ChatInputSendFilter(this);
    connect(sendFilter, SIGNAL(sendPressed()), this, SLOT(on_sendButton_clicked()));
    ui->sayLineEdit->installEventFilter(sendFilter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------//

void MainWindow::onChatMessageReceived(QString jsonMessage)
{
    QString htmlMessage = ChatHtmlFormatter::formatChatMessageToHtml(jsonMessage);
    appendHtml(htmlMessage);
}

//----------------------------------------------------------------------------//

void MainWindow::updateConnectButton()
{
    if(m_connectionState == ConnectionState::Disconnected)
    {
        const bool fieldsFilled = (!ui->hostLineEdit->text().isEmpty() && !ui->nameLineEdit->text().isEmpty());
        ui->connectButton->setText(QStringLiteral("Connect"));
        ui->connectButton->setEnabled(fieldsFilled);
    }
    else
    {
        ui->connectButton->setText(QStringLiteral("Disconnect"));
        ui->connectButton->setEnabled(true);
    }
}

void MainWindow::updateSendButton()
{
    ui->sendButton->setEnabled(m_connectionState == ConnectionState::Connected);
}

void MainWindow::changeConnectionState(ConnectionState newState)
{
    if(m_connectionState != newState)
    {
        m_connectionState = newState;
        updateConnectButton();
        updateSendButton();

        if(newState == ConnectionState::Connecting)
        {
            m_connectionTimeout.start();
        }
        else
        {
            m_connectionTimeout.stop();

            if(newState == ConnectionState::Disconnected)
            {
                cleanup();
            }
        }
    }
}

void MainWindow::appendHtml(const QString & htmlString)
{
    QTextCursor oldCursor = ui->chatWindow->textCursor();
    ui->chatWindow->moveCursor(QTextCursor::End);
    ui->chatWindow->insertHtml(htmlString);
    ui->chatWindow->setTextCursor(oldCursor);
}

void MainWindow::cleanup()
{
    if(m_tcpClient)
    {
        m_tcpClient->disconnect();
        m_tcpClient->deleteLater();
        m_tcpClient.release();
    }

    if(m_gameState)
    {
        m_gameState->disconnect();
        this->disconnect(m_gameState.get());
        m_gameState->deleteLater();
        m_gameState.release();
    }
}

void MainWindow::handleDisconnection(const QString & reportString)
{
    changeConnectionState(ConnectionState::Disconnected);

    qDebug() << qPrintable(reportString);
    appendHtml(QString("<b>%1</b><br>").arg(reportString));
}

//----------------------------------------------------------------------------//

void MainWindow::on_sendButton_clicked()
{
    if(!ui->sayLineEdit->text().isEmpty())
    {
        emit chatMessageSent(ui->sayLineEdit->text());
        ui->sayLineEdit->clear();
    }
}

void MainWindow::on_connectButton_clicked()
{
    if(m_connectionState == ConnectionState::Disconnected)
    {
        QString host = ui->hostLineEdit->text();
        quint16 port = ui->portSpinBox->value();
        QString name = ui->nameLineEdit->text();

        m_tcpClient.reset(new nc::TcpClient);
        m_gameState.reset(new GameState(host, port, name, *m_tcpClient.get()));

        connect(m_tcpClient.get(), SIGNAL(messageRead(QByteArray)), m_gameState.get(), SLOT(onInboundMessage(QByteArray)));
        connect(m_gameState.get(), SIGNAL(outboundMessage(QByteArray)), m_tcpClient.get(), SLOT(writeMessage(QByteArray)));
        connect(m_gameState.get(), SIGNAL(chatMessageReceived(QString)), this, SLOT(onChatMessageReceived(QString)));
        connect(this, SIGNAL(chatMessageSent(QString)), m_gameState.get(), SLOT(onChatMessageSent(QString)));
        connect(m_tcpClient.get(), SIGNAL(connected()), this, SLOT(onConnected()));
        connect(m_tcpClient.get(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        connect(m_tcpClient.get(), SIGNAL(socketError(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

        m_tcpClient->connectToHost(host, port);
        changeConnectionState(ConnectionState::Connecting);

        qDebug() << "Connecting.";
        appendHtml(QString("<b>Connecting to %1:%2.</b><br>").arg(host).arg(port));
    }
    else
    {
        m_tcpClient->disconnectFromHost();
    }
}

void MainWindow::on_hostLineEdit_textChanged(const QString & text)
{
    Q_UNUSED(text);

    updateConnectButton();
}

void MainWindow::on_nameLineEdit_textChanged(const QString & text)
{
    Q_UNUSED(text);

    updateConnectButton();
}

void MainWindow::onConnected()
{
    m_gameState->run();

    changeConnectionState(ConnectionState::Connected);

    qDebug() << "Connected.";
    appendHtml("<b>Connected.</b><br>");
}

void MainWindow::onDisconnected()
{
    handleDisconnection("Disconnected.");
}

void MainWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
    handleDisconnection("Socket error! Disconnected.");
    qDebug() << "Socket error:" << socketError;
}

void MainWindow::onConnectTimeout()
{
    handleDisconnection("Connection timed out. Disconnected.");
}

//----------------------------------------------------------------------------//
