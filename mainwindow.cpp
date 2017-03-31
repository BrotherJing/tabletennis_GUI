#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cv.h>
#include <highgui.h>

#include "Codebook.h"
#include "NNTracker.h"

#include "config.h"
#include "tools.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    szSmall.width = szSmall.width/2;
    szSmall.height = szSmall.height/2;
    temp = Mat(szSmall, frameLeft.type());
    temp2 = Mat(szSmall, frameLeft.type());
    bgLeft = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, 4);
    bgRight = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, 4);
}

void MainWindow::nextFrame(){
    capLeft>>frameLeft;
    capRight>>frameRight;
    if(!frameLeft.empty()&&!frameRight.empty()){
        cv::resize(frameLeft, temp, temp.size());
        cvtColor(temp, temp2, CV_BGR2YCrCb);
        bgLeft->process(temp2, temp);
        imageLeft = Mat2QImage(temp);
        ui->label->setPixmap(QPixmap::fromImage(imageLeft));

        cv::resize(frameRight, temp, temp.size());
        cvtColor(temp, temp2, CV_BGR2YCrCb);
        bgRight->process(temp2, temp);
        imageRight = Mat2QImage(temp);
        ui->label_2->setPixmap(QPixmap::fromImage(imageRight));
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
