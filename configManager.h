#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QStringList>
#include <QDataStream>

struct Config {
    QString sourceDir;
    QString destinationFile;
    QStringList filesFilters;
    QString regex;
    int regexNo;
};

class ConfigManager
{

public:
    ConfigManager();
    bool save(const Config &config, const QString &path);
    Config* load(const QString &path);
    QStringList getRecent();

private:
    void addRecentConfig(const QString &path);

};

#endif // CONFIG_H
