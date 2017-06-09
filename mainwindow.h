#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <QTimer>
#include <cv.h>
#include <highgui.h>

#include "Codebook.h"
#include "Reconstruct.h"
#include "NNTracker.h"
#include "TrajPredict.h"

#include "pingpongtablearea.h"
#include "ballprops.h"

//#define TIMING

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWidget *centralWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QHBoxLayout *horizontalLayout_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    PingPongTableArea *table;

    explicit MainWindow(QWidget *parent = 0);

    void openVideo(const char *leftFilename=NULL, const char *rightFilename=NULL);

    void loadCameraMatrices(const string camera_matrix_path);

    void initTracker(Classifier &classifier);

    void loadTrajPredict(const string graph);

    ~MainWindow();

private slots:
    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void nextFrame();

private:
    static const int SCALE;
    static const int MAX_NO_TRACK_CNT;

    bool _track(cv::Mat frame, bool left, Point *pt, bool draw=true);
    CvPoint3D32f _sampleUntilLanding(CvPoint3D32f point);

    //Ui::MainWindow *ui;
    VideoCapture capLeft, capRight;
    Mat frameLeft, frameRight;
    Mat temp, temp2;

    BgSubtractor *bgLeft, *bgRight;
    NNTracker *trackLeft, *trackRight;
    Reconstruct *reconstruct;
    TrajPredict *trajPredict;

    QImage imageLeft, imageRight;
    QTimer *timer;

    BallProps ballProps;
    int no_track_cnt;

    //for recording
    int track_no;
    float distance(CvPoint3D32f a, CvPoint3D32f b);

    void retranslateUi();
};

#endif // MAINWINDOW_H
