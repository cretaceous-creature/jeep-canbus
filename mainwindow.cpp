﻿ #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("PKU", "CanCollector", this);
    QString save_path = settings->value("save_path", ".").toString();
    ui->lineEdit_filename->setText(save_path);
    connect(ui->pushButton_open, &QPushButton::clicked, this, &MainWindow::openCan);
    connect(ui->pushButton_choose, &QPushButton::clicked, [=]() {
        QString folder = QFileDialog::getExistingDirectory(this, tr("Folder to save log"), "C:\\");
        ui->lineEdit_filename->setText(folder);
        settings->setValue("save_path", folder);
    });
    connect(&storage, &CanDataStorage::openError, this, [=] () {
        ui->statusBar->showMessage("");
        QMessageBox::critical(this, "Error", "Something happend while opening device.");
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        clear();
    });
    connect(&storage, &CanDataStorage::openOK, this, [=] () {
        ui->statusBar->showMessage("Device is opened.");
        ui->pushButton_open->setEnabled(false);
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setEnabled(true);
    });
    storage.moveToThread(&canThread);
    connect(&storage, &CanDataStorage::startError, this, [=] () {
        qDebug() << "Start error";
        QMessageBox::critical(this, "Error", "Something happend while starting receiving.");
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        clear();
    }, Qt::QueuedConnection);
    connect(&storage, &CanDataStorage::startOK, this, [=] () {
        ui->statusBar->showMessage("Data is being stored.");
        ui->pushButton_open->setEnabled(false);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(true);
    }, Qt::QueuedConnection);

    connect(&storage, &CanDataStorage::closeError, this, [=] () {
        QMessageBox::critical(this, "Error", "Something happend while clocing device.");
        clear();
    }, Qt::QueuedConnection);
    connect(&storage, &CanDataStorage::closeOK, this, [=] () {
        ui->statusBar->showMessage("Device is Closed.");
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        clear();
    }, Qt::QueuedConnection);

    connect(ui->pushButton_start, &QPushButton::clicked, &storage, &CanDataStorage::start);
    connect(ui->pushButton_stop, &QPushButton::clicked, &storage, &CanDataStorage::stop, Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    clear();
    delete ui;
}

void MainWindow::openCan()
{
    if (ui->lineEdit_filename->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please choose storage folder.");
        return;
    }
    qDebug() << __FUNCTION__;
    storage.setFilePath(ui->lineEdit_filename->text());
    storage.open();
    canThread.start();
}

void MainWindow::clear()
{
    if (canThread.isRunning()) {
        qDebug() << __FUNCTION__;
        storage.stop();
        canThread.quit();
        canThread.wait();
    }
}
