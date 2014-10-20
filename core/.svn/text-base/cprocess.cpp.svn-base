#include "cprocess.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

CProcess::CProcess()
{
    cout << "CProcess::CProcess()" << endl;
    m_pOnTestFinished = NULL;
    m_pOnWorkStatus = NULL;
    m_pOnTotalNum = NULL;
    m_pOnYQBusy = NULL;
    m_pOnDebugInfo = NULL;
    m_pOnTempValue = NULL;
    m_pOnPaintCurve = NULL;
    m_pOnStatusValue = NULL;
    m_pOnScopeData = NULL;
    m_pOnAFSData = NULL;
    m_pOnGasRunInfo = NULL;
    m_pOnGasResult = NULL;
    m_pOnGasCoef = NULL;
    m_pOnHhInfo = NULL;
    m_pOnBsReadValue = NULL;
    m_pOnYQConsumeAlarm = NULL;
    m_pSpectrum = NULL;
    m_pOnShowCurve = NULL;
    m_pOnAlarm = NULL;
    m_pDB = new CDBBusiness("yqdata","yq.db");
    m_Qtime = QDateTime::currentDateTime();
    pCalc = new CArithmetic();
    pDriver = NULL;
    pDriver1 = NULL;
    bExit = false;
    m_bRun = false;
    m_bIdle = true;
    m_bExitFlag = false;
    m_bStart = false;
    m_bDrawCurve = false;
    m_bStopThread = false;
    m_bCaliTag = false;
    m_bSleepTag = false;
    m_nMcuBusy = 0;
    m_bUpSwitchCurve = false;
    m_bDowSwitchCurve = false;
    m_nFinishResult = PRO_OK;
    m_nLastCodeID = 0;
    m_nLastWorkPID = -1;
    m_nType = -1;
    m_nCurrSize = 0;
    m_nTestWayTag = 0;
    m_iSubYQType =  GetAfxYQSubType();
    bExitThread = false;
    m_bRunThread = false;
    m_bInitFlag = true;
    InitConsumemap();
    InitYQLevelVal();
}
CProcess::~CProcess()
{
    m_vtConsume.clear();
    if(pDriver)delete pDriver;
    if(pDriver1)delete pDriver1;
    if(m_pSpectrum) delete m_pSpectrum;
    if(m_pDB)
    {
        delete m_pDB;
        m_pDB = NULL;
    }
    if(pCalc)delete pCalc;
     cout << "void CProcess::~CProcess()" << endl;
}
void CProcess::SetOnTestFinished(unsigned long userdata,OnTestFinished* pOnTestFinished)
{
    m_userdata = userdata;
    m_pOnTestFinished = pOnTestFinished;
}
void CProcess::SetOnWorkStatus(unsigned long userdata,OnWorkStatus* pOnWorkStatus)
{
    m_userdata = userdata;
    m_pOnWorkStatus = pOnWorkStatus;
}
void CProcess::SetOnTotalNum(unsigned long userdata,OnTotalNum* pOnTotalNum)
{
    m_userdata = userdata;
    m_pOnTotalNum = pOnTotalNum;
}
void CProcess::SetOnYQBusy(unsigned long userdata,OnYQBusy* pOnYQBusy)
{
    m_userdata = userdata;
    m_pOnYQBusy = pOnYQBusy;
}
void CProcess::SetOnDebugInfo(unsigned long userdata,OnDebugInfo* pOnDebugInfo)
{
    m_userdata = userdata;
    m_pOnDebugInfo = pOnDebugInfo;
}
void CProcess::SetOnTempValue(unsigned long userdata,OnTempValue* pOnTempValue)
{
    m_userdata = userdata;
    m_pOnTempValue = pOnTempValue;
}
void CProcess::SetOnPaintCurve(unsigned long userdata,OnPaintCurve* pOnPaintCurve)
{
    m_userdata = userdata;
    m_pOnPaintCurve = pOnPaintCurve;
}
void CProcess::Voc_SetOnRealData(unsigned long userdata,Voc_OnRealData* pOnRealData)
{
    m_userdata = userdata;
    m_pOnRealData = pOnRealData;

}
void CProcess::SetOnYQReady(unsigned long userdata, OnYQReady *pOnYQReady)
{
    m_userdata = userdata;
    m_pOnYQReady = pOnYQReady;

}
void CProcess::SetOnStatusValue(unsigned long userdata,OnStatusValue* pOnStatusValue)
{
    m_userdata = userdata;
    m_pOnStatusValue = pOnStatusValue;
}
void CProcess::SetOnScopeData(unsigned long userdata,OnScopeData* pOnScopeData)
{
    m_userdata = userdata;
    m_pOnScopeData = pOnScopeData;
}
void CProcess::SetOnAFSData(unsigned long userdata,OnAFSData* pOnAFSData)
{
    m_userdata = userdata;
    m_pOnAFSData = pOnAFSData;
}
void CProcess::SetOnGasRunInfo(unsigned long userdata,OnGasRunInfo* pOnGasRunInfo)
{
    m_userdata = userdata;
    m_pOnGasRunInfo = pOnGasRunInfo;
}
void CProcess::SetOnGasResult(unsigned long userdata,OnGasResult* pOnGasResult)
{
    m_userdata = userdata;
    m_pOnGasResult = pOnGasResult;
}
void CProcess::SetOnGasCoef(unsigned long userdata,OnGasCoef* pOnGasCoef)
{
    m_userdata = userdata;
    m_pOnGasCoef = pOnGasCoef;
}
void CProcess::SetOnHhInfo(unsigned long userdata,OnHhInfo* pOnHhInfo)
{
    m_userdata = userdata;
    m_pOnHhInfo = pOnHhInfo;
}
void CProcess::SetOnRealCurveDot(unsigned long userdata, OnRealCurveDot *pOnCurveDot)
{
    m_userdata = userdata;
    m_pOnCurveDot = pOnCurveDot;
}

void CProcess::SetOnInitInfo(unsigned long userdata,OnInitInfo* pOnInitInfo)
{
    m_userdata = userdata;
    m_pOnInitInfo = pOnInitInfo;
}
void CProcess::SetOnShowCurve(unsigned long userdata, OnShowCurve *pOnShowCurve)
{
    m_userdata = userdata;
    m_pOnShowCurve = pOnShowCurve;
}
void CProcess::SetOnAlarm(unsigned long userdata, OnAlarm *pOnAlarm)
{
    m_userdata = userdata;
    m_pOnAlarm = pOnAlarm;
}
void CProcess::RefeshConfig()
{
    return;
}

//4 Begin 比色法
void CProcess::SetOnBsReadValue(unsigned long userdata,OnBsReadValue* pOnBsReadValue)
{
    m_userdata = userdata;
    m_pOnBsReadValue = pOnBsReadValue;
}
//耗材报警
void CProcess::setOnYQConsumeAlarm(unsigned long userdata, OnYQConsumeAlarm *pOnYQConsumeAlarm)
{
    m_userdata = userdata;
    m_pOnYQConsumeAlarm = pOnYQConsumeAlarm;
}

