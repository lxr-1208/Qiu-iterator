#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    imageViewer *view = new imageViewer;
    view->show();
    return a.exec();
}
