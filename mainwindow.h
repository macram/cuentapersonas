#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GL/glut.h"


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <rcdraw.h>


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


#include <iostream>



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    void updateViewers();
    cv::Point computeCentroid(const cv::Mat &mask);
    void drawPoint(Point &p, Mat &img);

    ~MainWindow();

private:
    int raw1,raw2;

    Ui::MainWindow *ui;
    QTimer timer; // Slot Timer
    VideoCapture *cap; //

    RCDraw *viewer_original;
    RCDraw *viewer_original_grey;
    RCDraw *viewer_original_diff;
    RCDraw *viewer_original_threshold;

    QImage *Q_current_image;
    QImage *Q_current_image_grey;
    QImage *Q_current_image_diff;
    QImage *Q_current_image_threshold;

    Mat  Previous_Image;
    Mat  Current_Image;
    Mat  Current_Image_grey;
    Mat  Current_Image_diff;
    Mat  Current_Image_threshold;

    Point centroid, previousCentroid;

    int people;
    int where; // de abajo a arriba, -1 es saliendo (bajando), 1 es entrando (subiendo).

public slots:
        void compute();

};

#endif // MAINWINDOW_H