//标定曲线、标样校准、空白校准时，显示基线值、样品值
//void CProcess::SetOnBsGratBaseSampleVal(unsigned long userdata,OnBsGratBaseSampleVal* pOnBsGratBaseSampleVal)
//{
//    m_userdata = userdata;
//    m_pOnBsGratBaseSampleVal = pOnBsGratBaseSampleVal;
//}
//4 End 比色法
void CProcess::SetYQs(CYQ* pYQ[YQMAXCOUNT])
{
    for(int i=0;i<YQMAXCOUNT;i++)
    {
        m_pYQ[i] = NULL;
        //只将用到本流程的仪器传进来
        if(pYQ[i])
        {
            if(pYQ[i]->m_pProcess->GetProcessType() == this->GetProcessType())
                m_pYQ[i] = pYQ[i];
        }

        //测试是否成功
        if( m_pYQ[i])
        {
            cout << "void CProcess::SetYQs(CYQ* pYQ[YQMAXCOUNT])" << endl;
        }
    }
}

CYQ* CProcess::GetYQ(const char* czID)
{
    for(int i=0;i<YQMAXCOUNT;i++)
    {
       SYQPARAM sYQ;
       m_pYQ[i]->GetYQParam(sYQ);
       if(strcmp(sYQ.czID,czID)==0)
           return m_pYQ[i];
    }
    return NULL;
}

void CProcess::CreateDriver(int iProcessType)
{
    char czGroup[64];memset(czGroup,0,64);
    char czComItem[64];memset(czComItem,0,64);
    char czCom[64];memset(czCom,0,64);
    sprintf(czGroup,"ProcessType_%d",iProcessType);
    sprintf(czComItem,"comport");
    GetAfxConfig()->GetIni(czGroup,czComItem,czCom);
    switch(iProcessType)
    {
    case 1:
        pDriver = new CDriver1(this->pSerialPorts[atoi(czCom)]);
        break;
    case 2:
        pDriver = new CDriver2(this->pSerialPorts[atoi(czCom)]);
        sprintf(czComItem,"comport1");
        memset(czCom,0,64);
        GetAfxConfig()->GetIni(czGroup,czComItem,czCom);
        pDriver1 = new CDriver2(this->pSerialPorts[atoi(czCom)]);
        break;
    case 3:
        pDriver = new CDriver3(this->pSerialPorts[atoi(czCom)]);
        sprintf(czComItem,"comport1");
        memset(czCom,0,64);
        GetAfxConfig()->GetIni(czGroup,czComItem,czCom);
        pDriver1 = new CDriver3(this->pSerialPorts[atoi(czCom)]);
        break;
    case 4:
        pDriver = new CDriver4(this->pSerialPorts[atoi(czCom)]);
        break;
    case 5:
        pDriver = new CDriver5(this->pSerialPorts[atoi(czCom)]);
        break;
    case 6:
        pDriver = new CDriver1(this->pSerialPorts[atoi(czCom)]);
        m_pSpectrum = new spectrumDevice(this);
        break;
    case 7:
        pDriver = new CDriver1(this->pSerialPorts[atoi(czCom)]);
        break;
    default:
        pDriver = new CDriver0(this->pSerialPorts[atoi(czCom)]);
        break;
    }
}

void CProcess::SetSerialPorts(CSerialPort* pSerialPorts[COMCOUNT])
{
    for(int i=0;i<COMCOUNT;i++)
    {
        this->pSerialPorts[i] = pSerialPorts[i];
    }
}

void CProcess::Run()
{
    if(!bExit)
    pthread_create(&id_autorun, NULL, AutoRun, this);
}
void CProcess::ExitRun()
{
    bExit = true;
    m_bRun = false;
    pthread_join(id_autorun,0);
}
void* CProcess::AutoRun(void* pParam)
{
    CProcess* pThis = (CProcess*)pParam;
    while(!pThis->bExit)
    {
        sleep(1);
        if(pThis->m_bRun)
        {
            pThis->DoProcess();
        }
    }
    return NULL;
}
void CProcess::RunThread()
{
    if(!bExitThread)
     pthread_create(&id_autorun, NULL, AutoRunThread, this);
}

void CProcess::ExitRunThread()
{
    bExitThread = true;
    m_bRunThread = false;
    pthread_join(id_autorun,0);
}

void* CProcess::AutoRunThread(void* pParam)
{
    CProcess* pThis = (CProcess*)pParam;
    while(!pThis->bExitThread)
    {
        sleep(1);
        if(pThis->m_bRunThread)
        {
            pThis->DoThread();
        }
    }
    return NULL;
}

