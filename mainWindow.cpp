#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "extractor.h"
#include <QPushButton>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidgetItem>
#include <configManager.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->sourcePath = this->ui->sourceInput->text();
    this->signalMapper = new QSignalMapper(this);
    this->loadRecentSettings();
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::on_sourceInput_textChanged(const QString &text)
{
    if (this->sourcePath != NULL) {
        QString destinationPath = this->ui->destinationInput->text();
        if (destinationPath == "") {
            QString newText = text;
            if (!newText.endsWith('/')) {
                newText.append('/');
            }
            this->ui->destinationInput->setText(newText + "phrases.php");
        } else if (destinationPath.startsWith(this->sourcePath)) {
            destinationPath.remove(0, this->sourcePath.length());

            QString newText = text;
            if (newText.endsWith('/')) {
                newText.chop(1);
            }

            if (!destinationPath.startsWith('/')) {
                destinationPath.prepend('/');
            }

            this->ui->destinationInput->setText(newText + destinationPath);
        }
    }

    this->sourcePath = text;
}

void MainWindow::on_sourceButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, tr("Select the source directory"), "", QFileDialog::ShowDirsOnly
    );

    if (dir != "") {
        this->ui->sourceInput->setText(dir);
    }
}

void MainWindow::on_destinationButton_clicked()
{
    QString file = QFileDialog::getSaveFileName(
        this, tr("Select the output file"), "", tr("PHP files (*.php)")
    );

    if (file != "") {
        this->ui->destinationInput->setText(file);
    }
}

void MainWindow::on_filesFiltersAddButton_clicked()
{
    QString text = this->ui->filesFiltersInput->text();
    if (text != "") {
        QListWidgetItem *item = new QListWidgetItem(text, this->ui->filesFiltersList);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        this->ui->filesFiltersInput->setText("");
    }
}

void MainWindow::on_filesFiltersEditButton_clicked()
{
    QList<QListWidgetItem*> items = this->ui->filesFiltersList->selectedItems();
    if (items.count() != 1) {
        return;
    }
    this->ui->filesFiltersList->editItem(items[0]);
}

void MainWindow::on_filesFiltersDeleteButton_clicked()
{
    QList<QListWidgetItem*> items = this->ui->filesFiltersList->selectedItems();
    if (items.count() != 1) {
        return;
    }
    delete items[0];
}

void MainWindow::on_startButton_clicked()
{
    QString sourcePath = this->ui->sourceInput->text();
    if (sourcePath == "" || !QDir(sourcePath).exists()) {
        this->showErrorMessage(tr("You must set the source directory."));
        return;
    }
    QString destinationPath = this->ui->destinationInput->text();
    if (destinationPath == "") {
        this->showErrorMessage(tr("You must set the destination path."));
        return;
    }
    QString regex = this->ui->regexInput->text();
    if (regex == "" || !QRegularExpression(regex).isValid()) {
        this->showErrorMessage(tr("You must set the valid regular expression."));
        return;
    }
    int regexNo = this->ui->regexNoSpinBox->value();

    QStringList filesFilters;
    for (int i = 0; i < this->ui->filesFiltersList->count(); i++) {
        filesFilters.append(this->ui->filesFiltersList->item(i)->text());
    }

    Extractor extractor;
    ExtractorResult result = extractor.extract(
        sourcePath, filesFilters, regex, regexNo, destinationPath
    );
    int changed = result.newPhrasesCount - result.oldPhrasesCount;

    QMessageBox message;
    message.setIcon(QMessageBox::Information);
    message.setWindowTitle(tr("Phrases extraction finished."));

    if (changed == 0) {
        message.setText(tr("There were no new phrases."));
    } else if (changed > 0) {
        message.setText(tr("Added %1 new phrases.").arg(changed));
    } else {
        message.setText(tr("Removed %1 old phrases.").arg(abs(changed)));
    }

    message.setInformativeText(
        tr("Before: %1, now: %2")
            .arg(result.oldPhrasesCount)
            .arg(result.newPhrasesCount)
    );
    message.exec();
}

void MainWindow::on_actionLoadSettings_triggered()
{
    QString configPath = QFileDialog::getOpenFileName(
        this, tr("Select configuration file"), "",
        tr("GettextExtractor config (*.gecfg)")
    );

    if (configPath == "") {
        return;
    }

    this->loadSettings(configPath);
    this->loadRecentSettings();
}

void MainWindow::on_actionSaveSettings_triggered()
{
    QString configPath = QFileDialog::getSaveFileName(
        this, tr("Select configuration file"), "",
        tr("GettextExtractor config (*.gecfg)")
    );

    if (configPath == "") {
        return;
    }

    Config config;
    config.sourceDir = this->ui->sourceInput->text();
    config.destinationFile = this->ui->destinationInput->text();
    QStringList filesFilters;
    for (int i = 0; i < this->ui->filesFiltersList->count(); i++) {
        filesFilters.append(this->ui->filesFiltersList->item(i)->text());
    }
    config.filesFilters = filesFilters;
    config.regex = this->ui->regexInput->text();
    config.regexNo = this->ui->regexNoSpinBox->value();

    ConfigManager cm;
    if (!cm.save(config, configPath)) {
        this->showErrorMessage(tr("Failed to save the configuration file."));
    } else {
        this->loadRecentSettings();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::onRecentSettingsActionTriggered(QString path)
{
    this->loadSettings(path);
}

void MainWindow::loadSettings(QString path)
{
    ConfigManager cm;
    Config *config = cm.load(path);
    if (config == NULL) {
        this->showErrorMessage(tr("Failed to load the configuration file."));
        return;
    }

    this->ui->sourceInput->setText(config->sourceDir);
    this->ui->destinationInput->setText(config->destinationFile);
    int count = this->ui->filesFiltersList->count();
    for (int i = 0; i < count; i++) {
        delete this->ui->filesFiltersList->takeItem(0);
    }
    foreach (QString itemText, config->filesFilters) {
        QListWidgetItem *item = new QListWidgetItem(itemText, this->ui->filesFiltersList);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    this->ui->regexInput->setText(config->regex);
    this->ui->regexNoSpinBox->setValue(config->regexNo);
    delete config;
}

void MainWindow::loadRecentSettings()
{
    ConfigManager cm;
    QStringList recentSettings = cm.getRecent();

    QMenu *menu = new QMenu();
    QAction *action;
    foreach (QString text, recentSettings) {
        action = new QAction(text, menu);
        menu->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), this->signalMapper, SLOT(map()));
        this->signalMapper->setMapping(action, text);
    }
    QObject::connect(this->signalMapper, SIGNAL(mapped(QString)), this, SLOT(onRecentSettingsActionTriggered(QString)));
    //delete this->ui->actionRecentSettings->menu();
    this->ui->actionRecentSettings->setMenu(menu);
}

void MainWindow::showErrorMessage(const QString &text)
{
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    message.setWindowTitle(tr("Error"));
    message.setText(text);
    message.exec();
}
