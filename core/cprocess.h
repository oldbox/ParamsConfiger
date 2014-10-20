#ifndef CPROCESS_H
#define CPROCESS_H

#include "cserialport.h"
#include <stdio.h>
#include "cdriver0.h"
#include "cdriver1.h"
#include "cdriver2.h"
#include "cdriver3.h"
#include "cdriver4.h"
#include "cdriver5.h"
#include "vardef.h"
#include "cdbbusiness.h"
#include "Arithmetic.h"
#include <QDateTime>
#include "cdbmanager.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "cyq.h"
#include <iostream>
#include <QMap>
#include "../spec/spectrumdevice.h"
#include <queue>
using namespace std;

//流程的事件通知
typedef void OnTestFinished(unsigned long userdata,int iCMD,const char* czYQID);
typedef void OnWorkStatus(unsigned long userdata,int iProcessType,int iWorkNum,const char *czName);
typedef void OnTotalNum(unsigned long userdata,int iProcessType,int iTotalNum);
typedef void OnYQBusy(unsigned long userdata,int iType,int iProcessType,bool bBusy,int iCmdType,bool bProcFile);
typedef void OnDebugInfo(unsigned long userdata,int iProcessType,const char *cStr);
typedef void OnTempValue(unsigned long userdata,float fTempValue);
typedef void OnPaintCurve(unsigned long userdata,struEchemVector* pSignal, int iType);
typedef void Voc_OnRealData(unsigned long userdata,stuRealData* realData);
typedef void OnStatusValue(unsigned long userdata,struGasStatusData* pTempValue);
typedef void OnScopeData(unsigned long userdata,struGasVector* pScope,int nType);
typedef void OnAFSData(unsigned long userdata,struAFSVector* pAFS,int nType);
typedef void OnGasRunInfo(unsigned long userdata,int nStatus);
typedef void OnGasResult(unsigned long userdata,float fResultZA0,float fResultZA1,float fResultZB2,float fResultZB4);
typedef void OnGasCoef(unsigned long userdata,float fCoef);
typedef void OnHhInfo(unsigned long userdata,int nType,int nTypeID,double fHeight);
typedef void OnRealCurveDot(unsigned long userdata,double fValue_X,double fValue_Y,double fValue_Y2,int nPos);
typedef void OnInitInfo(unsigned long userdata,bool bInit);
typedef void OnYQReady(unsigned long userdata,bool bOk);
typedef void OnYQConsumeAlarm(unsigned long userdata,const char* cID,int iAlarmLevel);
typedef void OnShowCurve(unsigned long userdata,vector<double>& dataX,vector<double>& dataY,int nType);
typedef void OnAlarm(unsigned long userdata,const char* czContext,int nType);

 //4 Begin 比色法
typedef void OnBsReadValue(unsigned long userdata,int iDataType,double dBaseSampleValue,QDateTime qTime);
//typedef void OnBsGratBaseSampleVal(unsigned long userdata,int iCMDType,int iDataType,double dBaseSampleValue);//标定曲线时，显示基线值、样品值
//4 End 比色法
class CYQ;
class CProcess
{
public:
    CProcess();
    virtual ~CProcess();

    void SetSerialPorts(CSerialPort* pSerialPorts[COMCOUNT]);
    CDriver* pDriver;
    CDriver* pDriver1;//对于部分仪器，一个流程可以需要从不同的串口读数据，比如气体分析仪。
    spectrumDevice* m_pSpectrum;//光谱仪对象
    void CreateDriver(int iProcessType);
    void Run();
    void ExitRun();

