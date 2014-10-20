#ifndef CDRIVER4_H
#define CDRIVER4_H

#include "cdriver.h"
#include "cmodbus.h"
#include "../core/vardef.h"

//typedef void ReadRtdTemp(unsigned int iTempType,const char *cStr);

class CDriver4 : public CDriver
{

public:
    CDriver4(CSerialPort* pSerialPort);
    ~CDriver4();

    void DriverTest();        

};

#endif // CDRIVER4_H
