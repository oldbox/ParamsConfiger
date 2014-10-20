#include "cdriver1.h"

CDriver1::CDriver1(CSerialPort* pSerialPort):CDriver(pSerialPort)
{
    cout << "CDriver1::CDriver1()" << endl;
    m_nCurAddress = 0;
    for (int i = 0;i < 7;i++)
    {
        m_arrAdd[i] = -1;
    }
}
CDriver1::~CDriver1()
{
}
void CDriver1::DriverTest()
{
    cout << "CDriver1::DriverTest" << endl;
}
bool CDriver1::LinkModuleExtraction(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4,int16_t wPrm5,int16_t wPrm6 )
{
    if(!pSerialPort)return false;
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wPrm0 = 40;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);
    bBuff[6] = HIBYTE(wPrm3);
    bBuff[7] = LOBYTE(wPrm3);
    bBuff[8] = HIBYTE(wPrm4);
    bBuff[9] = LOBYTE(wPrm4);
    bBuff[10] = HIBYTE(wPrm5);
    bBuff[11] = LOBYTE(wPrm5);
    bBuff[12] = HIBYTE(wPrm6);
    bBuff[13] = LOBYTE(wPrm6);

    if(pModbus->Write10Data(3,40001,6+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleSingleExtraction(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3)
{
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wPrm0 = 45;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);
    if(wPrm3>65535)
    {
        wPrm3 = 65535;
    }
    bBuff[6] = HIBYTE(wPrm3);
    bBuff[7] = LOBYTE(wPrm3);
    if(pModbus->Write10Data(3,40001,3+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleSinglePush(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3)
{
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wPrm0 = 46;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);
    bBuff[6] = HIBYTE(wPrm3);
    bBuff[7] = LOBYTE(wPrm3);
    if(pModbus->Write10Data(3,40001,3+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleDischargeWaste(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4)
{
    if(!pSerialPort)return false;
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wPrm0 = 47;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);
    bBuff[6] = HIBYTE(wPrm3);
    bBuff[7] = LOBYTE(wPrm3);
    bBuff[8] = HIBYTE(wPrm4);
    bBuff[9] = LOBYTE(wPrm4);
    if(pModbus->Write10Data(3,40001,4+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleLiquidLevelTest(int16_t wPrm1)
{
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wPrm0 = 47;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    if(pModbus->Write10Data(3,40001,1+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleAdjustment(int16_t wPrm1)
{
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wPrm0 = 43;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    if(pModbus->Write10Data(3,40001,1+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::LinkModuleAddReagents(int16_t wPrm1,int16_t wPrm2,int16_t wPrm3,int16_t wPrm4,int16_t wPrm5,int16_t wPrm6)
{
    int16_t wPrm0 = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wPrm0 = 48;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);
    bBuff[6] = HIBYTE(wPrm3);
    bBuff[7] = LOBYTE(wPrm3);
    bBuff[8] = HIBYTE(wPrm4);
    bBuff[9] = LOBYTE(wPrm4);
    bBuff[10] = HIBYTE(wPrm5);
    bBuff[11] = LOBYTE(wPrm5);
    bBuff[12] = HIBYTE(wPrm6);
    bBuff[13] = LOBYTE(wPrm6);

    if(pModbus->Write10Data(3,40001,6+1,bBuff))
        return true;
    else
        return false;
}
bool CDriver1::ReadModuleData(int nCurAdd,u_int16_t* wValue)
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    if(pModbus->Read03Data(nCurAdd,40047,12))
    {
        pModbus->GetRegisterValue(12,bBuff);
        wValue[0] = MAKEWORD(bBuff[1],bBuff[0]);
        wValue[1] = MAKEWORD(bBuff[3],bBuff[2]);
        wValue[2] = MAKEWORD(bBuff[5],bBuff[4]);
        wValue[3] = MAKEWORD(bBuff[7],bBuff[6]);
        wValue[4] = MAKEWORD(bBuff[9],bBuff[8]);
        wValue[5] = MAKEWORD(bBuff[11],bBuff[10]);
        wValue[6] = MAKEWORD(bBuff[13],bBuff[12]);
        wValue[7] = MAKEWORD(bBuff[15],bBuff[14]);
        wValue[8] = MAKEWORD(bBuff[17],bBuff[16]);
        wValue[9] = MAKEWORD(bBuff[19],bBuff[18]);
        wValue[10] = MAKEWORD(bBuff[21],bBuff[20]);
        wValue[11] = MAKEWORD(bBuff[23],bBuff[22]);

        return true;
    }
    return false;
}
