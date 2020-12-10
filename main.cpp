#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));

    MainWindow w;
    w.show();

    return a.exec();
}
