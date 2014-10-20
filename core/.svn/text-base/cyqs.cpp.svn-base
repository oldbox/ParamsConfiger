#include <stdio.h>
#include "cyqs.h"
#include "cdbbusiness.h"
#include "cprocess0.h"
#include "cprocess1.h"
#include "cprocess2.h"
#include "cprocess3.h"
#include "cprocess4.h"
#include "cprocess5.h"
#include "cprocess6.h"
#include "cprocess7.h"
#include <stdlib.h>
#include "math.h"

CYQs::CYQs(CSerialPort* pSerialPorts[COMCOUNT],CComm *pComm)
{
    m_pComm = pComm;
    m_pDB = new CDBBusiness("yqdata","yq.db");
    for(int iPortC=0;iPortC<COMCOUNT;iPortC++)
    {
        this->pSerialPorts[iPortC] = pSerialPorts[iPortC];
    }
    for(int iP=0;iP<YQMAXCOUNT;iP++)
    {
        pYQ[iP] = NULL;
        pProcess[iP] = NULL;
    }

    m_iYQCount = 0;

    vector<SYQPARAM> sctYQs;
    vector<struCurveInfo> sctCurves;
    sctYQs.clear();
    sctCurves.clear();
    //从数据库读取仪器代码
    if(m_pDB->GetYQs(sctYQs))
    {
        m_pDB->GetYQCurves(sctYQs,sctCurves);
        m_iYQCount = sctYQs.size();
        for(int i=0;i<m_iYQCount;i++)
        {
           SYQPARAM sYQ = sctYQs.at(i);
           struCurveInfo curve = sctCurves.at(i);
           //根据配置创建不同的流程
           if(!pProcess[sYQ.iProcessType])
           {
               switch(sYQ.iProcessType)
               {
               case 0:
                   pProcess[sYQ.iProcessType] = new CProcess0();                   
                   break;
               case 1:
                   pProcess[sYQ.iProcessType] = new CProcess1();
                   break;
               case 2:
                   pProcess[sYQ.iProcessType] = new CProcess2();
                   break;
               case 3:
                   pProcess[sYQ.iProcessType] = new CProcess3();
                   break;
               case 4:
                   pProcess[sYQ.iProcessType] = new CProcess4();
                   break;
               case 5:
                   pProcess[sYQ.iProcessType] = new CProcess5();
                   break;
               case 6:
                   pProcess[sYQ.iProcessType] = new CProcess6();
                   break;
               case 7:
                   pProcess[sYQ.iProcessType] = new CProcess7();
                   break;
               default:
                   pProcess[sYQ.iProcessType] = new CProcess0();
                   break;
               }
               //将串口传进去，并创建驱动
               pProcess[sYQ.iProcessType]->SetSerialPorts(pSerialPorts);
               pProcess[sYQ.iProcessType]->CreateDriver(sYQ.iProcessType);
               pProcess[sYQ.iProcessType]->SetOnTestFinished((unsigned long)this,OnTestFinished);
               pProcess[sYQ.iProcessType]->SetOnWorkStatus((unsigned long)this,OnWorkStatus);
               pProcess[sYQ.iProcessType]->SetOnTotalNum((unsigned long)this,OnTotalNum);
               pProcess[sYQ.iProcessType]->SetOnYQBusy((unsigned long)this,OnYQBusy);
               pProcess[sYQ.iProcessType]->SetOnDebugInfo((unsigned long)this,OnDebugInfo);
               pProcess[sYQ.iProcessType]->SetOnTempValue((unsigned long)this,OnTempValue);
               pProcess[sYQ.iProcessType]->SetOnPaintCurve((unsigned long)this,OnPaintCurve);
               pProcess[sYQ.iProcessType]->Voc_SetOnRealData((unsigned long)this,Voc_OnRealData);
               pProcess[sYQ.iProcessType]->SetOnStatusValue((unsigned long)this,OnStatusValue);
               pProcess[sYQ.iProcessType]->SetOnScopeData((unsigned long)this,OnScopeData);
               pProcess[sYQ.iProcessType]->SetOnAFSData((unsigned long)this,OnAFSData);
               pProcess[sYQ.iProcessType]->SetOnGasRunInfo((unsigned long)this,OnGasRunInfo);
               pProcess[sYQ.iProcessType]->SetOnGasResult((unsigned long)this,OnGasResult);
               pProcess[sYQ.iProcessType]->SetOnGasCoef((unsigned long)this,OnGasCoef);
               pProcess[sYQ.iProcessType]->SetOnHhInfo((unsigned long)this,OnHhInfo);
               pProcess[sYQ.iProcessType]->SetOnRealCurveDot((unsigned long)this,OnRealCurveDot);
               pProcess[sYQ.iProcessType]->setOnYQConsumeAlarm((unsigned long)this,OnYQConsumeAlarm);
               pProcess[sYQ.iProcessType]->SetOnInitInfo((unsigned long)this,OnInitInfo);
               pProcess[sYQ.iProcessType]->SetOnBsReadValue((unsigned long)this,OnBsReadValue);
               pProcess[sYQ.iProcessType]->SetOnShowCurve((unsigned long)this,OnShowCurve);
               //pProcess[sYQ.iProcessType]->SetOnBsGratBaseSampleVal((unsigned long)this,OnBsGratBaseSampleVal);
               pProcess[sYQ.iProcessType]->SetOnYQReady((unsigned long)this,OnYQReady);
               pProcess[sYQ.iProcessType]->SetOnAlarm((unsigned long)this,OnAlarm);

               pProcess[sYQ.iProcessType]->Run();

           }

           //创建仪器对象，并传入流程对象
           pYQ[i] = new CYQ(pProcess[sYQ.iProcessType]);
           //设置仪器属性
           pYQ[i]->SetYQParam(sYQ);
           //添加一个初始化仪器对象
           pYQ[i]->SetCurveInfo(curve);
        }

        //将仪器对象传到流程中
        for(int iPs=0; iPs<YQMAXCOUNT; iPs++)
        {
            if(pProcess[iPs])
            {
                 pProcess[iPs]->SetYQs(pYQ);
            }
        }
    }

    //外部接口放最后启动
    pMbTcpserver = new modbustcpslave();
    pMbTcpserver->SetOnModbusTcp03Data((void*)this,OnModbusTcp03Data);
    pMbTcpserver->SetOnModbusTcp16Data((void*)this,OnModbusTcp16Data);
}
CYQs::~CYQs()
{
    delete pMbTcpserver;

    for(int i=0;i<YQMAXCOUNT;i++)
    {        
        if(pProcess[i])
        {
            pProcess[i]->ExitRun();
            delete pProcess[i];
            pProcess[i] = NULL;
        }
        if(pYQ[i])
        {
            delete pYQ[i];
            pYQ[i] = NULL;
        }
    }
    delete m_pDB;
    m_pDB = NULL;
}



 CYQ* CYQs::GetYQ(const char* czID)
 {
     for(int i=0;i<m_iYQCount;i++)
     {
        SYQPARAM sYQ;
        pYQ[i]->GetYQParam(sYQ);
        if(strcmp(sYQ.czID,czID)==0)
            return pYQ[i];
     }
     return NULL;
 }

 CYQ* CYQs::GetYQByName(const char* czName)
 {
     for(int i=0;i<m_iYQCount;i++)
     {
        SYQPARAM sYQ;
        pYQ[i]->GetYQParam(sYQ);
        if(strcmp(sYQ.czName,czName)==0)
            return pYQ[i];
     }
     return NULL;
 }

 CYQ* CYQs::GetYQBySlaveID(int iSlaveID)
{
     for(int i=0;i<m_iYQCount;i++)
     {
        SYQPARAM sYQ;
        pYQ[i]->GetYQParam(sYQ);
        if(iSlaveID == sYQ.SlaveID)
            return pYQ[i];
     }
     return NULL;
}

