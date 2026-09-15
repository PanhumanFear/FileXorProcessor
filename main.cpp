#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("FileXorProcessor");
    QApplication::setOrganizationName("FileXorProcessor");
    QApplication::setApplicationVersion("1.0.0");

    MainWindow window;
    window.show();

    return app.exec();
}