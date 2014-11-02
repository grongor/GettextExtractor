#ifndef WRITER_H
#define WRITER_H

#include <QString>
#include <QStringList>

class Writer
{

public:
    Writer(const QString phrasesFile);
    int removeOldPhrases();
    void write(const QMap<QString, QSet<QString>> phrases);

private:
    QString phrasesFile;

};

#endif // WRITER_H