int CYQs::GetYQCount()
{
    return m_iYQCount;
}

//返回
//1;系统忙正在测试，
//0;系统空闲，
//2;故障，如没找到流程文件等问题
//3流程已启动
//CmdFrom:1表示手动测试 2表示网络测试 3表示自动测试
int CYQs::SendCmd(const char* czYQID,int CmdID,int CmdFrom,QDateTime dt)
{
    int iRT = 0;
    CYQ* pYQ = GetYQ(czYQID);
    if(!pYQ)return -1;
    if(CMD_STOPTEST == CmdID)
    {
        pYQ->m_pProcess->SetThreadExit(true);
        return -1;
    }
    iRT = pYQ->m_pProcess->StartTestCmd(CmdID,CmdFrom,dt);

    return iRT;
}
bool CYQs::GetYQbIdle(const char* czYQID)
{
    bool bIdle = true;
    CYQ* pYQ = GetYQ(czYQID);
    if(!pYQ)return true;
    bIdle = pYQ->m_pProcess->GetbIdle();
    return bIdle;
}
void CYQs::OnTestFinished(unsigned long userdata,int iCMD,const char* czYQID)
{
    CYQs* pThis = (CYQs*)userdata;
    CYQ* pYQ = pThis->GetYQ(czYQID);
    if(!pYQ)return;
    char DateTime[64];memset(DateTime,0,64);

    QDateTime qDateTime = pYQ->m_RealInfo.TestTime;
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());

    SYQPARAM sYQ;
    switch (iCMD)
    {
    case 8051:
       pThis->m_pDB->Ins8051(pYQ->m_RealInfo,pYQ->m_pProcess->GetProcessType());
       //保存到输出寄存器
       pYQ->Set8051(pYQ->m_RealInfo.fResult);
       pYQ->SetWaterTag(pYQ->m_RealInfo.cDataID,strlen(pYQ->m_RealInfo.cDataID));
       pYQ->SetLastValue(pYQ->m_RealInfo.fResult);
       pYQ->SetLastTime(pYQ->m_RealInfo.TestTime);

       pYQ->GetYQParam(sYQ);
       if(1 == pYQ->GetProcessType())//电化学保存数据相联的详细数据
       {
            pThis->m_pDB->InsEchemDataInfo(pYQ->m_RealInfo);
       }
       else if(3 == pYQ->GetProcessType())//有机物保存数据相联的详细数据
       {
           pThis->m_pDB->SaveVocDataInfo(iCMD,pYQ->m_RealInfo.TestTime,sYQ.czID,pYQ->m_RealInfo);
       }
       else if(4 == pYQ->GetProcessType() || 6 == pYQ->GetProcessType())//比色法保存数据相联的详细数据
       {
            pThis->m_pDB->SaveBsDataInfo(pYQ->m_RealInfo);
       }
       //通知界面
       emit pThis->NotifyYQValue(pYQ);
       break;
    case 8052://保存线性扫描斜率
       pThis->m_pDB->InsLinear(pYQ->m_RealInfo.TestTime,pYQ->m_pProcess->GetProcessType(),pYQ->m_RealInfo.m_EchemDataInfo.fLinearSlope);
       break;
    case 8053://保存电极维护，标样校准，空白校准，初始化信息
       pThis->m_pDB->Ins2091(DateTime,pYQ->m_pProcess->GetProcessType(),"维护",LOG_NOMAL,pYQ->m_RealInfo.m_EchemDataInfo.czMaintain);
       break;
    case 3016:
        pThis->m_pDB->Ins3016(pYQ->m_RealInfo);
        //保存到输出寄存器
        pYQ->Set3016(pYQ->m_RealInfo.fCheckResult);
        pYQ->SetFixValue(pYQ->m_RealInfo.CheckChroma);
        pYQ->SetSampleTag(pYQ->m_RealInfo.cDataID,strlen(pYQ->m_RealInfo.cDataID));
        pYQ->SetLastValue(pYQ->m_RealInfo.fCheckResult);
        pYQ->SetLastTime(pYQ->m_RealInfo.TestTime);
        pYQ->GetYQParam(sYQ);
        if(1 == pYQ->GetProcessType())//电化学保存数据相联的详细数据
        {
             pThis->m_pDB->InsEchemDataInfo(pYQ->m_RealInfo);
        }
        else if(3 == pYQ->GetProcessType())//有机物保存数据相联的详细数据
        {
            pThis->m_pDB->SaveVocDataInfo(iCMD,pYQ->m_RealInfo.TestTime,sYQ.czID,pYQ->m_RealInfo);
        }
        else if(4 == pYQ->GetProcessType())//比色法保存数据相联的详细数据
        {
            pThis->m_pDB->SaveBsDataInfo(pYQ->m_RealInfo);
        }
        break;
    case CMD_BANKADJUST://空白校准
        pThis->m_pDB->UpdateCurveManger(CMD_BANKADJUST,pYQ->m_CurveInfo);
        break;
    case CMD_SAMPLEADJUST://标样校准
        if(pYQ->GetProcessType() == 3)
        {
            char tmp[64];memset(tmp,64,0);
            GetAfxConfig()->GetIni("VOC_SET","AdjustMode",tmp);
            int nOnOff = atoi(tmp);
            if(nOnOff == 0)//与内标设置的保留时间比较
            {
                pThis->m_pDB->InsVocinternal(pYQ->m_RealInfo.TestTime,pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_RealInfo.m_vocDataInfo.RemainTime,pYQ->m_RealInfo.CheckChroma,pYQ->m_RealInfo.fCheckResult,pYQ->m_RealInfo.m_vocDataInfo.Area);
            }
            else//与前一次内标校准的保留时间
            {
                float RemainTime,RTime,Conc,RConc;
                if(pThis->m_pDB->GetLastVocinternal(RemainTime,RTime,Conc,RConc))
                    pThis->m_pDB->InsVocinternal(pYQ->m_RealInfo.TestTime,RemainTime,pYQ->m_RealInfo.m_vocDataInfo.RemainTime,pYQ->m_RealInfo.CheckChroma,pYQ->m_RealInfo.fCheckResult,pYQ->m_RealInfo.m_vocDataInfo.Area);
                else
                    pThis->m_pDB->InsVocinternal(pYQ->m_RealInfo.TestTime,pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_RealInfo.m_vocDataInfo.RemainTime,pYQ->m_RealInfo.CheckChroma,pYQ->m_RealInfo.fCheckResult,pYQ->m_RealInfo.m_vocDataInfo.Area);
            }
        }
        else
            pThis->m_pDB->UpdateCurveManger(CMD_SAMPLEADJUST,pYQ->m_CurveInfo);
        break;
    default:
        break;
    }
}
void CYQs::OnWorkStatus(unsigned long userdata, int iProcessType,int iWorkNum, const char *czName)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyWorkStatus(iProcessType,iWorkNum,czName);
}
void CYQs::OnTotalNum(unsigned long userdata, int iProcessType,int iTotalNum)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyTotalNumber(iProcessType,iTotalNum);
}
void CYQs::OnYQBusy(unsigned long userdata,int iType,int iProcessType, bool bBusy,int iCmdType,bool bProcFile)
{
    CYQs* pThis = (CYQs*)userdata;
    switch(iType)
    {
    case MSG_WORK:
        emit pThis->NotifyYQBusy(iProcessType,bBusy);
        break;
    case MSG_CMD:
        emit pThis->NotifyCmdType(iProcessType,iCmdType);
        break;
//    case MSG_PROCFILE:
//        emit pThis->NotifyProcFileStatus(iProcessType,bProcFile);
//        break;
    default:
        break;
    }
}
void CYQs::OnDebugInfo(unsigned long userdata, int iProcessType, const char *cStr)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyDebugInfo(iProcessType,cStr);
}
void CYQs::OnTempValue(unsigned long userdata, float fTempValue)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyTempValue(fTempValue);
}
void CYQs::OnPaintCurve(unsigned long userdata,struEchemVector* pSignal, int iType)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyPaintCurve(pSignal,iType);
}
void CYQs::Voc_OnRealData(unsigned long userdata,stuRealData* pOnRealData)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyRealData(pOnRealData);
}
void CYQs::OnStatusValue(unsigned long userdata,struGasStatusData* pTempValue)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyStatusValue(pTempValue);
}
void CYQs::OnScopeData(unsigned long userdata,struGasVector* pScope,int nType)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyScopeData(pScope,nType);
}
void CYQs::OnAFSData(unsigned long userdata,struAFSVector* pAFS,int nType)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyAFSData(pAFS,nType);
}
void CYQs::OnGasRunInfo(unsigned long userdata,int nStatus)
{
    CYQs* pThis = (CYQs*)userdata;
    if(0 == nStatus)
    {
        emit pThis->NotifyEnterMaintain();//紫外烟气通知进入维护界面
    }
}
//void CYQs::OnShowCurve(unsigned long userdata, vector<double>& dataX,vector<double>& dataY, int nType)
//{
//    CYQs* pThis = (CYQs*)userdata;
//    emit pThis->NotifyShowCurve(dataX,dataY,nType);
//}
void CYQs::OnShowCurve(unsigned long userdata, vector<double>& dataX,vector<double>& dataY, int nType)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyShowCurve(&dataX,&dataY,nType);
}
//4 Begin 比色法
void CYQs::OnGasResult(unsigned long userdata,float fResultZA0,float fResultZA1,float fResultZB2,float fResultZB4)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyGasResult(fResultZA0,fResultZA1,fResultZB2,fResultZB4);
}
void CYQs::OnGasCoef(unsigned long userdata,float fCoef)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyGasCoef(fCoef);
}
////标定曲线时，显示基线值、样品值
//void CYQs::OnBsGratBaseSampleVal(unsigned long userdata,int iCMDType,int iDataType,double dBaseSampleValue)
//{
//    CYQs* pThis = (CYQs*)userdata;
//    emit pThis->NotifyBsGratBaseSampleVal(iCMDType,iDataType,dBaseSampleValue);
//}
//4 End 比色法

