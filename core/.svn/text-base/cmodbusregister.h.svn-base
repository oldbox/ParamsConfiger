#ifndef CMODBUSREGISTER_H
#define CMODBUSREGISTER_H

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "cconfig.h"

#define REGCOUNT 1024
class CModbusRegister
{
public:
    CModbusRegister(const char* czID);
    ~CModbusRegister();

    void SetFloatToRegister(int iFirstRegister,float fValue);
    float GetFloatFromRegister(int iFirstRegister);

    void SetInt16(int iFirstRegister,short stValue);
    short GetInt16(int iFirstRegister);

    void SetInt32(int iFirstRegister,int iValue);
    int GetInt32(int iFirstRegister);

    void SetString(int iFirstRegister,const char* czValue,int iByteCount);
    void GetString(int iFirstRegister,char* czValue,int iByteCount);

    void Get03Data(int iFirstRegister,int iRegCount,char* czValue);
    void Set03Data(int iFirstRegister,const char* czValue, bool bExcg=true);

private:
    CConfig* m_pConfig;
    char m_czRegisterValue[REGCOUNT];
    void GetRegisterValueFromINI();

};

#endif // CMODBUSREGISTER_H
