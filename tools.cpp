#include "tools.h"

QImage Mat2QImage(Mat &image){
    QImage img;
    if(image.channels()==3){
        cvtColor(image, image, CV_BGR2RGB);
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    }else if(image.channels()==1){
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_ARGB32);
    }else {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                image.cols*image.channels(), QImage::Format_RGB888);
    }
    return img;
}
