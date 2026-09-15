#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QFileInfo>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(new Settings())
    , processor(new FileProcessor(this))
    , pollingTimer(new QTimer(this))
    , scanWatcher(new QFutureWatcher<QStringList>(this))
{
    ui->setupUi(this);
    setupUI();
    loadSettings();

    connect(processor, &FileProcessor::progressUpdated,
            this, &MainWindow::onProgressUpdated);
    connect(processor, &FileProcessor::fileProcessed,
            this, &MainWindow::onFileProcessed);
    connect(processor, &FileProcessor::processingFinished,
            this, &MainWindow::onProcessingFinished);
    connect(processor, &FileProcessor::statusMessage,
            this, &MainWindow::onStatusMessage);

    connect(pollingTimer, &QTimer::timeout,
            this, &MainWindow::onTimerTick);

    connect(scanWatcher, &QFutureWatcher<QStringList>::finished,
            this, &MainWindow::onScanFinished);
    connect(this, &MainWindow::scanProgressChanged,
            this, &MainWindow::onScanProgress);

    connect(ui->browseInputButton,  &QPushButton::clicked,
            this, &MainWindow::onBrowseInput);
    connect(ui->browseOutputButton, &QPushButton::clicked,
            this, &MainWindow::onBrowseOutput);
    connect(ui->startButton,        &QPushButton::clicked,
            this, &MainWindow::onStartProcessing);
    connect(ui->stopButton,         &QPushButton::clicked,
            this, &MainWindow::onStopProcessing);
    connect(ui->pauseButton,        &QPushButton::clicked,
            this, &MainWindow::onPauseProcessing);
    connect(ui->resumeButton,       &QPushButton::clicked,
            this, &MainWindow::onResumeProcessing);

    updateFileList();
    updateUIState();
}

MainWindow::~MainWindow()
{
    if (processor->isProcessing()) {
        processor->stopProcessing();
    }

    if (scanWatcher->isRunning()) {
        scanWatcher->waitForFinished();
    }

    saveSettings();

    delete settings;
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("File XOR Processor");
    resize(900, 700);

    statusBarProgress = new QProgressBar(this);
    statusBarProgress->setRange(0, 100);
    statusBarProgress->setValue(0);
    statusBarProgress->setFixedWidth(200);
    statusBarProgress->setVisible(false);

    statusBarLabel = new QLabel(this);
    statusBarLabel->setText("Готов к работе");

    statusBar()->addWidget(statusBarLabel, 1);
    statusBar()->addPermanentWidget(statusBarProgress);

    ui->fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->xorKeyEdit->setText("1234567890ABCDEF");
}

void MainWindow::loadSettings()
{
    settings->load();

    ui->fileMaskEdit->setText(settings->getFileMask());
    ui->inputPathEdit->setText(settings->getInputPath());
    ui->outputPathEdit->setText(settings->getOutputPath());
    ui->deleteSourceCheckBox->setChecked(settings->getDeleteSource());
    ui->overwriteCheckBox->setChecked(settings->getOverwriteSame());
    ui->timerCheckBox->setChecked(settings->getTimerEnabled());
    ui->intervalSpinBox->setValue(settings->getPollingInterval());
    ui->xorKeyEdit->setText(
        QString::number(settings->getXorKey(), 16).toUpper()
        );
}

void MainWindow::saveSettings()
{
    settings->setFileMask(ui->fileMaskEdit->text());
    settings->setInputPath(ui->inputPathEdit->text());
    settings->setOutputPath(ui->outputPathEdit->text());
    settings->setDeleteSource(ui->deleteSourceCheckBox->isChecked());
    settings->setOverwriteSame(ui->overwriteCheckBox->isChecked());
    settings->setTimerEnabled(ui->timerCheckBox->isChecked());
    settings->setPollingInterval(ui->intervalSpinBox->value());

    bool ok = false;
    quint64 key = ui->xorKeyEdit->text().toULongLong(&ok, 16);
    if (ok) {
        settings->setXorKey(key);
    }

    settings->save();
}

