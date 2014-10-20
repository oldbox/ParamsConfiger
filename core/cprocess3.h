#ifndef CPROCESS3_H
#define CPROCESS3_H
#include "cprocess.h"
#include <queue>
#define MAX_DATABUFFER 7000 //下位机数据最大长度

struct stuIniTempt
{
    stuIniTempt(){PJJTemp_Init=FIDTemp_Init=MxzTemp_Init=YQTemp_Safe=BoxTemp_Init=PJJTemp_Jx=PJJTemp_Hk=40;\
                  SampleTime=PJJTime_jx=2;CSTime_BY=CSTime=GCTime=PJJTime_hk=2.0;CSPress=Nitrpress=0.0;\
                 DescTemp[0]=DescTemp[1]=DescTemp[2]=DescTemp[3]=DescTemp[4]=50;\
                 TempRate[0]=TempRate[1]=TempRate[2]=TempRate[3]=TempRate[4]=5;\
                 TempRTime[0]=TempRTime[1]=TempRTime[2]=TempRTime[3]=TempRTime[4]=1;FireOnOff=FireAddr=FireDCF1=FireDCF2=1;}
    int PJJTemp_Init;     //捕集阱初始温度
    int PJJTemp_Jx;       //捕集阱解析温度
    int PJJTemp_Hk;       //捕集阱烘烤温度
    int FIDTemp_Init;     //FID初始温度
    int MxzTemp_Init;   //毛细柱
    int BoxTemp_Init;   //箱体温度检测
    int YQTemp_Safe;    //仪器安全温度
    float CSPress;      //吹扫压力
    float Nitrpress;    //氮气压力


    int SampleTime;//挥发性：进样时间(单位:分钟) ，半挥发性则为是抽样次数
    float CSTime_BY;//吹扫时间(单位:分钟),标样
    float CSTime;//吹扫时间(单位:分钟)
    float GCTime;//干吹时间(单位:分钟)
    int PJJTime_jx;//解析时间(挥发性：秒，半挥发性：分钟)
    float PJJTime_hk;//烘烤时间(单位:分钟)

    int GainDataRate;//采样速率设置
    int GainDataRTime;//采样时间

    int DescTemp[5];//目标温度
    int TempRate[5];//速率
    int TempRTime[5];//持续时间

    int FireAddr;//地址
    int FireDCF1;//氢气阀
    int FireDCF2;//空气阀
    int FireOnOff;//流程完成是否关闭火
};

class CVocArithmetic;
class CProcess3 : public CProcess
{
public:
    CProcess3();
    virtual ~CProcess3();
    int GetProcessType();

    int StartTestCmd(int nCmd,int nKind,QDateTime dt);

    void RefeshConfig();//刷新配置温度
private:
   //流程执行主线程
   void DoProcess();
   //测试之前初始化
   int InitBeginOfTest(int nKind=0);
   //读配置文件
   bool ReadIniFile(const char czTeam[],int& nValue);
   bool ReadIniFile(const char czTeam[],float& fValue);


   void SaveSrcDataToTxt();
   //寻峰
   bool SeachPeak();
   //计算结果
   void Calculate();

   //源始数据点
   void HaveNewData(unsigned int nId,unsigned int nRemainTime, double fVolValue, double fTemptValue);

   //根据相对窗宽和保留时间查找峰
   int SeachIndexFromPeaks(double fRTime,float fWinFactor,float fAdjustDiff = 0.0);

   void ReadData();
   //读主流程文件
   int ScanFlowFile(const char *filename);
   //执行命令
   STATUS DoCurrentCmd();
   int VocDoCurrentCmd();
   //读取总线上模块状态信息
   int QueryStatus();
   //处理下位机返回状态数据包
   void GetMCUWorkStatus(unsigned char* bBuff);
   //处理系统PID
   int GetSysModuleStatus();
   //处理协调模块一些温度值
   void ProcessRealData(unsigned char *bBuff,bool bFault = false);

   //保存数据
   void SaveResultToDatabase(int nCodeID = 0);

   bool ReadModbus(int nReg, int nCount,unsigned char* czData);

   void PutPotData(unsigned int nRemainTime,double fVolValue,double fTemptValue);
   //第1个点返回，第2个点不返回，第3个点以后返回
   bool GetPotData(unsigned int &nRemainTime,double &fVolValue,double &fTemptValue,bool bFinish = false);

   int ReadFlowFile();

   //更新耗材记录
   void RefreshConsumeData();

   //点火灭火保护机制
   void  ReFireProtect();

   //点火
   void CtrlFire(int nKind,bool bOnOff);
private:
   int m_nGainDataRate;//采集数据速率，(单位:个/秒)
   int m_nGainDataTime;//采集数据时间,(单位:分钟)


   stuRealData m_data;//工况温度等相关信息
   //保留时间、电压値、温度
   vector<double> m_vtRT,m_vtVol,m_vtTp;
   vector<double> m_vtNitr;//氮气压力

   deque<double> m_deTempVol,m_deTempTp;
   deque<unsigned int> m_deTempRT;

   CVocArithmetic* m_pCalc;

   int m_DetecAddress;//协调模块地址
   pthread_t m_subthread;
   bool m_bProcThread;//读协调模块的线程

   bool m_bSubThread;//读数据线程
   //读数据
   int m_nReadDataResult;
   bool m_bReadDataTag;
   unsigned int m_nStartReg;
   unsigned int m_nRemainTime;
   unsigned int m_nReadDataTime;

   bool m_bReadBaseLine;//读基线标识false为测试 ,true为基线
   bool m_bCalcluteTag;//计算结果标识
   int m_nNoiceValue;

   float m_fNitMin;//氮气压力量程
   float m_fNitMax;//氮气压力量程
   float m_fAirMin;//空气压力量程
   float m_fAirMax;//空气压力量程

   int m_nDetector;//检测器0 FID   1:ECD


   stuIniTempt m_stuInitTempt;
   int m_nFireTimes;
   int m_nFireWaitTag;//等待标识  0.点燃状态  1.开阀标识  2.关阀标识  3.点火标识
   //耗材计时器
   QTime m_qtHcTime;
   QTime m_qtFireTime;

private:
   void MainWindowDebug(const char *cStr);

   void RunSubThread();
   void ExitSubThread();

   void StopDealEvent(bool bTag = false);//中止流程处理，包括停止采集数据等
   static void* ReadDataThread(void* pParam);

   void DoThread();

   void Work();

   //基础温度等条件初始化,一般开机前得运行
   bool BaseInit_PJJ();
   bool BaseInit_FID();
   bool BaseInit_BOX();
   bool BaseInit_MXZ();
   bool MXZJJ_SW();//阶级升温

   //仪器空闲时,判断基础条件是否达到测试条件,仪器测试时,则不再检测
   bool ReadyGo();

   //测试过程检测仪器各部件工作状态是否正常
   bool SelftCheck();

   //仪器内部温度判断,安全温度检测
   bool YQTemptCheck();

   void CloseAllTempt();

   //等待温度是否达到目标温度
   int SysWaitTemplate(const int nKind,const float fDescValue);
   bool WaitForTemplate(const int nKind,const float fDescValue);
   bool WaitForTime(int nTime);

   //协调模块状态字处理
   void DetectError(int nStatus);

   //检查恒温开关是否打开
   bool CheckHWOnOff(int nChannal);
   //检查线性升级开关是否打开
   bool CheckLineOnOff(int nChannal);
};

#endif // CPROCESS3_H
