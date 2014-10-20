#include "cyq.h"

CYQ::CYQ(CProcess* pProcess)
{
    m_pProcess = pProcess;
    m_pModbusRegister = NULL;
}
CYQ::~CYQ()
{
    delete m_pModbusRegister;
}

void CYQ::SetYQParam(SYQPARAM& sYQ)
{
    memset(m_sYQ.czID,0,16);
    memset(m_sYQ.czName,0,64);

    memcpy(m_sYQ.czID,sYQ.czID,16);
    memcpy(this->m_RealInfo.czID,sYQ.czID,16);
    memcpy(this->m_CurveInfo.czID,sYQ.czID,16);

    m_sYQ.iProcessType = sYQ.iProcessType;
    memcpy(m_sYQ.czName,sYQ.czName,64);
    m_sYQ.InTypeID = sYQ.InTypeID;
    m_sYQ.SlaveID = sYQ.SlaveID;
    m_sYQ.DataDigits = sYQ.DataDigits;
    m_sYQ.downlimit = sYQ.downlimit;
    m_sYQ.uplimit = sYQ.uplimit;
    m_sYQ.Max = sYQ.Max;
    m_sYQ.Min = sYQ.Min;
    m_sYQ.UnitType = sYQ.UnitType;
    m_sYQ.TopLimit = sYQ.TopLimit;
    m_sYQ.LowerLimit = sYQ.LowerLimit;

    //不能在构造函数中创建，因为需要拿到参数代码
    if(!m_pModbusRegister)
    {
        m_pModbusRegister = new CModbusRegister(sYQ.czID);
        m_pModbusRegister->SetString(40035,sYQ.czID,strlen(sYQ.czID));
    }
}

void CYQ::GetYQParam(SYQPARAM& sYQ)
{
    memset(sYQ.czID,0,16);
    memset(sYQ.czName,0,64);

    memcpy(sYQ.czID,m_sYQ.czID,16);
    sYQ.iProcessType = m_sYQ.iProcessType;
    memcpy(sYQ.czName,m_sYQ.czName,64);

    sYQ.InTypeID = m_sYQ.InTypeID;
    sYQ.SlaveID =  m_sYQ.SlaveID;
    sYQ.DataDigits =  m_sYQ.DataDigits;
    sYQ.downlimit = m_sYQ.downlimit;
    sYQ.uplimit =  m_sYQ.uplimit;
    sYQ.Max = m_sYQ.Max;
    sYQ.Min =  m_sYQ.Min;
    sYQ.UnitType = m_sYQ.UnitType;
    sYQ.TopLimit = m_sYQ.TopLimit;
    sYQ.LowerLimit = m_sYQ.LowerLimit;
}

void CYQ::SetCurveInfo(struCurveInfo curveinfo)
{
    m_CurveInfo.B = curveinfo.B;
    m_CurveInfo.CurveID = curveinfo.CurveID;
    m_CurveInfo.CalcMode = curveinfo.CalcMode;
     m_CurveInfo.DataTime = curveinfo.DataTime;
     m_CurveInfo.R = curveinfo.R;
     m_CurveInfo.K = curveinfo.K;
     m_CurveInfo.B = curveinfo.B;
     m_CurveInfo.BlankAbsorb = curveinfo.BlankAbsorb;
     m_CurveInfo.CheckChroma = curveinfo.CheckChroma;
     m_CurveInfo.StdAbsorb = curveinfo.StdAbsorb;
     m_CurveInfo.MinValue = curveinfo.MinValue;
     m_CurveInfo.MaxValue = curveinfo.MaxValue;

    m_CurveInfo.m_EchemCurve.PeakSearchDown = curveinfo.m_EchemCurve.PeakSearchDown;     //寻峰下限
    m_CurveInfo.m_EchemCurve.PeakSearchUp = curveinfo.m_EchemCurve.PeakSearchUp;       //寻峰上限
    m_CurveInfo.m_EchemCurve.SouceStandValue = curveinfo.m_EchemCurve.SouceStandValue;  //标样浓度,加标浓度
    m_CurveInfo.SampleAdjust = curveinfo.SampleAdjust;    //标样校准系数
    m_CurveInfo.m_EchemCurve.BlankChroma = curveinfo.m_EchemCurve.BlankChroma;     //空白浓度
    m_CurveInfo.m_EchemCurve.BaseWaveLength = curveinfo.m_EchemCurve.BaseWaveLength;//基线波长

    m_CurveInfo.m_GasCurve.IntegrationTime = curveinfo.m_GasCurve.IntegrationTime;  //气体积分时间
    m_CurveInfo.m_GasCurve.IntegrationNum = curveinfo.m_GasCurve.IntegrationNum;     //气体积分次数
    m_CurveInfo.m_GasCurve.ModulusA = curveinfo.m_GasCurve.ModulusA;           //气体反演系数a
    m_CurveInfo.m_GasCurve.ModulusB = curveinfo.m_GasCurve.ModulusB;           //气体反演系数b

    m_CurveInfo.m_VocCurve.RemainTime = curveinfo.m_VocCurve.RemainTime;
    m_CurveInfo.m_VocCurve.WinFactor = curveinfo.m_VocCurve.WinFactor;
    m_CurveInfo.m_VocCurve.Kind = curveinfo.m_VocCurve.Kind;
    m_CurveInfo.m_VocCurve.MinPW = curveinfo.m_VocCurve.MinPW;
    m_CurveInfo.m_VocCurve.MaxPW = curveinfo.m_VocCurve.MaxPW;
    m_CurveInfo.m_VocCurve.Minimum = curveinfo.m_VocCurve.Minimum;
    m_CurveInfo.m_VocCurve.MinArea = curveinfo.m_VocCurve.MinArea;
}

