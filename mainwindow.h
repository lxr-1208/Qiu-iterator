#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QComboBox>
#include <QTouchEvent>
#include <QGraphicsPixmapItem>
#include <QSet>
#include <QMap>
#include <QDialog>
#include <QSpinBox>

extern QMap<QString,int> mp;
extern QSize Size;

class TAG:public QDialog
{
public:
    TAG();
    QComboBox *comboBox;
    QStringList beizhu;
    QMap<QString,int> mp;
    int cnt=0;
    void addtag(QString,int);
    int searchtag();
    QString getSelectedItem() const { return comboBox->currentText(); }
    QString showComboBoxDialog();
};

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
    imageViewer(myScene *scene,TAG *Tag,QGraphicsView *parent = nullptr);
    ~imageViewer();
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    myScene *scene=nullptr;
    void turnto(int i);
    void turnto1(int y);
    int currentPage;
    bool scrollblock = false;
    QSpinBox *spinbox;
    TAG *tag;
    imageViewer *sons[200];
    int cntSon = 0;
protected:
    void scrollContentsBy(int dx,int dy) override;
public slots:
    void onFind();
    void highlight();
    void addtag();
    void searchtag();
    void copy();
};

class FluorescentRectItem : public QGraphicsRectItem {
public:
    FluorescentRectItem(const QRectF &rect,QGraphicsItem *parent = nullptr) :
        QGraphicsRectItem(rect,parent) {
        QPen pen;
        pen.setColor(Qt::yellow);
        pen.setWidth(3); // 边框宽度
        pen.setCosmetic(true); // 确保边框在缩放时保持一致
        setPen(pen);

        QColor c = Qt::yellow;
        c.setAlpha(120);
        QBrush brush(c);
        brush.setStyle(Qt::SolidPattern);
        setBrush(brush);
        setZValue(1);
    }
};
#endif // MAINWINDOW_H
