#include "parser.h"
#include <QFile>
#include <QSet>
#include <QTextStream>
#include <QRegularExpression>

Parser::Parser()
{
}

QSet<QString> Parser::parse(const QString &filePath, const QString &regex, const int &regexNo)
{
    QFile file(filePath);

    QRegularExpression qregex(regex);
    QRegularExpressionMatchIterator result;

    QString captured;

    QSet<QString> phrases;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString content = in.readAll();
        result = qregex.globalMatch(content);
        while (result.hasNext()) {
            QRegularExpressionMatch match = result.next();
            captured = match.captured(regexNo);
            if (captured != "") {
                phrases.insert(captured);
            }
        }
    }

    return phrases;
}
