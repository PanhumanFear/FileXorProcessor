#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QListWidgetItem>
#include <QFutureWatcher>
#include "fileprocessor.h"

class Settings;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onBrowseInput();
    void onBrowseOutput();
    void onStartProcessing();
    void onStopProcessing();
    void onPauseProcessing();
    void onResumeProcessing();

    void onTimerTick();
    void updateFileList();

    void onScanFinished();
    void onScanProgress(int current, int total);

    void onProgressUpdated(int percent, const QString &currentFile);
    void onFileProcessed(const QString &fileName, bool success);
    void onProcessingFinished(bool completed);
    void onStatusMessage(const QString &message);

signals:
    void scanProgressChanged(int current, int total);

private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    void updateUIState();
    void displayFoundFiles(const QStringList &files);

    Ui::MainWindow *ui;

    Settings *settings;
    FileProcessor *processor;
    QTimer *pollingTimer;

    QFutureWatcher<QStringList> *scanWatcher;
    bool isScanning = false;

    QStringList pendingFiles;
    bool isProcessing = false;

    QProgressBar *statusBarProgress;
    QLabel *statusBarLabel;

    static const int MAX_FILES = 10000;
};

#endif // MAINWINDOW_H