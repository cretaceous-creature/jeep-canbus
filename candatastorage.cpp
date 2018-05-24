#include "candatastorage.h"

CanDataStorage::CanDataStorage(QObject *parent)
    : QObject(parent)
{
    m_DevIndex = 0;
    m_DevType = VCI_USBCAN2;
    Reserved = 0;
    InitInfo.Timing0 = 0x00; // 03 for jeep. 00 for c4
    InitInfo.Timing1 = 0x1C;
    InitInfo.Filter = 0;
    InitInfo.AccCode = 0x80000008;
    InitInfo.AccMask = 0xFFFFFFFF;
    InitInfo.Mode = 1;

    canIsOpen = false;
    canIsStart = false;
}

void CanDataStorage::processData() {
    int len = VCI_Receive(m_DevType, m_DevIndex, 0, receivedata, 2500, 200);
    // save brake data
    short brake_final, yawrate_final;
    unsigned int time_brake;
    // save speed data
    short speed_final;
    unsigned int time_speed;
    // save steerwheel data
    short steerwheel_final;
    unsigned int time_steerwheel;
    // save loc data
    unsigned int time_loc;
    // save gas data
    short gas_final;
    unsigned int time_gas;

    for (int i = 0; i < len; i++) {
        if (receivedata[i].RemoteFlag != 0 || receivedata[i].ExternFlag != 0) {
            continue;
        }
        unsigned int id = receivedata[i].ID;
        unsigned int timestamp_can = round(receivedata[i].TimeStamp * 0.1);
        int datalen = receivedata[i].DataLen;
        unsigned int sysTime;
        if (!timeIsInit) {
            QTime time = QTime::currentTime();
            sysTime = (time.hour() * 3600 + time.minute() * 60 + time.second()) * 1000 + time.msec();
            offset = sysTime - timestamp_can;
            timeIsInit = true;
        }
        if (id != 0x3CD && id != 0x30D && id != 0x305 && id != 0x208) continue;
        sysTime = timestamp_can + offset;
        QTime ctime = QTime::currentTime();
        unsigned int testTime = (ctime.hour() * 3600 + ctime.minute() * 60 + ctime.second()) * 1000 + ctime.msec();
        qDebug() << testTime - timestamp_can - offset;
        if (id == 0x3CD && datalen == 8) {
            // 处理yawrate, 大端序，第2-3个字节
            // 刹车 第5字节和第6字节的低4位
            char tmp[2];
            tmp[0] = receivedata[i].Data[3];
            tmp[1] = receivedata[i].Data[2];
            short yawrate = *(short*)tmp;

            tmp[0] = receivedata[i].Data[5];
            tmp[1] = receivedata[i].Data[6];
            short brake = (tmp[0] << 4) + (tmp[1] >> 4);

        } else if (id == 0x30D && datalen == 8) {
            // 处理wheel speed，FL-01 FR-23 RL-45 RR 67
            char tmp[2];
            tmp[0] = receivedata[i].Data[1];
            tmp[1] = receivedata[i].Data[0];
            short fl = *(short*)tmp;
            tmp[0] = receivedata[i].Data[3];
            tmp[1] = receivedata[i].Data[2];
            short fr = *(short*)tmp;
            tmp[0] = receivedata[i].Data[5];
            tmp[1] = receivedata[i].Data[4];
            short rl = *(short*)tmp;
            tmp[0] = receivedata[i].Data[7];
            tmp[1] = receivedata[i].Data[6];
            short rr = *(short*)tmp;
        } else if (id == 0x305 && datalen == 6) {
            // steer wheel
            char tmp[2];
            tmp[0] = receivedata[i].Data[1];
            tmp[1] = receivedata[i].Data[0];
            short steerwheel = *(short*)tmp;
            time_steerwheel = sysTime;
            steerwheel_final = steerwheel;
        } else if (id == 0x208 && datalen == 8) {
            char tmp[2];
            tmp[0] = receivedata[i].Data[3];
            short gas = *(unsigned char*)tmp;
            time_gas = sysTime;
            gas_final = gas;
        }
    }
}

void CanDataStorage::open() {
    if (!canIsOpen) {
        if(VCI_OpenDevice(m_DevType, m_DevIndex, Reserved) != OPENSUCCESS) {
            return;
        }
        Sleep(100);
        if(VCI_InitCAN(m_DevType, m_DevIndex, 0, &InitInfo) != OPENSUCCESS) {
            return;
        }
        canIsOpen = true;
        canIsStart = false;
    } else {
         if (VCI_CloseDevice(m_DevType, m_DevIndex) != OPENSUCCESS) {
             return;
         }
         canIsOpen = false;
         canIsStart = false;
    }
}

void CanDataStorage::start() {
    if (VCI_StartCAN(m_DevType, m_DevIndex, 0) != OPENSUCCESS) {
        return;
    }
}

void CanDataStorage::stop()
{

}
