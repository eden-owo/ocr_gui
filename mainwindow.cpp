// mainwindow.cpp
#include "mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ocr_module.h"
#include "folder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    btnSelectInput = new QPushButton("Select Input Folder");
    btnSelectOutput = new QPushButton("Select Output Folder");
    btnProcess = new QPushButton("Run OCR");
    statusLabel = new QLabel("Ready.");

    layout->addWidget(btnSelectInput);
    layout->addWidget(btnSelectOutput);
    layout->addWidget(btnProcess);
    layout->addWidget(statusLabel);

    setCentralWidget(central);

    connect(btnSelectInput, &QPushButton::clicked, this, &MainWindow::selectInputFolder);
    connect(btnSelectOutput, &QPushButton::clicked, this, &MainWindow::selectOutputFolder);
    connect(btnProcess, &QPushButton::clicked, this, &MainWindow::processImages);
}

MainWindow::~MainWindow() {}

void MainWindow::selectInputFolder() {
    std::string inputPath = ReadFolder(); // 使用你自己的快取+選擇邏輯
    inputFolderPath = QString::fromStdString(inputPath);
    statusLabel->setText("Input Folder: " + inputFolderPath);
}

void MainWindow::selectOutputFolder() {
    std::string outputPath = ReadFolder(); // 同樣使用你自訂的快取選擇
    outputFolderPath = QString::fromStdString(outputPath);
    statusLabel->setText("Output Folder: " + outputFolderPath);
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
