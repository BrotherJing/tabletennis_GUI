#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cv.h>
#include <highgui.h>

#include "Codebook.h"
#include "Reconstruct.h"
#include "NNTracker.h"

#include "config.h"
#include "tools.h"

using namespace cv;

const int MainWindow::SCALE = 2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    trackLeft(NULL),
    trackRight(NULL),
    reconstruct(NULL)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    capLeft.release();
    capRight.release();
    //if(trackLeft!=NULL)delete trackLeft;
    //if(trackRight!=NULL)delete trackRight;
}

void MainWindow::loadCameraMatrices(const string camera_matrix_path){
    reconstruct = new Reconstruct(camera_matrix_path);
}

void MainWindow::openVideo(const char *leftFilename, const char *rightFilename){
    if(leftFilename==NULL||rightFilename==NULL){
        capLeft.open(0);
        capRight.open(1);
    }else{
        printf("%s %s", leftFilename, rightFilename);
        capLeft.open(leftFilename);
        capRight.open(rightFilename);
    }
    capLeft>>frameLeft;
    capRight>>frameRight;
    if(frameLeft.empty()||frameRight.empty()){
        return;
    }
    Size szSmall = frameLeft.size();
    szSmall.width = szSmall.width/SCALE;
    szSmall.height = szSmall.height/SCALE;
    temp = Mat(szSmall, frameLeft.type());
    temp2 = Mat(szSmall, frameLeft.type());
    bgLeft = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, 4);
    bgRight = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, 4);
}

void MainWindow::initTracker(Classifier &classifier){
    if(trackLeft!=NULL||trackRight!=NULL){
        printf("CNN tracker already initialized!\n");
        return;
    }
    trackLeft = new NNTracker(classifier);
    trackRight = new NNTracker(classifier);
}

void MainWindow::nextFrame(){
    capLeft>>frameLeft;
    capRight>>frameRight;
    bool found = false;
    Point left ,right;
    if(!frameLeft.empty()&&!frameRight.empty()){
        cv::resize(frameLeft, temp, temp.size());
        cvtColor(temp, temp2, CV_BGR2YCrCb);
        if(bgLeft->process(temp2)){
            float prob;
            Rect bbox = Rect(0,0,0,0);
            found = trackLeft->track(temp, (Rect*)bgLeft->bboxes, bgLeft->numComponents, &prob, &bbox);
            if(found){
                rectangle(temp, bbox, CV_RGB(0x00, 0xff, 0x00), 4);
                left = Point(bbox.x*SCALE+bbox.width*SCALE/2, bbox.y*SCALE+bbox.height*SCALE/2);
            }
        }
        imageLeft = Mat2QImage(temp);
        ui->label->setPixmap(QPixmap::fromImage(imageLeft));

        cv::resize(frameRight, temp, temp.size());
        cvtColor(temp, temp2, CV_BGR2YCrCb);
        if(bgRight->process(temp2)){
            float prob;
            Rect bbox = Rect(0,0,0,0);
            found &= trackRight->track(temp, (Rect*)bgRight->bboxes, bgRight->numComponents, &prob, &bbox);
            if(found){
                rectangle(temp, bbox, CV_RGB(0x00, 0xff, 0x00), 4);
                right = Point(bbox.x*SCALE+bbox.width*SCALE/2, bbox.y*SCALE+bbox.height*SCALE/2);
            }
        }
        imageRight = Mat2QImage(temp);
        ui->label_2->setPixmap(QPixmap::fromImage(imageRight));
        if(found){
            CvPoint3D32f coord_world = reconstruct->uv2xyz(left, right);
            printf("%f, %f, %f\n", coord_world.x, coord_world.y, coord_world.z);
        }
    }
}

void MainWindow::on_playButton_clicked()
{
    int rate = capLeft.get(CV_CAP_PROP_FPS);
    capLeft>>frameLeft;
    capRight>>frameRight;
    if(!frameLeft.empty()&&!frameRight.empty()){
        imageLeft = Mat2QImage(frameLeft);
        imageRight = Mat2QImage(frameRight);
        ui->label->setPixmap(QPixmap::fromImage(imageLeft));
        ui->label_2->setPixmap(QPixmap::fromImage(imageRight));
        timer = new QTimer(this);
        //timer->setInterval(1000/rate);
        connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        timer->start();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    if(timer){
        timer->stop();
    }
}
