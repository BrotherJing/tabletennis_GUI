#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

#include <opencv2/opencv.hpp>

#include "Classifier.h"
#include "TrajPredict.h"

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    QCommandLineOption leftCameraOption("left",
                                        QCoreApplication::translate("main", "video from left camera"),
                                        QCoreApplication::translate("main", "left"));
    commandLineParser.addOption(leftCameraOption);
    QCommandLineOption rightCameraOption("right",
                                         QCoreApplication::translate("main", "video from right camera"),
                                         QCoreApplication::translate("main", "right"));
    commandLineParser.addOption(rightCameraOption);
    QCommandLineOption cnnModelFileOption("model",
                                    QCoreApplication::translate("main", "model prototxt file"),
                                    QCoreApplication::translate("main", "model"));
    commandLineParser.addOption(cnnModelFileOption);
    QCommandLineOption cnnWeightsFileOption("weights",
                                    QCoreApplication::translate("main", ".caffemodel weights file"),
                                    QCoreApplication::translate("main", "weights"));
    commandLineParser.addOption(cnnWeightsFileOption);
    QCommandLineOption cnnMeanFileOption("mean",
                                    QCoreApplication::translate("main", ".binaryproto mean file"),
                                    QCoreApplication::translate("main", "mean"));
    commandLineParser.addOption(cnnMeanFileOption);
    QCommandLineOption rnnGraphFileOption("graph",
                                    QCoreApplication::translate("main", ".pb LSTM model file"),
                                    QCoreApplication::translate("main", "graph"));
    commandLineParser.addOption(rnnGraphFileOption);
    QCommandLineOption cameraMatrixDirOption("camera_matrix_dir",
                                    QCoreApplication::translate("main", "directory that contains camera matrices"),
                                    QCoreApplication::translate("main", "matrix_dir"));
    commandLineParser.addOption(cameraMatrixDirOption);
    commandLineParser.process(QCoreApplication::arguments());

    MainWindow w;
    if(!commandLineParser.isSet(cnnModelFileOption)||
            !commandLineParser.isSet(cnnWeightsFileOption)||
            !commandLineParser.isSet(cnnMeanFileOption)){
        std::cerr<<"please specify some files required by the CNN model"<<std::endl;
        return -1;
    }
    QString model = commandLineParser.value(cnnModelFileOption);
    QString weights = commandLineParser.value(cnnWeightsFileOption);
    QString mean = commandLineParser.value(cnnMeanFileOption);
    Classifier classifier(model.toStdString(), weights.toStdString(), mean.toStdString());
    w.initTracker(classifier);

    if(!commandLineParser.isSet(rnnGraphFileOption)){
        std::cerr<<"please specify some files required by the LSTM model"<<std::endl;
        return -1;
    }
    QString graph = commandLineParser.value(rnnGraphFileOption);
    TrajPredict trajPredict(graph.toStdString());

    if(!commandLineParser.isSet(cameraMatrixDirOption)){
        std::cerr<<"please specify directory that contains the camera matrices"<<std::endl;
        return -1;
    }
    QString cameraMatrixDir = commandLineParser.value(cameraMatrixDirOption);
    w.loadCameraMatrices(cameraMatrixDir.toStdString());

    if(commandLineParser.isSet(leftCameraOption)&&commandLineParser.isSet(rightCameraOption)){
        QString left = commandLineParser.value(leftCameraOption);
        QString right = commandLineParser.value(rightCameraOption);
        w.openVideo(left.toStdString().c_str(), right.toStdString().c_str());
    }else{
        w.openVideo();
    }
    w.show();

    return a.exec();
}
