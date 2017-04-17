#ifndef PINGPONGTABLEAREA_H
#define PINGPONGTABLEAREA_H

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QPixmap>

#include <opencv2/opencv.hpp>

class PingPongTableArea : public QWidget
{
    Q_OBJECT
public:
    static const int SCALE;
    static const int ORIGIN_WIDTH;
    static const int ORIGIN_HEIGHT;

    explicit PingPongTableArea(QWidget *parent = 0);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setCurrentPoint(QPoint point);
    void setCurrentPoint(CvPoint3D32f point);
    void setLandingPoint(QPoint point);
    void setLandingPoint(CvPoint3D32f point);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPen pen;
    QBrush brush;

    bool currentPointSet, landingPointSet;
    QPoint currentPoint, landingPoint;
};

#endif // PINGPONGTABLEAREA_H
