#include "ballprops.h"
#include "pingpongtablearea.h"

BallProps::BallProps()
{
    clearState();
}

void BallProps::clearState(){
    is_first_point = true;
    is_rebound = false;
    is_crossing_half_court = false;
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
    } else {
        float velocityZ_new = point.z - last_point.z;
        if(velocityZ<0&&velocityZ_new>0){
            is_rebound = true;
        }else{
            is_rebound = false;
        }
        velocityZ = velocityZ_new;

        if((last_point.y-PingPongTableArea::ORIGIN_WIDTH/2)*(point.y-PingPongTableArea::ORIGIN_WIDTH/2)<0){
            is_crossing_half_court = true;
        }else{
            is_crossing_half_court = false;
        }
        last_point = point;
    }
}
