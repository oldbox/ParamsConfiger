#include "cdriver.h"

CDriver::CDriver(CSerialPort* pSerialPort)
{
    this->pSerialPort = pSerialPort;
    pModbus = new CModbus(pSerialPort);
}
CDriver::~CDriver()
{
    delete pModbus;
}
void CDriver::DriverTest()
{
    cout << "DriverTest" << endl;
}
bool CDriver::SendControlCmd(int nAdd,int mCurCmd,int nValue,bool bOnOff)
{
    if(SendIOCmd(nAdd,mCurCmd,nValue,bOnOff))
        return true;
    else
        return false;
}

bool CDriver::SendIOCmd(int nAdd,int cmd,int nIO,bool bOnOff)
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
    wValue = 0;//持续时间
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);
    if (pModbus->Write10Data(nAdd,40001,4,bBuff))
         return true;
      else
         return false;
}
//单个模块复位
bool CDriver::SingleModuleReset(int nCurAdd)
{
    if(!pSerialPort)return false;
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 0;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);
    if(pModbus->Write10Data(nCurAdd,40001,1,bBuff))
        return true;
    else
        return false;
}
//所有模块复位
bool CDriver::AllModuleReset()
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 0;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);
    if(pModbus->Write10Data(0xFF,40001,1,bBuff))
        return true;
    else
        return false;
}
//写Modbus
bool CDriver::WriteModbus(int nAddr,int FisrtRegister,int Regs,int nTimeout,unsigned char* czData)
{
    if(pModbus->Write10Data(nAddr,FisrtRegister,Regs,nTimeout,czData))
        return true;
    else
        return false;
}
//读Modbus
bool CDriver::ReadModbus(int nAddr,int nReg, int nCount,int ntimeout,unsigned char* czData)
{
    if(pModbus->Read03Data(nAddr,nReg,nCount,ntimeout))
    {
        pModbus->GetRegisterValue(nCount,czData);
        return true;
    }
    else
    {
        return false;
    }
}
//写Modbus
bool CDriver::WriteModbus(int nAddr,int FisrtRegister,int Regs,unsigned char* czData)
{
    if(pModbus->Write10Data(nAddr,FisrtRegister,Regs,czData))
        return true;
    else
        return false;
}
//读Modbus
bool CDriver::ReadModbus(int nAddr,int nReg, int nCount,unsigned char* czData)
{
    if(pModbus->Read03Data(nAddr,nReg,nCount))
    {
        pModbus->GetRegisterValue(nCount,czData);
        return true;
    }
    else
    {
        return false;
    }
}
bool CDriver::CDriverBroadcast(int nAddr, int nReg, int nCount)
{
    if(pModbus->ReadbroadcastData(nAddr,nReg,nCount))
        return true;
    else
        return false;
}
bool CDriver::ModuleScanning(unsigned char* czData)
{
    if(pModbus->ReadbroadcastData(0xff,40050,2))
    {
        memcpy(czData,pModbus->m_czData03,MAX_SIZE);
        return true;
    }
    else
    {
        return false;
    }
}
