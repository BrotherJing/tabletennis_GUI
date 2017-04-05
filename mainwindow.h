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

#include "pingpongtablearea.h"

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

    ~MainWindow();

private slots:
    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void nextFrame();

private:
    static const int SCALE;

    //Ui::MainWindow *ui;
    VideoCapture capLeft, capRight;
    Mat frameLeft, frameRight;
    Mat temp, temp2;

    BgSubtractor *bgLeft, *bgRight;
    NNTracker *trackLeft, *trackRight;
    Reconstruct *reconstruct;

    QImage imageLeft, imageRight;
    QTimer *timer;

    bool firstPoint;
    float velocityZ;
    CvPoint3D32f lastPoint;

    void retranslateUi();
};

#endif // MAINWINDOW_H