void CProcess::DoThread()
{

}
int CProcess::StartTestCmd(int nCmd,int nKind,QDateTime dt)
{
    if((!GetbIdle() && CMD_STOPTEST != nCmd) || m_bRun)
    {
        cout << "yq is busing now,cmd cancel..." << endl;
        return 0;
    }
    //清除报警
    if(CMD_INIT != nCmd)
    {
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"",-1);
        }
    }
    m_Qtime = dt;
    InitBeginOfTest(nKind);
    m_nCmd = nCmd;
    m_nTestWayTag = nKind;
    int nPIDCount = 0;
    nPIDCount = ReadFlowFile();
    if(nPIDCount > 0)
    {
        for(int i=0;i<YQMAXCOUNT;i++)
        {
            if(m_pYQ[i])
            {
                m_pYQ[i]->m_RealInfo.TestTime = m_Qtime;
            }
        }
        DebugInfo("流程文件读取成功",0);

        if(m_pOnYQBusy)
        {
            m_pOnYQBusy(m_userdata,MSG_PROCFILE,this->GetProcessType(),true,0,true);
            m_pOnYQBusy(m_userdata,MSG_CMD,this->GetProcessType(),true,nCmd,true);
        }
        SetbIdle(false);
        m_bExitFlag = false;//线程是否退出
        m_bRun = true;//执行线程
        return MCU_IDEL;
    }
    else
    {
        if(m_pOnYQBusy)
        {
            m_pOnYQBusy(m_userdata,MSG_PROCFILE,this->GetProcessType(),false,0,false);
        }
        DebugInfo("流程文件读取失败",0);
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"流程文件没找到",LOG_FAULT);
        }
        return MCU_FAULT;//流程文件没找到
    }
}
void CProcess::StartGasTestCmd(QDateTime time)
{

}
//设置仪器是否空闲,且将状态写入文件 0表示空闲 1表示忙
void CProcess::SetbIdle(bool bFlag)
{
    m_bIdle = bFlag;
    if(m_bIdle)
    {
        GetAfxConfig()->SetIni("SYSSET","IdleFlag","0");
    }
    else
    {
        GetAfxConfig()->SetIni("SYSSET","IdleFlag","1");
    }
}
void CProcess::RecordLog(LOG_LEVEL Level, const char *Content)
{
    QDateTime qDateTime = QDateTime::currentDateTime();
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());

    if(m_pDB)
    {
        m_pDB->Ins2091(DateTime,this->GetProcessType(),"仪器",(int)Level, Content);
    }
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->SetLog(Content,strlen(Content));
            m_pYQ[j]->SetLogType(Level);
        }
    }
}
void CProcess::DebugInfo(const char *cStr, int nNum)
{
    char str[255];memset(str,0,255);
    char str1[255];

    strcat(str,cStr);

    if(1 == nNum)
    {
        memset(str1,0,255);
        sprintf(str1,"Number = %d;PID = %d;流程名:",m_nCurrSize,m_pro.PID);
        strcat(str,str1);
        strcat(str,m_pro.cProcName);
    }else if(2 == nNum)
    {
        memset(str1,0,255);
        sprintf(str1,"%d",m_nCmd);
        strcat(str,str1);
    }
    else
    {
        //do nothing
    }

    char czChildPath[256];memset(czChildPath,0,256);
    char czLogName[256];memset(czLogName,0,256);
    char czLogFile[256];
    memset(czLogFile,0,256);
    sprintf(czChildPath,"%s/%d/log",DB_ROOT_PATH,this->GetProcessType());
    mkdir_r(czChildPath);
    sprintf(czLogName,"log%04d%02d%02d-%02d%02d.txt",m_Qtime.date().year(),m_Qtime.date().month(),m_Qtime.date().day(),m_Qtime.time().hour(),m_Qtime.time().minute());
    sprintf(czLogFile,"%s/%s",czChildPath,czLogName);

    InsLogData(czLogFile,QDateTime::currentDateTime(),str);
}
bool CProcess::InsLogData(const char* czPath,QDateTime qDateTime,char *Content)
{
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());

    bool bRet = false;
    QString qsTxtFileName = QString(czPath);
    QFile file(qsTxtFileName);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append| QIODevice::Text) )
    {
        QTextStream stream(&file);
        stream << DateTime <<"\t"<<QString::fromUtf8(Content)<< "\r\n";
        file.close();
        bRet = true;
    }
    return  bRet;
}
int CProcess::mkdir_r(const char *path)
{

        if (path == NULL) {

                return -1;

        }

        char *temp = strdup(path);

        char *pos = temp;



        /* 去掉开头的 './' 或 '/' */

        if (strncmp(temp, "/", 1) == 0) {

                pos += 1;

        } else if (strncmp(temp, "./", 2) == 0) {

                pos += 2;

        }



        /* 循环创建目录 */

        for ( ; *pos != '\0'; ++ pos) {

                if (*pos == '/') {

                        *pos = '\0';

                        mkdir(temp, 0644);

                        printf("for %s\n", temp);

                        *pos = '/';

                }

        }



        /* 如果最后一级目录不是以'/'结尾，

        遇到'\0'就中止循环，

        不会创建最后一级目录 */

        if (*(pos - 1) != '/') {

                printf("if %s\n", temp);

                mkdir(temp, 0644);

        }

        free(temp);

        return 0;

}

void CProcess::InitConsumemap()
{
    m_vtConsume.clear();
    sqlite_data datalist;
    if(m_pDB->GetConsumeInfo(datalist))
    {
        if(!datalist.IsEmpty())
        {
             struConsumeInfo consume;
            for(int i = 1;i <= datalist.RowCount();i++)
            {
                strcpy(consume.cID,datalist.GetData(i,1));
                consume.ProcessType = ((QString)datalist.GetData(i,2)).toInt();
                strcpy(consume.Name,datalist.GetData(i,3));
                consume.MaxValue =  ((QString)datalist.GetData(i,4)).toFloat();
                consume.RealValue =  ((QString)datalist.GetData(i,5)).toFloat();
                consume.WaringValue = ((QString)datalist.GetData(i,6)).toFloat();
                consume.AlarmValue =  ((QString)datalist.GetData(i,7)).toFloat();
                consume.FaultValue =  ((QString)datalist.GetData(i,8)).toFloat();
                consume.ModifyTime = QDateTime::fromString((QString)datalist.GetData(i,9),"yyyy-MM-dd HH:mm:ss");
                consume.iEditType = 0;
                m_vtConsume.push_back(consume);
            }
        }
    }
    datalist.Free();
}

void CProcess::InitYQLevelVal()
{
    char czGroup[64];memset(czGroup,0,64);
    char czItem[64];
    char czValue[64];
    strcpy(czGroup,"YQLevel_SET");

    for(int i = 1;i <= 5;i++)
    {
        memset(czItem,0,64);
        memset(czValue,0,64);

        sprintf(czItem,"YQLevel%d",i);
        GetAfxConfig()->GetIni(czGroup,czItem,czValue);

        if(strlen(czValue) > 0)
        {
            m_mapYQLevVal.insert(i,atof(czValue));
        }
    }
}

void CProcess::OnNotifyConsumeSub(char *cID,int iAlarmLevel)
{
    if(m_pOnYQConsumeAlarm)
    {
        m_pOnYQConsumeAlarm(m_userdata,cID,iAlarmLevel);
    }
}

