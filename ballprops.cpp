#include "ballprops.h"

const int BallProps::ORIGIN_HEIGHT = 1525;
const int BallProps::ORIGIN_WIDTH = 2740;

BallProps::BallProps()
{
    clearState();
}

void BallProps::clearState(){
    is_first_point = true;
    is_rebound = false;
    is_crossing_half_court = false;
    has_predict_point = false;
    is_hit = false;
    velocityX = 0;
    velocityY = 0;
    velocityZ = 0;
}

bool BallProps::isRebound(){
    return is_rebound;
}

bool BallProps::crossHalfCourt(){
    return is_crossing_half_court;
}

CvPoint3D32f BallProps::lastPoint(){
    return last_point;
}

void BallProps::feed(CvPoint3D32f point){
    if (is_first_point) {
        is_first_point = false;
        last_point = point;
        velocityZ = 0;
        velocityY = 0;
        velocityX = 0;
    } else {
        float velocityZ_new = point.z - last_point.z;
        if(velocityZ<0&&velocityZ_new>0&&point.z<100){
            is_rebound = true;
        }else{
            is_rebound = false;
        }
        velocityZ = velocityZ_new;
        float velocityY_new = point.y - last_point.y;
        if(velocityY_new*velocityY<0){
            is_hit = true;
        }else{
            is_hit = false;
        }
        velocityY = velocityY_new;
        velocityX = point.x - last_point.x;

        if((last_point.y-ORIGIN_WIDTH/2)*(point.y-ORIGIN_WIDTH/2)<0){
            is_crossing_half_court = true;
        }else{
            is_crossing_half_court = false;
        }
        last_point = point;
    }
}

CvPoint3D32f BallProps::predictPoint(){
    return predict_point;
}

CvPoint3D32f BallProps::predictLandingPoint(){
    return predict_landing_point;
}

void BallProps::setPredictPoint(CvPoint3D32f point){
    has_predict_point = true;
    predict_point = point;
}


void BallProps::setPredictLandingPoint(CvPoint3D32f point){
    predict_landing_point = point;
}