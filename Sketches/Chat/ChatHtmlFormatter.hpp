#ifndef CHATHTMLFORMATTER_HPP
#define CHATHTMLFORMATTER_HPP

#include <QMap>
#include <QString>
#include <QPair>
#include <QJsonObject>

//----------------------------------------------------------------------------//

class ChatHtmlFormatter
{
public:
    static QString formatChatMessageToHtml(const QString & jsonString);

private:
    static QString assembleTextExtra(const QJsonObject & textObject);
    static bool hasTextModifier(const QJsonObject & extraObject, const QString & modifier);
    static QPair<QString, QString> makeTextModifiersAsHtml(const QJsonObject & extraObject);

private:
    static const QMap<QString, QString> m_minecraftColorsAsHtml;
    static const QMap<QString, QString> m_textModifiersAsHtml;
};

//----------------------------------------------------------------------------//

#endif // CHATHTMLFORMATTER_HPP

//----------------------------------------------------------------------------//
