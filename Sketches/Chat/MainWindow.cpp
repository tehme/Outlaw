#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include "ChatHtmlFormatter.hpp"

//----------------------------------------------------------------------------//

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChatMessageReceived(QString jsonMessage)
{
    QTextCursor oldCursor = ui->textEdit->textCursor();
    ui->textEdit->moveCursor(QTextCursor::End);

    QString htmlMessage = ChatHtmlFormatter::formatChatMessageToHtml(jsonMessage);
    ui->textEdit->insertHtml(htmlMessage);

    ui->textEdit->setTextCursor(oldCursor);
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        emit chatMessageSent(ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

//----------------------------------------------------------------------------//
