#include "mainwindow.h"

#include <QApplication>
#include <QMap>
#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
//QSize Size(1000,1414);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/quetopage.txt");
    if (!file.open(QIODevice::ReadOnly)) {
       QMessageBox::information(nullptr,"","sdfs");
       return 0; // 或者处理错误
   }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList parts = line.split(' ');
        QString name = parts[2];
        int page = parts[6].toInt();
        int y = parts[7].toInt()/3509.0*Size.height();
        mp[name]=page*Size.height()+y;
    }

    file.close();

    TAG *Tag = new TAG;
    myScene *scene = new myScene;
    imageViewer *viewer = new imageViewer(scene,Tag);
    viewer->show();

    return a.exec();
}