float CYQ::Get8051()
{
    return m_pModbusRegister->GetFloatFromRegister(40001);
}

float CYQ::GetLastValue()
{
    return m_pModbusRegister->GetFloatFromRegister(40061);
}

void CYQ::Set8051(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40001,fValue);
}
void CYQ::Set3016(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40003,fValue);
}
//标样额定值
void CYQ::SetFixValue(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40005,fValue);
}
//平行样
void CYQ::Set3025(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40007,fValue);
}
//加标回收值
void CYQ::Set3005(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40009,fValue);
}
//水样数据标识
void CYQ::SetWaterTag(const char *czValue,int iByteCount)
{
    m_pModbusRegister->SetString(40011,czValue,iByteCount);
}
//标样数据标识
void CYQ::SetSampleTag(const char *czValue,int iByteCount)
{
    m_pModbusRegister->SetString(40017,czValue,iByteCount);
}
//平行样数据标识
void CYQ::SetParallelTag(const char *czValue,int iByteCount)
{
    m_pModbusRegister->SetString(40023,czValue,iByteCount);
}
//加标回收数据标识
void CYQ::SetRecoveryTag(const char *czValue,int iByteCount)
{
    m_pModbusRegister->SetString(40029,czValue,iByteCount);
}
//参数代码
//void CYQ::SetYQID(char *czValue,int iByteCount)
//{
//    m_pModbusRegister->SetString(40035,czValue,iByteCount);
//}
//工作模式
void CYQ::SetWorkMode(unsigned short iMode)
{
    m_pModbusRegister->SetInt16(40037,iMode);
}
//工作状态
void CYQ::SetWorkStatus(unsigned short iStatus)
{
    m_pModbusRegister->SetInt16(40038,iStatus);
}
//日志类型
void CYQ::SetLogType(unsigned short iStatus)
{
    m_pModbusRegister->SetInt16(40039,iStatus);
}
//日志
void CYQ::SetLog(const char *czValue,int iByteCount)
{
    m_pModbusRegister->SetString(40040,czValue,iByteCount);
}
unsigned short CYQ::GetProcessType()
{
    return this->m_sYQ.iProcessType;
}
//流程代码
void CYQ::SetProcessCode(unsigned short iCode)
{
    m_pModbusRegister->SetInt16(40055,iCode);
}
//故障代码
void CYQ::SetFaultType(unsigned short iStatus)
{
    m_pModbusRegister->SetInt16(40056,iStatus);
}
QDateTime CYQ::GetLastTime()
{
    int iDate = m_pModbusRegister->GetInt32(40057);
    int iTime = m_pModbusRegister->GetInt32(40059);
    char czDateTime[64];memset(czDateTime,0,64);
    sprintf(czDateTime,"%d-%d",iDate,iTime);
    return QDateTime::fromString(czDateTime,"yyyyMMdd-HHmmss");
}
void CYQ::SetLastTime(QDateTime dt)
{
    char czDate[64];memset(czDate,0,64);
    char czTime[64];memset(czTime,0,64);
    sprintf(czDate,"%04d%02d%02d",dt.date().year(),dt.date().month(),dt.date().day());
    sprintf(czTime,"%02d%02d%02d",dt.time().hour(),dt.time().minute(),dt.time().second());
    int iDate = atoi(czDate);
    int iTime = atoi(czTime);
    m_pModbusRegister->SetInt32(40057,iDate);
    m_pModbusRegister->SetInt32(40059,iTime);
}
//最后测试值
void CYQ::SetLastValue(float fValue)
{
    m_pModbusRegister->SetFloatToRegister(40061,fValue);
}

