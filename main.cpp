#include "mainwindow.h"
#include <QApplication>

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.openVideo(argc, argv);
    w.show();

    return a.exec();
}
