#ifndef CANDATASTORAGE_H
#define CANDATASTORAGE_H

#define OPENSUCCESS 1

#include <QObject>
#include <QMessageBox>
#include <QTime>
#include <QDebug>
#include <QThread>

#include <atomic>

#ifdef WIN32
#include "controlcan_win32.h"
#else
#include "controlcan_unix.h"
#endif

class CanDataStorage : public QObject
{
    Q_OBJECT
public:
    CanDataStorage(QObject *parent = 0);
    ~CanDataStorage();
    void processData();

    VCI_INIT_CONFIG InitInfo;
    VCI_CAN_OBJ receivedata[2500];
    int m_DevIndex;
    int m_DevType;
    unsigned long Reserved;

    bool canIsOpen;
    bool isStoped;

    QString filePath;
    FILE *fp;

    std::atomic<bool> isRunning;
public slots:
    void open();
    void start();
    void stop();
    void save(VCI_CAN_OBJ &data);
    void setFilePath(QString path);
signals:
    void openError();
    void startError();
    void closeError();
    void openOK();
    void startOK();
    void closeOK();
};

#endif // CANDATASTORAGE_H
