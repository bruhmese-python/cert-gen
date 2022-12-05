#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QString("Fusion"));
    MainWindow w;
    w.setWindowTitle("Batch-Certificate (Unititled)");
    w.show();
    return a.exec();
}
