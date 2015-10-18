#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>


const QMap<QString, QString> MainWindow::m_minecraftColorsAsHtml =
{
    {"black",        "#000000"},
    {"dark_blue",    "#0000aa"},
    {"dark_green",   "#00aa00"},
    {"dark_aqua",    "#00aaaa"},
    {"dark_red",     "#aa0000"},
    {"dark_purple",  "#aa00aa"},
    {"gold",         "#ffaa00"},
    {"gray",         "#aaaaaa"},
    {"dark_gray",    "#555555"},
    {"blue",         "#5555ff"},
    {"green",        "#55ff55"},
    {"aqua",         "#55ffff"},
    {"red",          "#ff5555"},
    {"light_purple", "#ff55ff"},
    {"yellow",       "#ffff55"},
    {"white",        "#ffffff"}
};


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
    QJsonDocument doc = QJsonDocument::fromJson(jsonMessage.toUtf8());
    QJsonObject jsonObject = doc.object();

    if(jsonObject.contains("text"))
    {
        qDebug() << "Appending as text";

        QTextCursor oldCursor = ui->textEdit->textCursor();

        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->insertHtml(assembleTextExtra(jsonObject));
        ui->textEdit->insertHtml(jsonObject.value("text").toString());
        ui->textEdit->insertHtml("<br>");

        ui->textEdit->setTextCursor(oldCursor);
    }
    else if(jsonObject.contains("translate"))
    {
        qDebug() << "Appending as translatable";
        QJsonValue withVal = jsonObject.value("with");
        QJsonObject withObj;
        withObj.insert("with", withVal);
        QString withStr = QString::fromUtf8(QJsonDocument(withObj).toJson(QJsonDocument::Compact));

        QString line = QString("%1: %2")
            .arg(jsonObject.value("translate").toString())
            .arg(withStr);

        ui->textEdit->append(line);
    }
    else
    {
        qDebug() << "Appending as unknown";
        ui->textEdit->append("Unknown: " + jsonMessage);
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        emit chatMessageSent(ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

QString MainWindow::assembleTextExtra(const QJsonObject & textObject)
{
    QJsonArray extraArray = textObject.value("extra").toArray();
    QString result;
    for(QJsonValue extraItem : extraArray)
    {
        QString itemText;
        if(extraItem.isString())
        {
            itemText = extraItem.toString();
        }
        else if(extraItem.isObject())
        {
            QJsonObject extraItemObj = extraItem.toObject();
            QString htmlColor = m_minecraftColorsAsHtml.value(extraItemObj.value("color").toString());

            itemText=QString("<font color=\"%1\">%2</font>")
                .arg(htmlColor)
                .arg(extraItemObj.value("text").toString());
        }
        else
        {
            QString warningText = QString("%1: %2")
                .arg("Unknown item type in text message")
                .arg(extraItem.type());

            qWarning() << warningText;

            itemText=QString("<font color=\"red\">======== %1 ========</font>").arg(warningText);

        }

        result.append(itemText);
    }

    return result;
}