    virtual int StartTestCmd(int nCmd,int nKind,QDateTime dt);//启动测试命令
    void SetOnTestFinished(unsigned long userdata,OnTestFinished* pOnTestFinished);
    void SetOnWorkStatus(unsigned long userdata,OnWorkStatus* pOnWorkStatus);
    void SetOnTotalNum(unsigned long userdata,OnTotalNum* pOnTotalNum);
    void SetOnYQBusy(unsigned long userdata,OnYQBusy* pOnYQBusy);
    void SetOnDebugInfo(unsigned long userdata,OnDebugInfo* pOnDebugInfo);
    void SetOnTempValue(unsigned long userdata,OnTempValue* pOnTempValue);
    void SetOnPaintCurve(unsigned long userdata,OnPaintCurve* pOnPaintCurve);
    void SetOnStatusValue(unsigned long userdata,OnStatusValue* pOnStatusValue);
    void SetOnScopeData(unsigned long userdata,OnScopeData* pOnScopeData);
    void SetOnAFSData(unsigned long userdata,OnAFSData* pOnAFSData);
    void SetOnGasRunInfo(unsigned long userdata,OnGasRunInfo* pOnGasRunInfo);
    void SetOnGasResult(unsigned long userdata,OnGasResult* pOnGasResult);
    void SetOnGasCoef(unsigned long userdata,OnGasCoef* pOnGasCoef);
    void SetOnHhInfo(unsigned long userdata,OnHhInfo* pOnHhInfo);
    void SetOnRealCurveDot(unsigned long userdata,OnRealCurveDot* pOnCurveDot);
    void SetOnInitInfo(unsigned long userdata,OnInitInfo* pOnInitInfo);
    void setOnYQConsumeAlarm(unsigned long userdata,OnYQConsumeAlarm* pOnYQConsumeAlarm);
    void SetOnShowCurve(unsigned long userdata,OnShowCurve* pOnShowCurve);
    void SetOnAlarm(unsigned long userdata,OnAlarm* pOnAlarm);
    //4 Begin 比色法
    void SetOnBsReadValue(unsigned long userdata,OnBsReadValue* pOnBsReadValue);     
    //void SetOnBsGratBaseSampleVal(unsigned long userdata,OnBsGratBaseSampleVal* pOnBsGratBaseSampleVal);//标定曲线时，显示基线值、样品值
    //4 End 比色法
    void SetYQs(CYQ* pYQ[YQMAXCOUNT]);
    virtual int GetProcessType()=0;

    bool GetbIdle(){return m_bIdle;}//获取仪器状态(空闲、工作)
    void SetbIdle(bool bFlag);//设置仪器是否空闲
    void SetbStart(bool bFlag){m_bStart = bFlag;}//进入主界面标志
    void SetThreadExit(bool bFlag){m_bExitFlag = bFlag;}//设置工作线程是否退出
    void SetDrawCurve(bool bFlag){m_bDrawCurve = bFlag;}//进入烟气显示光谱曲线界面标志
    void SetStopThread(bool bFlag){m_bStopThread = bFlag;}//暂停线程
    void SetCaliStart(bool bFlag){m_bCaliTag = bFlag;}//启动校准 true为启动
    void SetCaliType(int nType){m_nType = nType;}//设定校准类型

    virtual void StartGasTestCmd(QDateTime time);

    void Voc_SetOnRealData(unsigned long userdata,Voc_OnRealData* pOnRealData);
    void SetOnYQReady(unsigned long userdata,OnYQReady* pOnYQReady);

    virtual void RefeshConfig();//刷新配置温度
protected:   
    static void* AutoRun(void* pParam);

    void SaveTestTime();
    void DealInitCmd();
    //线程
    void RunThread();
    void ExitRunThread();
    static void* AutoRunThread(void* pParam);
    virtual void DoThread();

    CDBBusiness* m_pDB;//保存过程数据的数据库对象
    OnTestFinished* m_pOnTestFinished;
    OnWorkStatus* m_pOnWorkStatus;
    OnTotalNum* m_pOnTotalNum;
    OnYQBusy* m_pOnYQBusy;
    OnDebugInfo* m_pOnDebugInfo;
    OnTempValue* m_pOnTempValue;
    OnPaintCurve* m_pOnPaintCurve;

    Voc_OnRealData* m_pOnRealData;
    OnStatusValue* m_pOnStatusValue;
    OnScopeData* m_pOnScopeData;
    OnAFSData* m_pOnAFSData;
    OnGasRunInfo* m_pOnGasRunInfo;
    OnGasResult* m_pOnGasResult;
    OnGasCoef* m_pOnGasCoef;
    OnHhInfo* m_pOnHhInfo;
    OnRealCurveDot* m_pOnCurveDot;
    OnInitInfo* m_pOnInitInfo;
    OnYQReady* m_pOnYQReady;
    OnYQConsumeAlarm* m_pOnYQConsumeAlarm;
    OnShowCurve* m_pOnShowCurve;
    OnAlarm* m_pOnAlarm;

