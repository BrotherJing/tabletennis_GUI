#ifndef BALLPROPS_H
#define BALLPROPS_H

#include<opencv2/opencv.hpp>

class BallProps
{
public:
	static const int ORIGIN_WIDTH;
    static const int ORIGIN_HEIGHT;
    BallProps();
    void feed(CvPoint3D32f point);
    void setPredictPoint(CvPoint3D32f point);
    void clearState();
    bool isRebound();
    bool crossHalfCourt();
    bool hasPredictPoint();
    CvPoint3D32f lastPoint();
    CvPoint3D32f predictPoint();
    CvPoint3D32f predictLandingPoint();

    bool has_predict_point, is_hit;
    float velocityX, velocityY, velocityZ;
private:
    bool is_first_point, is_rebound, is_crossing_half_court;
    CvPoint3D32f last_point, predict_point, predict_landing_point;
};

#endif // BALLPROPS_H
