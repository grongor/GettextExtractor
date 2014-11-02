#include "writer.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QDebug>

Writer::Writer(const QString phrasesFile) : phrasesFile(phrasesFile)
{

}

int Writer::removeOldPhrases()
{
    QFile file(this->phrasesFile);
    int phrasesCount = -2; // first two lines doesn't count

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            in.readLine();
            phrasesCount++;
        }
        file.resize(0);
        file.close();
    }

    return phrasesCount;
}

void Writer::write(const QMap<QString, QSet<QString>> phrases)
{
    QFile file(this->phrasesFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);
        out << "<?php" << endl << endl;
        foreach (QString phrase, phrases.keys()) {
            out << QString("_('%1');").arg(phrase.replace("'", "\'")) << " // ";
            QStringList list = QList<QString>::fromSet(phrases.value(phrase));
            list.sort();
            out << list.join(", ") << endl;
        }

        file.close();
    }
}