    //4 Begin 比色法
    OnBsReadValue* m_pOnBsReadValue;
    //4 End 比色法
    unsigned long m_userdata;
    bool bExitThread;
    bool m_bRunThread;
    pthread_t id_autorun;

    bool m_bRun;
    CArithmetic* pCalc;
    CYQ* m_pYQ[YQMAXCOUNT];
    bool m_bExitFlag;
    CYQ* GetYQ(const char* czID);

    QDateTime m_Qtime;
    bool m_bStart;//是否启动流程
    bool m_bDrawCurve;//暂停流程234
    bool m_bStopThread;//暂停线程
    bool  m_bCaliTag;
    int m_nType;
    int m_iSubYQType;//子类型

    //子类基本上都有的，统一放到基类
    int m_nCmd;//当前执行的流程文件 ，当前测试命令
    bool m_bInitFlag;//是否初始化,也是流程是否正常结束的标志，当m_bInitFlag为false时，则流程正常结束不需初始化

    stuProc m_pro;
    stuProc m_prepro;//前一条流程

    //记录调试日志到txt文件

    vector<struConsumeInfo> m_vtConsume;
    QMap<int,float> m_mapYQLevVal;



    int mkdir_r(const char *path);

    //对设备进行剩余减1
    bool ConsumeSub(int nProcessType,char *ID,float fVolume = 1);


//===========================================================================
//流程整理 begin
//===========================================================================
protected:
    ///////////////////////////////
    //流程线程主体函数
    //////////////////////////////
    //1.流程主线程
    virtual void DoProcess();
    //执行流程命令
    virtual STATUS DoCurrentCmd();
    //2.查询模块状态
    virtual int QueryStatus();
    //3.查询模块状态之一:处理下位机返回状态数据包
    virtual void GetMCUWorkStatus(unsigned char* bBuff);
    //4.查询模块状态之二:处理系统PID
    virtual int GetSysModuleStatus();

    ///////////////////////////////
    //流程线程支持函数
    //////////////////////////////
    //1.测试前初始化
    virtual int InitBeginOfTest(int nKind=0);
    //2.读流程文件
    virtual int ReadFlowFile();
    //3.读主流程文件
    virtual int ScanFlowFile(const char *filename);
    //4.读子流程文件
    virtual int ScanSubFlowFile();
    //5.保存数据
    virtual void SaveResultToDatabase(int nCodeID = 0);
    //8.处理数据标识
    virtual void DealDataTag(int nModelAddress,int nValue);
    //9.处理通用故障
    virtual void JudeFaultType(int nFault);
    //中止流程处理，包括停止采集数据等
    virtual void StopDealEvent(bool bTag = false);
    virtual bool VerifyRange(const char* czID,int nCurrentCurve,double dCurrentValue);
    virtual void MainWindowDebug(const char *cStr);

    //修改试剂耗量
    virtual void UpdateYQConsume(int iPID,int iTestType,int iLevelNum);

    virtual void SetRegisterWorkStatus(unsigned short nStatus);
    virtual void SetRegisterFaultType(unsigned short nType);

private:
    int m_nLastCodeID;//记录当前流程是哪个参数的流程,用于查找对应曲线

protected:
    std::queue <stuProc> m_quMyPro;//流程PID组
    int	m_nMcuBusy;//下位机是否空闲
    QTime m_SpacTime;//等待温度时间控制
    int m_nLastWorkPID;//上一条PID
    bool m_bSleepTag;
    bool m_bUpSwitchCurve;//向上切曲线标识位
    bool m_bDowSwitchCurve;//向下切曲线标识位;

    int m_nCurrSize;//当前执行流程编号
    int m_nTestWayTag;
    int m_nFinishResult;//流程是否正常结束
//===========================================================================
//流程整理 end
//===========================================================================


private:
    CSerialPort* pSerialPorts[COMCOUNT];
    bool bExit;
    bool m_bIdle;//是否空闲，用来防止重复发命令的
private:
    //写调试日志
    bool InsLogData(const char* czPath,QDateTime qDateTime,char *Content);

public:
    void InitConsumemap();
    void InitYQLevelVal();
    void OnNotifyConsumeSub(char *cID,int iAlarmLevel);

    virtual void DebugInfo(const char *cStr,int nNum = 0);
    //记录仪器运行日志到主数据库
    void RecordLog(LOG_LEVEL Level, const char *Content);
};

#endif // CPROCESS_H