//0表示h 1表示H 2表示清空峰高，测试结果 3表示测试结果
void CYQs::OnHhInfo(unsigned long userdata, int nType,int nTypeID,double fHeight)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyHhInfo(nType,nTypeID,fHeight);
}
void CYQs::OnRealCurveDot(unsigned long userdata, double fValue_X, double fValue_Y, double fValue_Y2,int nPos)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyCurveDot(nPos,fValue_X,fValue_Y,fValue_Y2);
}
//流程结束通知是否进行初始化
void CYQs::OnInitInfo(unsigned long userdata,bool bInit)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyInitInfo(bInit);
}
//界面显示基线值、样品值，计算吸光度
//iDataType为0时，表示显示原子荧光峰高或峰面积
void CYQs::OnBsReadValue(unsigned long userdata,int iDataType,double dBaseSampleValue,QDateTime qTime)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyBsReadValue(iDataType,dBaseSampleValue,qTime);
}
void CYQs::OnYQReady(unsigned long userdata, bool bOk)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyYQReady(bOk);
}
void CYQs::OnAlarm(unsigned long userdata, const char *czContext, int nType)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyAlarm(czContext,nType);
}

void CYQs::OnModbusTcp03Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* outData,int& ByteCount)
{
    CYQs* pThis = (CYQs*)userdata;
    CYQ* pYQ = pThis->GetYQBySlaveID(SlaveID);
    if(pYQ)
    {
        pYQ->m_pModbusRegister->Get03Data(iFirstReg,iRegCount,outData);
        ByteCount = iRegCount*2;
    }
}

