#include "configManager.h"
#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QTextStream>

ConfigManager::ConfigManager()
{
}

bool ConfigManager::save(const Config &config, const QString &path)
{
    bool result = false;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "v1.0" << "\n";
        out << config.sourceDir << "\n";
        out << config.destinationFile << "\n";
        out << config.filesFilters.join(';') << "\n";
        out << config.regex << "\n";
        out << config.regexNo << "\n";
        file.close();

        result = true;
        this->addRecentConfig(path);
    }

    return result;
}

Config* ConfigManager::load(const QString &path)
{
    Config *config = new Config;

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        if (in.readLine() != "v1.0") {
            file.close();
            return NULL;
        }
        config->sourceDir = in.readLine();
        config->destinationFile = in.readLine();
        config->filesFilters = in.readLine().split(';');
        config->regex = in.readLine();
        config->regexNo = in.readLine().toInt();
        file.close();

        this->addRecentConfig(path);
    }

    return config;
}

QStringList ConfigManager::getRecent()
{
    QSettings settings("grongor", "GettextExtractor");
    return settings.value("recentConfigs").toStringList();
}

void ConfigManager::addRecentConfig(const QString &path)
{
    QSettings settings("grongor", "GettextExtractor");
    QStringList recentList = settings.value("recentConfigs").toStringList();
    if (!recentList.contains(path)) {
        recentList.prepend(path);
    } else {
        recentList.removeOne(path);
        recentList.prepend(path);
    }
    if (recentList.count() > 10) {
        recentList.erase(recentList.begin() + 10, recentList.end());
    }
    settings.setValue("recentConfigs", recentList);
}
