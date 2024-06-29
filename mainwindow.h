#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QTouchEvent>
#include <QGraphicsPixmapItem>
#include <QSet>
#include <QMap>
#include <QSpinBox>

extern QMap<QString,int> mp;
class myScene: public QGraphicsScene
{
    Q_OBJECT
public:
    myScene();
    void myUpdate(int);
    void updatepage(int);
    void del(int);
    QGraphicsPixmapItem *item[720];
    int cnt[720];
};

class imageViewer : public QGraphicsView
{
    Q_OBJECT

public:
    imageViewer(myScene *scene,QGraphicsView *parent = nullptr);
    ~imageViewer();
    void resizeEvent(QResizeEvent *event) override;
    myScene *scene=nullptr;
    void turnto(int i);
    void turnto1(int y);
    int currentPage;
    bool scrollblock = false;
    QSpinBox *spinbox;
    imageViewer *sons[100];
    int cntSon = 0;
protected:
    void scrollContentsBy(int dx,int dy) override;
public slots:
    void onFind();
};


#endif // MAINWINDOW_H
