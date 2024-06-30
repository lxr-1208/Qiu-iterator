#include "mainwindow.h"
#include<iostream>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QSpinBox>
#include <QComboBox>
#include <QScrollBar>
#include <QShortcut>
#include <QInputDialog>
#include <QDir>
#include <QtGlobal>
#include <QVBoxLayout>
#include <QPushButton>


QSize Size(1000,1414);
QMap<QString,int> mp;
imageViewer::imageViewer(myScene *sce,TAG *Tag,QGraphicsView *parent ):QGraphicsView(parent),scene(sce)
{
    tag=Tag;
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

    QShortcut *shortcut1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this);
    QObject::connect(shortcut1, &QShortcut::activated, this, &imageViewer::highlight);

    QShortcut *shortcut2 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    QObject::connect(shortcut2, &QShortcut::activated, this, &imageViewer::searchtag);

    QShortcut *shortcut3 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this);
    QObject::connect(shortcut3, &QShortcut::activated, this, &imageViewer::addtag);

    QShortcut *shortcut4 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);
    QObject::connect(shortcut4, &QShortcut::activated, this, &imageViewer::copy);

    centerOn(Size.width()/2,Size.height()/2);

    spinbox->blockSignals(true);
    spinbox->setValue(1);
    spinbox->blockSignals(false);

    resize(1000,800);
}
imageViewer::~imageViewer()
{
    scene->del(currentPage);
}
void imageViewer::resizeEvent(QResizeEvent *event)
{
    int w = event->oldSize().width();
    int ww = event->size().width();
//    qDebug()<<w<<" "<<ww<<endl;
    qreal s=1.0*ww/w;
    if(w!=-1)
    {
        scale(s,s);
    }
}
void imageViewer::onFind()
{
    bool ok;
    QString input = QInputDialog::getText(this, "输入框","请输入想跳转的题号：\n参考:7.1.1定理1 8.2.2例2",QLineEdit::Normal,"",&ok);
    if(ok==false) return;
//    qDebug()<<input<<endl;
    if(mp.contains(input)==false)
    {
        QMessageBox::information(this,"Error","格式错误或者对象不存在，参考：7.1.1定理1",QMessageBox::Ok);
        return;
    }
    sons[++cntSon] = new imageViewer(scene,tag);
    sons[cntSon]->turnto1(mp[input]);
    QPoint pos = mapToGlobal(QPoint(0,0));
    sons[cntSon]->move(pos.x()+50,pos.y()+50);
    sons[cntSon]->show();
}
void imageViewer::turnto(int i)
{
    scrollblock = true;
    centerOn(Size.width()/2,Size.height()*(i-1)+size().height()/2);
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
//    qDebug() << y<<" Currentpage:"<<currentPage;
    spinbox->blockSignals(true);
    spinbox->setValue(currentPage);
    spinbox->blockSignals(false);
    QGraphicsView::scrollContentsBy(dx,dy);

}
void imageViewer::closeEvent(QCloseEvent *event)
{
//    qDebug()<<"DELETE";
    delete this;
}
void imageViewer::highlight()
{
     bool ok;
     QString input = QInputDialog::getText(this, "输入框","你想标注哪道例题上：",QLineEdit::Normal,"",&ok);
     if(ok==false) return;
     int y = mp[input];
     FluorescentRectItem *high = new FluorescentRectItem(QRect(150,y,50,25),nullptr);
     high->setZValue(1);
     scene->addItem(high);
}
void imageViewer::addtag()
{
    bool ok;
    QString input = QInputDialog::getText(this, "输入框","您要在本页插入书签并备注为：",QLineEdit::Normal,"",&ok);
    if(ok==false) return;
    tag->addtag(input,currentPage);
}
void imageViewer::searchtag()
{
    int page = tag->searchtag();
    if(page==-1)
    {
        QMessageBox::information(nullptr,"Error","没有找到该备注");
    }
    else spinbox->setValue(page);
}
void imageViewer::copy()
{
    sons[++cntSon] = new imageViewer(scene,tag);
    sons[cntSon]->turnto(currentPage);
    QPoint pos = mapToGlobal(QPoint(0,0));
    sons[cntSon]->move(pos.x()+50,pos.y()+50);
    sons[cntSon]->show();
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
//        qDebug()<<"del:"<<i+page<<endl;
        cnt[i+page]--;
        if(cnt[i+page]!=0) continue;
        item[page+i]->setPixmap(QPixmap());
        removeItem(item[page+i]);
    }
}
void myScene::myUpdate(int page)
{
    QDir cur = QDir::currentPath();
    cur.cdUp();
    for(int i=qMax(-1,-page+1);i<=1;i++)
    {
//        qDebug()<<"add:"<<i+page<<endl;
        cnt[i+page]++;
        if(cnt[i+page]!=1) continue;
        QPixmap pixmap(QString(cur.path()+"/NewTry/gddsImage/%1.png").arg(page+i-1));
        pixmap = pixmap.scaled(Size.width(),Size.height(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        item[page+i]->setPixmap(pixmap);

        addItem(item[page+i]);
        item[page+i]->setPos(0,Size.height()*(page+i-1));
    }
}

QString TAG::showComboBoxDialog()
{
    if (exec() == QDialog::Accepted) {
        return getSelectedItem();
    }
    return QString();
}

TAG::TAG():comboBox(new QComboBox(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *okButton = new QPushButton(tr("确定"), this);
    QPushButton *cancelButton = new QPushButton(tr("取消"), this);

    layout->addWidget(comboBox);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    connect(okButton, &QPushButton::clicked, this, &TAG::accept);
    connect(cancelButton, &QPushButton::clicked, this, &TAG::reject);
    setLayout(layout);
    setWindowTitle("查找标签");
    resize(400,200);
}
void TAG::addtag(QString input,int page)
{
    beizhu.append(input);
    mp[input]=page;
    comboBox->addItem(input);
}
int TAG::searchtag()
{
    QString input=showComboBoxDialog();
    if(mp.contains(input))
    {
        return mp[input];
    }
    else return -1;
}
