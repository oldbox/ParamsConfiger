#include "cdriver3.h"

CDriver3::CDriver3(CSerialPort* pSerialPort):CDriver(pSerialPort)
{
    cout << "CDriver3::CDriver3()" << endl;
}
CDriver3::~CDriver3()
{
}
void CDriver3::DriverTest()
{
    cout << "CDriver3::DriverTest" << endl;
}
bool CDriver3::ReadComData(int nAddr,int nReg, int nCount,int ntimeout,unsigned char* czData)
{
    if(!pModbus)return false;
    bool bRet = false;
    m_mutex.lock();
    bRet = ReadModbus(nAddr,nReg,nCount,ntimeout,czData);
    usleep(200000);
    m_mutex.unlock();
    return bRet;
}

bool CDriver3::WriteComData(int nAddr, int FisrtRegister, int Regs, unsigned char *czData)
{
    if(!pModbus)return false;
    bool bRet = false;
    m_mutex.lock();
    bRet =  pModbus->Write10Data(nAddr,FisrtRegister,Regs,czData);
    usleep(200000);
    m_mutex.unlock();
    return bRet;
}
bool CDriver3::SetLineTemptDesc_154(int nAddr,int nChannel, int nTempt1, int nTempt2, int nTempt3, int nTempt4, int nTempt5)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 154;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nTempt1;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    wValue = (int16_t)nTempt2;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);
    wValue = (int16_t)nTempt3;
    bBuff[8] = HIBYTE(wValue);
    bBuff[9] = LOBYTE(wValue);
    wValue = (int16_t)nTempt4;
    bBuff[10] = HIBYTE(wValue);
    bBuff[11] = LOBYTE(wValue);
    wValue = (int16_t)nTempt5;
    bBuff[12] = HIBYTE(wValue);
    bBuff[13] = LOBYTE(wValue);

    return WriteComData(nAddr,40001,7,bBuff);
}

bool CDriver3::SetLineTemptK_155(int nAddr, int nChannel, int nK1, int nK2, int nK3, int nK4, int nK5)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 155;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nK1;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    wValue = (int16_t)nK2;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);
    wValue = (int16_t)nK3;
    bBuff[8] = HIBYTE(wValue);
    bBuff[9] = LOBYTE(wValue);
    wValue = (int16_t)nK4;
    bBuff[10] = HIBYTE(wValue);
    bBuff[11] = LOBYTE(wValue);
    wValue = (int16_t)nK5;
    bBuff[12] = HIBYTE(wValue);
    bBuff[13] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,7,bBuff);
}
bool CDriver3::SetLineTemptRTime_156(int nAddr, int nChannel, int nRT1, int nRT2, int nRT3, int nRT4, int nRT5)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 156;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)(nRT1*60);
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    wValue = (int16_t)(nRT2*60);
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);
    wValue = (int16_t)(nRT3*60);
    bBuff[8] = HIBYTE(wValue);
    bBuff[9] = LOBYTE(wValue);
    wValue = (int16_t)(nRT4*60);
    bBuff[10] = HIBYTE(wValue);
    bBuff[11] = LOBYTE(wValue);
    wValue = (int16_t)(nRT5*60);
    bBuff[12] = HIBYTE(wValue);
    bBuff[13] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,7,bBuff);
}


bool CDriver3::SetTemptSingle_153(int nAddr, int nChannel, int nTempt)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 153;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nTempt;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,3,bBuff);
}
bool CDriver3::IOCtrl_LD(int nAddr, int nChannel, bool bOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    wValue = 42; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    bBuff[2] = HIBYTE(nChannel);
    bBuff[3] = LOBYTE(nChannel);

    if(bOnOff)
    {
        bBuff[4] = HIBYTE(0x00);
        bBuff[5] = LOBYTE(0x01);
    }
    else
    {
        bBuff[4] = HIBYTE(0x00);
        bBuff[5] = LOBYTE(0x00);
    }
     return WriteComData(nAddr,40001,3,bBuff);
}