bool CProcess::ConsumeSub(int nProcessType,char *ID,float fVolume)
{
    if(!m_vtConsume.empty())
    {
        struConsumeInfo*  consume;
        for(int i = 0;i < m_vtConsume.size();i++)
        {
            consume = &m_vtConsume[i];
            if(strcmp(consume->cID,ID) == 0 && consume->ProcessType == nProcessType)
            {
                char chValue[255];
                memset(chValue,0,255);
                QDateTime qDateTime = QDateTime::currentDateTime();
                int iAlarmLevel = LOG_NOMAL; //正常
                if(consume->RealValue >= 0)
                {
                    consume->RealValue += fVolume;
                    if(consume->RealValue < 0 )
                        consume->RealValue = 0;
                    consume->ModifyTime = qDateTime;

                    if(fVolume < 0)//使用规则是递减,比如试剂
                    {
                        if(consume->RealValue == 0)
                        {
                            sprintf(chValue,"%s%s",consume->Name,"已经用完");
                            RecordLog(LOG_ALARM,chValue);
                            iAlarmLevel = LOG_ALARM;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                            }
                        }
                        else if(consume->RealValue > consume->AlarmValue && consume->RealValue <= consume->WaringValue)//警告区间
                        {
                            sprintf(chValue,"%s%s",consume->Name," 低于警告限值");
                            RecordLog(LOG_WARNING,chValue);
                            iAlarmLevel = LOG_WARNING;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_WARNING);
                            }
                        }
                        else if(consume->RealValue > consume->FaultValue && consume->RealValue <= consume->AlarmValue)//报警区间
                        {
                            sprintf(chValue,"%s%s",consume->Name," 低于报警限值");
                            RecordLog(LOG_ALARM,chValue);
                            iAlarmLevel = LOG_ALARM;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                            }
                        }
                        else if(consume->RealValue <= consume->FaultValue)//故障区
                        {
                            sprintf(chValue,"%s%s",consume->Name,"低于故障限值");
                            RecordLog(LOG_ALARM,chValue);
                            iAlarmLevel = LOG_ALARM;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                            }
                        }
                    }
                    else//使用规则是递增,比如氮气发生器产生的气体
                    {
                        if(consume->RealValue < consume->AlarmValue && consume->RealValue >= consume->WaringValue)//警告区间
                        {
                            sprintf(chValue,"%s%s",consume->Name," 达到警告限值");
                            RecordLog(LOG_WARNING,chValue);
                            iAlarmLevel = LOG_WARNING;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_WARNING);
                            }
                        }
                        else if(consume->RealValue < consume->FaultValue && consume->RealValue >= consume->AlarmValue)//报警区间
                        {
                            sprintf(chValue,"%s%s",consume->Name," 达到报警限值");
                            RecordLog(LOG_ALARM,chValue);
                            iAlarmLevel = LOG_ALARM;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                            }
                        }
                        else if(consume->RealValue >= consume->FaultValue)//故障区
                        {
                            sprintf(chValue,"%s%s",consume->Name,"达到故障限值");
                            RecordLog(LOG_ALARM,chValue);
                            iAlarmLevel = LOG_ALARM;
                            if(m_pOnAlarm)
                            {
                                m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                            }
                        }
                    }

                    //更新到数据库
                    m_pDB->UpdateConsumeInfo(*consume);
                }
                else
                {
                    sprintf(chValue,"%s%s",consume->Name,"预设使用次数已经用完");
                    RecordLog(LOG_ALARM,chValue);
                    iAlarmLevel = LOG_ALARM;
                    if(m_pOnAlarm)
                    {
                        m_pOnAlarm(m_userdata,chValue,LOG_ALARM);
                    }
                }
                if(iAlarmLevel != 0)
                {
                    if(m_pOnYQConsumeAlarm)
                    {
                        m_pOnYQConsumeAlarm(m_userdata,consume->cID,iAlarmLevel);
                    }
                }
                break;
            }
        }
    }
}
//保存测试时间，当流程正常结束，则保存测试时间，否则不保存
void CProcess::SaveTestTime()
{
    char czPtr[64];memset(czPtr,0,64);
    if(!m_bInitFlag)
    {
        QDateTime currDT = QDateTime::currentDateTime();
        int iPtr = abs(m_Qtime.secsTo(currDT));

        sprintf(czPtr,"%d",iPtr/60);//转成分钟
        //记录每次测试所用时间
        if(CMD_TEST == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_TEST",czPtr);
        }
        else if(CMD_CURVECALIBRATION == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_CURVECALIBRATION",czPtr);
        }
        else if(CMD_BANKADJUST == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_BANKADJUST",czPtr);
        }
        else if(CMD_SAMPLEADJUST == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_SAMPLEADJUST",czPtr);
        }
        else if(CMD_WASH == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_WASH",czPtr);
        }
        else if(CMD_INIT == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_INIT",czPtr);
        }
        else if(CMD_ELECMAINTAIN == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_ELECMAINTAIN",czPtr);
        }
        else if(CMD_LINESCAN == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_LINESCAN",czPtr);
        }
        else if(CMD_SAMPLETEST == m_nCmd)
        {
            GetAfxConfig()->SetIni("CountDown","CMD_SAMPLETEST",czPtr);
        }
    }
}
void CProcess::DealInitCmd()
{
    if((m_bInitFlag)&&(CMD_INIT != m_nCmd))
    {
        if(CMD_STOPTEST == m_nCmd)
        {
            if(m_pOnInitInfo)
            {
                m_pOnInitInfo(m_userdata,false);
            }
        }
        else
        {
            if(m_pOnInitInfo)
            {
                m_pOnInitInfo(m_userdata,true);
            }
        }
    }
    else
    {
        //do nothing
    }
}
void CProcess::SaveResultToDatabase(int nCodeID)
{
    if(m_pOnTestFinished)
    {
        CYQ* pYQ = NULL;
        if(nCodeID == 0)//所有参数
        {
            for(int i=0;i<YQMAXCOUNT;i++)
            {
                pYQ = m_pYQ[i];
                if(pYQ)
                {
                    if(CMD_TEST == m_nCmd)
                    {
                        if(m_pOnTestFinished)
                        {
                            m_pOnTestFinished(m_userdata,8051,pYQ->m_sYQ.czID);
                        }
                    }
                    else if(m_nCmd == CMD_SAMPLETEST) //标样核查
                    {
                        if(m_pOnTestFinished)
                        {
                            m_pOnTestFinished(m_userdata,3016,pYQ->m_sYQ.czID);
                        }
                    }
                    pYQ = NULL;
                }
            }
        }
        else//单个参数
        {
            for(int i=0;i<YQMAXCOUNT;i++)
            {
                pYQ = m_pYQ[i];
                if(pYQ && pYQ->m_sYQ.InTypeID == nCodeID)
                {
                    if(CMD_TEST == m_nCmd)
                    {
                        if(m_pOnTestFinished)
                        {
                            m_pOnTestFinished(m_userdata,8051,pYQ->m_sYQ.czID);
                        }
                    }
                    else if(m_nCmd == CMD_SAMPLETEST) //标样核查
                    {
                        if(m_pOnTestFinished)
                        {
                            m_pOnTestFinished(m_userdata,3016,pYQ->m_sYQ.czID);
                        }
                    }
                    pYQ = NULL;
                    break;
                }
            }
        }
    }
}

