#include "cdriver0.h"

CDriver0::CDriver0(CSerialPort* pSerialPort):CDriver(pSerialPort)
{
     cout << "CDriver0::CDriver0()" << endl;
}

void CDriver0::DriverTest()
{
    cout << "CDriver0::DriverTest" << endl;
}
