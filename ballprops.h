#ifndef BALLPROPS_H
#define BALLPROPS_H

#include<opencv2/opencv.hpp>

class BallProps
{
public:
    BallProps();
    void feed(CvPoint3D32f point);
    void clearState();
    bool isRebound();
    bool crossHalfCourt();
    CvPoint3D32f lastPoint();
private:
    float velocityX, velocityY, velocityZ;
    bool is_first_point, is_rebound, is_crossing_half_court;
    CvPoint3D32f last_point;
};

#endif // BALLPROPS_H
