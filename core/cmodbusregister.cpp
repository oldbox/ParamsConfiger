#include "cmodbusregister.h"
#include "vardef.h"
#include "cconfig.h"

CModbusRegister::CModbusRegister(const char *czID)
{
    memset(m_czRegisterValue,0,REGCOUNT);
    char czIniName[64];memset(czIniName,0,64);
    sprintf(czIniName,"%s_register.ini",czID);
    m_pConfig = new CConfig(NULL,czIniName);
    GetRegisterValueFromINI();
}
CModbusRegister::~CModbusRegister()
{
    memset(m_czRegisterValue,0,REGCOUNT);
    if(m_pConfig)delete m_pConfig;
}

void CModbusRegister::Get03Data(int iFirstRegister,int iRegCount,char *czValue)
{
    int i = (iFirstRegister - 40001) * 2 % (REGCOUNT/2) ;
    for(int j=0;j<iRegCount;j++)
    {
        czValue[j*2] = m_czRegisterValue[i+j*2];
        czValue[j*2+1] = m_czRegisterValue[i+j*2+1];
    }
}

//bExcg,交换高低字节,本地需要交换，但网络来的字节已交换了，所以不需再交换，本地设置true,网络来的设置false
void CModbusRegister::Set03Data(int iFirstRegister,const char* czValue,bool bExcg)
{
    //1032的排列顺序
    int i = (iFirstRegister - 40001) * 2 % (REGCOUNT/2) ;
    if(bExcg)
    {
        m_czRegisterValue[i] = czValue[1];
        m_czRegisterValue[i+1] = czValue[0];
    }
    else
    {
        m_czRegisterValue[i] = czValue[0];
        m_czRegisterValue[i+1] = czValue[1];
    }

    //写到文件中持久化，且启动时需要加载
    //[40001]
    //1=34
    //2=52
    char czGroup[64],czItem1[64],czItem2[64],czRegValue1[64],czRegValue2[64];
    memset(czGroup,0,64);
    memset(czItem1,0,64);
    memset(czItem2,0,64);
    memset(czRegValue1,0,64);
    memset(czRegValue2,0,64);
    sprintf(czGroup,"%d",iFirstRegister);
    sprintf(czItem1,"1");
    sprintf(czItem2,"2");
    sprintf(czRegValue1,"%d",m_czRegisterValue[i]);
    sprintf(czRegValue2,"%d",m_czRegisterValue[i+1]);
    m_pConfig->SetIni(czGroup,czItem1,czRegValue1);
    m_pConfig->SetIni(czGroup,czItem2,czRegValue2);
}

void CModbusRegister::GetRegisterValueFromINI()
{
    memset(m_czRegisterValue,0,REGCOUNT);
    char czGroup[64],czItem1[64],czItem2[64],czRegValue1[64],czRegValue2[64];
    memset(czItem1,0,64);
    memset(czItem2,0,64);
    sprintf(czItem1,"1");
    sprintf(czItem2,"2");
    for(int i=0;i<REGCOUNT/2;i++)
    {
        memset(czGroup,0,64);
        memset(czRegValue1,0,64);
        memset(czRegValue2,0,64);
        sprintf(czGroup,"%d",i+40001);
        m_pConfig->GetIni(czGroup,czItem1,czRegValue1);
        m_pConfig->GetIni(czGroup,czItem2,czRegValue2);

        m_czRegisterValue[i*2] = atoi(czRegValue1);
        m_czRegisterValue[i*2+1] = atoi(czRegValue2);
    }
}
void CModbusRegister::SetFloatToRegister(int iFirstRegister,float fValue)
{
    char czData[4];memset(czData,0,4);
    memcpy(czData,&fValue,4);
    Set03Data(iFirstRegister,czData);
    Set03Data(iFirstRegister+1,czData+2);
}
float CModbusRegister::GetFloatFromRegister(int iFirstRegister)
{
    char czValue[4];
    memset(czValue,0,4);
    Get03Data(iFirstRegister,2,czValue);
    char czValue1[4];
     memset(czValue1,0,4);
     czValue1[0] = czValue[1];
     czValue1[1] = czValue[0];
     czValue1[2] = czValue[3];
     czValue1[3] = czValue[2];
     float fValue=0.0;
     memcpy(&fValue,czValue1,4);
     return fValue;
}

void CModbusRegister::SetInt16(int iFirstRegister,short stValue)
{
    char czData[2];memset(czData,0,2);
    memcpy(czData,&stValue,2);
    Set03Data(iFirstRegister,czData);
}
short CModbusRegister::GetInt16(int iFirstRegister)
{
    char czValue[2];
    memset(czValue,0,2);
    Get03Data(iFirstRegister,1,czValue);
    char czValue1[2];
     memset(czValue1,0,2);
     czValue1[0] = czValue[1];
     czValue1[1] = czValue[0];
     short stValue=0.0;
     memcpy(&stValue,czValue1,2);
     return stValue;
}

void CModbusRegister::SetInt32(int iFirstRegister,int iValue)
{
    char czData[4];memset(czData,0,4);
    memcpy(czData,&iValue,4);
    Set03Data(iFirstRegister,czData);
    Set03Data(iFirstRegister+1,czData+2);
}
int CModbusRegister::GetInt32(int iFirstRegister)
{
    char czValue[4];
    memset(czValue,0,4);
    Get03Data(iFirstRegister,2,czValue);
    char czValue1[4];
     memset(czValue1,0,4);
     czValue1[0] = czValue[1];
     czValue1[1] = czValue[0];
     czValue1[2] = czValue[3];
     czValue1[3] = czValue[2];
     int iValue=0.0;
     memcpy(&iValue,czValue1,4);
     return iValue;
}

void CModbusRegister::SetString(int iFirstRegister,const char *czValue,int iByteCount)
{
    //转换成GB2312
    char sOut[256];memset(sOut,0,256);
    int iGB2312Len = Utf8ToGb2312(sOut,256,czValue,iByteCount);
    for(int i=0;i<(iGB2312Len+1)/2;i++)//奇数加1变偶再取余
    {
        Set03Data(iFirstRegister+i,sOut+i*2,false);
    }
}
void CModbusRegister::GetString(int iFirstRegister,char *czValue,int iByteCount)
{
    Get03Data(iFirstRegister,(iByteCount+1)/2,czValue);
}