bool CDriver3::SinglePoll_LD(int nAddr, int nSource, int nSpeed, int nStep)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = 45; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = nSource;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);//源

    wValue = nSpeed;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);//速度

    wValue = nStep;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);//步数

    return WriteComData(nAddr,40001,4,bBuff);
}
bool CDriver3::SinglePush_LD(int nAddr, int nDes1, int nSpeed, int nDest2)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = 46; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = nDes1;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);//目标1

    wValue = nSpeed;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);//速度

    wValue = nDest2;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);//目标2

    return WriteComData(nAddr,40001,4,bBuff);
}
bool CDriver3::RePollPush_LD(int nAddr, int nSrcDcf1, int nSrcDcf2,int nDest,int nSpeed, int nStep, int nTimes)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = 52; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = nSrcDcf1;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);

    wValue = nSrcDcf2;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);

    wValue = nDest;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);

    wValue = nSpeed;
    bBuff[8] = HIBYTE(wValue);
    bBuff[9] = LOBYTE(wValue);//速度

    wValue = nStep;
    bBuff[10] = HIBYTE(wValue);
    bBuff[11] = LOBYTE(wValue);//速度

    wValue = nTimes;
    bBuff[12] = HIBYTE(wValue);
    bBuff[13] = LOBYTE(wValue);//目标2

    return WriteComData(nAddr,40001,7,bBuff);
}

bool CDriver3::IOCtrl_JDQ(int nAddr, int nChannel, bool bOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = 10; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    bBuff[2] = HIBYTE(nChannel);
    bBuff[3] = LOBYTE(nChannel);

    if(bOnOff)
    {
        bBuff[4] = HIBYTE(0x00);
        bBuff[5] = LOBYTE(0x01);
    }
    else
    {
        bBuff[4] = HIBYTE(0x00);
        bBuff[5] = LOBYTE(0x00);
    }
    bBuff[6] = HIBYTE(0x00);
    bBuff[7] = LOBYTE(0x00);
    return WriteComData(nAddr,40001,4,bBuff);
}
bool CDriver3::SetDataSpeed_150(int nAddr, int nGainDataRate, int nGainDataRTime)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 150;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nGainDataRate;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nGainDataRTime;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,3,bBuff);
}
bool CDriver3::SetPIDSet_157(int nAddr, int nChannel, int P, int I, int D, int K)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 157;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)P;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    wValue = (int16_t)I;
    bBuff[6] = HIBYTE(wValue);
    bBuff[7] = LOBYTE(wValue);
    wValue = (int16_t)D;
    bBuff[8] = HIBYTE(wValue);
    bBuff[9] = LOBYTE(wValue);
    wValue = (int16_t)K;
    bBuff[10] = HIBYTE(wValue);
    bBuff[11] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,6,bBuff);
}
bool CDriver3::StartLineTempt_152(int nAddr, int nChannel, int nOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 152;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nOnOff;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,3,bBuff);
}

bool CDriver3::StartTemptSigle_151(int nAddr, int nChannel, int nOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 151;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nChannel;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = (int16_t)nOnOff;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,3,bBuff);
}
bool CDriver3::GYOupput_158(int nAddr, int nOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 158;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nOnOff;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
bool CDriver3::Fire_159(int nAddr)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 159;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = 1;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
bool CDriver3::StartGetData_160(int nAddr, int nKind)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 160;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nKind;;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
bool CDriver3::SetDAValue_161(int nAddr, int nAD)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 161;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nAD;;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
bool CDriver3::ECDPower_162(int nAddr, int nOnOff)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 162;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nOnOff;;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
bool CDriver3::DataSpeed_163(int nAddr, int nValue)
{
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);

    //命令字
    wValue = 163;
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);

    wValue = (int16_t)nValue;;
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    return WriteComData(nAddr,40001,2,bBuff);
}
