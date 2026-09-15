#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>

class Settings
{
public:
    Settings();

    QString getFileMask() const { return fileMask; }
    void setFileMask(const QString &mask) { fileMask = mask; }

    bool getDeleteSource() const { return deleteSource; }
    void setDeleteSource(bool delete_) { deleteSource = delete_; }

    QString getOutputPath() const { return outputPath; }
    void setOutputPath(const QString &path) { outputPath = path; }

    QString getInputPath() const { return inputPath; }
    void setInputPath(const QString &path) { inputPath = path; }

    bool getOverwriteSame() const { return overwriteSame; }
    void setOverwriteSame(bool overwrite) { overwriteSame = overwrite; }

    bool getTimerEnabled() const { return timerEnabled; }
    void setTimerEnabled(bool enabled) { timerEnabled = enabled; }

    int getPollingInterval() const { return pollingInterval; }
    void setPollingInterval(int interval) { pollingInterval = interval; }

    quint64 getXorKey() const { return xorKey; }
    void setXorKey(quint64 key) { xorKey = key; }

    void save();
    void load();

private:
    QString fileMask = "*.bin";
    bool deleteSource = false;
    QString outputPath = QDir::currentPath() + "/output";
    QString inputPath = QDir::currentPath() + "/input";
    bool overwriteSame = false;
    bool timerEnabled = false;
    int pollingInterval = 5000;
    quint64 xorKey = 0x1234567890ABCDEFULL;
};

#endif // SETTINGS_H