#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <cv.h>
#include <highgui.h>

#include "Codebook.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void openVideo(int argc, char **argv);

    ~MainWindow();

private slots:
    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void nextFrame();

private:

    Ui::MainWindow *ui;
    VideoCapture capLeft, capRight;
    Mat frameLeft, frameRight;
    Mat temp, temp2;
    BgSubtractor *bgLeft, *bgRight;

    QImage imageLeft, imageRight;
    QTimer *timer;
};

#endif // MAINWINDOW_H
