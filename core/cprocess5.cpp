#include "cprocess5.h"
#include "stdlib.h"
#include <QDateTime>
#include <iostream>
#include "VocArithmetic.h"
using namespace std;

CProcess5::CProcess5()
{
    m_bYQBusy = false;
    m_nCmd = 0;
    m_fRangeValue = 1.0;
    m_bEndFlag  = true;
    m_nCount = 0;
    m_nKind = 0;
    m_bExitRead = false;
    m_bUpSwitchCurve = false;
    m_bDowSwitchCurve = false;
    m_nValueCount = 0;
    m_nParaRate = 0;
    cout << "CProcess5::CProcess5()" << endl;
}

CProcess5::~CProcess5()
{
}

int CProcess5::GetProcessType()
{
    return 5;
}
int CProcess5::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);
    char czCurveID[2];memset(czCurveID,0,2);
    sqlite_data datalist;
    CYQ* pYQ = NULL;
    for(int i=0;i<YQMAXCOUNT;i++)
    {
        if(m_pYQ[i])
        {
            pYQ = m_pYQ[i];
            //初使化数据结构对象
            pYQ->m_RealInfo.TestTime = m_Qtime;//测试时间
            pYQ->m_RealInfo.fResult = 0.0;
            pYQ->m_RealInfo.fCheckResult = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.BaseValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.SampleValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.Absorbency = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.K = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.B = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.R = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = 0.0;//空白吸光度
            pYQ->m_RealInfo.CheckChroma = 0.0;//标样浓度
            pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = 0.0;//标样吸光度

            pYQ->m_RealInfo.m_EchemDataInfo.fHeight1 = 0.0;
            pYQ->m_RealInfo.m_EchemDataInfo.fHeight2 = 0.0;
            memset(pYQ->m_RealInfo.cDataID,0,16);
            if (1 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"hd",2);
            }
            else if(2 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"nt",2);
            }

            char cItem[64];memset(cItem,0,64);
            sprintf(cItem,"WorkCurveID_%s",pYQ->m_sYQ.czID);
            GetAfxConfig()->GetIni("SYSSET",cItem,czCurveID);

            if(strlen(czCurveID) > 0)
            {
                pYQ->m_CurveInfo.CurveID = atoi(czCurveID);
                bool isResult = m_pDB->GetCurveManger(pYQ->m_sYQ.czID,atoi(czCurveID),0,datalist);
                if(isResult)
                {
                    if(!datalist.IsEmpty())
                    {
                        pYQ->m_CurveInfo.R = atof(datalist.GetData(1,5));
                        pYQ->m_CurveInfo.K = atof(datalist.GetData(1,6));
                        pYQ->m_CurveInfo.B = atof(datalist.GetData(1,7));
                        pYQ->m_CurveInfo.BlankAbsorb = atof(datalist.GetData(1,8));
                        pYQ->m_CurveInfo.CheckChroma = atof(datalist.GetData(1,9));
                        pYQ->m_CurveInfo.StdAbsorb = atof(datalist.GetData(1,10));
                    }
                }
            }
        }
    }
    datalist.Free();
    m_struAFSVector.m_vtnWaterSignal.clear();
    m_struAFSVector.m_vtnWaterTime.clear();
    m_vProcFile.clear();

    m_bExitFlag = false;
    m_nLastWorkPID = -1;
    m_bExitRead = false;

    //通知清除上次测试峰高与结果
    if(m_pOnHhInfo)
    {
        m_pOnHhInfo(m_userdata,2,0,1.0);
    }
    return 0;
}
//nKind为1表示手动测试 2表示网络测试 3表示自动测试
int CProcess5::StartTestCmd(int nCmd,int nKind,QDateTime dt)
{
    char str[64];
    int nPIDCount = 0;
    MainWindowDebug("准备测试");
    if(GetbIdle())
    { 
        m_nTestWayTag = nKind;
        m_Qtime = dt;
        m_nCmd = nCmd;
        InitBeginOfTest(nKind);

        MainWindowDebug("启动测试命令");
        memset(str,0,64);
        strcpy(str,"启动测试命令:");
        DebugInfo(str,2);
        RecordLog(LOG_NOMAL,"启动测试命令");

        //通知主界面测试类型
        if(m_pOnYQBusy)
        {
            m_pOnYQBusy(m_userdata,2,this->GetProcessType(),m_bYQBusy,m_nCmd,true);
        }
        nPIDCount = ReadFlowFile();
        if(nPIDCount>0)
        {
            m_bRun = true;//执行线程
            SetbIdle(false);

            memset(str,0,64);
            strcpy(str,"流程文件读取成功");
            DebugInfo(str,0);
            //通知主界面流程文件正常
            if(m_pOnYQBusy)
            {
                m_pOnYQBusy(m_userdata,3,this->GetProcessType(),m_bYQBusy,0,true);
            }
            return MCU_IDEL;
        }
        else
        {
            //通知主界面流程文件异常
            if(m_pOnYQBusy)
            {
                m_pOnYQBusy(m_userdata,3,this->GetProcessType(),m_bYQBusy,0,false);
            }
            RecordLog(LOG_ALARM,"没找到相应的流程文件");

            return MCU_FAULT;//流程文件没找到
        }
    }
    else
    {
        memset(str,0,64);
        strcpy(str,"系统忙，正在测试");
        DebugInfo(str,0);
        return MCU_BUSY;
    }
}

