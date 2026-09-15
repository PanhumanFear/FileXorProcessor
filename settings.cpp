#include "settings.h"
#include <QSettings>
#include <QDir>


Settings::Settings()
{
    QDir().mkdir(inputPath);
    QDir().mkdir(outputPath);

    load();
}


void Settings::save()
{
    QSettings settings("FileXorProcessor", "Settings");

    settings.setValue("fileMask",        fileMask);
    settings.setValue("deleteSource",    deleteSource);
    settings.setValue("outputPath",      outputPath);
    settings.setValue("inputPath",       inputPath);
    settings.setValue("overwriteSame",   overwriteSame);
    settings.setValue("timerEnabled",    timerEnabled);
    settings.setValue("pollingInterval", pollingInterval);
    settings.setValue("xorKey",          QVariant::fromValue(xorKey));

    settings.sync();
}


void Settings::load()
{
    QSettings settings("FileXorProcessor", "Settings");

    fileMask = settings.value("fileMask", "*.bin").toString();

    deleteSource = settings.value("deleteSource", false).toBool();

    outputPath = settings.value("outputPath",
                                QDir::currentPath() + "/output").toString();

    inputPath = settings.value("inputPath",
                               QDir::currentPath() + "/input").toString();

    overwriteSame = settings.value("overwriteSame", false).toBool();

    timerEnabled = settings.value("timerEnabled", false).toBool();

    pollingInterval = settings.value("pollingInterval", 5000).toInt();

    xorKey = settings.value("xorKey",
                            0x1234567890ABCDEFULL).toULongLong();
}