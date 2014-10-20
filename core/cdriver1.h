#ifndef CDRIVER1_H
#define CDRIVER1_H

#include "cdriver.h"
#include "cmodbus.h"


class CDriver1 : public CDriver
{
public:
    CDriver1(CSerialPort* pSerialPort);
    ~CDriver1();
    bool LinkModuleExtraction(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4,int16_t wPrm5,int16_t wPrm6);
    bool LinkModuleSingleExtraction(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3);
    bool LinkModuleSinglePush(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3);
    bool LinkModuleDischargeWaste(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4);
    bool LinkModuleLiquidLevelTest(int16_t wPrm1);
    bool LinkModuleAdjustment(int16_t wPrm1);
    bool LinkModuleAddReagents(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4,int16_t wPrm5,int16_t wPrm6);
    bool ReadModuleData(int nCurAdd,u_int16_t* wValue);
    void DriverTest();

private:
    int m_arrAdd[7];
    int m_nCurAddress;
};

#endif // CDRIVER1_H
