#include "fileprocessor.h"
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>



FileProcessor::FileProcessor(QObject *parent)
    : QObject(parent)
{
    processing = false;
    paused = false;
    stopRequested = false;
    progress = 0;
}


FileProcessor::~FileProcessor()
{
    stopProcessing();
    if (future.isRunning())
        future.waitForFinished();
}


void FileProcessor::processFiles(const QStringList &files)
{
    if (files.isEmpty()) {
        emit statusMessage("Нет файлов для обработки");
        return;
    }

    if (processing) {
        emit statusMessage("Обработка уже выполняется");
        return;
    }

    QDir outputDir(outputPath);
    if (!outputDir.exists()) {
        if (!outputDir.mkpath(".")) {
            emit statusMessage("Не удалось создать папку вывода: " + outputPath);
            return;
        }
    }


    processing = true;
    paused = false;
    stopRequested = false;
    progress = 0;
    totalFiles = files.size();
    processedFiles = 0;

    emit statusMessage(QString("Начинаем обработку %1 файлов").arg(totalFiles));


    future = QtConcurrent::run([this, files]() {
        for (const QString &file : files) {
            if (stopRequested) {
                break;
            }

            mutex.lock();
            while (paused && !stopRequested) {
                pauseCondition.wait(&mutex);
            }
            mutex.unlock();

            if (stopRequested) {
                break;
            }

            processSingleFile(file);
            processedFiles++;
            updateProgress();


            emit progressUpdated(progress, QFileInfo(file).fileName());
        }

        processing = false;
        emit processingFinished(!stopRequested);
        emit statusMessage(stopRequested
                               ? "Обработка остановлена"
                               : "Обработка завершена");
    });
}


void FileProcessor::processSingleFile(const QString &filePath)
{
    QFile inputFile(filePath);
    QFileInfo fileInfo(filePath);

    if (!inputFile.exists()) {
        emit fileProcessed(fileInfo.fileName(), false);
        emit statusMessage("Файл не найден: " + fileInfo.fileName());
        return;
    }


    QString baseName = fileInfo.baseName();
    QString extension = fileInfo.suffix();
    QString outputFileName = baseName + "_xor." + extension;
    QString outputFilePath = QDir(outputPath).filePath(outputFileName);


    if (!overwriteSame && QFile::exists(outputFilePath)) {
        int counter = 1;
        while (true) {
            outputFileName = baseName + "_xor_" + QString::number(counter)
            + "." + extension;
            outputFilePath = QDir(outputPath).filePath(outputFileName);
            if (!QFile::exists(outputFilePath))
                break;
            counter++;
        }
    }

    if (!inputFile.open(QIODevice::ReadOnly)) {
        emit fileProcessed(fileInfo.fileName(), false);
        emit statusMessage("Не удалось открыть файл: " + fileInfo.fileName());
        return;
    }

    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        inputFile.close();
        emit fileProcessed(fileInfo.fileName(), false);
        emit statusMessage("Не удалось создать файл: " + outputFilePath);
        return;
    }

    const quint64 BUFFER_SIZE = 1024 * 1024;
    QByteArray buffer;
    buffer.resize(BUFFER_SIZE);

    qint64 totalBytes = inputFile.size();
    qint64 processedBytes = 0;

    quint8 keyBytes[8];
    for (int i = 0; i < 8; ++i) {
        keyBytes[i] = (xorKey >> (i * 8)) & 0xFF;
    }

    while (!inputFile.atEnd() && !stopRequested) {
        // Проверяем паузу перед каждым блоком
        mutex.lock();
        while (paused && !stopRequested) {
            pauseCondition.wait(&mutex);
        }
        mutex.unlock();

        if (stopRequested) {
            break;
        }

        qint64 bytesRead = inputFile.read(buffer.data(), BUFFER_SIZE);
        if (bytesRead <= 0)
            break;

        quint8 *data = reinterpret_cast<quint8*>(buffer.data());
        for (qint64 i = 0; i < bytesRead; ++i) {
            data[i] ^= keyBytes[i % 8];
        }

        if (outputFile.write(buffer.data(), bytesRead) != bytesRead) {
            emit fileProcessed(fileInfo.fileName(), false);
            emit statusMessage("Ошибка записи в файл: " + outputFilePath);
            break;
        }

        processedBytes += bytesRead;

        int fileProgress = (totalBytes > 0)
                               ? static_cast<int>(processedBytes * 100 / totalBytes)
                               : 0;
        emit progressUpdated(fileProgress, fileInfo.fileName());
    }

    inputFile.close();
    outputFile.close();

    if (!stopRequested && !paused) {
        if (deleteSource) {
            if (QFile::remove(filePath)) {
                emit statusMessage("Исходный файл удален: " + fileInfo.fileName());
            } else {
                emit statusMessage("Не удалось удалить файл: " + fileInfo.fileName());
            }
        }
        emit fileProcessed(fileInfo.fileName(), true);
        emit statusMessage("Обработан: " + fileInfo.fileName()
                           + " -> " + outputFileName);
    }
}



void FileProcessor::pauseProcessing()
{
    if (processing && !paused) {
        paused = true;
        emit statusMessage("Обработка приостановлена");
    }
}


void FileProcessor::resumeProcessing()
{
    if (processing && paused) {
        paused = false;
        pauseCondition.wakeAll();
        emit statusMessage("Обработка возобновлена");
    }
}


void FileProcessor::stopProcessing()
{
    if (processing) {
        stopRequested = true;
        if (paused) {
            paused = false;
            pauseCondition.wakeAll();
        }
        if (future.isRunning()) {
            future.waitForFinished();
        }
        emit statusMessage("Остановка обработки...");
    }
}


void FileProcessor::updateProgress()
{
    if (totalFiles > 0) {
        progress = (processedFiles * 100) / totalFiles;
    }
}