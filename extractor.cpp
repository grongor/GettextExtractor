#include "extractor.h"
#include "parser.h"
#include "writer.h"
#include <QDirIterator>
#include <QMap>
#include <QSet>
#include <QtDebug>

Extractor::Extractor()
{
}


ExtractorResult Extractor::extract(const QString &sourcePath, const QStringList &filesFilter,
                                   const QString &regex, const int &regexNo, const QString &destinationPath)
{
    Parser parser;

    QMap<QString, QSet<QString>> allPhrases;
    QSet<QString> phrases;
    QDir basePathDir(sourcePath);
    QStringList files = this->getAllMatchingFiles(sourcePath, filesFilter);
    foreach (QString file, files) {
        phrases = parser.parse(file, regex, regexNo);
        foreach (QString phrase, phrases) {
            allPhrases[phrase].insert(basePathDir.relativeFilePath(file));
        }
    }

    Writer writer(destinationPath);
    int oldPhrasesCount = writer.removeOldPhrases();
    writer.write(allPhrases);

    return ExtractorResult {oldPhrasesCount, allPhrases.count()};
}

QStringList Extractor::getAllMatchingFiles(const QString &path, const QStringList &filesFilters)
{
    QStringList result;

    QDirIterator dirIterator(path, filesFilters, QDir::Files | QDir::NoDotAndDotDot,
                             QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    while (dirIterator.hasNext()) {
        result.append(dirIterator.next());
    }

    return result;
}