void CProcess::DealDataTag(int nModelAddress,int nValue)
{
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            if(2 == nModelAddress)//温度控制模块
            {
                if(2 == nValue)//2	表示参数设置错误
                {
                }
                else if(3 == nValue)//3	表示第一路温度故障
                {
                }
                else if(4 == nValue)//4	表示第二路温度故障
                {
                }
                else if(5 == nValue)//5	表示第一路、第二路温度故障
                {
                }
                else if(6 == nValue)//6	表示第一路负载故障
                {
                }
                else if(7 == nValue)//7	表示第一路温度、第一路负载故障
                {
                }
                else if(8 == nValue)//8	表示第一负载故障、第二路温度故障
                {
                }
                else if(9 == nValue)//9	表示第一路温度、第一路负载、第二路温度故障
                {
                }
                else if(10 == nValue)//10	表示第二路负载故障
                {
                }
                else if(11 == nValue)//11	表示第一路温度故障、第二路负载故障
                {
                }
                else if(12 == nValue)//12	表示第二路温度故障、第二路负载故障
                {
                }
                else if(13 == nValue)//13	表示第一路温度、第二路温度、第二路负载故障
                {
                }
                else if(14 == nValue)//14	表示第一路负载、第二路负载故障
                {
                }
                else if(15 == nValue)//15	表示第一路温度、第一路负载、第二路负载故障
                {
                }
                else if(16 == nValue)//16	表示第一路负载、第二路温度、第二路负载故障
                {
                }
                else if(17 == nValue)//17	表示第一路温度、负载、第二路温度、负载故障。
                {
                }
            }
            else if(3 == nModelAddress)//联动模块
            {
                if(2 == nValue)//表示参数设置错误
                {
                }
                else if(3 == nValue)//表示电机故障
                {
                }
                else if(4 == nValue)//表示液位1校准故障
                {
                }
                else if(5 == nValue)//表示液位2校准故障
                {
                }
                else if(6 == nValue)//表示液位3校准故障
                {
                }
                else if(7 == nValue)//表示液位4校准故障
                {
                }
                else if(8 == nValue)//表示缺试剂故障
                {
                    if(4 == m_pro.PID)//缺水样
                     {
                        if(3 == m_nTestWayTag)
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"lw");
                        }
                        else
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lw");
                        }
                     }
                    else if((76 == m_pro.PID)||(3 == m_pro.PID)||(81 == m_pro.PID))//缺试剂
                     {
                        if(3 == m_nTestWayTag)
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"lr");
                        }
                        else
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lr");
                        }
                     }
                    else if((12 == m_pro.PID)||(14 == m_pro.PID)||(53 == m_pro.PID)||(21 == m_pro.PID))//缺纯水
                     {
                        if(3 == m_nTestWayTag)
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"lp");
                        }
                        else
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lp");
                        }
                     }
                     else if(82 == m_pro.PID)//缺标样
                     {
                        if(3 == m_nTestWayTag)
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"ls");
                        }
                        else
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"/ls");
                        }
                     }
                    else
                    {
                        if(3 == m_nTestWayTag)
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"lr");
                        }
                        else
                        {
                            strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lr");
                        }
                    }
                }
            }
            if(6 == nModelAddress)//比色法检测模块
            {
                //模块工作状态查询。0表示空闲，1表示正常工作，2表示参数设置错误，3表示校准AD值错误
                if(2 == nValue)//2表示参数设置错误
                {
                    if(3 == m_nTestWayTag)
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"bc");
                    }
                    else
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/bc");
                    }
                }
                else if(3 == nValue)//3表示校准AD值错误
                {
                    if(3 == m_nTestWayTag)
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"ae");
                    }
                    else
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/ae");
                    }
                }
            }
            else if(255 == nModelAddress)
            {
                if(7 == nValue)
                {
                    if(3 == m_nTestWayTag)//自动测试
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"D");
                    }
                    else
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/D");
                    }
                }
                else if(9 == nValue)
                {
                    if(3 == m_nTestWayTag)
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"C");//通讯故障(或数据采集失败)
                    }
                    else
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/C");
                    }
                }
                else if(10 == nValue)
                {
                    if(3 == m_nTestWayTag)
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"cc");//手动中止测试
                    }
                    else
                    {
                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/cc");
                    }
                }
            }

//            switch(nValue)
//            {
//             case 2:
//             case 3:
//             case 4:
//             case 5:
//             case 6:
//             case 7:
//                if(3 == m_nTestWayTag)//自动测试
//                {
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"D");
//                }
//                else{
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"/D");
//                }
//                break;
//             case 8:
//                if(4 == m_pro.PID)//缺水样
//                 {
//                    if(3 == m_nTestWayTag)
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"lw");
//                    }else{
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lw");
//                    }
//                 }
//                else if((76 == m_pro.PID)||(3 == m_pro.PID)||(81 == m_pro.PID))//缺试剂
//                 {
//                    if(3 == m_nTestWayTag)
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"lr");
//                    }else{
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lr");
//                    }

//                 }
//                else if((12 == m_pro.PID)||(14 == m_pro.PID)||(53 == m_pro.PID)||(21 == m_pro.PID))//缺纯水
//                 {
//                    if(3 == m_nTestWayTag)
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"lp");
//                    }else{
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lp");
//                    }
//                 }
//                 else if(82 == m_pro.PID)//缺标样
//                 {
//                    if(3 == m_nTestWayTag)
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"ls");
//                    }else
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/ls");
//                    }
//                 }
//                else{
//                    if(3 == m_nTestWayTag)
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"lr");
//                    }
//                    else
//                    {
//                        strcat(m_pYQ[j]->m_RealInfo.cDataID,"/lr");
//                    }

