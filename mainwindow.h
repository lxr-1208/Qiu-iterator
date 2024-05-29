#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QTouchEvent>
#include <QGraphicsPixmapItem>

class imageViewer : public QGraphicsView
{
    Q_OBJECT

public:
    imageViewer(QGraphicsView *parent = nullptr);
    ~imageViewer();
    QGraphicsScene *scene;
protected:
    void wheelEvent(QWheelEvent *event)override;
    void touchEvent(QTouchEvent *event);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
