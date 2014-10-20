#ifndef CPROCESS2_H
#define CPROCESS2_H

#include "cprocess.h"
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "cgetresult.h"
#include <QMessageBox>

class CProcess2 : public CProcess
{
public:
    CProcess2();
    virtual ~CProcess2();

    int GetProcessType();
    int InitBeginOfTest(int nKind=0);
    void StartGasTestCmd(QDateTime time);
    void DoProcess();
    void DealProcess1();
    void DealProcess2();
    void DealProcess3();
    void DealProcess4();
    void CaclTestResult();
    void ReadStatusData();
    int StartInitYQ();
    int StartPreHeat();
    void InitCaliCoef();
    void DoThread();
    void DealCaliCoef(int nCaliKind);
    void DealFlowCaliCoef();
    void SaveResultData();
    void ZeroCaliRestoreFactorySetting();
    void SpanCaliRestoreFactorySetting();
    void FlowCaliRestoreFactorySetting();
    void GetScopeData(struGasVector* pScope);
    void SetStartMaintain(bool bFlag){m_bStartMaintain = bFlag;}//进入模块维护界面标志
    bool SendControlTemp(int nAdd,int cmd,int nIO,int nOnOff);
    void RecordLog(LOG_LEVEL Level,const char *Type,const char *Content);//记录日志


private:
    void SaveStatusData();
    void NotifyDrawCurve(int nType);
    void GetCaclScope(int nType);
    void SaveGasSpecData();
    void CtrlTempChange();//温控
    bool SendControlCmd(int nAdd,int cmd,int nIO,int nOnOff);
    void CreateDataBase();//创建数据库
    int  TestM1Connect();
    int  TestM2Connect();
    int  TestM3Connect();
    int  TestM4Connect();
    int  TestM5Connect();
    float ChangeChromaToPPM(int nKind);
    void DealInputStatus(int nIO1,int nIO2,int nIO3,int nIO4);
    bool DealAutoSpanCali(int nKind);
    bool DealAutoZeroCali(int nKind);
    void DealOutputResult(float fS02,float fN0,float f02);
    void DealOutputAlarm();
    bool DealDCFStatus(int nAdd,int cmd,int nIO,int nOnOff);
    bool ReadDCFStatus(int nIO,int nOnOff);
    int GetDataLCMax(int nKind);
    void ConsumeManage();
    void NotifyAlarmInfo();


     unsigned short m_NrPixels;
     double m_StatusValue[YQSTATUSCOUNT];//[0]光源温度,[1]光谱仪温度,[2]板载温度,[3]制热块温度,[4]气室入口,[5]气室出口,[6]机箱,[7]流速，[8]氧含量
     struGasStatusData m_struTemp;
     struGasVector m_struVector;
     bool m_bSaveTag;
     bool m_bScopeSaveFlag;
     QTime m_SpacTime1;
     QTime m_SpacTime2;
     QTime m_SpacTime3;
     QTime m_SpacTime4;
     QTime m_SaveSpacTime;
     QTime m_SpacConsume;
     bool m_bConsumeFlag;
     bool m_bLightTag;//制冷室风扇开关标记
     bool m_bSpecTag;//光谱仪风扇开关标记
     bool m_bBoxTag;//机箱风扇开关标记
     bool m_bFanTag1;//散热风扇1
     bool m_bFanTag2;//散热风扇2
     double m_dScopeLamp;
     double m_dScopeBk;
     int m_nCount;
     int m_nIndex;
     float m_fResultN0;//测试结果
     float m_fResultN02;
     float m_fResultS02;
     float m_fResult02;
     float m_fOutPutN0;//输出值，用于保存原有输出
     float m_fOutPutS02;
     float m_fOutPut02;
     float m_fFlowValue;
     bool m_bProcessFlag;
     bool m_bSpanTag;
     float m_fN0SpanCoef;//校准系数
     float m_fNOZeroCoef;
     float m_fN02SpanCoef;
     float m_fNO2ZeroCoef;
     float m_fS02SpanCoef;
     float m_fSO2ZeroCoef;
     float m_f02SpanCoef;
     float m_fO2ZeroCoef;
     float m_fFlowSpanCoef;
     float m_fFlowZeroCoef;
     analysisData * m_aAnalysis;
     resultData * m_aresult;
     float m_fCaliValueN0[5];//用来保存校准时的5组数据
     float m_fCaliValueS02[5];
     float m_fCaliValue02[5];
     float m_fCaliFlowSpan[5];//流速校准的跨度流速值
     float m_fCaliFlowZero[5];//流速校准的零点流速值
     int m_nCountCali;//启动校准后，需保存5组连续数据，再求平均值，故需m_nCountCali计数
     int m_nCountCaliFlow;//计数流速校准次数
     bool m_bStartMaintain;//模块维护标记
     bool m_bAutoCali[2];//校准气体标记，[0]为true,[1]为false表示从气体1开始校准，[0]为false,[1]为true气体1校准完成，开始校准气体2
     bool m_bAutoCailTag;//正在执行校准的标记
     bool m_bAutoSpanTag;//自动跨度校准标记,当为true时是自动跨度校准，false时为零点校准
     int m_nTimes;
     float m_fAutoResult[20];
     bool m_bStopFlag;//排放停炉标记
     bool m_bMaintainFlag;//仪器维护标记
     bool m_bAlarmFlag[8];//输出报警标记，上次是否输出高低平，分别对应8个配置通道
     bool m_bHighLevel;//开关量输入有高电平为true,否则为false;用于保持原有输出判断

public:
    QDateTime m_qTestTime;
    bool m_bM1Flag;//模块M1通讯故障标记
    bool m_bM2Flag;//模块M2通讯故障标记
    bool m_bM3Flag;//模块M3通讯故障标记
    bool m_bM4Flag;//模块M4通讯故障标记
    bool m_bM5Flag;//模块M5通讯故障标记
};

#endif // CPROCESS2_H
