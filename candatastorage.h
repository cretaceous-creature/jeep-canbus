#ifndef CANDATASTORAGE_H
#define CANDATASTORAGE_H

#define OPENSUCCESS 1

#include <QObject>
#include <QTime>
#include <QDebug>

#include "ControlCAN.h"


class CanDataStorage : public QObject
{
    Q_OBJECT
public:
    CanDataStorage(QObject *parent = 0);
    void processData();

    VCI_INIT_CONFIG InitInfo;
    VCI_CAN_OBJ receivedata[2500];
    int m_DevIndex;
    int m_DevType;
    unsigned long Reserved;

    bool canIsOpen;
    bool canIsStart;
    bool timeIsInit;
    long long offset;

    QString filePath;
    FILE *fp;

public slots:
    void open();
    void start();
    void stop();
};

#endif // CANDATASTORAGE_H