void CYQs::OnModbusTcp16Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* czRegContent,int ByteCount)
{
    CYQs* pThis = (CYQs*)userdata;
    char czTmp[255];
    char DateTime[64];
    char czValue[255];

    //获取运行模式
    memset(czValue,0,255);
    GetAfxConfig()->GetIni("SYSSET","MOD",czValue);
    int iMod = atoi(czValue);

    CYQ* pYQ = pThis->GetYQBySlaveID(SlaveID);
    if(pYQ)
    {
        for(int i=0;i<iRegCount;i++)
        {
            pYQ->m_pModbusRegister->Set03Data(iFirstReg+i,czRegContent+i*2,false);
            if((40300 == iFirstReg)&&(3 == iMod))
            {
                 memset(czTmp,0,255);
                 memset(DateTime,0,64);
                 int16_t wCmdID = MAKEWORD(czRegContent[1],czRegContent[0]);
                 QDateTime dt = QDateTime::currentDateTime();
                 sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",dt.date().year(),dt.date().month(),dt.date().day(),dt.time().hour(),dt.time().minute(),dt.time().second());
                 pYQ->m_pProcess->StartTestCmd(wCmdID,2,dt);//2表示网络命令
                 if( 0x0000 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络中止测试命令");
                 }
                 else if(0x0001 == wCmdID)
                 {
                      strcpy(czTmp,"发送水样测试命令");
                 }
                 else if(0x0002 == wCmdID)
                 {
                      strcpy(czTmp,"发送网络标定曲线命令");
                 }
                 else if(0x0003 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络空白校准命令");
                 }
                 else if(0x0004 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络标样校准命令");
                 }
                 else if(0x0005 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络管路清洗命令");
                 }
                 else if(0x0007 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络初始化命令");
                 }
                 else if(0x0008 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络电极维护命令");
                 }
                 else if(0x0009 == wCmdID)
                 {
                     strcpy(czTmp,"发送网络线性扫描命令");
                 }
                 else if(0x000A == wCmdID)
                 {
                     strcpy(czTmp,"发送网络标样核查命令");
                 }
                 pThis->m_pDB->Ins2091(DateTime,pYQ->m_pProcess->GetProcessType(),"仪器",LOG_NOMAL,czTmp);
            }

        }
    }
}

void CYQs::OnYQConsumeAlarm(unsigned long userdata, const char* cID,int iAlarmLevel)
{
    CYQs* pThis = (CYQs*)userdata;
    emit pThis->NotifyYQConsumeAlarm(cID,iAlarmLevel);
}

bool CYQs::UpdateYQInfo()
{
    vector<SYQPARAM> sctYQs;
    vector<struCurveInfo> sctCurves;
    sctYQs.clear();
    sctCurves.clear();
    //从数据库读取仪器代码
    if(m_pDB->GetYQs(sctYQs))
    {
        m_pDB->GetYQCurves(sctYQs,sctCurves);
        CYQ* pYQ = NULL;
        for(int i = 0;i < sctYQs.size();i++)
        {
           SYQPARAM sYQ = sctYQs.at(i);
           struCurveInfo curve = sctCurves.at(i);
           pYQ = GetYQ(sYQ.czID);
           if(pYQ)
           {
               pYQ->SetYQParam(sYQ);
               pYQ->SetCurveInfo(curve);
               pYQ = NULL;
           }
        }
    }
}
