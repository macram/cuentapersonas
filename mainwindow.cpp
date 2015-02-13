/*
  Se trata básicamente de diseñar una aplicación que mediante una máquina de
  estados controle el tráfico de personas a través de la puerta de un aula
  tomando como base los datos adquiridos a través de las ímagenes de video
  de una cámara cenita.
  Se propone de entrada utilizar un aproximación centrada en determinar
  las diferencias entre una imagen base inicial neutra y las sucesivas imágenes del
  tren de video y a partir de ellas seguir el tracking del centroide considerando
  dos barreras virtuales para la contrucción de la máquina de estados.
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <iostream>   // std::cout
#include <string>

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

using namespace std;

// Constructor member
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Object video creation
    cap= new VideoCapture();
    cap->open("video.wmv"); // original video

    // settings sizes
    cap->set(CV_CAP_PROP_FRAME_WIDTH,320); // fix width
    cap->set(CV_CAP_PROP_FRAME_HEIGHT,240); // fix heigth

    // Barriers positions
    raw1=128;raw2=200;


    // Qimages for viewers
    Q_current_image = new QImage(320,240, QImage::Format_RGB888);
    Q_current_image_grey = new QImage(320,240, QImage::Format_Indexed8);
    Q_current_image_diff = new QImage(320,240, QImage::Format_Indexed8);
    Q_current_image_threshold = new QImage(320,240,QImage::Format_Indexed8);

    // Viewers
    viewer_original = new RCDraw(320,240, Q_current_image, ui->viewer_original);
    viewer_original_grey = new RCDraw(320,240, Q_current_image_grey, ui->viewer_original_grey);
    viewer_original_diff = new RCDraw(320,240, Q_current_image_diff, ui->viewer_original_diff);
    viewer_original_threshold = new RCDraw(320,240, Q_current_image_threshold, ui->viewer_original_threshold);

    // Get first image
    *cap >> Previous_Image;
    cvtColor(Previous_Image, Previous_Image, CV_BGR2GRAY);
    //imshow("First", Previous_Image);

    people = 1;

    // Run connect and start timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    timer.start(100);
}

// Destructor member
MainWindow::~MainWindow()
{
    delete ui;
    delete cap;
    delete Q_current_image;
    delete Q_current_image_grey;
    delete Q_current_image_diff;
    delete Q_current_image_threshold;
 }


// Process loop implementation on SLOT
void MainWindow::compute()
{

    ////////////////// CAMERA  //////////////////////////
    if(!cap->isOpened())  // check if we succeeded
        exit(-1);
    // Get a image from video
    *cap >> Current_Image;
    cv::resize(Current_Image, Current_Image,Size(320,240));

    // BGR2RGB
    cvtColor(Current_Image, Current_Image, CV_BGR2RGB, 1);

    // Frame to greyscale
    cvtColor(Current_Image, Current_Image_grey, CV_RGB2GRAY, 1);
    //imshow("", Current_Image_grey);

    // Diff frame
    absdiff(Current_Image_grey, Previous_Image, Current_Image_diff);

    // Threshold and centroid
    threshold(Current_Image_diff, Current_Image_threshold, 50, 255, 0 /*Binary*/);

    centroid = computeCentroid(Current_Image_threshold);
    if (centroid.x==0 && centroid.y==0) { // Si es 0,0 suele ser porque no hay puntos de los que calcular el centroide. "Lo saco" fuera de la pantalla.
        centroid = Point(-1,-1);
    }
    else {
        drawPoint(centroid, Current_Image_threshold);
    }

    if (centroid.y>0 && centroid.y<raw1) { // Zona de arriba
        if (where == -1) {
            people++;
            cout << people <<endl;
        }
        where = 2;
    }
    else if (centroid.y>raw1 && centroid.y<raw2) { // Zona del centro
        if (where == 0) where = -1; // Si estabas abajo estás en el centro subiendo;
        else if (where == 2) where = 1; // si estabas arriba estás en el centro bajando
    }
    else if (centroid.y>raw2) { // Zona de abajo
        if (where == 1) {
            people--;
            cout << people<<endl;
        }
        where = 0;
    }
    else if (centroid.y==-1) where = 3; // No estaba
    ui->lcdNumber->display(people);

    // Update Previous_Image
    //Previous_Image = Current_Image.clone();
    //cvtColor(Previous_Image, Previous_Image, CV_BGR2GRAY);

    //Draw barrier lines and copy to current image viewer
    cv::line(Current_Image,Point(0,raw1),Point(Current_Image.cols-1,raw1),CV_RGB(255,0,255),1);
    cv::line(Current_Image,Point(0,raw2),Point(Current_Image.cols-1,raw2),CV_RGB(0,255,255),1);


    // Just updating UI video viewers
    updateViewers();
}

/**
 * @brief MainWindow::updateViewers just updates UI viewers
 */
void MainWindow::updateViewers()
{
    memcpy(Q_current_image->bits(),Current_Image.data, Current_Image.rows*Current_Image.cols*sizeof(uchar)*3 );
    memcpy(Q_current_image_grey->bits(), Current_Image_grey.data, Current_Image_grey.rows*Current_Image_grey.cols);
    memcpy(Q_current_image_diff->bits(), Current_Image_diff.data, Current_Image_diff.rows*Current_Image_diff.cols);
    memcpy(Q_current_image_threshold->bits(), Current_Image_threshold.data, Current_Image_threshold.rows*Current_Image_threshold.cols);

    viewer_original->update();
    viewer_original_grey->update();
    viewer_original_diff->update();
    viewer_original_threshold->update();
}

/**
 * @brief calculates centroid
 * @param mask (Mat) with points
 * @return centroid point
 *
 * Code from http://stackoverflow.com/questions/9074202/opencv-2-centroid
 */
cv::Point MainWindow::computeCentroid(const cv::Mat &mask)
{
    cv::Moments m = moments(mask, true);
    cv::Point center(m.m10/m.m00, m.m01/m.m00);
    return center;
}

void MainWindow::drawPoint(Point &p, Mat &img)
{
    circle(img, p, 4, cvScalar(100, 100, 100, 0 ), 2, 8, 0);
}
