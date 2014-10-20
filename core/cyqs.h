#ifndef CCMD_H
#define CCMD_H

#include "./cserialport.h"
#include "./cyq.h"
#include "./vardef.h"
#include <vector>
#include <QDateTime>

#include "./modbustcpslave.h"
#include "./ccomm.h"

using namespace std;

class CYQs:public QObject
{
    Q_OBJECT
public:
    CYQs(CSerialPort* pSerialPorts[COMCOUNT],CComm* pComm);
    virtual ~CYQs();

    CYQ* GetYQ(const char* czID);
    CYQ* GetYQByName(const char* czName);
    CYQ* GetYQBySlaveID(int iSlaveID);
    int GetYQCount();
    CYQ* pYQ[YQMAXCOUNT];
    CProcess* pProcess[YQMAXCOUNT];
    int SendCmd(const char* czYQID,int CmdID,int CmdFrom,QDateTime dt=QDateTime::currentDateTime());
    bool GetYQbIdle(const char* czYQID);

    bool UpdateYQInfo();//更新仪器的相关配置信息
signals:
    void NotifyWorkStatus(int,int,const char*);//第一参数为流程类型,第二参数为工作进度，第三参数为进度的名称
    void NotifyTotalNumber(int,int);//第一参数为流程类型,第二参数为流程总步数
    void NotifyYQValue(CYQ*);
    void NotifyYQBusy(int,bool);//第一参数为流程类型,第二参数为系统状态
    void NotifyCmdType(int,int);//第一参数为流程类型,第二参数为测试命令
    void NotifyDebugInfo(int,const char*);//第一参数为流程类型,第二参数为输出信息
    void NotifyTempValue(float);
    void NotifyProcFileStatus(int,bool);//第一参数为流程类型,第二参数测试文件状态
    void NotifyPaintCurve(struEchemVector* pSignal, int iType);//第二参数曲线类型:1表示水样曲线，2表示标样曲线，3表示线性扫描

    void NotifyRealData(stuRealData*);//传输实时数据到UI

    void NotifyStatusValue(struGasStatusData*);
    void NotifyScopeData(struGasVector*,int);
    void NotifyAFSData(struAFSVector*,int);
    void NotifyEnterMaintain();
    void NotifyGasResult(float,float,float,float);
    void NotifyGasCoef(float);
    void NotifyHhInfo(int,int,double);//通知主界面曲线峰高,第一参数是峰高类型，水样类型为0，标样类型为1,清除峰高类型为2,二参数为参数代码数字类型，第三参数为峰高
    void NotifyCurveDot(int,double,double,double);//实时曲线点
    void NotifyInitInfo(bool);
    void NotifyYQReady(bool);//通信仪器是否已经准备好
    void NotifyYQConsumeAlarm(const char*,int);
    void NotifyShowCurve(vector<double>*,vector<double>*, int nType);

    void NotifyAlarm(const char* czContext,int nType);

    //4 Begin 比色法
    void NotifyBsReadValue(int,double,QDateTime);     
    //void NotifyBsGratBaseSampleVal(int,int,double);//标定曲线时，显示基线值、样品值
    //4 End 比色法
private:
    CDBBusiness* m_pDB;
    int m_iYQCount;
    CSerialPort* pSerialPorts[COMCOUNT];

    static void OnTestFinished(unsigned long userdata,int iCMD, const char *czYQID);
    static void OnWorkStatus(unsigned long userdata,int iProcessType,int iWorkNum,const char *czName);
    static void OnTotalNum(unsigned long userdata,int iProcessType,int iTotalNum);
    static void OnYQBusy(unsigned long userdata,int iType,int iProcessType,bool bBusy,int iCmdType,bool bProcFile);
    static void OnDebugInfo(unsigned long userdata,int iProcessType,const char *cStr);
    static void OnTempValue(unsigned long userdata,float fTempValue);
    static void OnPaintCurve(unsigned long userdata,struEchemVector* pSignal, int iType);
    //VOC实时传输温度等返调函数
    static void Voc_OnRealData(unsigned long userdata,stuRealData* pOnRealData);

    static void OnStatusValue(unsigned long userdata,struGasStatusData* pTempValue);
    static void OnScopeData(unsigned long userdata,struGasVector* pScope,int nType);
    static void OnAFSData(unsigned long userdata,struAFSVector* pAFS,int nType);
    static void OnGasRunInfo(unsigned long userdata,int nStatus);
    static void OnGasResult(unsigned long userdata,float fResultZA0,float fResultZA1,float fResultZB2,float fResultZB4);
    static void OnGasCoef(unsigned long userdata,float fCoef);
    static void OnHhInfo(unsigned long userdata, int nType,int nTypeID,double fHeight);
    static void OnYQConsumeAlarm(unsigned long userdata,const char* cID,int iAlarmLevel);
    static void OnShowCurve(unsigned long userdata, vector<double>& dataX,vector<double>& dataY, int nType);
    //4 Begin 比色法
    static void OnBsReadValue(unsigned long userdata,int iDataType,double dBaseSampleValue,QDateTime qTime);
    //static void OnBsGratBaseSampleVal(unsigned long userdata,int iCMDType,int iDataType,double dBaseSampleValue);//标定曲线时，显示基线值、样品值
    //4 End 比色法
    static void OnRealCurveDot(unsigned long userdata,double fValue_X,double fValue_Y,double fValue_Y2,int nPos);
    static void OnInitInfo(unsigned long userdata,bool bInit);
    static void OnYQReady(unsigned long userdata,bool bOk);

    static void OnAlarm(unsigned long userdata, const char *czContext, int nType);

    modbustcpslave* pMbTcpserver;
    //ModbusTCP回调
    static void OnModbusTcp03Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* outData,int& ByteCount);
    static void OnModbusTcp16Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* czRegContent,int ByteCount);

    CComm* m_pComm;//通信对象

};

#endif // CCMD_H
