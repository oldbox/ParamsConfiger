#ifndef CMODBUS_H
#define CMODBUS_H

#define MAX_SIZE 255

#define LOBYTE(w) ((unsigned char)((w) & 0xff))
#define HIBYTE(w) ((unsigned char)((w) >> 8))

#include "cserialport.h"
#include <QTime>
class CModbus
{
public:
    CModbus(CSerialPort* pCom);

    bool Read03Data(int nAddr,int nReg, int nCount);
    bool Read03Data(int nAddr,int nReg, int nCount,int nTimeout);
    bool Write10Data(int nAddr,int FisrtRegister,int Regs,unsigned char* czData);
    bool Write10Data(int nAddr,int FisrtRegister,int Regs,int nTimeout,unsigned char* czData);
    bool ReadbroadcastData(int nAddr,int nReg, int nCount);
    //重载，解决广播时共它线程可能还在读串口，导致缓冲区数据不对
    bool ReadbroadcastData(int nAddr,int nReg, int nCount,unsigned char* czData);
    unsigned char m_czData03[MAX_SIZE];
    void GetRegisterValue(int nCount,unsigned char* czData);
private:
    CSerialPort* m_pCom;
    int16_t CRC16(unsigned char *pBuf,int nLength);
    QTime t;
};

#endif // CMODBUS_H
