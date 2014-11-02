#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QSet>

class Parser
{

public:
    Parser();
    QSet<QString> parse(const QString &filePath, const QString &regex, const int &regexNo);

private:


};

#endif // PARSER_H
