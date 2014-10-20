#include "cprocess1.h"
#include "stdlib.h"
#include <QDateTime>
#include <iostream>
using namespace std;

CProcess1::CProcess1()
{
    m_bYQBusy = false;
    m_bRongChuTag = true;
    m_nCmd = 0;
    m_fRangeValue = 1.0;
    m_bEndFlag  = true;
    m_nIncrement = 0;
    m_bAjustFlag = true;
    cout << "CProcess1::CProcess1()" << endl;
}

CProcess1::~CProcess1()
{
}
int CProcess1::GetProcessType()
{
    return 1;
}
int CProcess1::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->m_RealInfo.fResult = 0.0;
            m_pYQ[j]->m_RealInfo.fCheckResult = 0.0;
            m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight1 = 0.0;
            m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight2 = 0.0;
            memset(m_pYQ[j]->m_RealInfo.cDataID,0,16);
            if (1 == nKind)
            {
                memcpy(m_pYQ[j]->m_RealInfo.cDataID,"hd",2);
            }
            else if(2 == nKind)
            {
                memcpy(m_pYQ[j]->m_RealInfo.cDataID,"nt",2);
            }
            //详细信息
            m_pYQ[j]->m_RealInfo.TestTime = m_Qtime;
            m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fLeftBase = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchDown;
            m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fRighBase = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            m_pYQ[j]->m_RealInfo.nCalWay =  m_pYQ[j]->m_CurveInfo.CalcMode;

        }
    }

    m_struEchemVector.m_vtnWaterSignal.clear();
    m_struEchemVector.m_vtnWaterVoltage.clear();
    m_struEchemVector.m_vtnSampleSignal.clear();
    m_struEchemVector.m_vtnSampleVoltage.clear();
    m_vProcFile.clear();
    m_vtnSyInitdata.clear();
    m_vtnByInitdata.clear();

    m_bRongChuTag = true;
    m_bExitFlag = false;
    m_bSleepTag = false;
    m_nLastWorkPID = -1;
    m_bAjustFlag = true;
    //pDriver->AllModuleReset();
    if(m_pOnHhInfo)
    {
        m_pOnHhInfo(m_userdata,2,0,1.0);
    }
    //临时用，将故障清除
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->SetFaultType(TYPE_NO);
        }
    }
    return 0;
}
//nKind为1表示手动测试 2表示网络测试 3表示自动测试
int CProcess1::StartTestCmd(int nCmd,int nKind,QDateTime dt)
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

        //清除报警
        if(CMD_INIT != nCmd)
        {
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"",-1);
            }
        }
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
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"流程文件没找到",LOG_FAULT);
            }
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

