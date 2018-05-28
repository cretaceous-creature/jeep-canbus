#include "candatastorage.h"

CanDataStorage::CanDataStorage(QObject *parent)
    : QObject(parent)
{
    canIsOpen = false;
    isRunning = false;
}

CanDataStorage::~CanDataStorage() {
    if (isRunning) {
        stop();
    }
    qDebug() << __FUNCTION__;
}

void CanDataStorage::processData() {
    int len = VCI_Receive(m_DevType, m_DevIndex, 0, receivedata, 2500, 200);
    for (int i = 0; i < len; i++) {
        if (receivedata[i].RemoteFlag != 0 || receivedata[i].ExternFlag != 0) {
            continue;
        }
        unsigned int id = receivedata[i].ID;
        unsigned int timestamp_can = round(receivedata[i].TimeStamp * 0.1);
        int datalen = receivedata[i].DataLen;

        if (id != 0x1E1 && id != 0x211 && id != 0x20E) continue;
        save(receivedata[i]);
    }
}

void CanDataStorage::open() {
    if (!canIsOpen) {
        m_DevIndex = 0;
        m_DevType = VCI_USBCAN2;
        Reserved = 0;
        InitInfo.Timing0 = 0x03; // 03 for jeep. 00 for c4
        InitInfo.Timing1 = 0x1C;
        InitInfo.Filter = 0;
        InitInfo.AccCode = 0x80000000;
        InitInfo.AccMask = 0xFFFFFFFF;
        InitInfo.Mode = 1;
        if(VCI_OpenDevice(m_DevType, m_DevIndex, Reserved) != OPENSUCCESS) {
            emit openError();
            return;
        }
        Sleep(100);
        if(VCI_InitCAN(m_DevType, m_DevIndex, 0, &InitInfo) != OPENSUCCESS) {
            emit openError();
            return;
        }
        canIsOpen = true;
    }
    qDebug() << __FUNCTION__;
    emit openOK();
    isStoped = false;
}

void CanDataStorage::start() {
    qDebug() << __FUNCTION__;
    if (VCI_StartCAN(m_DevType, m_DevIndex, 0) != OPENSUCCESS) {
        qDebug() << "Start error!!!";
        emit startError();
        return;
    }
    emit startOK();
    isRunning = true;
    while (isRunning) {
        processData();
    }
    fclose(fp);
    fp = nullptr;
}

void CanDataStorage::stop()
{
    if (isStoped) return;
    isRunning = false;
    if (VCI_CloseDevice(m_DevType, m_DevIndex) != OPENSUCCESS) {
        emit closeError();
        return;
    }
    qDebug() << __FUNCTION__;
    emit closeOK();
    isStoped = true;
}

void CanDataStorage::save(VCI_CAN_OBJ &data)
{
    if (!fp) return;
    QTime ctime = QTime::currentTime();
    long long sysTime = (ctime.hour() * 3600 + ctime.minute() * 60 + ctime.second()) * 1000 + ctime.msec();
    fprintf(fp, "%lld\t%x", sysTime, data.ID);
    for (int i = 0; i < data.DataLen; ++i) {
        fprintf(fp, "\t%d", data.Data[i]);
    }
    fprintf(fp, "\n");
}

void CanDataStorage::setFilePath(QString path)
{
    if (!path.endsWith('/')) path += '/';
    filePath = path;
    QTime ctime = QTime::currentTime();
    filePath += ctime.toString("HH-mm-ss.log");
    fp = fopen(filePath.toStdString().c_str(), "w");
}