void CProcess5::DoProcess()
{
    bool bFinish = true;
    int nResult = 1;
    int nTimeOutTimes = 0;
    int nTotsize = 0;
    m_bYQBusy = true;
    m_bEndFlag = true;
    m_bInitFlag = true;
    char str[64];
    char czLog[255];
    nTotsize = m_vProcFile.size();
    //通知主界面仪器忙
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,1,this->GetProcessType(),m_bYQBusy,0,true);
    }

    while(true)
    {
        if(m_bExitFlag)
        {
            pDriver->AllModuleReset();
            DealDataTag(255,10);
            memset(str,0,64);
            strcpy(str,"中止测试");
            DebugInfo(str,0);
            MainWindowDebug("中止测试");
            m_nCmd = CMD_STOPTEST;
            RecordLog(LOG_WARNING,"中止测试");
            m_bEndFlag = false;
            m_bExitRead = true;
            break;
        }
        if(bFinish)
        {
            if(m_vProcFile.size() == 0)
            {
                printf("normal end %d \n",m_pro.PID);
                MainWindowDebug("流程正常结束");
                RecordLog(LOG_NOMAL,"流程正常结束");
                m_bInitFlag = false;
                m_bExitRead = true;
                break;
            }
            else
            {
                m_pro.PID = m_vProcFile.front().PID;
                m_pro.Cmd = m_vProcFile.front().Cmd;
                memset(m_pro.cProcName,0,64);
                memcpy(m_pro.cProcName,m_vProcFile.front().cProcName,strlen(m_vProcFile.front().cProcName));
                m_pro.ModelAddress = m_vProcFile.front().ModelAddress;
                m_pro.ParaCount = m_vProcFile.front().ParaCount;
                m_pro.LogTag = m_vProcFile.front().LogTag;
                m_nCurrSize = nTotsize - m_vProcFile.size() + 1;
                if(m_pOnWorkStatus)
                {
                    m_pOnWorkStatus(m_userdata,this->GetProcessType(),m_nCurrSize,m_pro.cProcName);
                }
                for(int k = 0;k < 6;k++)
                {
                    m_pro.arrParaValue[k] = m_vProcFile.front().arrParaValue[k];
                }
                if(DoCurrentCmd() == CMD_OK)//执行流程
                {
                    m_vProcFile.erase(m_vProcFile.begin());
                    usleep(500000);//延时500毫秒才处理读状态
                    bFinish = false;

                    memset(str,0,64);
                    strcpy(str,"写modbus 10指令成功 ");
                    DebugInfo(str,1);

                 }else if(CMD_TIMEOUT)
                 {
                    DealDataTag(255,9);
                    memset(str,0,64);
                    strcpy(str,"写modbus 10指令失败 ");
                    DebugInfo(str,1);

                    memset(czLog,0,255);
                    memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                    strcat(czLog," 通讯故障");
                    RecordLog(LOG_ALARM,czLog);
                    m_bExitRead = true;
                    break;
                 }
            }
        }
        else
        {
            usleep(200000);
            nResult = ReadMCUStatusInfo();
            if(nResult == MCU_TIMEOUT)
            {
                 nTimeOutTimes++;
                 memset(czLog,0,255);
                 memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                 strcat(czLog," 读取状态超时");
                 RecordLog(LOG_WARNING,czLog);

                 if(nTimeOutTimes > 10)
                 {
                     printf("Read Modbus fail %d \n",m_pro.PID);
                     DealDataTag(255,9);

                     memset(str,0,64);
                     strcpy(str,"读modbus 03指令失败,超时 ");
                     DebugInfo(str,1);

                     MainWindowDebug("读取状态超时");
                     memset(czLog,0,255);
                     memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                     strcat(czLog," 读取状态失败");
                     RecordLog(LOG_ALARM,czLog);
                     m_bExitRead = true;
                     break;
                 }
                 else
                 {
                     printf("read data timeout %d \n",m_pro.PID);

                     memset(str,0,64);
                     strcpy(str,"读modbus 03指令,等待");
                     DebugInfo(str,1);
                 }
            }
            else if(nResult == MCU_IDEL)
            {
                m_prepro.ModelAddress = m_pro.ModelAddress;
                m_prepro.PID = m_pro.PID;
                nTimeOutTimes = 0;
                bFinish = true;
                MainWindowDebug("流程正常");
                if(1 == m_pro.LogTag)
                {
                    RecordLog(LOG_NOMAL,m_pro.cProcName);
                }
            }
            else if(nResult >= MCU_FAULT)
            {
                 //通知UI工作状态
                MainWindowDebug("流程故障终止");
                DealDataTag(m_pro.ModelAddress,nResult);

                memset(str,0,64);
                strcpy(str,"读modbus 03指令失败，故障");
                DebugInfo(str,1);

                JudeFaultType(nResult);
                m_bExitRead = true;
                break;
            }
        }
    }
    SaveResultToDatabase(m_nCmd);
    SetbIdle(true);
    DealInitCmd();
}
STATUS CProcess5::DoCurrentCmd()
{
    STATUS nRet = CMD_OK;
    char czLog[MAX_SIZE];
    if (m_pro.ModelAddress == 0)//系统模块
    {
        nRet = CMD_OK;
    }
    else
    {
        int16_t wValue = 0;
        unsigned char bBuff[MAX_SIZE];
        memset(bBuff,0,MAX_SIZE);

        if( 78 == m_pro.PID)//信号采集，采集数据小于等于10000
        {
            if(m_pro.arrParaValue[2] > 10000)
            {
                m_pro.arrParaValue[2] = 10000;
            }
            m_nValueCount = m_pro.arrParaValue[2];
            m_nParaRate = m_pro.arrParaValue[0];
        }

        //命令字
        wValue = m_pro.Cmd;
        bBuff[0] = HIBYTE(wValue);
        bBuff[1] = LOBYTE(wValue);
        //参数
        for (int i = 0;i < m_pro.ParaCount;i++)
        {
            wValue = m_pro.arrParaValue[i];
            bBuff[i*2+2] = HIBYTE(wValue);
            bBuff[i*2+3] = LOBYTE(wValue);
        }
        int nIndex;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(pDriver->WriteModbus(m_pro.ModelAddress,40001,m_pro.ParaCount+1,bBuff))
            {
                usleep(200000);
                DealReagentsChange();
                break;
            }
            else
            {
                usleep(100000);
                memset(czLog,0,MAX_SIZE);
                memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                strcat(czLog," 写超时");
                RecordLog(LOG_WARNING,czLog);
            }
        }
        if (nIndex == 3)
        {
            nRet = CMD_TIMEOUT;//发送命令失败
        }
        else
        {
            nRet = CMD_OK;

        }
    }
    return nRet;
}
int CProcess5::ReadMCUStatusInfo()
{
    int nResult = MCU_BUSY;
    unsigned char bBuff[MAX_SIZE];

    //信号采集时直接返回空闲，让流程进行下次操作
    if(78 == m_pro.PID)
    {
        nResult = MCU_IDEL;
    }
    else if((40 == m_pro.PID)||(75 == m_pro.PID))
    {
        if(m_bSleepTag)
        {
            int spac = m_SpacTime.elapsed();
            if(spac/1000 >= m_pro.arrParaValue[0])
            {
                m_bSleepTag = false;
                nResult = MCU_IDEL;
            }
        }
        else{
            m_SpacTime.start();
            m_bSleepTag = true;
        }
    }
    else
    {
        if(m_pro.ModelAddress == 0)
        {
            if(GetModuleDataInfo())
            {
                nResult = MCU_IDEL;
            }
            else
            {
                nResult = MCU_TIMEOUT;
            }
        }
        else
        {
            memset(bBuff,0,MAX_SIZE);
            if(pDriver->ReadModbus(m_pro.ModelAddress,40051,13,100,bBuff))
            {
                GetMCUWorkStatus(bBuff);
                nResult = m_nMcuBusy;
            }
            else
            {
                nResult = MCU_TIMEOUT;
            }
         }
    }
    return nResult;

}
void CProcess5::GetMCUWorkStatus(unsigned char* bBuff)
{
    int16_t wModbus40051 = 0;
    int16_t wModels40052 = 0;
    int16_t wStatus40053 = 0;
    char str[64];

    wModbus40051 = MAKEWORD(bBuff[1],bBuff[0]); //40051	模块名称 0：无，1：继电器模块 2：加热温度控制模块，3：联动模块，
    wModels40052 = MAKEWORD(bBuff[3],bBuff[2]);	//40052 模块的工作状态
    wStatus40053 = MAKEWORD(bBuff[5],bBuff[4]); //40053

    m_nMcuBusy = wModels40052;
    switch(wModels40052)
    {
    case 0x00:
        if((m_pro.PID == 9)||(m_pro.PID == 10))//加速冷却或者自然冷却,要求实时显示温度值
        {
            if(m_nLastWorkPID != m_pro.PID)
            {
                m_nLastWorkPID = m_pro.PID;
                m_SpacTime.start();

            }

            float fValue = MAKEWORD(bBuff[9],bBuff[8]) / 10.0;
            if (fValue > m_pro.arrParaValue[3]) //如果还没有降到指定温度,则置温度模块为忙,继续降温
                m_nMcuBusy = MCU_BUSY;
            else
                m_nMcuBusy = MCU_IDEL;

            if(m_SpacTime.elapsed() > m_pro.arrParaValue[4]*60*1000)
            {
                m_nMcuBusy = MCU_FAULT;
                MainWindowDebug("温度降不下来");
                memset(str,0,64);
                strcpy(str,"温度降不下来");
                DebugInfo(str,0);
            }

        }
        break;
    case 0x01:
        break;
    default:
        break;

    }

}
bool CProcess5::ReadData()
{
    bool bRet = false;
    float fTime = 0.0;
    int nDataCount = 0;
    unsigned char bBuff[MAX_SIZE];
    int iRedLenCou = 0;//娄据长度
    int iNum = 20;
    int nStartReg = 40101;
    short int uValue = 0;

    while(true)
    {
        usleep(500000);
        memset(bBuff,0,MAX_SIZE);
        if(pDriver->ReadModbus(16,40100,5,100,bBuff))
        {
            iRedLenCou = MAKEWORD(bBuff[1],bBuff[0]);
            if(m_bExitRead)//流程结束，马上结束读数据
            {
                break;
            }
            if(iRedLenCou >= m_nValueCount)
            {
                break;
            }
        }
    }

    while(m_nValueCount>0)
    {
        usleep(2000);
        if(m_nValueCount<20)
            iNum = m_nValueCount;
        else
            iNum = 20;
        memset(bBuff,0,MAX_SIZE);
        if(pDriver->ReadModbus(16,nStartReg,iNum,5,bBuff))
        {
            int iLx = 0;
            int iRx = 0;

            for(int i = 0;i<iNum;i++)
            {
                iLx = 2 * i;
                iRx = 2 * i + 1;
                nDataCount++;
                uValue = MAKEWORD(bBuff[iRx],bBuff[iLx]);
                fTime = (1.0/m_nParaRate)*nDataCount;

                m_struAFSVector.m_vtnWaterSignal.push_back(uValue);
                m_struAFSVector.m_vtnWaterTime.push_back(fTime);
            }
            m_nValueCount -= iNum;
            nStartReg += iNum;
            if(m_bExitRead)
            {
                break;
            }
            bRet = true;
        }
        else
        {
            usleep(10000);
        }
    }
    return bRet;
}
int CProcess5::ScanFlowFile(const char *filename)
{
    char czItem[64];memset(czItem,0,64);
    char czTmp[64];
    int nSubFlow = 0;


    for(int i=0;i<MAXSIZE;i++)
    {
        sprintf(czItem,"%d",i);

        memset(czTmp,0,64);
        if(!GetAfxConfig("processfile",filename)->GetIni(czItem,"PID",czTmp))
        {
            break;
        }else{
            m_pro.PID = atoi(czTmp);
        }
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"Cmd",czTmp);
        m_pro.Cmd = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"name",czTmp);
        memcpy(m_pro.cProcName,czTmp,64);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"LogTag",czTmp);
        m_pro.LogTag = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"time",czTmp);
        m_pro.CostTime = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"address",czTmp);
        m_pro.ModelAddress = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"para",czTmp);
        m_pro.ParaCount = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"1",czTmp);
        m_pro.arrParaValue[0] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"2",czTmp);
        m_pro.arrParaValue[1] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"3",czTmp);
        m_pro.arrParaValue[2] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"4",czTmp);
        m_pro.arrParaValue[3] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"5",czTmp);
        m_pro.arrParaValue[4] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"6",czTmp);
        m_pro.arrParaValue[5] = atoi(czTmp);
        if(8 == m_pro.PID)
        {
            m_pro.arrParaValue[0] = m_pro.arrParaValue[0]*10;
        }   
        else if((75 == m_pro.PID)&&(m_nCmd != CMD_ELECMAINTAIN))
        {
            memset(czTmp,0,64);
            GetAfxConfig()->GetIni("MainParaSet","Time",czTmp);
            m_pro.arrParaValue[0] = atoi(czTmp);
        }
        else if(((63 == m_pro.PID)||(66 == m_pro.PID))&&(m_nCmd != CMD_ELECMAINTAIN))
        {
            memset(czTmp,0,64);
            GetAfxConfig()->GetIni("MainParaSet","Voltage",czTmp);
            m_pro.arrParaValue[0] = atoi(czTmp);
        }
        else if(((65 == m_pro.PID)||(68 == m_pro.PID))&&(m_nCmd != CMD_ELECMAINTAIN))
        {
            memset(czTmp,0,64);
            GetAfxConfig()->GetIni("MainParaSet","EndVoltage",czTmp);
            m_pro.arrParaValue[1] = atoi(czTmp);
        }
        else if((57 == m_pro.PID)&&(CMD_TEST == m_nCmd))
        {
            memset(czTmp,0,64);
            GetAfxConfig()->GetIni("RangeSet","RangeWay",czTmp);
            nSubFlow = atoi(czTmp);

            if(0 == nSubFlow)
            {
                memset(czTmp,0,64);
                GetAfxConfig()->GetIni("RangeSet","Range1Value",czTmp);
                m_fRangeValue = atof(czTmp);
                ScanSubFlowFile("test_1.dll");
            }else if(1 == nSubFlow)
            {
                memset(czTmp,0,64);
                GetAfxConfig()->GetIni("RangeSet","Range2Value",czTmp);
                m_fRangeValue = atof(czTmp);
                ScanSubFlowFile("test_2.dll");
            }
            else if(2 == nSubFlow)
            {
                memset(czTmp,0,64);
                GetAfxConfig()->GetIni("RangeSet","xiaojieValue",czTmp);
                m_fRangeValue = atof(czTmp);
                ScanSubFlowFile("test_3.dll");
            }else
            {
                //do nothing
            }
        }

        m_vProcFile.push_back(m_pro);

    }
    if(m_pOnTotalNum)
    {
        m_pOnTotalNum(m_userdata,this->GetProcessType(),m_vProcFile.size());
    }
    return m_vProcFile.size();
}
void CProcess5::ScanSubFlowFile(const char *filename)
{
    char czItem[64];memset(czItem,0,64);
    char czTmp[64];


    for(int i=0;i<MAXSIZE;i++)
    {
        sprintf(czItem,"%d",i);

        memset(czTmp,0,64);
        if(!GetAfxConfig("processfile",filename)->GetIni(czItem,"PID",czTmp))
        {
            break;
        }else{
            m_pro.PID = atoi(czTmp);
        }
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"Cmd",czTmp);
        m_pro.Cmd = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"name",czTmp);
        memcpy(m_pro.cProcName,czTmp,64);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"LogTag",czTmp);
        m_pro.LogTag = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"time",czTmp);
        m_pro.CostTime = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"address",czTmp);
        m_pro.ModelAddress = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"para",czTmp);
        m_pro.ParaCount = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"1",czTmp);
        m_pro.arrParaValue[0] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"2",czTmp);
        m_pro.arrParaValue[1] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"3",czTmp);
        m_pro.arrParaValue[2] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"4",czTmp);
        m_pro.arrParaValue[3] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"5",czTmp);
        m_pro.arrParaValue[4] = atoi(czTmp);
        memset(czTmp,0,64);
        GetAfxConfig("processfile",filename)->GetIni(czItem,"6",czTmp);
        m_pro.arrParaValue[5] = atoi(czTmp);

        m_vProcFile.push_back(m_pro);
    }
}
void CProcess5::SaveSrcDataToTxt()
{
    char czChildPath[256];memset(czChildPath,0,256);
    char czDataName[256];memset(czDataName,0,256);
    char czDataFile[256];memset(czDataFile,0,256);
    sprintf(czChildPath,"%s/%d/data",DB_ROOT_PATH,this->GetProcessType());
    mkdir_r(czChildPath);
    sprintf(czDataName,"%04d%02d%02d-%02d%02d.txt",m_Qtime.date().year(),m_Qtime.date().month(),m_Qtime.date().day(),m_Qtime.time().hour(),m_Qtime.time().minute());
    sprintf(czDataFile,"%s/%s",czChildPath,czDataName);

    QString qsTxtFileName = QString(czDataFile);
    QFile file(qsTxtFileName);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
       QTextStream stream(&file);
       stream << "fTime"<<"\t"<<"Value"<<"\r\n";
       for(int i = 0;i < m_struAFSVector.m_vtnWaterSignal.size(); i++)
       {
           stream << m_struAFSVector.m_vtnWaterTime[i]<<"\t"<<m_struAFSVector.m_vtnWaterSignal[i]<<"\t"<< "\r\n";
       }
       file.close();
    }
}
void CProcess5::MainWindowDebug(const char *cStr)
{
    char str[64];memset(str,0,64);
    if(m_pOnDebugInfo)
    {
        m_pOnDebugInfo(m_userdata,this->GetProcessType(),cStr);
    }
}
void CProcess5::SaveResultToDatabase(int nCodeID)
{
    if(m_bEndFlag)
    {
        if(CMD_TEST == m_nCmd)
        {
            if(m_pOnTestFinished)
            {
                for(int j=0;j<YQMAXCOUNT;j++)
                {
                    if(m_pYQ[j])
                    {
                        SYQPARAM sYQ;
                        char cStr[255];memset(cStr,0,255);
                        char str1[64];memset(str1,0,64);
                        m_pYQ[j]->GetYQParam(sYQ);
                        strcpy(cStr,"保存数据至数据库; ");
                        strcat(cStr,sYQ.czID);
                        sprintf(str1," fchroma:%.2f h:%.2f",m_pYQ[j]->m_RealInfo.fResult,m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight1);
                        strcat(cStr,str1);
                        strcat(cStr,m_pYQ[j]->m_RealInfo.cDataID);
                        DebugInfo(cStr);
                        m_pOnTestFinished(m_userdata,8051,sYQ.czID);
                        MainWindowDebug("保存水样测试数据");
                     }
                }
            }
        }
        else if(CMD_SAMPLETEST == m_nCmd)
        {
            if(m_pOnTestFinished)
            {
                for(int k=0;k<YQMAXCOUNT;k++)
                {
                    if(m_pYQ[k])
                    {
                        m_pOnTestFinished(m_userdata,3016,m_pYQ[k]->m_sYQ.czID);
                        MainWindowDebug("保存标样核查数据");
                    }
                }
            }
        }
        else if(CMD_INIT == m_nCmd)
        {
            if(m_pOnTestFinished)
            {
                for(int i=0;i<YQMAXCOUNT;i++)
                {
                    if(m_pYQ[i])
                    {
                        memset(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                        memcpy(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,"初始化",strlen("初始化"));
                        m_pOnTestFinished(m_userdata,8053,m_pYQ[i]->m_sYQ.czID);
                        break;
                    }
                }
            }
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        if(m_pOnTestFinished)
        {
            for(int i=0;i<YQMAXCOUNT;i++)
            {
                if(m_pYQ[i])
                {
                    SYQPARAM sYQ;
                    m_pYQ[i]->GetYQParam(sYQ);
                    memset(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                    memcpy(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,"中止测试",strlen("中止测试"));
                    m_pOnTestFinished(m_userdata,8053,sYQ.czID);
                    break;
                }
            }
        }
    }
    m_bYQBusy = false;
    m_bRun = false;
    //通知主界面仪器空闲
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,1,this->GetProcessType(),m_bYQBusy,0,true);
    }
}
void CProcess5::DealReagentsChange()
{
    char czTmp[64];
    char czValue[64];
    int nSurplus = 0;
    if(82 == m_pro.PID)
    {
        memset(czTmp,0,64);
        GetAfxConfig()->GetIni("Reagents","ReagentA",czTmp);
        nSurplus = atoi(czTmp)-5;
        if(nSurplus<0)
        {
            nSurplus = 0;
        }
        memset(czValue,0,64);
        sprintf(czValue,"%d",nSurplus);
        GetAfxConfig()->SetIni("Reagents","ReagentA",czValue);
    }
    else if(81 == m_pro.PID)
    {
        memset(czTmp,0,64);
        GetAfxConfig()->GetIni("Reagents","ReagentB",czTmp);
        nSurplus = atoi(czTmp)-5;
        if(nSurplus<0)
        {
            nSurplus = 0;
        }
        memset(czValue,0,64);
        sprintf(czValue,"%d",nSurplus);
        GetAfxConfig()->SetIni("Reagents","ReagentB",czValue);
    }
    else if(76 == m_pro.PID)
    {
        memset(czTmp,0,64);
        GetAfxConfig()->GetIni("Reagents","ReagentC",czTmp);
        nSurplus = atoi(czTmp)-1;
        if(nSurplus<0)
        {
            nSurplus = 0;
        }
        memset(czValue,0,64);
        sprintf(czValue,"%d",nSurplus);
        GetAfxConfig()->SetIni("Reagents","ReagentC",czValue);
    }
    else if(6 == m_pro.PID)
    {
        memset(czTmp,0,64);
        GetAfxConfig()->GetIni("Reagents","ReagentD",czTmp);
        nSurplus = atoi(czTmp)-1;
        if(nSurplus<0)
        {
            nSurplus = 0;
        }
        memset(czValue,0,64);
        sprintf(czValue,"%d",nSurplus);
        GetAfxConfig()->SetIni("Reagents","ReagentD",czValue);
    }
    else if(37 == m_pro.PID)
    {
        memset(czTmp,0,64);
        GetAfxConfig()->GetIni("Reagents","ReagentE",czTmp);
        nSurplus = atoi(czTmp)-1;
        if(nSurplus<0)
        {
            nSurplus = 0;
        }
        memset(czValue,0,64);
        sprintf(czValue,"%d",nSurplus);
        GetAfxConfig()->SetIni("Reagents","ReagentE",czValue);
    }
    else
    {
        //do nothing
    }
}
int CProcess5::ReadFlowFile()
{
    char czPath[255];
    char czValue[255];
    char czTmp[255];
    int nPIDCount = 0;

    memset(czPath,0,255);
    memset(czValue,0,255);
    memset(czTmp,0,255);
    sprintf(czPath,"%s",INI_ROOT_PATH);
    strcat(czPath,"/processfile/");
    switch(m_nCmd)
    {
    case CMD_TEST://立即测试
        {
            RecordLog(LOG_NOMAL,"发送立即测试命令");
            GetAfxConfig()->GetIni("ProcessFile","WaterTest",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加立即测试流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_WASH://清洗
        {
            RecordLog(LOG_NOMAL,"发送清洗命令");
            GetAfxConfig()->GetIni("ProcessFile","Wash",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添清洗流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_INIT://初始化
        {
            RecordLog(LOG_NOMAL,"发送初始化命令");
            GetAfxConfig()->GetIni("ProcessFile","InitTest",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添初始化流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_SAMPLETEST://标样核查
        {
            RecordLog(LOG_NOMAL,"发送标样核查命令");
            GetAfxConfig()->GetIni("ProcessFile","SampleTest",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加标样核查流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_SAMPLEADJUST://标样校准
        {
            RecordLog(LOG_NOMAL,"发送标样校准命令");
            GetAfxConfig()->GetIni("ProcessFile","SampleAdjust",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加标样校准流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_BANKADJUST://空白校准
        {
            RecordLog(LOG_NOMAL,"发送空白校准命令");
            GetAfxConfig()->GetIni("ProcessFile","BlankAdjust",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加标定曲线流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_CURVECALIBRATION://标定工作曲线
        {
            RecordLog(LOG_NOMAL,"发送曲线标定命令");
            GetAfxConfig()->GetIni("ProcessFile","CurveCali",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加标定曲线流程文件");
            }
            nPIDCount = ScanFlowFile(czValue);
        }
    default:
         break;
    }
    return nPIDCount;
}


//计算结果值
bool CProcess5::CalcTestValue(int nKey)
{
    char str[64];
    char str1[64];

    //计算结果
    if (nKey >= 0)
    {
        double dChroma = 0.0;
        CYQ* pYQ = NULL;
        pYQ = m_pYQ[nKey];
        if(pYQ)
        {
            pYQ->m_RealInfo.TestTime = m_pYQ[nKey]->m_RealInfo.TestTime;
            pYQ->m_RealInfo.m_BsDataInfo.K = pYQ->m_CurveInfo.K;
            pYQ->m_RealInfo.m_BsDataInfo.B = pYQ->m_CurveInfo.B;
            pYQ->m_RealInfo.m_BsDataInfo.R = pYQ->m_CurveInfo.R;
            pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = pYQ->m_CurveInfo.BlankAbsorb;//空白吸光度
            pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = pYQ->m_CurveInfo.StdAbsorb;//标样吸光度
            pYQ->m_RealInfo.CheckChroma = pYQ->m_CurveInfo.CheckChroma;//标样浓度

            if(pYQ->m_CurveInfo.CheckChroma > 0)//表示有校准，则需计算标样浓度误差
            {
                //标样浓度误差 =标样浓度 -( K * (标样吸光度 - 空白吸光度) + B )
                //浓度 = K * (X - 空白吸光度) + B +(-) 标样浓度误差
                float fErrSampChroma = 0.0;//标样的误差
                fErrSampChroma = pYQ->m_CurveInfo.CheckChroma - (pYQ->m_CurveInfo.K * (pYQ->m_CurveInfo.StdAbsorb - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B);

                pYQ->m_RealInfo.fResult = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B + fErrSampChroma;
            }
            else
            {
                pYQ->m_RealInfo.fResult = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B ;
            }
            if(pYQ->m_RealInfo.fResult < 0)//测试结果，不为负数
            {
                pYQ->m_RealInfo.fResult = 0.0;
            }
            //保存流水日志
            memset(str,0,64);
            memset(str1,0,64);
            strcpy(str,"计算浓度结果:");
            sprintf(str1,"Z%d,浓度:%0.3f",pYQ->m_RealInfo.fResult,dChroma);
            DebugInfo(str,3);
            //通知界面测试结果
            if(m_pOnBsReadValue)
            {
                m_pOnBsReadValue(m_userdata,15,pYQ->m_RealInfo.fResult,m_Qtime);
            }

            //是否自动切换曲线
            char czValue[64];
            memset(czValue,0,64);
            GetAfxConfig()->GetIni("AutoAdjust_SET","RangeAutoChangeSet",czValue);
            if(!strcmp(czValue,"1") && m_nCmd == CMD_TEST)//仅当是测试命令时才自动切换曲线
            {
                if(VerifyRange(pYQ->m_sYQ.czID,pYQ->m_CurveInfo.CurveID,(double)pYQ->m_RealInfo.fResult))
                {
                    return true;
                }
                else//不切换曲线,标识位置FALSE
                {
                    m_bUpSwitchCurve = false;
                    m_bDowSwitchCurve = false;
                }
            }

            return true;
        }
    }
    return false;
}
bool CProcess5::VerifyRange(const char* czID,int nCurrentCurve,double dCurrentValue)
{
    float fValue = 0.0;
    bool bRet = false;
    int nNewCurve = nCurrentCurve;

    char cStandardErrRange[10];memset(cStandardErrRange,0,10);
    GetAfxConfig()->GetIni("AutoAdjust_SET","StandardErrRange",cStandardErrRange);

    if(strlen(cStandardErrRange) > 0)
        fValue = (atof(cStandardErrRange) / 100);
    double fCurveMinValue =0;
    double fCurveMaxValue = 0;

    sqlite_data datalist;
    bool isResult = m_pDB->GetCurveManger(czID,0,datalist);
    if(isResult)
    {
        if(!datalist.IsEmpty())
        {
            for (int i = 1; i <= 5; i++)
            {
                fCurveMinValue = atof(datalist.GetData(i,11));
                fCurveMaxValue = atof(datalist.GetData(i,12));
                if (dCurrentValue <= fCurveMaxValue - fCurveMaxValue * fValue)
                {
                    nNewCurve = i;
                    break;
                }
            }
        }
    }
    datalist.Free();

    if (nNewCurve != nCurrentCurve)//可能需要切换曲线
    {
        if (!m_bUpSwitchCurve && !m_bDowSwitchCurve)//第一次切换曲线
        {
            if (nNewCurve > nCurrentCurve)//向上切
                m_bUpSwitchCurve = true;
            else
                m_bDowSwitchCurve = true;
            bRet = true;
        }
        else if (m_bUpSwitchCurve)//切换
        {
            if (nNewCurve > nCurrentCurve)//向上切
                bRet = true;
            else //不往回切
                bRet = false;
        }
        else if (m_bDowSwitchCurve)
        {
            if (nNewCurve < nCurrentCurve)//向下切
                bRet = true;
            else //不往回切
                bRet = false;
        }
        if (bRet)//切换曲线满足条件
        {
            char czValue[64];
            memset(czValue,0,64);
            sprintf(czValue,"%d",nNewCurve);
            char cItem[64];memset(cItem,0,64);
            sprintf(cItem,"WorkCurveID_%s",czID);
            GetAfxConfig()->SetIni("SYSSET",cItem,czValue);// 测量值超出量程范围,更改工作曲线

           char czLog[255];
           memset(czLog,0,255);
           sprintf(czLog,"系统原工作曲线[曲线%d],当前测试结果[%.4f]超过该曲线量程,工作曲线自动切换为[曲线%d]",nCurrentCurve,dCurrentValue,nNewCurve);
           RecordLog(LOG_WARNING,czLog);
        }
    }
    else//不切换曲线
    {
        bRet = false;
    }
    return bRet;
}
bool CProcess5::GetModuleDataInfo()
{
    int nMiddle = 0;
    int nMin1 = 0;
    int nMin2 = 0;
    bool bRet = false;

    switch(m_pro.PID)
    {
    case 11:
        if(ReadData())
        {
            bRet = true;
            //去掉第一个异常值
            m_struAFSVector.m_vtnWaterSignal.erase(m_struAFSVector.m_vtnWaterSignal.begin());
            m_struAFSVector.m_vtnWaterTime.erase(m_struAFSVector.m_vtnWaterTime.begin());

            SaveSrcDataToTxt();
            //通知主界面描绘曲线
            if(pCalc->Smooth(3,m_struAFSVector.m_vtnWaterSignal))
            {
                m_struAFSVector.m_vtnWaterSignal.clear();
                m_struAFSVector.m_vtnWaterSignal = pCalc->m_vtnSmoothY;
                if(m_pOnAFSData)
                {
                    m_pOnAFSData(m_userdata,&m_struAFSVector,1);
                }
                pCalc->SetData(m_struAFSVector.m_vtnWaterSignal,m_struAFSVector.m_vtnWaterTime);
                for(int t1=0;t1<YQMAXCOUNT;t1++)
                {
                    if(m_pYQ[t1])
                    {
                        pCalc->AFSCalculate(m_pYQ[t1]->m_CurveInfo.CalcMode,m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchDown, m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchUp,m_pYQ[t1]->m_RealInfo.m_BsDataInfo.Absorbency,nMiddle,nMin1,nMin2);
                        //通知主界面峰高或峰面积
                        if(m_pOnHhInfo)
                        {
                            m_pOnHhInfo(m_userdata,0,m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.m_BsDataInfo.Absorbency);
                        }
                        //通知曲线标定界面峰高或峰面积
                        if(m_pOnBsReadValue)
                        {
                            m_pOnBsReadValue(m_userdata,0, m_pYQ[t1]->m_RealInfo.m_BsDataInfo.Absorbency,m_Qtime);
                        }
                        //计算结果
                        CalcTestValue(t1);
                        //通知主界面计算结果
                        if(m_pOnHhInfo)
                        {
                            m_pOnHhInfo(m_userdata,3,m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.fResult);
                        }
                    }
                }
            }
            else
            {
                DebugInfo("平滑处理失败",0);
            }
        }
        else
        {
            DebugInfo("数据读取失败",0);
        }
        break;
    default:
        bRet = true;
        break;
    }
     return bRet;
}
