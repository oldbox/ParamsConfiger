#ifndef CDRIVER2_H
#define CDRIVER2_H

#include "cdriver.h"
#include "cmodbus.h"

class CDriver2 : public CDriver
{
public:
    CDriver2(CSerialPort* pSerialPort);
    ~CDriver2();

    void DriverTest();
    bool CtrlTempChange(int nAdd,int mCurCmd,int nValue,bool bOnOff);
    bool CtrlDCFOnOff(int nAdd,int cmd,int nIO,bool bOnOff);

private:
};

#endif // CDRIVER2_H
