// mainwindow.h
#pragma once

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectInputFolder();
    void selectOutputFolder();
    void processImages();

private:
    QString inputFolderPath;
    QString outputFolderPath;

    QPushButton *btnSelectInput;
    QPushButton *btnSelectOutput;
    QPushButton *btnProcess;
    QLabel *statusLabel;

    QLineEdit* inputLineEdit;
    QLineEdit* outputLineEdit;

};
