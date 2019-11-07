/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
