#ifndef READER_H
#define READER_H

#include <QStringList>

struct ExtractorResult {
    int oldPhrasesCount;
    int newPhrasesCount;
};

class Extractor
{

public:
    Extractor();
    ExtractorResult extract(const QString &path, const QStringList &filesFilter,
                            const QString &regex, const int &regexNo, const QString &destinationPath);

private:
    QStringList getAllMatchingFiles(const QString &path, const QStringList &filesFilter);

};

#endif // READER_H
