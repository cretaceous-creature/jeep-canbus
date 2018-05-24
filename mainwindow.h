#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QFileDialog>

#include "candatastorage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CanDataStorage storage;
    QThread canThread;

private slots:
    void openCan();
    void clear();
};

#endif // MAINWINDOW_H
