#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QList>
#include <QPen>
#include<QDebug>
#include<QVector>
#include<QPoint>


class PaintArea : public QWidget
{
    Q_OBJECT
public:
    PaintArea(QWidget *parent=0);
    QColor penColor() const{return myPenColor;}
    void setPenColor(const QColor &newColor);
    int penWidth() const{return myPenWidth;}
    void setPenWidth(int newWidth);
    void setState(int choice);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:


    //QVector<QVector<QPoint>> _lines;
    QVector<QVector<QPen>> _pensAttribute;
    QList<QPainterPath> paths;
    //qDebug() << "choice";
    QList<QPen> pens;
    QPoint startPoint, endPoint;
    QPainterPath path;
    QPen myPen;
    int myPenWidth;
    QColor myPenColor;
    int state;



signals:

public slots:

};
#endif // PAINTAREA_H
