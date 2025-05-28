// mainwindow.cpp
#include "mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ocr_module.h"
#include "folder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(central);
    QHBoxLayout *hLayout_1 = new QHBoxLayout;
    QHBoxLayout *hLayout_2 = new QHBoxLayout;

    inputLineEdit = new QLineEdit;
    outputLineEdit = new QLineEdit;

    btnSelectInput = new QPushButton("Select Input Folder");
    btnSelectInput->setFixedWidth(150);
    inputLineEdit->setPlaceholderText("");
    btnSelectOutput = new QPushButton("Select Output Folder");
    btnSelectOutput->setFixedWidth(150);
    outputLineEdit->setPlaceholderText("");
    btnProcess = new QPushButton("Run OCR");
    statusLabel = new QLabel("Ready.");

    hLayout_1->addWidget(btnSelectInput);
    hLayout_1->addWidget(inputLineEdit);
    hLayout_2->addWidget(btnSelectOutput);
    hLayout_2->addWidget(outputLineEdit);

    vLayout->addLayout(hLayout_1);
    vLayout->addLayout(hLayout_2);
    vLayout->addWidget(btnProcess);
    vLayout->addWidget(statusLabel);

    setCentralWidget(central);

    connect(btnSelectInput, &QPushButton::clicked, this, &MainWindow::selectInputFolder);
    connect(btnSelectOutput, &QPushButton::clicked, this, &MainWindow::selectOutputFolder);
    connect(btnProcess, &QPushButton::clicked, this, &MainWindow::processImages);
}

MainWindow::~MainWindow() {}

void MainWindow::selectInputFolder(){
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Input Folder");
    if(!folderPath.isEmpty()){
        inputLineEdit->setText(folderPath);
        statusLabel->setText("Input Folder: " + folderPath);
        qDebug() << "Returned path:" << folderPath;
    } else {
        qDebug() << "User canceled folder selection.";
    }
}

void MainWindow::selectOutputFolder(){
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Input Folder");
    if(!folderPath.isEmpty()){
        outputLineEdit->setText(folderPath);
        statusLabel->setText("Output Folder: " + folderPath);
        qDebug() << "Returned path:" << folderPath;
    } else {
        qDebug() << "User canceled folder selection.";
    }
}

void MainWindow::processImages() {
    if (inputFolderPath.isEmpty() || outputFolderPath.isEmpty()) {
        statusLabel->setText("Please select both folders.");
        return;
    }

    std::vector<std::string> imageFiles = ListFilesInFolder(inputFolderPath.toStdString());

    for (const auto& filePath : imageFiles) {
        cv::Mat img = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        if (img.empty()) {
            std::cerr << "Cannot read image: " << filePath << std::endl;
            continue;
        }

        cv::Mat img_rgba;
        if (img.channels() == 3) {
            cv::cvtColor(img, img_rgba, cv::COLOR_BGR2BGRA);
        } else if (img.channels() == 4) {
            img_rgba = img;
        } else {
            std::cerr << "Unsupported image format: " << filePath << std::endl;
            continue;
        }

        Img ig = {
            3,
            img_rgba.cols,
            img_rgba.rows,
            0,
            (__int64)img_rgba.step,
            (__int64)img_rgba.data
        };

        ocr_module ocr;
        ocr.run_ocr(ig);
    }

    statusLabel->setText("Processing completed.");
}
