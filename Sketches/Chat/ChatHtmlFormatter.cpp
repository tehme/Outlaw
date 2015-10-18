#include "ChatHtmlFormatter.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

//----------------------------------------------------------------------------//

const QMap<QString, QString> ChatHtmlFormatter::m_minecraftColorsAsHtml =
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

//----------------------------------------------------------------------------//

QString ChatHtmlFormatter::formatChatMessageToHtml(const QString & jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    QString result;

    if(jsonObject.contains("text"))
    {
        result.append(assembleTextExtra(jsonObject));
        result.append(jsonObject.value("text").toString());
    }
    else if(jsonObject.contains("translate"))
    {
        QJsonValue withValue = jsonObject.value("with");
        QJsonObject withObject;
        withObject.insert("with", withValue);
        QString withString = QString::fromUtf8(QJsonDocument(withObject).toJson(QJsonDocument::Compact));

        result.append(jsonObject.value("translate").toString());
        result.append(": ");
        result.append(withString);
    }
    else
    {
        result.append("<font color=\"red\">Unknown:</color> ");
        result.append(jsonString);
    }

    result.append("<br>");

    return result;
}

QString ChatHtmlFormatter::assembleTextExtra(const QJsonObject & textObject)
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

            if(extraItemObj.contains("bold") && extraItemObj.value("bold").toBool() == true)
            {
                itemText.prepend("<b>");
                itemText.append("</b>");
            }
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

//----------------------------------------------------------------------------//
