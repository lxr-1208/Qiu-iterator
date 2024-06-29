#include "mainwindow.h"
#include<iostream>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QSpinBox>
#include <QScrollBar>
#include <QShortcut>
#include <QInputDialog>
#include <QDir>
#include <QtGlobal>

QSize Size(1000,1414);
QMap<QString,int> mp;
imageViewer::imageViewer(myScene *sce,QGraphicsView *parent ):QGraphicsView(parent),scene(sce)
{
    setScene(scene);
    resize(Size);
    currentPage = 1;
    scene->myUpdate(1);
    spinbox = new QSpinBox(this);
    spinbox->setRange(1,718);
    spinbox->resize(100,50);
    scene->addWidget(spinbox);
    connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &imageViewer::turnto);

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    QObject::connect(shortcut, &QShortcut::activated, this, &imageViewer::onFind);

    centerOn(Size.width()/2,Size.height()/2);

    spinbox->blockSignals(true);
    spinbox->setValue(1);
    spinbox->blockSignals(false);
}
imageViewer::~imageViewer()
{

}
void imageViewer::resizeEvent(QResizeEvent *event)
{
    int w = event->oldSize().width();
    int ww = event->size().width();
    qreal s=1.0*ww/w;
    if(w!=-1)
    {
        scale(s,s);
    }
}
void imageViewer::onFind()
{
    QString input = QInputDialog::getText(this, "输入框","请输入想跳转的题号：");
    qDebug()<<input<<endl;
    if(mp.contains(input)==false)
    {
        QMessageBox::information(this,"Error","格式错误，参考：7.1.1定理1",QMessageBox::Ok);
        return;
    }
    sons[++cntSon] = new imageViewer(scene);
    sons[cntSon]->turnto(mp[input]+1);
    sons[cntSon]->move(50,50);
    sons[cntSon]->show();
}
void imageViewer::turnto(int i)
{
    scrollblock = true;
    centerOn(Size.width()/2,Size.height()*(i-1)+707);
    scrollblock = false;
    if(currentPage!=i)
    {
        scene->del(currentPage);
        scene->myUpdate(i);
    }
    currentPage = i;
}
void imageViewer::turnto1(int y)
{
    scrollblock = true;
    centerOn(Size.width()/2,y);
    scrollblock = false;
    int i = (y-Size.height()/2)/Size.height() +1;
    if(currentPage!=i)
    {
        scene->del(currentPage);
        scene->myUpdate(i);
    }
    currentPage = i;
}
void imageViewer::scrollContentsBy(int dx, int dy)
{
    // 获取视图中心点在视图坐标系中的位置（通常是宽度和高度的一半）
    if(scrollblock==true)
    {
        QGraphicsView::scrollContentsBy(dx,dy);
        return;
    }
    QPoint centerInView = rect().topLeft();
    QPointF centerInScene = mapToScene(centerInView); // 将视图中心点转换为场景坐标
    int y = centerInScene.y();
    int t = (y/Size.height())+1;
    if(currentPage!=t)
    {
        scene->del(currentPage);
        scene->myUpdate(t);
    }
    currentPage = t;
    qDebug() << y<<" Currentpage:"<<currentPage;
    spinbox->blockSignals(true);
    spinbox->setValue(currentPage);
    spinbox->blockSignals(false);
    QGraphicsView::scrollContentsBy(dx,dy);

}
myScene::myScene()
{
    setSceneRect(0,0,Size.width(),Size.height()*718);
    memset(cnt,0,sizeof(cnt));
    for(int i=1;i<=718;i++)
    {
        item[i] = new QGraphicsPixmapItem;
        item[i]->setPixmap(QPixmap());
    }
}
void myScene::del(int page)
{
    for(int i=qMax(-1,-page+1);i<=1;i++)
    {
        qDebug()<<"del:"<<i+page<<endl;
        cnt[i+page]--;
        if(cnt[i+page]!=0) continue;
        item[page+i]->setPixmap(QPixmap());
        removeItem(item[page+i]);
    }
}
void myScene::myUpdate(int page)
{
    QDir cur = QDir::currentPath();
    for(int i=qMax(-1,-page+1);i<=1;i++)
    {
        qDebug()<<"add:"<<i+page<<endl;
        cnt[i+page]++;
        if(cnt[i+page]!=1) continue;
        QPixmap pixmap(QString(cur.path()+"/gddsImage/%1.png").arg(page+i-1));
        pixmap = pixmap.scaled(1000,1414,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        item[page+i]->setPixmap(pixmap);
        addItem(item[page+i]);
        item[page+i]->setPos(0,Size.height()*(page+i-1));
    }
}

