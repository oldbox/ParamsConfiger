#include "cdriver2.h"

CDriver2::CDriver2(CSerialPort* pSerialPort):CDriver(pSerialPort)
{
    cout << "CDriver2::CDriver2()" << endl;
}
CDriver2::~CDriver2()
{
}
void CDriver2::DriverTest()
{
    cout << "CDriver2::DriverTest" << endl;
}
bool CDriver2::CtrlTempChange(int nAdd,int mCurCmd,int nValue,bool bOnOff)
{
    if(CtrlDCFOnOff(nAdd,mCurCmd,nValue,bOnOff))
        return true;
    else
        return false;
}

bool CDriver2::CtrlDCFOnOff(int nAdd,int cmd,int nIO,bool bOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = cmd; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);
    wValue = nIO;//通道
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = bOnOff;//开关
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);

    if (pModbus->Write10Data(nAdd,40001,4,bBuff))
         return true;
      else
         return false;
}