void CProcess1::DoProcess()
{
    bool bFinish = true;
    int nResult = 1;
    int nTimeOutTimes = 0;
    int nTotsize = 0;
    m_bYQBusy = true;
    m_bEndFlag = true;
    m_bInitFlag = true;
    char str[64];
    char czLog[MAX_SIZE];
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
            break;
        }
        if(bFinish)
        {
            if(m_vProcFile.size() == 0)
            {
                MainWindowDebug("流程正常结束");
                RecordLog(LOG_NOMAL,"流程正常结束");
                m_bInitFlag = false;
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
                    SetRegisterProcessCode();
                    m_vProcFile.erase(m_vProcFile.begin());
                    usleep(100000);//延时100毫秒才处理读状态
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

                    memset(czLog,0,MAX_SIZE);
                    memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                    strcat(czLog," 通讯故障");
                    RecordLog(LOG_ALARM,czLog);
                    RecordLog(LOG_NOMAL,"异常结束");
                    if(m_pOnAlarm)
                    {
                        m_pOnAlarm(m_userdata,"执行流程时通讯故障",LOG_FAULT);
                    }
                    break;
                 }
            }
        }
        else
        {
            usleep(200000);
            nResult = EchemModuleStatus();
            if(nResult == MCU_TIMEOUT)
            {
                 nTimeOutTimes++;
                 memset(czLog,0,MAX_SIZE);
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
                     memset(czLog,0,MAX_SIZE);
                     memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                     strcat(czLog," 读取状态失败");
                     RecordLog(LOG_ALARM,czLog);
                     RecordLog(LOG_NOMAL,"异常结束");
                     if(m_pOnAlarm)
                     {
                         m_pOnAlarm(m_userdata,"读取状态超时超过10次",LOG_FAULT);
                     }
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
                //故障类型,写入寄存器
                SetRegisterFaultType(nResult);
                //仪器故障或缺试剂，切换为维护模式
                GetAfxConfig()->SetIni("SYSSET","MOD","4");
                RecordLog(LOG_ALARM,"故障或缺试剂,切换为维护模式");
                RecordLog(LOG_NOMAL,"异常结束");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"缺试剂或仪器故障",LOG_FAULT);
                }
                break;
            }
        }
    }
    if((CMD_TEST == m_nCmd)||(CMD_SAMPLETEST == m_nCmd)||(CMD_SAMPLEADJUST == m_nCmd)||(CMD_BANKADJUST == m_nCmd)||(CMD_ELECMAINTAIN == m_nCmd))
    {
        SaveSrcDataToTxt();
    }
    SaveResultToDatabase(m_nCmd);
    SetbIdle(true);
    DealInitCmd();
    SaveTestTime();//保存测试时间

}
STATUS CProcess1::DoCurrentCmd()
{
    STATUS nRet = CMD_OK;
    char czLog[MAX_SIZE];
    char tmp[64];
    if (m_pro.ModelAddress == 0)//系统模块
    {
        nRet = CMD_OK;
    }
    else
    {
        int16_t wValue = 0;
        unsigned char bBuff[MAX_SIZE];
        memset(bBuff,0,MAX_SIZE);

        //命令字
        wValue = m_pro.Cmd;
        bBuff[0] = HIBYTE(wValue);
        bBuff[1] = LOBYTE(wValue);
        //参数
        for (int i = 0;i < m_pro.ParaCount;i++)
        {
            wValue = m_pro.arrParaValue[i];

            if (m_pro.PID == 4 || m_pro.PID == 3 || m_pro.PID == 59)//抽取样品/润洗液位管/试剂补抽,根据系统设置改变参数
            {
                if (i == 0)//动态修改第一个参数，源电磁阀
                {

                    switch (m_nCmd)
                    {
                    case CMD_TEST://测试
                        {
                            memset(tmp,0,64);
                            GetAfxConfig()->GetIni("ValveNO","ByOrSy",tmp);
                            if(1 == atoi(tmp))
                            {
                                memset(tmp,0,64);
                                GetAfxConfig()->GetIni("ValveNO","SampleValveNO",tmp);
                                wValue = atoi(tmp);
                            }
                            else if(0 == atoi(tmp))
                            {
                                memset(tmp,0,64);
                                GetAfxConfig()->GetIni("ValveNO","WaterValveNO",tmp);
                                wValue = atoi(tmp);
                            }
                            else
                            {
                                //do nothing
                            }
                        }
                        break;
                        /*标样校准 标样核查 空白校准都从标样管进样,防止润洗液位管时蒸馏水回流，污染蒸馏水*/
                    case CMD_SAMPLEADJUST://标样校准
                    case CMD_SAMPLETEST://标样核查
                        {
                            memset(tmp,0,64);
                            GetAfxConfig()->GetIni("ValveNO","SampleValveNO",tmp);
                            wValue = atoi(tmp);
                        }
                        break;
                    case CMD_BANKADJUST://空白校准
                        {
                            memset(tmp,0,64);
                            GetAfxConfig()->GetIni("ValveNO","DistilledValveNO",tmp);
                            wValue = atoi(tmp);
                        }
                        break;
                    }
                }
            }

            bBuff[i*2+2] = HIBYTE(wValue);
            bBuff[i*2+3] = LOBYTE(wValue);
        }

            int nIndex;
            for (nIndex = 0; nIndex < 3;nIndex++)
            {
                if(pDriver->WriteModbus(m_pro.ModelAddress,40001,m_pro.ParaCount+1,300,bBuff))
                {
                    if((77 == m_pro.PID)||(82 == m_pro.PID)||(76 == m_pro.PID)||(6 == m_pro.PID)||(37 == m_pro.PID))
                    {
                        DealReagentsChange();
                    }
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
int CProcess1::ReadMCUStatusInfo()
{
    int nResult = MCU_BUSY;
    float fCurrenTemp = 0.0;
    unsigned char bBuff[MAX_SIZE];
    char str[64];

    if((m_pro.PID == 9)||(m_pro.PID == 10))
    {
        unsigned char bBuff[MAX_SIZE];
        memset(bBuff,0,MAX_SIZE);

        if(pDriver->ReadModbus(m_prepro.ModelAddress,40051,13,200,bBuff))
        {
            GetMCUWorkStatus(bBuff);
            nResult = m_nMcuBusy;

            fCurrenTemp = MAKEWORD(bBuff[9],bBuff[8])/10.0;
            if(m_pOnTempValue)
            {
                m_pOnTempValue(m_userdata,fCurrenTemp);
            }
        }
        else
        {
            nResult = MCU_TIMEOUT;//超时
        }

        if (MCU_BUSY != nResult)//需要关闭加速冷却
        {
            if(m_pro.PID = 10)
            m_pro.arrParaValue[1]  = 0;//关闭
            if(DoCurrentCmd() == CMD_OK)
            {
                MainWindowDebug("发送命令关闭加速冷却,返回成功");
                memset(str,0,64);
                strcpy(str,"发送命令关闭加速冷却,返回成功:");
                DebugInfo(str,0);
            }
            else
            {
                MainWindowDebug("发送命令关闭加速冷却,返回失败");
                memset(str,0,64);
                strcpy(str,"发送命令关闭加速冷却,返回失败:");
                DebugInfo(str,0);
            }

            usleep(500000);//延时500毫秒才处理
        }
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
    }else{
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
        else{

            memset(bBuff,0,MAX_SIZE);
            if(pDriver->ReadModbus(m_pro.ModelAddress,40051,13,200,bBuff))
            {
                GetMCUWorkStatus(bBuff);
                nResult = m_nMcuBusy;

                if(8 == m_pro.PID)
                {
                    fCurrenTemp = MAKEWORD(bBuff[9],bBuff[8])/10.0;
                    if(m_pOnTempValue)
                    {
                        m_pOnTempValue(m_userdata,fCurrenTemp);
                    }
                }
            }
            else
            {
                nResult = MCU_TIMEOUT;
            }
        }
    }
    return nResult;

}
int CProcess1::EchemModuleStatus()
{
    int nResult = ReadMCUStatusInfo();
    if (m_pro.PID == 68 || m_pro.PID == 65)//溶出则忽略下位机无返回的情况，下位机工作时不应答，自行处理超时问题
    {
        if (nResult == MCU_TIMEOUT)
        {
            if (m_bRongChuTag)
            {
               m_SpacTime.start();
                m_bRongChuTag = false;
            }
            if(m_SpacTime.elapsed() < 10*60*1000)
            {
                nResult = MCU_BUSY;
            }
        }
        else
        {
            m_bRongChuTag = true;//恢复标识
        }
    }
    return nResult;
}
void CProcess1::GetMCUWorkStatus(unsigned char* bBuff)
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
bool CProcess1::GetModuleDataInfo()
{
    char str[64];
    char str1[64];
    char cValue[64];
    bool bRet = false;
    int nMiddle = 0;
    int nMin1 = 0;
    int nMin2 = 0;

    switch(m_pro.PID)
    {
    case 51:
        {
            //通知UI工作状态
            if(ReadData(51,0))
            {
                NotifyShowCurve(51);
                SaveScanDataToTxt();
                bRet = true;
            }

        }
        break;
    case 13:
        if(ReadData(13,m_pro.arrParaValue[0]))
        {
            memset(cValue,0,64);
            GetAfxConfig()->GetIni("INITDATA","CaclFlag",cValue);
            if(atoi(cValue))
            {
                CaclInitData(13);
            }

            SetDataCacl(m_struEchemVector.m_vtnSampleSignal,m_struEchemVector.m_vtnSampleVoltage);
            NotifyShowCurve(13);
            bRet = true;
            if (m_pro.arrParaValue[0] == 0)//如果参数代码为0，则表示一直计算所有参数的峰高
            {
                for(int t1=0;t1<YQMAXCOUNT;t1++)
                {
                    if(m_pYQ[t1])
                    {
                        pCalc->Calculate(m_pYQ[t1]->m_CurveInfo.CalcMode,m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchDown,m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchUp,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight2,nMiddle,nMin1,nMin2);
                        memset(str,0,64);
                        memset(str1,0,64);
                        strcpy(str,"计算所有参数标样峰高:");
                        sprintf(str1,"Z%d,H:%0.3f",m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight2);
                        strcat(str,str1);
                        DebugInfo(str,0);
                        if(m_pOnHhInfo)
                        {
                            m_pOnHhInfo(m_userdata,1,m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight2);
                        }
                    }
                }

              }
              else//如果有参数代码，则表示该参数分开计算峰高
              {
                  for(int t2=0;t2<YQMAXCOUNT;t2++)
                  {
                      if((m_pYQ[t2]) && (m_pro.arrParaValue[0] == m_pYQ[t2]->m_sYQ.InTypeID))
                      {
                          pCalc->Calculate(m_pYQ[t2]->m_CurveInfo.CalcMode,m_pYQ[t2]->m_CurveInfo.m_EchemCurve.PeakSearchDown,m_pYQ[t2]->m_CurveInfo.m_EchemCurve.PeakSearchUp,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight2,nMiddle,nMin1,nMin2);
                          memset(str,0,64);
                          memset(str,0,64);
                          strcpy(str,"计算单个参数标样峰高:");
                          sprintf(str1,"Z%d,H:%0.3f",m_pYQ[t2]->m_sYQ.InTypeID,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight2);
                          strcat(str,str1);
                          DebugInfo(str,0);
                          if(m_pOnHhInfo)
                          {
                              m_pOnHhInfo(m_userdata,1,m_pYQ[t2]->m_sYQ.InTypeID,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight2);
                          }
                      }

                  }
              }
        }
        break;
    case 11:
        if(ReadData(11,m_pro.arrParaValue[0]))
        {
            memset(cValue,0,64);
            GetAfxConfig()->GetIni("INITDATA","CaclFlag",cValue);
            if(atoi(cValue))
            {
                CaclInitData(11);
            }

            SetDataCacl(m_struEchemVector.m_vtnWaterSignal,m_struEchemVector.m_vtnWaterVoltage);
            NotifyShowCurve(11);
            bRet = true;
            if (m_pro.arrParaValue[0] == 0)//如果参数代码为0，则表示一起计算所有参数的峰高
            {
                for(int t1=0;t1<YQMAXCOUNT;t1++)
                {
                    if(m_pYQ[t1])
                    {
                        pCalc->Calculate(m_pYQ[t1]->m_CurveInfo.CalcMode,m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchDown,m_pYQ[t1]->m_CurveInfo.m_EchemCurve.PeakSearchUp,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight1,nMiddle,nMin1,nMin2);
                        memset(str,0,64);
                        memset(str1,0,64);
                        strcpy(str,"计算所有参数水样峰高:");
                        sprintf(str1,"Z%d,h:%0.3f",m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight1);
                        strcat(str,str1);
                        DebugInfo(str,0);
                        if(m_pOnHhInfo)
                        {
                            m_pOnHhInfo(m_userdata,0,m_pYQ[t1]->m_sYQ.InTypeID,m_pYQ[t1]->m_RealInfo.m_EchemDataInfo.fHeight1);
                        }
                    }
                }
             }
             else//如果有参数代码，则表示该参数分开计算峰高
             {
                for(int t2=0;t2<YQMAXCOUNT;t2++)
                {

                    if((m_pYQ[t2]) && (m_pro.arrParaValue[0] == m_pYQ[t2]->m_sYQ.InTypeID))
                    {
                        pCalc->Calculate(m_pYQ[t2]->m_CurveInfo.CalcMode,m_pYQ[t2]->m_CurveInfo.m_EchemCurve.PeakSearchDown,m_pYQ[t2]->m_CurveInfo.m_EchemCurve.PeakSearchUp,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight1,nMiddle,nMin1,nMin2);
                        memset(str,0,64);
                        memset(str1,0,64);
                        strcpy(str,"计算单个参数水样峰高:");
                        sprintf(str1,"Z%d,h:%0.3f",m_pYQ[t2]->m_sYQ.InTypeID,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight1);
                        DebugInfo(str,3);
                        if(m_pOnHhInfo)
                        {
                            m_pOnHhInfo(m_userdata,0,m_pYQ[t2]->m_sYQ.InTypeID,m_pYQ[t2]->m_RealInfo.m_EchemDataInfo.fHeight1);
                        }
                    }

                 }
             }
        }
        break;
    case 52:
        {
            if(m_pro.arrParaValue[0] == 0)//所有仪器
            {
                for(int t1=0;t1<YQMAXCOUNT;t1++)
                {
                    if(m_pYQ[t1])
                    {
                        CalcTestValue(t1);
                    }
                }
            }
            else//单个仪器
            {
                for(int t1=0;t1<YQMAXCOUNT;t1++)
                {

                    if((m_pYQ[t1]) && (m_pro.arrParaValue[0] == m_pYQ[t1]->m_sYQ.InTypeID))
                    {
                        CalcTestValue(t1);
                    }
                }
            }
            dealRangeSwitch();
            bRet = true;
        }
        break;
    default:
        bRet = true;
        break;
    }
     return bRet;
}
//因为读数据命令地址为0,故上次PID的地址m_prepro.ModelAddress
bool CProcess1::ReadData(int nKind,int nCode)
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    int nStartReg = 40100;
    int num = 10;
    int nCount = 0;
    float fValue = 0.0;
    int nInitialVol = 0;
    int nIncrement = 0;
    bool bRet = false;
    unsigned char pValue[4];
    char str[64];
    char cValue[64];
    m_prepro.ModelAddress = CMD_ElectrCheck;//防止出错，故将地址写死为电化学模块


    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","CaclFlag",cValue);


    if(m_pro.PID == 51)//暗信号用来读取线性扫描的数据
    {
        nInitialVol = -4000;
        nIncrement = 80;

        if(pDriver->ReadModbus(m_prepro.ModelAddress,nStartReg,1,200,bBuff))
        {
            nCount = MAKEWORD(bBuff[1],bBuff[0]);
            nStartReg += 1;

            memset(str,0,64);
            sprintf(str,"%d",nCount);
            strcat(str,"读线性扫描数据量成功");
            DebugInfo(str,0);

            m_SpacTime.start();
            while(nCount>0)
            {
                usleep(1000);
                if(nCount<10)
                    num = nCount;
                else
                    num = 10;
                if(pDriver->ReadModbus(m_prepro.ModelAddress,nStartReg,num,5,bBuff))
                {
                    for(int i = 0;i < num/2;i++)
                    {
                        fValue = 0;
                        memset(pValue, 0, 4);
                        pValue[0] = bBuff[1+i*4];
                        pValue[1] = bBuff[0+i*4];
                        pValue[2] = bBuff[3+i*4];
                        pValue[3] = bBuff[2+i*4];
                        memcpy(&fValue, pValue, 4);

                        m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
                        m_struEchemVector.m_vtnSampleVoltage.push_back(nInitialVol/1000.0);
                        nInitialVol += nIncrement;
                     }
                    nCount -= num;
                    nStartReg += num;
                    bRet = true;
                }
                else{
                    memset(str,0,64);
                    strcpy(str,"读线性扫描数据失败");
                    DebugInfo(str,0);
                    usleep(100000);
                    if(m_SpacTime.elapsed() >10*60*1000)
                        break;
                }
            }
        }
        else{
            memset(str,0,64);
            strcpy(str,"读线性扫描数据量失败");
            DebugInfo(str,0);
        }
    }
    else if(pDriver->ReadModbus(m_prepro.ModelAddress,40063,3,200,bBuff))//读水样标样原始数据 且将原始数据保存至TXT
    {
        short tmp = 0;
        char c[2];
        c[0] = bBuff[1];
        c[1] = bBuff[0];
        memcpy(&tmp,c,2);
        nInitialVol = tmp;
        nIncrement = MAKEWORD(bBuff[5],bBuff[4]);
        m_nIncrement = nIncrement;

//        QString name =m_Qtime.toString("yyyy-MM-dd hh:mm:ss");
//        QString qsTxtFilePath = QString(DB_ROOT_PATH);
//        qsTxtFilePath.append("/1/InitData/");
//        qsTxtFilePath.append(name);
//        if (nKind == 13)
//        {
//            qsTxtFilePath.append("by.txt");
//        }
//        else
//        {
//            qsTxtFilePath.append("sy.txt");
//        }
//        QFile file(qsTxtFilePath);
//        QTextStream stream(&file);
//        file.open(QIODevice::WriteOnly | QIODevice::Text);

        if(pDriver->ReadModbus(m_prepro.ModelAddress,nStartReg,1,200,bBuff))
        {
            nCount = MAKEWORD(bBuff[1],bBuff[0]);
            nStartReg += 1;

            memset(str,0,64);
            sprintf(str,"%d",nCount);
            strcat(str,"读过程数据量成功");
            DebugInfo(str,0);

            m_SpacTime.start();
            while(nCount>0)
            {
                usleep(1000);
                if(nCount<10)
                    num = nCount;
                else
                    num = 10;
                if(pDriver->ReadModbus(m_prepro.ModelAddress,nStartReg,num,5,bBuff))
                {
                    for(int i = 0;i < num/2;i++)
                    {
                        fValue = 0;
                        memset(pValue, 0, 4);
                        pValue[0] = bBuff[1+i*4];
                        pValue[1] = bBuff[0+i*4];
                        pValue[2] = bBuff[3+i*4];
                        pValue[3] = bBuff[2+i*4];
                        memcpy(&fValue, pValue, 4);


                        if (nKind == 13) //读基线值,加标后的
                        {
                            if(atoi(cValue))
                            {
                                m_vtnByInitdata.push_back(fValue);
//                                stream << fValue <<"\r\n";
                            }
                            else
                            {
                                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
                                m_struEchemVector.m_vtnSampleVoltage.push_back(nInitialVol/1000.0);
                            }

                        }
                        else//读样品值
                        {
                            if(atoi(cValue))
                            {
                                m_vtnSyInitdata.push_back(fValue);
/*                                stream << fValue <<"\r\n";*///上位软件处理数据时，才保存数据
                            }
                            else
                            {
                                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
                                m_struEchemVector.m_vtnWaterVoltage.push_back(nInitialVol/1000.0);
                            }         
                        }
                        nInitialVol += nIncrement;
                     }
                    nCount -= num;
                    nStartReg += num;
                    bRet = true;
                }
                else{
                    memset(str,0,64);
                    strcpy(str,"读过程数据失败");
                    DebugInfo(str,0);
                    usleep(200000);
                    if(m_SpacTime.elapsed() > 15*60*1000)
                    {
                        bRet = false;
                        break;
                    }
                }
            }
        }
        else{
            memset(str,0,64);
            strcpy(str,"读过程数据量失败");
            DebugInfo(str,0);
        }
//        file.close();
    }
    else
    {
        memset(str,0,64);
        strcpy(str,"查询溶出起始电压及增量失败");
        DebugInfo(str,0);
    }
    return bRet;
}
void CProcess1::CaclInitData(int nKind)
{
    char  czTmp[64];
    char  cValue[64];
    int   nCount = 0;
    float fValue1 = 0.0;
    float fValue2 = 0.0;
    float fValue = 0.0;
    int   nZ07Down = 0;
    int   nZ07Up = 0;
    int   nZ08Down = 0;
    int   nZ08Up = 0;
    int   nZ24Down = 0;
    int   nZ24Up = 0;
    int   nZ27Down = 0;
    int   nZ27Up = 0;
    int   nCurrVol = 0;
    if(13 == nKind)
    {
        m_struEchemVector.m_vtnSampleSignal.clear();
        m_struEchemVector.m_vtnSampleVoltage.clear();
    }
    else
    {
        m_struEchemVector.m_vtnWaterSignal.clear();
        m_struEchemVector.m_vtnWaterVoltage.clear();
    }

    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z07A",cValue);
    int nZ07A = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z07B",cValue);
    int nZ07B = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z08A",cValue);
    int nZ08A = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z08B",cValue);
    int nZ08B = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z24A",cValue);
    int nZ24A = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z24B",cValue);
    int nZ24B = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z27A",cValue);
    int nZ27A = atoi(cValue);
    memset(cValue,0,64);
    GetAfxConfig()->GetIni("INITDATA","Z27B",cValue);
    int nZ27B = atoi(cValue);

    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            if(!strcmp(m_pYQ[j]->m_sYQ.czID,"Z07"))
            {
                 nZ07Down = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchDown;
                 nZ07Up = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            }
            else if(!strcmp(m_pYQ[j]->m_sYQ.czID,"Z08"))
            {
                 nZ08Down = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchDown;
                 nZ08Up = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            }
            else if(!strcmp(m_pYQ[j]->m_sYQ.czID,"Z24"))
            {
                 nZ24Down = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchDown;
                 nZ24Up = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            }
            else if(!strcmp(m_pYQ[j]->m_sYQ.czID,"Z27"))
            {
                 nZ27Down = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchDown;
                 nZ27Up = m_pYQ[j]->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            }
        }
    }
    memset(czTmp,0,64);
    GetAfxConfig()->GetIni("MainParaSet","Voltage",czTmp);
    int  nInitialVol = atoi(czTmp);

    if(13 == nKind)
    {
        nCount = m_vtnByInitdata.size()/20;
        for(int i=2;i<=nCount;i++)
        {
            m_struEchemVector.m_vtnSampleVoltage.push_back((nInitialVol+(i*m_nIncrement))/1000.0);
        }
    }
    else
    {
        nCount = m_vtnSyInitdata.size()/20;
        for(int i=2;i<=nCount;i++)
        {
            m_struEchemVector.m_vtnWaterVoltage.push_back((nInitialVol+(i*m_nIncrement))/1000.0);
        }
    }

    for(int i=2;i<=nCount;i++)
    {
        nCurrVol = nInitialVol + i*m_nIncrement;
        if((nCurrVol >= nZ27Down)&&(nCurrVol <= nZ27Up))
        {
            if (nKind == 13) //读基线值,加标后的
            {
                fValue1 = m_vtnByInitdata[i*20-nZ27B];
                fValue2 = m_vtnByInitdata[i*20-nZ27A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
            }
            else//读样品值
            {
                fValue1 = m_vtnSyInitdata[i*20-nZ27B];
                fValue2 = m_vtnSyInitdata[i*20-nZ27A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
            }

        }
        else if((nCurrVol >= nZ07Down)&&(nCurrVol <= nZ07Up))
        {
            if (nKind == 13) //读基线值,加标后的
            {
                fValue1 = m_vtnByInitdata[i*20-nZ07B];
                fValue2 = m_vtnByInitdata[i*20-nZ07A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
            }
            else//读样品值
            {
                fValue1 = m_vtnSyInitdata[i*20-nZ07B];
                fValue2 = m_vtnSyInitdata[i*20-nZ07A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
            }
        }
        else if((nCurrVol >= nZ08Down)&&(nCurrVol <= nZ08Up))
        {
            if (nKind == 13) //读基线值,加标后的
            {
                fValue1 = m_vtnByInitdata[i*20-nZ08B];
                fValue2 = m_vtnByInitdata[i*20-nZ08A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
            }
            else//读样品值
            {
                fValue1 = m_vtnSyInitdata[i*20-nZ08B];
                fValue2 = m_vtnSyInitdata[i*20-nZ08A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
            }
        }
        else if((nCurrVol >= nZ24Down)&&(nCurrVol <= nZ24Up))
        {
            if (nKind == 13) //读基线值,加标后的
            {
                fValue1 = m_vtnByInitdata[i*20-nZ24B];
                fValue2 = m_vtnByInitdata[i*20-nZ24A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
            }
            else//读样品值
            {
                fValue1 = m_vtnSyInitdata[i*20-nZ24B];
                fValue2 = m_vtnSyInitdata[i*20-nZ24A];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
            }
        }
        else
        {
            if (nKind == 13) //读基线值,加标后的
            {
                fValue1 = m_vtnByInitdata[i*20-16];
                fValue2 = m_vtnByInitdata[i*20-5];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnSampleSignal.push_back(fValue);
            }
            else//读样品值
            {
                fValue1 = m_vtnSyInitdata[i*20-16];
                fValue2 = m_vtnSyInitdata[i*20-5];
                fValue = fValue2 - fValue1;
                m_struEchemVector.m_vtnWaterSignal.push_back(fValue);
            }
        }
    }
}
int CProcess1::ScanFlowFile(const char *filename)
{
    char czItem[64];memset(czItem,0,64);
    char czTmp[64];
    char czValue[64];
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
            memset(czValue,0,64);
            GetAfxConfig()->GetIni("TempSet","TempValue",czValue);
            m_pro.arrParaValue[0] = atof(czValue)*10;
            memset(czTmp,0,64);
            GetAfxConfig()->GetIni("TempSet","HeatTime",czTmp);
            m_pro.arrParaValue[1] = atoi(czTmp);

            for(int j=0;j<YQMAXCOUNT;j++)
            {
                if(m_pYQ[j])
                {
                    m_pYQ[j]->m_RealInfo.fHeatValue = atof(czValue);
                    m_pYQ[j]->m_RealInfo.nHeatTime = atoi(czTmp);
                 }
            }
//            m_pro.arrParaValue[0] = m_pro.arrParaValue[0]*10;
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
            GetAfxConfig()->GetIni("MainParaSet","Voltage",czTmp);
            m_pro.arrParaValue[0] = atoi(czTmp);
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
void CProcess1::ScanSubFlowFile(const char *filename)
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
//        if(8 == m_pro.PID)
//        {
//            memset(czTmp,0,64);
//            GetAfxConfig()->GetIni("TempSet","TempValue",czTmp);
//            m_pro.arrParaValue[0] = atoi(czTmp)*10;
//            memset(czTmp,0,64);
//            GetAfxConfig()->GetIni("TempSet","HeatTime",czTmp);
//            m_pro.arrParaValue[1] = atoi(czTmp);
//        }

        m_vProcFile.push_back(m_pro);
    }
}
bool CProcess1::CalcTestValue(int nKey)
{
    char str[64];
    if (nKey >= 0)
    {
        double dChroma = 0.0;
        if (m_pYQ[nKey]->m_RealInfo.m_EchemDataInfo.fHeight2 > 0)
        {
            dChroma = m_pYQ[nKey]->m_CurveInfo.m_EchemCurve.SouceStandValue * m_pYQ[nKey]->m_RealInfo.m_EchemDataInfo.fHeight1 / m_pYQ[nKey]->m_RealInfo.m_EchemDataInfo.fHeight2;//浓度 = 加标浓度 * (h / H)
            memset(str,0,64);
            sprintf(str,"Z%d,浓度:%0.3f",m_pYQ[nKey]->m_sYQ.InTypeID,dChroma);
            DebugInfo(str,3);
        }
        else
        {
            dChroma = 0;
        }
        if (CMD_SAMPLETEST == m_nCmd)
        {
            m_pYQ[nKey]->m_RealInfo.fCheckResult =  dChroma*m_pYQ[nKey]->m_CurveInfo.SampleAdjust - m_pYQ[nKey]->m_CurveInfo.m_EchemCurve.BlankChroma;
            if(m_pOnHhInfo)
            {
                m_pOnHhInfo(m_userdata,3,m_pYQ[nKey]->m_sYQ.InTypeID,m_pYQ[nKey]->m_RealInfo.fCheckResult);
            }
        }
        else if(CMD_TEST == m_nCmd)
        {
            m_pYQ[nKey]->m_RealInfo.fResult =  m_fRangeValue*dChroma*m_pYQ[nKey]->m_CurveInfo.SampleAdjust - m_pYQ[nKey]->m_CurveInfo.m_EchemCurve.BlankChroma;
            if(m_pOnHhInfo)
            {
                m_pOnHhInfo(m_userdata,3,m_pYQ[nKey]->m_sYQ.InTypeID,m_pYQ[nKey]->m_RealInfo.fResult);
            }
        }
        else
        {
            //do nothing
        }
        if(m_pYQ[nKey]->m_RealInfo.fResult < 0)//测试结果不为负数
        {
            m_pYQ[nKey]->m_RealInfo.fResult = 0.0;
        }
        return true;
    }
    return false;
}
void CProcess1::NotifyShowCurve(int nKind)
{
    float fScan[100];
    for(int i=0;i<100;i++)
    {
        fScan[i] = 0;
    }
    float fSum = 0;
    float fSlope = 0;
    int nCount = 0;

    if (13 == nKind) //标样数据保存
    {
        if(m_pOnPaintCurve)
        {
            m_pOnPaintCurve(m_userdata,&m_struEchemVector,2);
        }

    }
    else if(51 == nKind)//线性扫描数据保存
    {
        if(m_pOnPaintCurve)
        {
            m_pOnPaintCurve(m_userdata,&m_struEchemVector,3);
        }
        for(int i = 10;i < m_struEchemVector.m_vtnSampleSignal.size()-10; i++)
        {
            fScan[i] = fabs(m_struEchemVector.m_vtnSampleSignal[i+1]-m_struEchemVector.m_vtnSampleSignal[i])/fabs(m_struEchemVector.m_vtnSampleVoltage[i+1]-m_struEchemVector.m_vtnSampleVoltage[i]);
            fSum += fScan[i];
            nCount++;
        }
        fSlope = fSum/nCount;
        char cStr[16];memset(cStr,0,16);
        sprintf(cStr,"%.3f",fSlope);
        for(int i=0;i<YQMAXCOUNT;i++)
        {
            if(m_pYQ[i])
            {
                m_pYQ[i]->m_RealInfo.m_EchemDataInfo.fLinearSlope = atof(cStr);
            }
        }
    }
    else if(11 == nKind)//水样数据保存
    {
        if(m_pOnPaintCurve)
        {
            m_pOnPaintCurve(m_userdata,&m_struEchemVector,1);
        }
    }
    else
    {
        //do nothing
    }

}

void CProcess1::MainWindowDebug(const char *cStr)
{
    char str[64];memset(str,0,64);
    if(m_pOnDebugInfo)
    {
        m_pOnDebugInfo(m_userdata,this->GetProcessType(),cStr);
    }
}
void CProcess1::SaveResultToDatabase(int nCodeID)
{
    char czGroup[64];
    char czValue[64];
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
                        char cStr[MAX_SIZE];memset(cStr,0,MAX_SIZE);
                        char str1[64];memset(str1,0,64);
                        m_pYQ[j]->GetYQParam(sYQ);

                        strcpy(cStr,"保存数据至数据库; ");
                        strcat(cStr,sYQ.czID);
                        sprintf(str1," fchroma:%.2f h:%.2f  H:%.2f ",m_pYQ[j]->m_RealInfo.fResult,m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight1,m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight2);
                        strcat(cStr,str1);
                        strcat(cStr,m_pYQ[j]->m_RealInfo.cDataID);
                        DebugInfo(cStr);
                        m_pOnTestFinished(m_userdata,8051,sYQ.czID);
                        MainWindowDebug("保存水样测试数据");
                     }
                }
            }
        }
        else if(CMD_LINESCAN == m_nCmd)
        {
            if(m_pOnTestFinished)
            {
                for(int i=0;i<YQMAXCOUNT;i++)
                {
                    if(m_pYQ[i])
                    {
                        SYQPARAM sYQ;
                        m_pYQ[i]->GetYQParam(sYQ);
                        m_pOnTestFinished(m_userdata,8052,sYQ.czID);
                        MainWindowDebug("保存斜率");
                        break;
                    }
                }
            }
        }
        else if(CMD_ELECMAINTAIN == m_nCmd)
        {
            if(m_pOnTestFinished)
            {
                for(int i=0;i<YQMAXCOUNT;i++)
                {
                    if(m_pYQ[i])
                    {
                        SYQPARAM sYQ;
                        m_pYQ[i]->GetYQParam(sYQ);
                        //m_pDB->InsEchemHh(sYQ.czID,m_pYQ[i]->m_DeviceInfo.PeakHeight1,m_pYQ[i]->m_DeviceInfo.PeakHeight2,m_pYQ[i]->m_DeviceInfo.curinfo.fChroma);
                        memset(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                        memcpy(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,"电极维护",strlen("电极维护"));
                        m_pOnTestFinished(m_userdata,8053,sYQ.czID);
                        break;
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
        else if(CMD_SAMPLEADJUST == m_nCmd)
        {
            for(int k=0;k<YQMAXCOUNT;k++)
            {
                if(m_pYQ[k])
                {
                    memset(czGroup,0,64);
                    memset(czValue,0,64);
                    sprintf(czGroup,"%s_SET",m_pYQ[k]->m_sYQ.czID);
                    if(0 == m_pYQ[k]->m_RealInfo.m_EchemDataInfo.fHeight1)
                    {
                         m_pYQ[k]->m_CurveInfo.SampleAdjust = 1.0;
                    }
                    else
                    {
                        m_pYQ[k]->m_CurveInfo.SampleAdjust = m_pYQ[k]->m_RealInfo.m_EchemDataInfo.fHeight2/m_pYQ[k]->m_RealInfo.m_EchemDataInfo.fHeight1;
                    }
                    sprintf(czValue,"%f",m_pYQ[k]->m_CurveInfo.SampleAdjust);
                    GetAfxConfig()->SetIni(czGroup,"SampleAdjust",czValue);

                    if(m_bAjustFlag)//循环中只保存一次日志信息
                    {
                        m_bAjustFlag = false;
                        memset(m_pYQ[k]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                        memcpy(m_pYQ[k]->m_RealInfo.m_EchemDataInfo.czMaintain,"标样校准",strlen("标样校准"));
                        m_pOnTestFinished(m_userdata,8053,m_pYQ[k]->m_sYQ.czID);
                    }
                }
            }
        }
        else if(CMD_BANKADJUST == m_nCmd)
        {
            for(int k=0;k<YQMAXCOUNT;k++)
            {
                if(m_pYQ[k])
                {
                    memset(czGroup,0,64);
                    memset(czValue,0,64);
                    m_pYQ[k]->m_CurveInfo.m_EchemCurve.BlankChroma = m_pYQ[k]->m_RealInfo.fResult;
                    sprintf(czGroup,"%s_SET",m_pYQ[k]->m_sYQ.czID);
                    GetAfxConfig()->SetIni(czGroup,"BlankChroma",czValue);

                    if(m_bAjustFlag)//循环中只保存一次日志信息
                    {
                        m_bAjustFlag = false;
                        memset(m_pYQ[k]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                        memcpy(m_pYQ[k]->m_RealInfo.m_EchemDataInfo.czMaintain,"空白校准",strlen("空白校准"));
                        m_pOnTestFinished(m_userdata,8053,m_pYQ[k]->m_sYQ.czID);
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
                        SYQPARAM sYQ;
                        m_pYQ[i]->GetYQParam(sYQ);
                        memset(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,0,64);
                        memcpy(m_pYQ[i]->m_RealInfo.m_EchemDataInfo.czMaintain,"初始化",strlen("初始化"));
                        m_pOnTestFinished(m_userdata,8053,sYQ.czID);
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
    m_bYQBusy = false;
    m_bRun = false;
    //通知主界面仪器空闲
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,1,this->GetProcessType(),m_bYQBusy,0,true);
    }
    SetRegisterWorkStatus(CMD_WAIT);
}
void CProcess1::dealRangeSwitch()
{
    char czTmp[64];
    bool bFlag = false;
    memset(czTmp,0,64);
    GetAfxConfig()->GetIni("RangeSet","RangeSwitch",czTmp);
    if((CMD_TEST == m_nCmd)&&(1 == atoi(czTmp)))
    {
        for(int j=0;j<YQMAXCOUNT;j++)
        {
            if(m_pYQ[j])
            {
                if((m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight1/m_pYQ[j]->m_RealInfo.m_EchemDataInfo.fHeight2) >= (8/m_fRangeValue))
                {
                    GetAfxConfig()->SetIni("RangeSet","RangeWay","1");
                    bFlag = true;
                    break;
                }
             }
         }
        if(!bFlag)
        {
            GetAfxConfig()->SetIni("RangeSet","RangeWay","0");
        }
     }
}
void CProcess1::DealReagentsChange()
{
    if(77 == m_pro.PID)
    {
        ConsumeSub(this->GetProcessType(),"AA",-5);//参比溶液
    }
    else if(82 == m_pro.PID)
    {
         ConsumeSub(this->GetProcessType(),"D",-5);//镀膜液
    }
    else if(76 == m_pro.PID)
    {
         ConsumeSub(this->GetProcessType(),"G",-1);//电解液
    }
    else if(6 == m_pro.PID)
    {
         ConsumeSub(this->GetProcessType(),"E",-1);//消解液
    }
    else if(37 == m_pro.PID)
    {
        ConsumeSub(this->GetProcessType(),"J",-1);//中和液
    }
    else
    {
        //do nothing
    }
}
int CProcess1::ReadFlowFile()
{
    char czPath[MAX_SIZE];
    char czValue[MAX_SIZE];
    char czTmp[MAX_SIZE];
    int nPIDCount = 0;
    bool bFlag = false;

    memset(czPath,0,MAX_SIZE);
    memset(czValue,0,MAX_SIZE);
    memset(czTmp,0,MAX_SIZE);
    sprintf(czPath,"%s",INI_ROOT_PATH);
    strcat(czPath,"/processfile/");
    SetRegisterWorkStatus(m_nCmd);
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
                RecordLog(LOG_ALARM,"未添加水样测试流程文件");
                bFlag = true;
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
                RecordLog(LOG_ALARM,"未添加清洗流程文件");
                bFlag = true;
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
                RecordLog(LOG_ALARM,"未添加初始化流程文件");
                bFlag = true;
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_ELECMAINTAIN://电极维护
        {
            RecordLog(LOG_NOMAL,"发送电极维护命令");
            GetAfxConfig()->GetIni("ProcessFile","ElecMaintain",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加电极维护流程文件");
                bFlag = true;
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
     case CMD_LINESCAN://线性扫描
        {
            RecordLog(LOG_NOMAL,"发送线性扫描命令");
            GetAfxConfig()->GetIni("ProcessFile","LineScan",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加线性扫描流程文件");
                bFlag = true;
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
                bFlag = true;
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
                bFlag = true;
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
                RecordLog(LOG_ALARM,"未添加空白校准流程文件");
                bFlag = true;
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    default:
         break;
    }
    if(bFlag)
    {
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"未添加测试流程文件",LOG_FAULT);
        }
    }
    return nPIDCount;
}
void CProcess1::SetRegisterWorkStatus(unsigned short nStatus)
{
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->SetWorkStatus(nStatus);
        }
    }
}
void CProcess1::SetRegisterFaultType(unsigned short nType)
{
    char tmp[64];
    unsigned short nFault = TYPE_NO;
    if(2 == m_pro.ModelAddress)
    {
        nFault = TYPE_TEMPERATURE;
    }
    else if(3 == m_pro.ModelAddress)
    {
        if(3 == nType)
        {
            nFault = TYPE_PUMP;//电机即是注射泵故障
        }
        else if(8 == nType)
        {
            if((4 == m_pro.PID)||(3 == m_pro.PID)||(59 == m_pro.PID))//润洗液位管
            {
                memset(tmp,0,64);
                GetAfxConfig()->GetIni("ValveNO","ByOrSy",tmp);
                if(1 == atoi(tmp))
                {
                    nFault = TYPE_SAMPLE;
                }
                else
                {
                    nFault = TYPE_TESTWATER;
                }
            }
            else if(77 == m_pro.PID)//抽取标准溶液
            {
                nFault = TYPE_SAMPLE;
            }
            else if(82 == m_pro.PID)//抽取镀膜液
            {
                nFault = TYPE_COATING;
            }
            else if(76 == m_pro.PID)//抽取电解液
            {
                nFault = TYPE_ELECTROLYTE;
            }
            else if(6 == m_pro.PID)//抽取消解液
            {
                nFault = TYPE_DIGESTION;
            }
            else if((12 == m_pro.PID)||(53 == m_pro.PID)||(14 == m_pro.PID))//清洗检测池 抽取清水 清洗管路
            {
                nFault = TYPE_DISTILLED;
            }
            else
            {
                //do nothing
            }

        }
    }
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->SetFaultType(nFault);
        }
    }
}
void CProcess1::SetRegisterProcessCode()
{
    unsigned short nTypeCode = CODE_WAIT;
    if((4 == m_pro.PID)||(77 == m_pro.PID))
    {
        nTypeCode = CODE_TESTSAMPLE;
    }
    else if(6 == m_pro.PID)
    {
        nTypeCode = CODE_GETDIGESTION;

    }else if(19 == m_pro.PID)
    {
        nTypeCode = CODE_UVDIGESTION;
    }
    else if(62 == m_pro.PID)
    {
        nTypeCode = CODE_INTOPOOL;
    }
    else if(76 == m_pro.PID)
    {
        nTypeCode = CODE_GETELECT;
    }
    else if(63 == m_pro.PID)
    {
        nTypeCode = CODE_ENRICHMENT;
    }
    else if(64 == m_pro.PID)
    {
        nTypeCode = CODE_STATIC;
    }
    else if(65 == m_pro.PID)
    {
        nTypeCode = CODE_STRIPPING;
    }
    else if(66 == m_pro.PID)
    {
        nTypeCode = CODE_ADDENRICHMENT;
    }
    else if(67 == m_pro.PID)
    {
        nTypeCode = CODE_ADDSTATIC;
    }
    else if(68 == m_pro.PID)
    {
        nTypeCode = CODE_ADDSTRIPPING;
    }
    else if(12 == m_pro.PID)
    {
        nTypeCode = CODE_CLEARPOOL;
    }

    if(CODE_WAIT != nTypeCode)
    {
        for(int j=0;j<YQMAXCOUNT;j++)
        {
            if(m_pYQ[j])
            {
                m_pYQ[j]->SetProcessCode(nTypeCode);
            }
        }
    }
}

//保存数据至TXT
void CProcess1::SaveSrcDataToTxt()
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
       stream << "Vol"<<"\t"<<"Sy"<<"\t"<<"By"<< "\r\n";
       int nSize = qMax(m_struEchemVector.m_vtnWaterVoltage.size(),m_struEchemVector.m_vtnSampleVoltage.size());
        for ( int i = 0; i < nSize; i++ )
        {
            //保存电压(Vol)数据
            if(nSize == m_struEchemVector.m_vtnWaterVoltage.size())
            {
                stream << m_struEchemVector.m_vtnWaterVoltage[i]<<"\t";
            }
            else
            {
                stream << m_struEchemVector.m_vtnSampleVoltage[i]<<"\t";
            }

            if(i < m_struEchemVector.m_vtnWaterSignal.size())
            {
                stream << m_struEchemVector.m_vtnWaterSignal[i]<<"\t";
            }
            else
            {
                stream <<"no data"<<"\t";
            }

            if(i < m_struEchemVector.m_vtnSampleSignal.size())
            {
                stream << m_struEchemVector.m_vtnSampleSignal[i]<<"\r\n";
            }
            else
            {
                stream <<"no data"<<"\r\n";
            }
        }
        file.close();
    }
}
//线性扫描数据保存
void CProcess1::SaveScanDataToTxt()
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
       stream << "Vol"<<"\t"<<"Scan"<<"\r\n";
       for(int i = 0;i < m_struEchemVector.m_vtnSampleSignal.size(); i++)
       {
           stream << m_struEchemVector.m_vtnSampleVoltage[i]<<"\t"<<m_struEchemVector.m_vtnSampleSignal[i]<<"\r\n";
       }
        file.close();
    }
}
void CProcess1::SetDataCacl(QVector<double> vtnY, QVector<double> vtnX)
{
    vector<double> vtnDataY,vtnDataX;
    if(vtnY.size() == vtnX.size())
    {
        for(int i=0;i<vtnY.size();i++)
        {
            vtnDataY.push_back(vtnY[i]);
            vtnDataX.push_back(vtnX[i]);
        }
    }
    pCalc->SetData(vtnDataY,vtnDataX);
}
