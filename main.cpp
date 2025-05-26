// main.cpp
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OCR GUI");
    w.resize(400, 200);
    w.show();
    return a.exec();
}