void MainWindow::onBrowseInput()
{
    QString path = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку для поиска файлов",
        ui->inputPathEdit->text()
        );

    if (path.isEmpty())
        return;

    QDir dir(path);
    if (dir.isRoot() || path.length() <= 3) {
        QMessageBox::warning(this, "Ошибка",
                             "Нельзя выбирать корень диска!\n"
                             "Выберите конкретную папку с файлами.");
        return;
    }

    QStringList forbiddenPaths = {
        "C:/Windows",
        "C:/Program Files",
        "C:/Program Files (x86)",
        "C:/ProgramData",
        "C:/Users"
    };

    for (const QString &forbidden : forbiddenPaths) {
        if (path.startsWith(forbidden, Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Ошибка",
                                 "Нельзя выбирать системную папку!\n"
                                 "Выберите папку с Вашими файлами.");
            return;
        }
    }

    ui->inputPathEdit->setText(path);
    updateFileList();
}

void MainWindow::onBrowseOutput()
{
    QString path = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку для сохранения результатов",
        ui->outputPathEdit->text()
        );

    if (!path.isEmpty()) {
        ui->outputPathEdit->setText(path);
    }
}

void MainWindow::updateFileList()
{
    if (isScanning) {
        statusBarLabel->setText("Сканирование уже выполняется...");
        return;
    }

    ui->fileListWidget->clear();
    pendingFiles.clear();

    QString inputPath = ui->inputPathEdit->text();
    if (inputPath.isEmpty())
        return;

    QDir dir(inputPath);
    if (!dir.exists()) {
        statusBarLabel->setText("Папка не существует: " + inputPath);
        return;
    }

    QString mask = ui->fileMaskEdit->text();
    if (mask.isEmpty())
        mask = "*.*";

    isScanning = true;
    statusBarLabel->setText("Сканирование папки...");
    statusBarProgress->setVisible(true);
    statusBarProgress->setRange(0, 0);   // "бегущая полоска"
    updateUIState();

    QFuture<QStringList> future = QtConcurrent::run(
        [inputPath, mask]() -> QStringList {
            QStringList files;

            QDirIterator it(inputPath,
                            QStringList() << mask,
                            QDir::Files,
                            QDirIterator::Subdirectories);

            while (it.hasNext()) {
                if (files.size() >= MainWindow::MAX_FILES)
                    break;

                files.append(it.next());
            }

            return files;
        }
        );

    scanWatcher->setFuture(future);
}

void MainWindow::onScanFinished()
{
    isScanning = false;
    statusBarProgress->setVisible(false);
    statusBarProgress->setRange(0, 100);
    statusBarProgress->setValue(0);

    QStringList files = scanWatcher->result();
    displayFoundFiles(files);

    if (files.size() >= MAX_FILES) {
        statusBarLabel->setText(
            QString("⚠ Найдено больше %1 файлов. Показаны первые %1. Уточните маску!")
                .arg(MAX_FILES));
    } else {
        statusBarLabel->setText(
            QString("Найдено файлов: %1").arg(files.size()));
    }

    updateUIState();
}

void MainWindow::onScanProgress(int current, int total)
{
    if (total > 0) {
        statusBarProgress->setRange(0, total);
        statusBarProgress->setValue(current);
        statusBarLabel->setText(
            QString("Сканирование: %1 из %2").arg(current).arg(total));
    }
}

void MainWindow::displayFoundFiles(const QStringList &files)
{
    pendingFiles = files;

    for (const QString &filePath : files) {
        QFileInfo fileInfo(filePath);
        QListWidgetItem *item = new QListWidgetItem(
            fileInfo.fileName() + " (" +
            QString::number(fileInfo.size() / 1024.0 / 1024.0, 'f', 2) +
            " MB)"
            );
        item->setData(Qt::UserRole, filePath);
        ui->fileListWidget->addItem(item);
    }
}

