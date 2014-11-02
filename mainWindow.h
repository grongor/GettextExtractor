#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString sourcePath = NULL;
    QSignalMapper *signalMapper;

    void loadSettings(QString path);
    void loadRecentSettings();
    void showErrorMessage(const QString &text);

public slots:

private slots:
    void on_sourceInput_textChanged(const QString &text);
    void on_sourceButton_clicked();
    void on_destinationButton_clicked();
    void on_filesFiltersAddButton_clicked();
    void on_filesFiltersEditButton_clicked();
    void on_filesFiltersDeleteButton_clicked();
    void on_startButton_clicked();
    void on_actionLoadSettings_triggered();
    void on_actionSaveSettings_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void onRecentSettingsActionTriggered(QString path);

};

#endif // MAINWINDOW_H
