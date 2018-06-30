#include "paintarea.h"
#include <QApplication>
#include<iostream>

PaintArea::PaintArea(QWidget *parent):QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setMinimumSize(500,400);
    myPenColor=Qt::blue;
    myPenWidth=3;
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    //std::cout << pens.length();
    QWidget::paintEvent(event);
    QPainter painter(this);
    myPen.setColor(myPenColor);
    myPen.setWidth(myPenWidth);
    painter.setPen(myPen);
    //painter.drawPath(xxx);
    //Qlist<QPainterPath>

    for(int i=0; i<paths.size();i++){
        //painter.setPen(pens[i]);
        painter.drawPath(paths[i]);
     }
        switch(state){
            case 1 :
                path = QPainterPath();
                path.moveTo(startPoint);
                path.lineTo(endPoint);
                painter.drawPath(path);
                break;
            case 2 :
                path = QPainterPath();
                path.moveTo(startPoint);
                path.addEllipse(startPoint.x(), startPoint.y(), endPoint.x() - startPoint.x(), endPoint.y() - startPoint.y());
                painter.drawPath(path);
                break;
            case 3 :
                path = QPainterPath();
                path.moveTo(startPoint);
                path.addRect(startPoint.x(), startPoint.y(), endPoint.x() - startPoint.x(), endPoint.y() - startPoint.y());
                painter.drawPath(path);
                break;
            default:
                break;
        }
}


 /*   for(int i=0; i<_lines.size();i++){
        const QVector<QPoint> &line = _lines.at(i);
        const QVector<QPen> &penAttribute = _pensAttribute.at(i);
        for(int j=0;j<line.size()-1;j++){
            painter.setPen(penAttribute.at(j));
            painter.drawLine(line.at(j), line.at(j+1));
        }
     }
}*/



void PaintArea::setPenColor(const QColor &newColor){
        myPenColor=newColor;
}
void PaintArea::setPenWidth(int newWidth){
        myPenWidth=newWidth;
}
void PaintArea::setState(int choice){
    state=choice;
}


void PaintArea::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        startPoint = event->QMouseEvent::pos();
    }
    /* //每次点击鼠标都会为之绘画一条新的线，并将该线的起点位置添加到_lines中
        QVector<QPoint> line;
        QVector<QPen> penAttribute;
       _lines.append(line);
       _pensAttribute.append(penAttribute);

       //记录下该条线当前的位置
       QVector<QPoint> &lastLine = _lines.last();
       lastLine.append(event->pos());
       QVector<QPen> &lastPenAttribute = _pensAttribute.last();
       lastPenAttribute.append(pen);
    */
}

void PaintArea::mouseMoveEvent(QMouseEvent *event){
    if(event->type() == QEvent::MouseMove){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            endPoint = mouseEvent->pos();
    }
        update();
    /*if(_lines.size() == 0)
        {
            QVector<QPoint> line;
            QVector<QPen> penAttribute;
            _lines.append(line);
            _pensAttribute.append(penAttribute);
        }

        QVector<QPoint> &lastLine = _lines.last();
        QVector<QPen> &lastPenAttribute=_pensAttribute.last();
        //记录该条线当前的位置
        lastLine.append(event->pos());
        lastPenAttribute.append(pen);
        //强制重绘
        update();
        */
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
            paths.append(path);
            //pens.append(myPen);
    }
    /* QVector<QPoint> &lastLine = _lines.last();
    QVector<QPen> &lastPenAttribute = _pensAttribute.last();
    lastLine.append(event->pos());
    lastPenAttribute.append(pen);
    */
}