//                }
//                 break;
//            case 9:
//                if(3 == m_nTestWayTag)
//                {
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"C");//通讯故障(或数据采集失败)
//                }
//                else
//                {
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"/C");
//                }
//                break;
//            case 10:
//                if(3 == m_nTestWayTag)
//                {
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"cc");//手动中止测试
//                }
//                else
//                {
//                    strcat(m_pYQ[j]->m_RealInfo.cDataID,"/cc");
//                }
//                break;
//            default:
//                break;
//              }


         }
    }
}
void CProcess::JudeFaultType(int nFault)
{
    char czLog[MAX_SIZE];
    char czValue[64];

    memset(czLog,0,MAX_SIZE);
    memset(czValue,0,64);
    memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
    LOG_LEVEL LOGLevelType;
    if((1 == m_pro.ModelAddress)||(2 == m_pro.ModelAddress)||(5 == m_pro.ModelAddress))//继电器 温度模块 电化学
    {
        sprintf(czValue,"%d",nFault);
        strcat(czLog,czValue);
        strcat(czLog,"类型故障");
        RecordLog(LOG_ALARM,czLog);
        LOGLevelType = LOG_ALARM;
    }
    else if(3 == m_pro.ModelAddress)//联动模块
    {
        if(2 == nFault)
        {
            strcat(czLog,"参数设置错误");
            RecordLog(LOG_ALARM,czLog);
            LOGLevelType = LOG_ALARM;
        }
        else if(3 == nFault)
        {
            strcat(czLog,"电机故障");
            RecordLog(LOG_FAULT,czLog);
            LOGLevelType = LOG_FAULT;
        }
        else if(4 == nFault)
        {
            strcat(czLog,"液位1故障");
            RecordLog(LOG_FAULT,czLog);
            LOGLevelType = LOG_FAULT;
        }
        else if(5 == nFault)
        {
            strcat(czLog,"液位2故障");
            RecordLog(LOG_FAULT,czLog);
            LOGLevelType = LOG_FAULT;
        }
        else if(6 == nFault)
        {
            strcat(czLog,"液位3故障");
            RecordLog(LOG_FAULT,czLog);
            LOGLevelType = LOG_FAULT;
        }
        else if(7 == nFault)
        {
            strcat(czLog,"液位4故障");
            RecordLog(LOG_FAULT,czLog);
            LOGLevelType = LOG_FAULT;
        }
        else if(8 == nFault)
        {
            strcat(czLog,"缺试剂");
            RecordLog(LOG_ALARM,czLog);
            LOGLevelType = LOG_ALARM;
        }
        else
        {
            strcat(czLog,"其它故障");
        }
    }
    else
    {
        //do nothing
    }

    if(m_pOnAlarm)
    {
        m_pOnAlarm(m_userdata,czLog,LOGLevelType);
    }

}
void CProcess::DoProcess()
{
    bool bFinish = true;
    m_bInitFlag = true;
    int nResult = 1;
    m_nFinishResult = PRO_OK;//流程执行返回结果
    char cLogTxt[255];
    int nTimeOutTimes = 0;
    int nPIDCount = m_quMyPro.size();
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,MSG_WORK,this->GetProcessType(),true,0,true);
    }
    if(m_pOnTotalNum)
    {
        m_pOnTotalNum(m_userdata,this->GetProcessType(),m_quMyPro.size());
    }
    while(true)
    {
        if(m_bExitFlag)
        {
            DebugInfo("终止测试,进入终止流程处理",0);
            m_nCmd = CMD_STOPTEST;
            StopDealEvent();//终止流程处理
            DealDataTag(255,10);//写入数据标记
            RecordLog(LOG_WARNING,"收到终止命令,终止测试");
            m_nFinishResult = PRO_ABORT;
            break;
        }
        if(bFinish)
        {
            if(m_quMyPro.size() == 0)
            {
                m_nFinishResult = PRO_OK;
                m_bInitFlag = false;
                DebugInfo("流程正常执行完成",0);
                break;
            }
            else
            {
                m_pro = m_quMyPro.front();
                m_nCurrSize = nPIDCount - m_quMyPro.size() + 1;
                if(m_pOnWorkStatus)
                {
                    m_pOnWorkStatus(m_userdata,this->GetProcessType(),m_nCurrSize,m_pro.cProcName);
                }
                if(DoCurrentCmd() == CMD_OK)//执行流程
                {
                    m_quMyPro.pop();
                    usleep(100000);//延时500毫秒才处理读状态
                    bFinish = false;

                    DebugInfo("写modbus 10指令成功 ",1);
                    nTimeOutTimes = 0;
                    if(m_pro.LogTag)
                    {
                       RecordLog(LOG_NOMAL,m_pro.cProcName);
                    }
                 }
                 else if(CMD_TIMEOUT)
                 {
                    DebugInfo("写modbus 10指令失败 ",1);
                    DealDataTag(255,9);
                    printf("写Modbus失败 %d \n",m_pro.PID);


                    memset(cLogTxt,0,255);
                    sprintf(cLogTxt,"[%s] %s",m_pro.cProcName," 执行流程时通讯故障");
                    RecordLog(LOG_FAULT,cLogTxt);
                    if(m_pOnAlarm)
                    {
                        m_pOnAlarm(m_userdata,"执行流程时通讯故障",LOG_FAULT);
                    }
                    m_nFinishResult = PRO_CMDOUT;
                    break;
                 }
            }
        }
        else
        {
            usleep(200000);
            nResult = QueryStatus();
            if(nResult == MCU_TIMEOUT)
            {
                 nTimeOutTimes++;
                 if(nTimeOutTimes > 10)
                 {
                     DebugInfo("读modbus 03指令失败,超时 255,进入终止流程处理",1);
                     StopDealEvent();//终止流程处理

                     DealDataTag(255,9);

                     memset(cLogTxt,0,255);
                     sprintf(cLogTxt,"[%s] %s",m_pro.cProcName," 读取状态超时超过10次,流程异常结束>>");
                     RecordLog(LOG_FAULT,cLogTxt);
                     if(m_pOnAlarm)
                     {
                         m_pOnAlarm(m_userdata,"读取状态超时超过10次",LOG_FAULT);
                     }
                     m_nFinishResult = PROC_READOUT;
                     break;
                 }
                 else
                 {
                     DebugInfo("读modbus 03指令,等待",1);
                 }
            }
            else if(nResult == MCU_IDEL)
            {
                m_prepro = m_pro;
                nTimeOutTimes = 0;
                bFinish = true;
            }
            else if(nResult == SYS_ABORT)//等待温度或时间，手工终止
            {
                DebugInfo("等待温度或时间时收到终止命令,进入终止流程处理",1);
                StopDealEvent();//终止流程处理

                RecordLog(LOG_ALARM,"等待温度或时间时收到终止命令,流程异常结束>>");
                m_nFinishResult = SYS_ABORT;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
            else if(nResult == SYS_TIMEOUT)//等待温度,超时20分钟
            {
                DebugInfo("等待温度,超时,进入终止流程处理",1);
                StopDealEvent();//终止流程处理

                RecordLog(LOG_FAULT,"等待温度超时,流程异常结束>>");
                m_nFinishResult = SYS_TIMEOUT;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
            else if(nResult == SYS_FIRFAULT)
            {
                DebugInfo("点火失败,进入终止流程处理",1);
                StopDealEvent();//终止流程处理

                RecordLog(LOG_FAULT,"重复点火失败,流程异常结束>>");
                m_nFinishResult = SYS_FIRFAULT;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
            else if(nResult == SPEC_FAULT)
            {
                DebugInfo("读光谱信号失败,进入终止流程处理",1);
                StopDealEvent();//终止流程处理

                RecordLog(LOG_FAULT,"读光谱信号失败,流程异常结束>>");
                m_nFinishResult = SPEC_FAULT;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
            else if(nResult >= MCU_FAULT)
            {
                DebugInfo("读modbus 03指令失败，故障 2,进入终止流程处理",1);
                StopDealEvent();//终止流程处理
                DealDataTag(m_pro.ModelAddress,nResult);
                JudeFaultType(nResult);

                memset(cLogTxt,0,255);
                sprintf(cLogTxt,"[%s:%d] %s",m_pro.cProcName,nResult," 缺试剂或仪器故障,流程异常结束>>");
                RecordLog(LOG_FAULT,cLogTxt);
                m_nFinishResult = PROC_ERROR;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
        }
    }
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,MSG_WORK,this->GetProcessType(),false,0,false);
    }
    //流程正常时保存数据放到各流程的计算结果函数,异常终止则保存数据,手工终止不保存数据
    if((m_nFinishResult != PRO_OK) && (m_nFinishResult != PRO_ABORT))
    {
        RecordLog(LOG_NOMAL,"异常结束");
        SaveResultToDatabase();
    }
    m_bRun = false;
    SetbIdle(true);
    DealInitCmd();
    SaveTestTime();//保存测试时间
}

STATUS CProcess::DoCurrentCmd()
{
    if (m_pro.ModelAddress == 0)//系统模块
        return CMD_OK;

    STATUS nRet = CMD_OK;
    int16_t wValue = 0;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    char czLog[MAX_SIZE];
    memset(czLog,0,MAX_SIZE);
    char tmp[64];
    memset(tmp,0,64);

    //取样水的类型
    int inTestType = -1;
    int inLevelNum = -1;

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
                            inTestType = 1;
                        }
                        else if(0 == atoi(tmp))
                        {
                            memset(tmp,0,64);
                            GetAfxConfig()->GetIni("ValveNO","WaterValveNO",tmp);
                            wValue = atoi(tmp);
                            inTestType = 2;
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
                        inTestType = 1;
                        memset(tmp,0,64);
                        GetAfxConfig()->GetIni("ValveNO","SampleValveNO",tmp);////标样阀
                        wValue = atoi(tmp);
                    }
                case CMD_BANKADJUST://空白校准
                    {
                        inTestType = 3;
                        memset(tmp,0,64);
                        GetAfxConfig()->GetIni("ValveNO","DistilledValveNO",tmp);//蒸馏水阀
                        DebugInfo(tmp,0);
                        DebugInfo("tmpvalue",0);
                        wValue = atoi(tmp);
                    }
                    break;
                }
            }else if(i == 4)//源2,取界面上源2阀编号
            {
                memset(tmp,0,64);
                GetAfxConfig()->GetIni("ValveNO","DCFSource2",tmp);
                wValue = atoi(tmp);
            }
        }

        bBuff[i*2+2] = HIBYTE(wValue);
        bBuff[i*2+3] = LOBYTE(wValue);
    }

    int nIndex;
    for (nIndex = 0; nIndex < 3;nIndex++)
    {
        if(pDriver->WriteModbus(m_pro.ModelAddress,40001,m_pro.ParaCount+1,bBuff))
        {
            if(m_pro.PID == 3 || m_pro.PID == 4 || m_pro.PID == 14 || m_pro.PID == 15 || m_pro.PID == 50
                    || m_pro.PID == 53 || m_pro.PID == 69 || m_pro.PID == 30 || m_pro.PID == 32 || m_pro.PID == 77
                    || m_pro.PID == 5 || m_pro.PID == 6 || m_pro.PID == 16 || m_pro.PID == 17 || m_pro.PID == 20
                    || m_pro.PID == 22 || m_pro.PID == 24 || m_pro.PID == 26 || m_pro.PID == 29 || m_pro.PID == 35
                    || m_pro.PID == 37 || m_pro.PID == 39 || m_pro.PID == 61 || m_pro.PID == 76 || 82)
            {
                UpdateYQConsume(m_pro.PID,inTestType,inLevelNum);
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
    return nRet;
}
int CProcess::QueryStatus()
{
    int nResult = MCU_BUSY;
    if((m_pro.PID == 9)||(m_pro.PID == 10))
    {
        unsigned char bBuff[MAX_SIZE];
        memset(bBuff,0,MAX_SIZE);

        if(pDriver->ReadModbus(m_prepro.ModelAddress,40051,13,200,bBuff))
        {
            GetMCUWorkStatus(bBuff);
            nResult = m_nMcuBusy;
        }
        else
        {
            nResult = MCU_TIMEOUT;//超时
        }

        if (MCU_BUSY != nResult)//需要关闭加速冷却
        {
            if(m_pro.PID == 10)
                m_pro.arrParaValue[1]  = 0;//关闭
            if(DoCurrentCmd() == CMD_OK)
            {
                DebugInfo("发送命令关闭加速冷却,返回成功:",0);
            }
            else
            {
                DebugInfo("发送命令关闭加速冷却,返回失败:",0);
            }
            usleep(500000);//延时500毫秒才处理
        }
    }else if(m_pro.PID == 40)
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
        else
        {
            m_SpacTime.start();
            m_bSleepTag = true;
        }
    }
    else
    {
        if(m_pro.ModelAddress == 0)//系统模块
        {
            nResult = GetSysModuleStatus();
        }
        else
        {
            unsigned char bBuff[MAX_SIZE];
            memset(bBuff,0,MAX_SIZE);
            if(pDriver->ReadModbus(m_pro.ModelAddress,40051,13,bBuff))
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
void CProcess::GetMCUWorkStatus(unsigned char *bBuff)
{
    int16_t wModbus40051 = 0;
    int16_t wModels40052 = 0;
    int16_t wStatus40053 = 0;

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
            if(m_pOnTempValue)
                m_pOnTempValue(m_userdata,fValue);
            if (fValue > m_pro.arrParaValue[3]) //如果还没有降到指定温度,则置温度模块为忙,继续降温
                m_nMcuBusy = MCU_BUSY;
            else
                m_nMcuBusy = MCU_IDEL;

            if(m_SpacTime.elapsed() > m_pro.arrParaValue[4]*60*1000)
            {
                m_nMcuBusy = MCU_FAULT;
                DebugInfo("温度降不下来",0);
            }
        }
        break;
    case 0x01:
        if(8 == m_pro.PID || 18 == m_pro.PID)
        {
            float fValue = MAKEWORD(bBuff[9],bBuff[8])/10.0;
            if(m_pOnTempValue)
                m_pOnTempValue(m_userdata,fValue);
        }
        break;
    default:
        break;
    }
}
int CProcess::GetSysModuleStatus()
{

}
int CProcess::InitBeginOfTest(int nKind)
{
    m_bSleepTag = false;
    m_bExitFlag = false;//线程是否退出
    m_nLastWorkPID = -1;//赋工作ID为无效

    return 0;
}

int CProcess::ReadFlowFile()
{
    while (!m_quMyPro.empty())
    {
        m_quMyPro.pop();
    }

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
    case CMD_SAMPLETEST://标样核查
    case CMD_SAMPLEADJUST://标样校准
    case CMD_BANKADJUST://空白校准
        {
            RecordLog(LOG_NOMAL,"发送测试命令");
            GetAfxConfig()->GetIni("ProcessFile","WaterTest",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加测试流程文件");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加测试流程文件",LOG_FAULT);
                }
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_CURVECALIBRATION://标定工作曲线
        {
            RecordLog(LOG_NOMAL,"发送标定命令");
            GetAfxConfig()->GetIni("SYSSET","BDCodeID",czTmp);//标定曲线对应的参数代码
            sprintf(czValue,"%s",&czTmp[1]);
            m_pro.PID = 56;//直接读取子流程
            m_pro.arrParaValue[0] = atoi(czValue);
            if(m_pro.arrParaValue[0] > 0)
            {
               nPIDCount = ScanSubFlowFile();
            }
            else
            {
                RecordLog(LOG_ALARM,"未找到标定流程文件");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未找到标定流程文件",LOG_FAULT);
                }
            }
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
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加清洗流程文件",LOG_FAULT);
                }
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
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加初始化流程文件",LOG_FAULT);
                }
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    default:
         break;
    }
    return nPIDCount;
}
int CProcess::ScanFlowFile(const char *filename)
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
        }
        else
        {
            m_pro.PID = atoi(czTmp);
        }

        if(m_pro.PID == 56 || 57 == m_pro.PID)
        {
            memset(czTmp,0,64);
            GetAfxConfig("processfile",filename)->GetIni(czItem,"1",czTmp);
            m_pro.arrParaValue[0] = atoi(czTmp);
            ScanSubFlowFile();
        }
        else
        {
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

            if(8 == m_pro.PID || 18 == m_pro.PID)
            {
                float fTempt = 80.0;
                int nTime = 2;
                if(m_pro.arrParaValue[0] == 0 || m_pro.arrParaValue[0] == 1)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime",czTmp);
                    nTime = atoi(czTmp);
                }else if(m_pro.arrParaValue[0] == 2)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue_2",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime_2",czTmp);
                    nTime = atoi(czTmp);
                }else if(m_pro.arrParaValue[0] == 3)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue_3",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime_3",czTmp);
                    nTime = atoi(czTmp);
                }
                m_pro.arrParaValue[0] = fTempt;
                m_pro.arrParaValue[1] = nTime;

                for(int j=0;j<YQMAXCOUNT;j++)
                {
                    if(m_pYQ[j])
                    {
                        m_pYQ[j]->m_RealInfo.fHeatValue = fTempt;
                        m_pYQ[j]->m_RealInfo.nHeatTime = nTime;
                     }
                }
            }else if(54 == m_pro.PID)
            {
                m_nLastCodeID = m_pro.arrParaValue[0];
            }

            m_quMyPro.push(m_pro);
        }
    }
    return m_quMyPro.size();
}
int CProcess::ScanSubFlowFile()
{
    char filename[64];memset(filename,0,64);
    if(m_pro.PID == 56)//流程子程序,根据参数1来读取相应的流程子程序
    {
        m_nLastCodeID = m_pro.arrParaValue[0];
        sprintf(filename,"test_Z%02d.dll",m_pro.arrParaValue[0]);
    }else if(m_pro.PID == 57)//抽样子程序,根据参数1来读取相应参数的工作曲线
    {
        char CurveID[64];memset(CurveID,0,64);
        char cItem[64];memset(cItem,0,64);
        if(m_nCmd == CMD_CURVECALIBRATION)
        {
            sprintf(cItem,"DemaCurveID_Z%02d",m_nLastCodeID);
        }
        else
        {
            sprintf(cItem,"WorkCurveID_Z%02d",m_nLastCodeID);
        }
        GetAfxConfig()->GetIni("SYSSET",cItem,CurveID);
        sprintf(filename,"test_%d.dll",atoi(CurveID));
    }

    char czItem[64];memset(czItem,0,64);
    char czTmp[64];
    for(int i=0;i<MAXSIZE;i++)
    {
        sprintf(czItem,"%d",i);

        memset(czTmp,0,64);
        if(!GetAfxConfig("processfile",filename)->GetIni(czItem,"PID",czTmp))
        {
            break;
        }
        else
        {
            m_pro.PID = atoi(czTmp);
        }
        if(m_pro.PID == 57)
        {
            memset(czTmp,0,64);
            GetAfxConfig("processfile",filename)->GetIni(czItem,"1",czTmp);
            m_pro.arrParaValue[0] = atoi(czTmp);
            ScanSubFlowFile();
        }
        else
        {
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
            if(8 == m_pro.PID || 18 == m_pro.PID)
            {
                float fTempt = 80.0;
                int nTime = 2;
                if(m_pro.arrParaValue[0] == 0 || m_pro.arrParaValue[0] == 1)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime",czTmp);
                    nTime = atoi(czTmp);
                }else if(m_pro.arrParaValue[0] == 2)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue_2",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime_2",czTmp);
                    nTime = atoi(czTmp);
                }else if(m_pro.arrParaValue[0] == 3)//第一路
                {
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","TempValue_3",czTmp);
                    fTempt = atof(czTmp)*10;
                    memset(czTmp,0,64);
                    GetAfxConfig()->GetIni("TempSet","HeatTime_3",czTmp);
                    nTime = atoi(czTmp);
                }
                m_pro.arrParaValue[0] = fTempt;
                m_pro.arrParaValue[1] = nTime;
            }else if(54 == m_pro.PID)
            {
                m_nLastCodeID = m_pro.arrParaValue[0];
            }

            m_quMyPro.push(m_pro);
        }
    }
    return m_quMyPro.size();
}
void CProcess::StopDealEvent(bool bTag)
{
    pDriver->AllModuleReset();
    DebugInfo("复位总线所有模块");
}
void CProcess::MainWindowDebug(const char *cStr)
{
    char str[64];memset(str,0,64);
    if(m_pOnDebugInfo)
    {
        m_pOnDebugInfo(m_userdata,this->GetProcessType(),cStr);
    }
}
void CProcess::UpdateYQConsume(int iPID,int iTestType,int iLevelNum)
{
    char czConName[64];memset(czConName,0,64);

    // SJ  试剂
    //3   润洗液位管   4   抽取样品
    if(iPID == 3 || iPID == 4)
    {
        if(iTestType == 1) //标样
        {
            strcpy(czConName,"标样");
        }
        else if(iTestType == 3) //蒸馏水
        {
            strcpy(czConName,"蒸馏水");
        }
        else if(iTestType == 2)//水样
        {
            //do nothing
        }
    }//14  清洗管路 15  清洗吹气池  50  清洗液位管  53  抽取清水  69  清洗消解池
    else if(iPID == 14 || iPID == 15 || iPID == 50 || iPID == 53 || iPID == 69)
    {
        strcpy(czConName,"蒸馏水");
    }
    //30  抽取标液一	 32  抽取标液二
    else if(iPID == 30 || iPID == 32 || iPID == 77)// 77  抽取标准溶液
    {
        strcpy(czConName,"标样");
    }
    else if(iPID == 5)//抽取掩蔽剂
    {
        strcpy(czConName,"掩蔽剂");
    }
    else if(iPID == 6)//抽取消解液
    {
        strcpy(czConName,"消解液");
    }
    else if(iPID == 16)//抽取调节液
    {
        strcpy(czConName,"调节液");
    }
    else if(iPID == 17)//抽取显色剂
    {
        strcpy(czConName,"显色剂");
    }
    else if(iPID == 20)//抽取还原液
    {
        strcpy(czConName,"还原液");
    }
    else if(iPID == 22)//抽取滴定液
    {
        strcpy(czConName,"滴定液");
    }
    else if(iPID == 24)//抽取缓冲液
    {
        strcpy(czConName,"缓冲液");
    }
    else if(iPID == 26)//抽取氧化剂
    {
        strcpy(czConName,"氧化剂");
    }
    else if(iPID == 29)//抽取释放剂
    {
        strcpy(czConName,"释放剂");
    }
    else if(iPID == 35)//抽取吸收液
    {
        strcpy(czConName,"吸收液");
    }
    else if(iPID == 37)//抽取中和液
    {
        strcpy(czConName,"中和液");
    }
    else if(iPID == 39)//抽取萃取液
    {
        strcpy(czConName,"萃取液");
    }
    else if(iPID == 61 || iPID == 76)//抽取电解液
    {
        strcpy(czConName,"电解液");
    }
    else if(iPID == 82)//抽取镀膜液
    {
        strcpy(czConName,"镀膜液");
    }

    if(strlen(czConName) > 0)
    {
        char czID[16];memset(czID,0,16);
        vector<struConsumeInfo>::iterator it;
        for(it=m_vtConsume.begin();it<m_vtConsume.end();it++)
        {
            if(!strcmp(czConName,(*it).Name))
            {
                strcpy(czID,(*it).cID);
                break;
            }
        }

        if(strlen(czID) > 0)
        {
            QMap<int,float>::iterator it = m_mapYQLevVal.find(iLevelNum);
            float fLevelValue = it.value();
            ConsumeSub(this->GetProcessType(),czID,-1*fLevelValue);
        }
    }
}
void CProcess::SetRegisterWorkStatus(unsigned short nStatus)
{
    for(int j=0;j<YQMAXCOUNT;j++)
    {
        if(m_pYQ[j])
        {
            m_pYQ[j]->SetWorkStatus(nStatus);
        }
    }
}
void CProcess::SetRegisterFaultType(unsigned short nType)
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
bool CProcess::VerifyRange(const char* czID,int nCurrentCurve,double dCurrentValue)
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
