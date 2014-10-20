#ifndef CYQ_H
#define CYQ_H

#include "cprocess.h"
#include "vardef.h"
#include <QDateTime>
#include "cmodbusregister.h"

class CProcess;
class CYQ
{
public:
    CYQ(CProcess* pProcess);
    ~CYQ();

    CProcess* m_pProcess;
    CModbusRegister* m_pModbusRegister;

    void SetYQParam(SYQPARAM& sYQ);
    void GetYQParam(SYQPARAM& sYQ);

    void SetCurveInfo(struCurveInfo curveinfo);

    //测试结果以及相关数据
    struWaterData m_RealInfo;//水样数据,标样核查数据

    //曲线和用于计算结果的信息
    struCurveInfo m_CurveInfo;

    //仪器公共属性
    SYQPARAM m_sYQ;

    float Get8051();
    float GetLastValue();
    unsigned short GetProcessType();
    QDateTime GetLastTime();

    void Set8051(float fValue);//水样值
    void Set3016(float fValue);//标样实测值
    void SetFixValue(float fValue);//标样额定值
    void Set3025(float fValue);//平行样
    void Set3005(float fValue);//加标回收值

    void SetWaterTag(const char *czValue,int iByteCount);//水样数据标识
    void SetSampleTag(const char *czValue,int iByteCount);//标样数据标识
    void SetParallelTag(const char *czValue,int iByteCount);//平行样数据标识
    void SetRecoveryTag(const char *czValue,int iByteCount);//加标回收数据标识
//    void SetYQID(char *czValue,int iByteCount);//参数代码
    void SetLog(const char *czValue,int iByteCount);//日志
    void SetLastValue(float fValue);
   void SetProcessCode(unsigned short iCode);
    void SetWorkMode(unsigned short iMode);
    void SetWorkStatus(unsigned short iStatus);
    void SetLogType(unsigned short iType);
    void SetFaultType(unsigned short iType);
    void SetLastTime(QDateTime dt);
private:

};

#endif // CYQ_H
