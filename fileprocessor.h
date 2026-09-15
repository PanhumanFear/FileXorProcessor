#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QObject>
#include <QFile>
#include <QFuture>
#include <QtConcurrent>
#include <QAtomicInt>
#include <QMutex>
#include <QWaitCondition>
#include <QProgressDialog>

class FileProcessor : public QObject
{
    Q_OBJECT

public:
    explicit FileProcessor(QObject *parent = nullptr);
    ~FileProcessor();

    void setXorKey(quint64 key) { xorKey = key; }
    void setDeleteSource(bool delete_ ) { deleteSource = delete_; }
    void setOutputPath(const QString &path) { outputPath = path; }
    void setOverwriteSame(bool overwrite) { overwriteSame = overwrite; }

    void processFiles(const QStringList &files);
    void pauseProcessing();
    void resumeProcessing();
    void stopProcessing();

    bool isProcessing() const { return processing; }
    bool isPaused() const { return paused; }
    int getProgress() const { return progress; }

signals:
    void progressUpdated(int percent, const QString &currentFile);
    void fileProcessed(const QString &fileName, bool success);
    void processingFinished(bool completed);
    void statusMessage(const QString &message);

private:
    void processSingleFile(const QString &filePath);
    void updateProgress();

    quint64 xorKey = 0;
    bool deleteSource = false;
    QString outputPath;
    bool overwriteSame = false;

    QFuture<void> future;
    QAtomicInt processing;
    QAtomicInt paused;
    QAtomicInt stopRequested;
    QAtomicInt progress;
    int totalFiles = 0;
    int processedFiles = 0;

    QMutex mutex;
    QWaitCondition pauseCondition;
    QProgressDialog *progressDialog = nullptr;
};

#endif // FILEPROCESSOR_H
