#ifndef CHATHTMLFORMATTER_HPP
#define CHATHTMLFORMATTER_HPP

#include <QMap>
#include <QString>
#include <QJsonObject>

//----------------------------------------------------------------------------//

class ChatHtmlFormatter
{
public:
    static QString formatChatMessageToHtml(const QString & jsonString);

private:
    static QString assembleTextExtra(const QJsonObject & textObject);

private:
    static const QMap<QString, QString> m_minecraftColorsAsHtml;
};

//----------------------------------------------------------------------------//

#endif // CHATHTMLFORMATTER_HPP

//----------------------------------------------------------------------------//