void MainWindow::onStartProcessing()
{
    if (isProcessing || isScanning)
        return;

    if (pendingFiles.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение",
                             "Нет файлов для обработки. Проверьте маску и путь.");
        return;
    }

    bool ok = false;
    quint64 key = ui->xorKeyEdit->text().toULongLong(&ok, 16);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка",
                             "Неверный формат XOR ключа.\n"
                             "Используйте hex формат (например, 1234567890ABCDEF)");
        return;
    }

    if (ui->outputPathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Не указана папка для сохранения результатов.");
        return;
    }

    processor->setXorKey(key);
    processor->setDeleteSource(ui->deleteSourceCheckBox->isChecked());
    processor->setOutputPath(ui->outputPathEdit->text());
    processor->setOverwriteSame(ui->overwriteCheckBox->isChecked());

    if (ui->timerCheckBox->isChecked()) {
        pollingTimer->start(ui->intervalSpinBox->value());
    } else {
        pollingTimer->stop();
    }

    isProcessing = true;
    processor->processFiles(pendingFiles);
    updateUIState();

    statusBarProgress->setVisible(true);
    statusBarProgress->setRange(0, 100);
    statusBarProgress->setValue(0);

    saveSettings();
}

void MainWindow::onStopProcessing()
{
    if (processor->isProcessing()) {
        processor->stopProcessing();

        while (processor->isProcessing()) {
            QCoreApplication::processEvents();
        }

        isProcessing = false;
        updateUIState();
        statusBarProgress->setVisible(false);
        statusBarLabel->setText("Остановлено пользователем");
        pollingTimer->stop();
    }
}

void MainWindow::onPauseProcessing()
{
    if (processor->isProcessing() && !processor->isPaused()) {
        processor->pauseProcessing();
        updateUIState();
    }
}

void MainWindow::onResumeProcessing()
{
    if (processor->isProcessing() && processor->isPaused()) {
        processor->resumeProcessing();
        updateUIState();
    }
}

void MainWindow::onTimerTick()
{
    if (isScanning || isProcessing)
        return;

    updateFileList();
}

void MainWindow::onProgressUpdated(int percent, const QString &currentFile)
{
    statusBarProgress->setValue(percent);
    statusBarLabel->setText(
        QString("Обработка: %1 (%2%)").arg(currentFile).arg(percent)
        );
}

void MainWindow::onFileProcessed(const QString &fileName, bool success)
{
    QList<QListWidgetItem*> items =
        ui->fileListWidget->findItems(fileName, Qt::MatchContains);

    if (!items.isEmpty()) {
        QListWidgetItem *item = items.first();
        item->setBackground(success ? Qt::green : Qt::red);
    }
}

void MainWindow::onProcessingFinished(bool completed)
{
    isProcessing = false;
    pollingTimer->stop();
    updateUIState();
    statusBarProgress->setVisible(false);

    if (completed) {
        statusBarLabel->setText("Обработка завершена");
        QMessageBox::information(this, "Готово",
                                 "Все файлы успешно обработаны!");
    } else {
        statusBarLabel->setText("Обработка прервана");
    }
}

void MainWindow::onStatusMessage(const QString &message)
{
    statusBarLabel->setText(message);
}

void MainWindow::updateUIState()
{
    bool processing = processor->isProcessing();
    bool paused     = processor->isPaused();
    bool busy       = processing || isScanning;

    // Кнопки
    ui->startButton->setEnabled(!busy);
    ui->stopButton->setEnabled(processing);
    ui->pauseButton->setEnabled(processing && !paused);
    ui->resumeButton->setEnabled(processing && paused);

    // Настройки
    ui->fileMaskEdit->setEnabled(!busy);
    ui->inputPathEdit->setEnabled(!busy);
    ui->outputPathEdit->setEnabled(!busy);
    ui->deleteSourceCheckBox->setEnabled(!busy);
    ui->overwriteCheckBox->setEnabled(!busy);
    ui->timerCheckBox->setEnabled(!busy);
    ui->intervalSpinBox->setEnabled(!busy);
    ui->xorKeyEdit->setEnabled(!busy);
    ui->browseInputButton->setEnabled(!busy);
    ui->browseOutputButton->setEnabled(!busy);
    ui->fileListWidget->setEnabled(!busy);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isScanning && scanWatcher->isRunning()) {
        statusBarLabel->setText("Ожидание завершения сканирования...");
        scanWatcher->waitForFinished();
    }

    if (processor->isProcessing()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Подтверждение",
            "Идёт обработка файлов. Вы уверены, что хотите выйти?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            processor->stopProcessing();

            while (processor->isProcessing()) {
                QCoreApplication::processEvents();
            }

            saveSettings();
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        saveSettings();
        event->accept();
    }
}