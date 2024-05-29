#include "mainwindow.h"
#include <QtMath>

#include<iostream>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QSpinBox>
#include <QScrollBar>

imageViewer::imageViewer(QGraphicsView *parent):QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    this->setScene(scene);
    QGraphicsPixmapItem *images[720];
    QString filePath = "C:/Users/yifan/Desktop/big homework/gditerator/gddsImage";
    int height=0;
    for(int i=0;i<=100;i++)
    {
        QString path = filePath + QString("/%1.png").arg(i);
        QPixmap pixmap(path);
        pixmap = pixmap.scaled(1000,1200,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        images[i] = new QGraphicsPixmapItem;
        images[i]->setPixmap(pixmap);
//        qDebug()<<height<<" "<<pixmap.height()<<endl;
        images[i]->setPos(0,height);
//        images[i]->setFlag(QGraphicsItem::ItemIsSelectable);
//        images[i]->setFlag(QGraphicsItem::ItemIsMovable);
        height+=pixmap.height();
        images[i]->setAcceptTouchEvents(true);
        scene->addItem(images[i]);
    }
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setAttribute(Qt::WA_AcceptTouchEvents,true);
    setWindowFlags(Qt::Window);
    resize(1000,1414);

     QSpinBox *spinbox = new QSpinBox(this);
     spinbox->setRange(1,718);
     spinbox->setValue(1);
     spinbox->resize(100,50);
     scene->addWidget(spinbox);
     connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
         qDebug() << "Spinbox value changed to:" << value;
         verticalScrollBar()->setValue(1414*(value-1)+50);
     });
}
imageViewer::~imageViewer()
{
}
//void imageViewer::wheelEvent(QWheelEvent *event)
//{
//    QGraphicsView::wheelEvent(event);
//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    qreal scaleFactor = qPow(2, event->delta() / 240.0);
//    scale(scaleFactor, scaleFactor);
//}
//bool imageViewer::eventFilter(QObject *watched, QEvent *event)
//{
////    if(event->type()!=QEvent::PolishRequest) qDebug()<<event->type()<<endl;
//    return QGraphicsView::eventFilter(watched, event);
//}
void imageViewer::resizeEvent(QResizeEvent *event)
{
    int w = event->oldSize().width();
    int ww = event->size().width();
    qDebug()<<event->oldSize()<<" "<<event->size()<<endl;
    qreal s=1.0*ww/w;
    if(w!=-1)
    {
        scale(s,s);
    }
}
