#include "mainwindow.h"
#include "pingpongtablearea.h"

#include <opencv2/opencv.hpp>
#include <cmath>

#include "Codebook.h"
#include "Reconstruct.h"
#include "NNTracker.h"

#include "config.h"
#include "tools.h"

using namespace cv;

const int MainWindow::SCALE = 2;
const int MainWindow::MAX_NO_TRACK_CNT = 10;
const int MainWindow::TRACK_MAX_PROPOSALS = 8;
const int MainWindow::MAX_PREDICT_LENGTH = 50;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //ui(new Ui::MainWindow),
    trackLeft(NULL),
    trackRight(NULL),
    no_track_cnt(MAX_NO_TRACK_CNT),
    track_no(0),
    reconstruct(NULL)
{
    if (objectName().isEmpty())
        setObjectName(QStringLiteral("MainWindow"));
    resize(1012, 711);
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);
    widget = new QWidget(centralWidget);
    widget->setObjectName(QStringLiteral("widget"));
    widget->setGeometry(QRect(10, 10, 992, 671));
    verticalLayout = new QVBoxLayout(widget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(10);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
    horizontalLayout->setContentsMargins(10, 10, 10, 10);
    label = new QLabel(widget);
    label->setObjectName(QStringLiteral("label"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);
    label->setMinimumSize(QSize(480, 270));
    QPalette palette;
    QBrush brush(QColor(136, 138, 133, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    QBrush brush1(QColor(239, 235, 231, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    label->setPalette(palette);

    horizontalLayout->addWidget(label);

    label_2 = new QLabel(widget);
    label_2->setObjectName(QStringLiteral("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);
    label_2->setMinimumSize(QSize(480, 270));
    QPalette palette1;
    palette1.setBrush(QPalette::Active, QPalette::Base, brush);
    palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
    palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    label_2->setPalette(palette1);

    horizontalLayout->addWidget(label_2);


    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
    horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
    playButton = new QPushButton(widget);
    playButton->setObjectName(QStringLiteral("playButton"));
    connect(playButton, SIGNAL(clicked(bool)),
                this, SLOT(on_playButton_clicked()));

    horizontalLayout_2->addWidget(playButton);

    pauseButton = new QPushButton(widget);
    pauseButton->setObjectName(QStringLiteral("pauseButton"));
    connect(pauseButton, SIGNAL(clicked(bool)),
                this, SLOT(on_pauseButton_clicked()));

    horizontalLayout_2->addWidget(pauseButton);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setSpacing(6);
    horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));

    label_3 = new QLabel(widget);
    label_3->setObjectName(QStringLiteral("label_3"));
    sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy1);
    label_3->setMinimumSize(QSize(200, 200));
    label_3->setPalette(palette1);
    horizontalLayout_4->addWidget(label_3);

    verticalLayout->addLayout(horizontalLayout_4);
    table = new PingPongTableArea;
    horizontalLayout_4->addWidget(table);

    setCentralWidget(centralWidget);
    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QStringLiteral("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 1012, 22));
    setMenuBar(menuBar);
    mainToolBar = new QToolBar(this);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    setStatusBar(statusBar);

    retranslateUi();

    label->setScaledContents(true);
    label_2->setScaledContents(true);
}

void MainWindow::retranslateUi(){
    setWindowTitle(QApplication::translate("MainWindow", "Table Tennis", Q_NULLPTR));
    label->setText(QApplication::translate("MainWindow", "", Q_NULLPTR));
    label_2->setText(QApplication::translate("MainWindow", "", Q_NULLPTR));
    playButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
    pauseButton->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
}

MainWindow::~MainWindow()
{
    //delete ui;
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
    bgLeft = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, TRACK_MAX_PROPOSALS);
    bgRight = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, TRACK_MAX_PROPOSALS);
}

void MainWindow::initTracker(Classifier &classifier){
    if(trackLeft!=NULL||trackRight!=NULL){
        printf("CNN tracker already initialized!\n");
        return;
    }
    trackLeft = new NNTracker(classifier);
    trackRight = new NNTracker(classifier);
    trackLeft->setProbThresh(0.7f);
    trackRight->setProbThresh(0.7f);
    trackLeft->setProposalRange(temp.size().width/12);
    trackRight->setProposalRange(temp.size().width/12);
    trackLeft->setMaxProposals(TRACK_MAX_PROPOSALS);
    trackRight->setMaxProposals(TRACK_MAX_PROPOSALS);
}

void MainWindow::loadTrajPredict(const string graph){
    trajPredict = new TrajPredict(graph);
}

bool MainWindow::_track(cv::Mat frame, bool left, Point *pt, bool draw){
    cv::resize(frame, temp, temp.size());
    cvtColor(temp, temp, CV_BGR2RGB);
    cvtColor(temp, temp2, CV_RGB2YCrCb);
    bool success;
    float prob;
    Rect bbox;
    if(left){
        success = bgLeft->process(temp2) &&
                trackLeft->track_vec(temp, bgLeft->bboxes_vec, &prob, &bbox);
    }else{
        success = bgRight->process(temp2) &&
                trackRight->track_vec(temp, bgRight->bboxes_vec, &prob, &bbox);
    }
    if(success){
        *pt = Point(bbox.x*SCALE+bbox.width*SCALE/2, bbox.y*SCALE+bbox.height*SCALE/2);
        if(draw){
            rectangle(temp, bbox, CV_RGB(0x00, 0xff, 0x00), 4);
        }
    }
#ifndef TIMING
    if(left){
        imageLeft = Mat2QImage(temp);
        label->setPixmap(QPixmap::fromImage(imageLeft));
    }else{
        imageRight = Mat2QImage(temp);
        label_2->setPixmap(QPixmap::fromImage(imageRight));
    }
#endif
    return success;
}

void MainWindow::nextFrame(){
    capLeft>>frameLeft;
    capRight>>frameRight;
    bool found = false;
    Point left ,right;
    if(frameLeft.empty() || frameRight.empty()){
        return;
    }
#ifdef TIMING
    struct timeval t1,t2;
    double timeuse;
    gettimeofday(&t1,NULL);
#endif
    found = _track(frameLeft, true, &left);
    found &= _track(frameRight, false, &right);
#ifdef TIMING
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec)*1000.0 + (t2.tv_usec - t1.tv_usec)/1000.0;
    printf("Track Use Time:%fms\n",timeuse);

    gettimeofday(&t1,NULL);
#endif
    if(found){
        no_track_cnt = 0;
        CvPoint3D32f coord_world = reconstruct->uv2xyz(left, right);
        coord_world.y = PingPongTableArea::ORIGIN_WIDTH - coord_world.y;
        table->setCurrentPoint(coord_world);
        ballProps.feed(coord_world);
        if(ballProps.isRebound()){
            table->setLandingPoint(ballProps.lastPoint());
            //std::cout<<ballProps.lastPoint().x<<' '<<ballProps.lastPoint().y<<std::endl;
        }
        if(ballProps.crossHalfCourt()){
            std::cout<<"try to predict landing point"<<std::endl;
            table->setPredictLandingPoint(_sampleUntilLanding(coord_world));
        }
        if(ballProps.is_hit){
            //std::cout<<"hit!"<<std::endl;
            trajPredict->clearState();
            track_no++;
        }
        ballProps.setPredictPoint(trajPredict->sample1(coord_world));
        //std::cout<<track_no<<" 0 "<<coord_world.x<<' '<<coord_world.y<<' '<<coord_world.z<<std::endl;
    }else{
        no_track_cnt++;
        if(no_track_cnt>MAX_NO_TRACK_CNT){
            ballProps.clearState();
            trajPredict->clearState();
            track_no++;
        }else{
            if(ballProps.has_predict_point){
                ballProps.feed(ballProps.predictPoint());
            }
            ballProps.setPredictPoint(trajPredict->sample1(ballProps.lastPoint()));
            //std::cout<<track_no<<" 0 "<<ballProps.lastPoint().x<<' '<<ballProps.lastPoint().y<<' '<<ballProps.lastPoint().z<<std::endl;
        }
    }
#ifdef TIMING
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec)*1000.0 + (t2.tv_usec - t1.tv_usec)/1000.0;
    printf("Predict Use Time:%fms\n",timeuse);
#endif
    char str_velocity[50];
    sprintf(str_velocity, "X:%4.2f\nY:%4.2f\nZ:%4.2f", ballProps.velocityX, ballProps.velocityY, ballProps.velocityZ);
    label_3->setText(QApplication::translate("MainWindow", str_velocity, Q_NULLPTR));
}

CvPoint3D32f MainWindow::_sampleUntilLanding(CvPoint3D32f point){
    BallProps tempProps;
    tempProps.feed(point);
    trajPredict->saveState();
    int cnt = 0;
    while(!tempProps.isRebound() && cnt<MAX_PREDICT_LENGTH){
        tempProps.feed(trajPredict->sample1(tempProps.lastPoint()));
        cnt++;
        //std::cout<<track_no<<" 1 "<<tempProps.lastPoint().x<<' '<<tempProps.lastPoint().y<<' '<<tempProps.lastPoint().z<<std::endl;
    }
    trajPredict->restoreState();
    //std::cout<<"predict landing point: ("<<tempProps.lastPoint().x<<", "<<tempProps.lastPoint().y<<")"<<std::endl;
    return tempProps.lastPoint();
}

float MainWindow::distance(CvPoint3D32f a, CvPoint3D32f b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}

void MainWindow::on_playButton_clicked()
{
    int rate = capLeft.get(CV_CAP_PROP_FPS);
    capLeft>>frameLeft;
    capRight>>frameRight;
    if(!frameLeft.empty()&&!frameRight.empty()){
        imageLeft = Mat2QImage(frameLeft);
        imageRight = Mat2QImage(frameRight);
        label->setPixmap(QPixmap::fromImage(imageLeft));
        label_2->setPixmap(QPixmap::fromImage(imageRight));
        timer = new QTimer(this);
        //timer->setInterval(1000/4);
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
