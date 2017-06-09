#include "pingpongtablearea.h"

#include <QPainter>

const int PingPongTableArea::SCALE = 5;
const int PingPongTableArea::ORIGIN_HEIGHT = 1525;
const int PingPongTableArea::ORIGIN_WIDTH = 2740;

PingPongTableArea::PingPongTableArea(QWidget *parent) : QWidget(parent)
{
    Qt::PenStyle style = Qt::PenStyle(Qt::SolidLine);
    Qt::PenCapStyle cap = Qt::PenCapStyle(Qt::SquareCap);
    Qt::PenJoinStyle join = Qt::PenJoinStyle(Qt::MiterJoin);
    pen = QPen(Qt::white, 2, style, cap, join);

    Qt::BrushStyle brushStyle = Qt::BrushStyle(Qt::SolidPattern);
    brush = QBrush(Qt::blue, brushStyle);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize PingPongTableArea::minimumSizeHint() const{
    return QSize(ORIGIN_WIDTH/SCALE, ORIGIN_HEIGHT/SCALE);
}

QSize PingPongTableArea::sizeHint() const{
    return QSize(ORIGIN_WIDTH/SCALE, ORIGIN_HEIGHT/SCALE);
}

void PingPongTableArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRect(QRect(0,0,width()-1,height()-1));
    painter.drawLine(QPointF(0, height()/2), QPointF(width()-1, height()/2));
    painter.drawLine(QPointF(width()/2, 0), QPointF(width()/2, height()-1));
    painter.setPen(QPen(Qt::green, 8, Qt::PenStyle(Qt::SolidLine), Qt::PenCapStyle(Qt::RoundCap)));
    painter.setBrush(Qt::NoBrush);
    if(currentPointSet){
        painter.drawPoint(currentPoint);
        currentPointSet = false;
    }
    if(landingPointSet){
        painter.setPen(QPen(Qt::red, 8, Qt::PenStyle(Qt::SolidLine), Qt::PenCapStyle(Qt::RoundCap)));
        painter.drawPoint(landingPoint);
    }
    painter.setPen(QPen(Qt::yellow, 8, Qt::PenStyle(Qt::SolidLine), Qt::PenCapStyle(Qt::RoundCap)));
    painter.drawPoint(predictLandingPoint);
}

void PingPongTableArea::setCurrentPoint(QPoint point){
    currentPoint = point;
    currentPointSet = true;
    update();
}

void PingPongTableArea::setCurrentPoint(CvPoint3D32f point){
    int x = (point.y)/SCALE;
    int y = (point.x)/SCALE;
    setCurrentPoint(QPoint(x,y));
}

void PingPongTableArea::setLandingPoint(QPoint point){
    landingPoint = point;
    landingPointSet = true;
    update();
}

void PingPongTableArea::setLandingPoint(CvPoint3D32f point){
    int x = (point.y)/SCALE;
    int y = (point.x)/SCALE;
    setLandingPoint(QPoint(x,y));
}

void PingPongTableArea::setPredictLandingPoint(CvPoint3D32f point){
    int x = (point.y)/SCALE;
    int y = (point.x)/SCALE;
    predictLandingPoint = QPoint(x,y);
    update();
}
