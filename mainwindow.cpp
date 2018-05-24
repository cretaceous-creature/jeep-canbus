 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_start, &QPushButton::clicked, this, &MainWindow::startCan);
    connect(ui->pushButton_choose, &QPushButton::clicked, [=]() {
        QString folder = QFileDialog::getExistingDirectory(this, tr("Folder to save log"), "C:\\");
        ui->lineEdit_filename->setText(folder);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCan()
{
    if (ui->lineEdit_filename->text().isEmpty()) {
        return;
    }
    storage = new CanDataStorage();

}

