/*
P. Moreno. Caćeres. 2014
*/

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication guau(argc, argv);
    MainWindow w;
    w.show();
    return guau.exec();
}
