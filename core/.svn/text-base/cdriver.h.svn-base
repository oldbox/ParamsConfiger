#ifndef CDRIVER_H
#define CDRIVER_H

//#include <hash_map>
#include "cserialport.h"
#include <iostream>
#include "cmodbus.h"
using namespace std;
#define MAKEWORD(a, b) ((u_int16_t)((((unsigned char) (a)) & 0xff) | ((u_int16_t)(((unsigned char) (b)) & 0xff)) << 8))
class CDriver
{
public:
    CDriver(CSerialPort* pSerialPort);
    ~CDriver();
    void DriverTest();
    bool SendControlCmd(int nAdd,int mCurCmd,int nValue,bool bOnOff);
    bool AllModuleReset();
    bool SingleModuleReset(int nCurAdd);
    bool WriteModbus(int nAddr,int FisrtRegister,int Regs,unsigned char* czData);
    bool ReadModbus(int nAddr,int nReg, int nCount,unsigned char* czData);
    bool WriteModbus(int nAddr,int FisrtRegister,int Regs,int nTimeout,unsigned char* czData);
    bool ReadModbus(int nAddr,int nReg, int nCount,int nTimeout,unsigned char* czData);
    bool CDriverBroadcast(int nAddr,int nReg, int nCount);
    bool ModuleScanning(unsigned char* czData);

protected:
    CSerialPort* pSerialPort;
    CModbus* pModbus;
private:
    bool SendIOCmd(int nAdd,int cmd,int nIO,bool nOnOff);
};

#endif // CDRIVER_H
