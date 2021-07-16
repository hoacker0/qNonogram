#include "mainwindow.h"
#include <QLocale>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/qnonogram.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
