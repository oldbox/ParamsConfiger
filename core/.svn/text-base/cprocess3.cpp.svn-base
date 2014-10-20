#include "cprocess3.h"
#include "VocArithmetic.h"
CProcess3::CProcess3()
{
    m_DetecAddress = 9;
    m_nGainDataRate = 100;
    m_nGainDataTime = 2;
    m_bProcThread = false;
    m_bSubThread = false;
    m_bReadBaseLine = false;
    m_bCalcluteTag = false;
    m_nFireWaitTag = 0;
    m_nFireTimes = 0;
    m_subthread = 0;
    ReadIniFile("LC_NitMin",m_fNitMin);
    ReadIniFile("LC_NitMax",m_fNitMax);
    ReadIniFile("LC_AirMin",m_fAirMin);
    ReadIniFile("LC_AirMax",m_fAirMax);

    char tmp[64];memset(tmp,0,64);
    GetAfxConfig()->GetIni("INFO","Detector",tmp);
    m_nDetector = atoi(tmp);

    RefeshConfig();

    m_pCalc = new CVocArithmetic();
    m_bRunThread = true;
    RunThread();
    m_qtHcTime.start();
}
CProcess3::~CProcess3()
{
    ExitRunThread();
    ExitSubThread();
    if(m_pCalc)
    {
        delete m_pCalc;
        m_pCalc = NULL;
    }
}

int CProcess3::GetProcessType()
{
    return 3;
}
//测试前初始化
int CProcess3::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);
    //清空源始数据区
    m_vtRT.clear();
    m_vtVol.clear();
    m_vtTp.clear();
    m_vtNitr.clear();
    m_bSleepTag = false;
    m_nFireTimes = 0;
    m_nFireWaitTag = 0;
    CYQ* pYQ = NULL;
    for(int i = 0;i<YQMAXCOUNT;i++)
    {
        if( m_pYQ[i])
        {
            pYQ = m_pYQ[i];

            //初使化数据结构对象
            pYQ->m_RealInfo.TestTime = m_Qtime;//测试时间
            pYQ->m_RealInfo.fResult = 0.0;
            pYQ->m_RealInfo.fCheckResult = 0.0;
            pYQ->m_RealInfo.m_vocDataInfo.RemainTime = 0;
            pYQ->m_RealInfo.m_vocDataInfo.PeakHight = 0;
            pYQ->m_RealInfo.m_vocDataInfo.Area = 0;
            pYQ->m_RealInfo.m_vocDataInfo.K = pYQ->m_CurveInfo.K;
            pYQ->m_RealInfo.m_vocDataInfo.R = pYQ->m_CurveInfo.R;
            pYQ->m_RealInfo.m_vocDataInfo.B = pYQ->m_CurveInfo.B;
            pYQ->m_RealInfo.SampleAdjust = pYQ->m_CurveInfo.SampleAdjust;
            pYQ->m_RealInfo.CheckChroma = pYQ->m_CurveInfo.CheckChroma;

            memset(pYQ->m_RealInfo.cDataID,0,16);
            if (1 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"hd",2);
            }
            else if(2 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"nt",2);
            }
        }
    }
    return 0;
}
void CProcess3::RefeshConfig()
{
    int nValue = 0;
    float fValue = 0.0;
    ReadIniFile("PJJTemp_Init",nValue);//捕集阱温度初始
    m_stuInitTempt.PJJTemp_Init = nValue;
    ReadIniFile("PJJTemp_JX",nValue);//捕集阱解析温度
    m_stuInitTempt.PJJTemp_Jx = nValue;
    ReadIniFile("PJJTemp_HK",nValue);//捕集阱烘烤温度
    m_stuInitTempt.PJJTemp_Hk = nValue;
    ReadIniFile("FIDTemp_Init",nValue);//检测器温度
    m_stuInitTempt.FIDTemp_Init = nValue;
    ReadIniFile("MxzTemp_Init",nValue);//毛细柱初始
    m_stuInitTempt.MxzTemp_Init = nValue;
    ReadIniFile("BoxTemp_Init",nValue);//箱体
    m_stuInitTempt.BoxTemp_Init = nValue;
    ReadIniFile("YQTemp_Safe",nValue);//仪器安全温度
    m_stuInitTempt.YQTemp_Safe = nValue;
    ReadIniFile("CSPress",fValue);//吹扫压力
    m_stuInitTempt.CSPress = fValue;
    ReadIniFile("Nitrpress",fValue);//氮气压力
    m_stuInitTempt.Nitrpress = fValue;


    ReadIniFile("SampleTime",nValue);
    m_stuInitTempt.SampleTime = nValue;

    ReadIniFile("CSTime_BY",fValue);//吹扫时间(单位:分钟),标样
    m_stuInitTempt.CSTime_BY = fValue;

    ReadIniFile("CSTime",fValue);//吹扫时间(单位:分钟)
    m_stuInitTempt.CSTime = fValue;

    ReadIniFile("GCTime",fValue);//干吹时间(单位:分钟)
    m_stuInitTempt.GCTime = fValue;

    ReadIniFile("PJJTime_jx",nValue);//解析时间(挥发性：秒，半挥发性：分钟)
    m_stuInitTempt.PJJTime_jx = nValue;

    ReadIniFile("PJJTime_hk",fValue);//烘烤时间(单位:分钟)
    m_stuInitTempt.PJJTime_hk = fValue;


    ReadIniFile("GainDataRate",nValue);
    m_stuInitTempt.GainDataRate = nValue;

    ReadIniFile("GainDataRTime",nValue);
    m_stuInitTempt.GainDataRTime = nValue;


    ReadIniFile("DescTemp_1",nValue);
    m_stuInitTempt.DescTemp[0] = nValue;
    ReadIniFile("DescTemp_2",nValue);
    m_stuInitTempt.DescTemp[1] = nValue;
    ReadIniFile("DescTemp_3",nValue);
    m_stuInitTempt.DescTemp[2] = nValue;
    ReadIniFile("DescTemp_4",nValue);
    m_stuInitTempt.DescTemp[3] = nValue;
    ReadIniFile("DescTemp_5",nValue);
    m_stuInitTempt.DescTemp[4] = nValue;


    ReadIniFile("TempRate_1",nValue);
    m_stuInitTempt.TempRate[0] = nValue;
    ReadIniFile("TempRate_2",nValue);
    m_stuInitTempt.TempRate[1] = nValue;
    ReadIniFile("TempRate_3",nValue);
    m_stuInitTempt.TempRate[2] = nValue;
    ReadIniFile("TempRate_4",nValue);
    m_stuInitTempt.TempRate[3] = nValue;
    ReadIniFile("TempRate_5",nValue);
    m_stuInitTempt.TempRate[4] = nValue;


    ReadIniFile("TempRTime_1",nValue);
    m_stuInitTempt.TempRTime[0] = nValue;
    ReadIniFile("TempRTime_2",nValue);
    m_stuInitTempt.TempRTime[1] = nValue;
    ReadIniFile("TempRTime_3",nValue);
    m_stuInitTempt.TempRTime[2] = nValue;
    ReadIniFile("TempRTime_4",nValue);
    m_stuInitTempt.TempRTime[3] = nValue;
    ReadIniFile("TempRTime_5",nValue);
    m_stuInitTempt.TempRTime[4] = nValue;


    ReadIniFile("FireAddr",nValue);
    m_stuInitTempt.FireAddr = nValue;
    ReadIniFile("FireDCF1",nValue);
    m_stuInitTempt.FireDCF1 = nValue;
    ReadIniFile("FireDCF2",nValue);
    m_stuInitTempt.FireDCF2 = nValue;


    ReadIniFile("FireOnOff",nValue);//测试完后是否关火
    m_stuInitTempt.FireOnOff = nValue;
}

//启动测试
int CProcess3::StartTestCmd(int nCmd, int nKind, QDateTime dt)
{
    if((!GetbIdle() && CMD_STOPTEST != nCmd) || m_bRun)
    {
        cout << "yq is busing now,cmd cancel..." << endl;
        return 0;
    }
    if(nCmd != CMD_LINESCAN && nCmd != CMD_INIT)//读基线不判断
    {
        if(!ReadyGo())//系统状态没准备好
        {
            cout << "warning:yq is not ready!" << endl;
            return 0;
        }
    }
    //清除报警
    if(m_pOnAlarm)
    {
        m_pOnAlarm(m_userdata,"",-1);
    }
    m_Qtime = dt;
    m_nCmd = nCmd;
    InitBeginOfTest(nKind);
    int nPIDCount =  0;
    switch(nCmd)
    {
    case CMD_TEST://立即测试
        {
            m_bReadBaseLine  = false;
            nPIDCount = ReadFlowFile();
        }
        break;
    case CMD_LINESCAN://读基线
        {
            m_bReadBaseLine  = true;
            nPIDCount = ReadFlowFile();
        }
        break;
    case CMD_SAMPLEADJUST://标样校准
        {
            m_bReadBaseLine  = false;
            nPIDCount = ReadFlowFile();
        }
        break;
    case CMD_INIT://初始化
        {
            m_bReadBaseLine  = false;
            nPIDCount = ReadFlowFile();
        }
        break;
    default:
        break;
    }
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

int CProcess3::ReadFlowFile()
{
    char czPath[MAX_SIZE];
    char czValue[MAX_SIZE];
    char czTmp[MAX_SIZE];
    int nPIDCount = 0;

    memset(czPath,0,MAX_SIZE);
    memset(czValue,0,MAX_SIZE);
    memset(czTmp,0,MAX_SIZE);
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
                RecordLog(LOG_ALARM,"未添加水样测试流程文件");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加水样测试流程文件",LOG_FAULT);
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
     case CMD_LINESCAN://线性扫描,有机物为读基线
        {
            RecordLog(LOG_NOMAL,"发送读基线命令");
            GetAfxConfig()->GetIni("ProcessFile","ElecMaintain",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加读基线流程文件");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加读基线流程文件",LOG_FAULT);
                }
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
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加标样核查流程文件",LOG_FAULT);
                }
            }
            nPIDCount = ScanFlowFile(czValue);
        }
        break;
    case CMD_SAMPLEADJUST://内标测试命令
         {
            RecordLog(LOG_NOMAL,"发送内标测试命令");
            GetAfxConfig()->GetIni("ProcessFile","SampleAdjust",czTmp);
            if(strlen(czTmp)>strlen(czPath))
            {
                memcpy(czValue,czTmp+strlen(czPath),strlen(czTmp)-strlen(czPath));
            }
            else
            {
                RecordLog(LOG_ALARM,"未添加内标测试流程文件");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"未添加内标测试流程文件",LOG_FAULT);
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
//执行流程主线程
void CProcess3::DoProcess()
{
    bool bFinish = true;
    m_bInitFlag = true;
    int nResult = 1;
    m_nFinishResult = PRO_OK;//流程执行返回结果
    int nTimeOutTimes = 0;
    char bContext[255];
    char bMain[255];
    memset(bMain,0,255);
    memset(bContext,0,255);
    int nPIDCount = m_quMyPro.size();
    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,MSG_WORK,this->GetProcessType(),true,0,true);
    }
    if(m_pOnTotalNum)
    {
        m_pOnTotalNum(m_userdata,this->GetProcessType(),m_quMyPro.size());
    }
    ConsumeSub(this->GetProcessType(),"HC_S",-1);//萃取头
    DebugInfo("<<<<<<<<<<<<<<<<<<<<<流程开始======================",0);
    while(true)
    {
        if(m_bExitFlag)
        {
            DebugInfo("终止测试,进入终止流程处理",0);

            StopDealEvent();//终止流程处理
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
                    if(m_pro.LogTag)
                    {
                        memset(bMain,0,255);
                        sprintf(bMain,"%s",m_pro.cProcName);
                    }
                    else if(m_pro.PID == 105 && m_pro.arrParaValue[0] == 120)//等待采集数据,特殊处理下
                    {
                        sprintf(bContext,"等待:采集数据");
                    }
                    else
                    {
                         sprintf(bContext,"%s:%s",bMain,m_pro.cProcName);
                    }
                    m_pOnWorkStatus(m_userdata,this->GetProcessType(),m_nCurrSize,bContext);
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
                 }else if(CMD_TIMEOUT)
                 {
                    DebugInfo("写modbus 10指令失败 ",1);

                    printf("写Modbus失败 %d \n",m_pro.PID);
                    RecordLog(LOG_FAULT,"执行流程时通讯故障");
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

                     RecordLog(LOG_FAULT,"读取状态超时超过10次,流程异常结束>>");
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
            else if(nResult >= MCU_FAULT)
            {
                DebugInfo("读modbus 03指令失败，故障 2,进入终止流程处理",1);
                StopDealEvent();//终止流程处理

                RecordLog(LOG_FAULT,"缺试剂或仪器故障,流程异常结束>>");
                m_nFinishResult = PROC_ERROR;
                cout<<"nResult = "<<nResult<<endl;
                break;
            }
        }
    }

    if((m_nFinishResult != PRO_OK) && (m_nFinishResult != PRO_ABORT))
    {
        RecordLog(LOG_NOMAL,"异常结束");
    }

    //FID检测器,流程结束是否关火
    if(m_nDetector == 0)
    {
        if(m_stuInitTempt.FireOnOff == 0)//关闭
        {
            CtrlFire(0,false);
            RecordLog(LOG_FAULT,"流程结束,关阀熄火");
        }
        else//保持,为了安全,检测火是否点燃状态
        {
            if(m_data.nFireTag == 0)//灭火状态
            {
                CtrlFire(0,false);//为了安全,把氢气关闭
                RecordLog(LOG_FAULT,"流程结束,FID熄火,关阀");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"流程结束,FID熄火,关阀",LOG_FAULT);
                }
            }
            else
            {
                RecordLog(LOG_FAULT,"流程结束,保持点火状态");
            }
        }
    }

    if(m_pOnYQBusy)
    {
        m_pOnYQBusy(m_userdata,MSG_WORK,this->GetProcessType(),false,0,false);
    }
    ConsumeSub(this->GetProcessType(),"HC_T",-1);//毛细柱

    DebugInfo("===============流程结束>>>>>>>>>>>>>>>>>>>>>>",0);
    m_bRun = false;
    SetbIdle(true);
    DealInitCmd();
    SaveTestTime();//保存测试时间
}

//解析流程，发送命令到模块执行
STATUS CProcess3::DoCurrentCmd()
{
    if (m_pro.ModelAddress == 0)//系统模块
        return CMD_OK;

    STATUS nRet = CMD_OK;
    int nResult = VocDoCurrentCmd();
    if(nResult == CMD_END)
    {
        int16_t wValue = 0;
        unsigned char bBuff[MAX_SIZE];
        memset(bBuff,0,MAX_SIZE);

        wValue = m_pro.Cmd;
        bBuff[0] = HIBYTE(wValue);
        bBuff[1] = LOBYTE(wValue);

        //参数
        for (int i = 0;i < m_pro.ParaCount;i++)
        {
            wValue = m_pro.arrParaValue[i];

//            if(m_pro.PID == 58)//控制测试后是否关闭火PID=58
//            {
//                if (i == 1)//动态修改第二个参数，开还是关
//                {
//                    int nValue = 0;
//                    ReadIniFile("FireOnOff",nValue);//测试完后是否关火
//                    wValue = nValue;
//                }
//            }

            bBuff[i*2+2] = HIBYTE(wValue);
            bBuff[i*2+3] = LOBYTE(wValue);
        }

        int nIndex;
        bool bret = false;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            bret = pDriver->WriteModbus(m_pro.ModelAddress,40001,m_pro.ParaCount+1,bBuff);

            if(bret)
            {
                usleep(100000);
                break;
            }
            else
            {
                usleep(200000);
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
    }else if(nResult == 2)
        nRet = CMD_TIMEOUT;//发送命令失败
    else if(nResult == 1)
        nRet = CMD_OK;
    return nRet;
}
int CProcess3::VocDoCurrentCmd()
{
    if(!pDriver1) return CMD_TIMEOUT;

    int nRet = CMD_OK;
    int nValue = 0;
    int nIndex = -1;
    if(m_pro.PID == 113)//采样速率设置
    {
        //ReadIniFile("GainDataRate",nValue);
        m_nGainDataRate = m_stuInitTempt.GainDataRate;
        //ReadIniFile("GainDataRTime",nValue);
        m_nGainDataTime = m_stuInitTempt.GainDataRTime;

        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetDataSpeed_150(m_pro.ModelAddress,m_nGainDataRate,m_nGainDataTime))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 114 || m_pro.PID == 132)//协调模块，设置捕集井或富集柱
    {
        if( m_pro.arrParaValue[0] == 109)//预解析
        {
            //ReadIniFile("PJJTemp_JX",nValue);
            nValue = m_stuInitTempt.PJJTemp_Jx;
        }else if( m_pro.arrParaValue[0] == 111)//烘烤
        {
            //ReadIniFile("PJJTemp_HK",nValue);
            nValue = m_stuInitTempt.PJJTemp_Hk;
        }else//其它情况则视为初始温度
        {
            //ReadIniFile("PJJTemp_Init",nValue);
            nValue = m_stuInitTempt.PJJTemp_Init;
        }
        nValue = nValue*10;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_pro.ModelAddress,PJJINIT_TEMP,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 115 || m_pro.PID == 133)//协调模块，FID检测器或ECD检测器
    {
        //ReadIniFile("FIDTemp_Init",nValue);
        nValue = m_stuInitTempt.FIDTemp_Init*10;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_pro.ModelAddress,FIDINIT_TEMP,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 116)//协调模块，设置箱体温度
    {
        //ReadIniFile("BoxTemp_Init",nValue);
        nValue = m_stuInitTempt.BoxTemp_Init*10;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_pro.ModelAddress,BOXINIT_TEMP,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 117)//协调模块，设置毛细柱初始温度
    {
        //ReadIniFile("MxzTemp_Init",nValue);
        nValue = m_stuInitTempt.MxzTemp_Init*10;
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_pro.ModelAddress,MXZINIT_TEMP,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 118)//协调模块，高压输出
    {
        nValue = m_pro.arrParaValue[0];
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->GYOupput_158(m_pro.ModelAddress,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 119)//协调模块，点火
    {
        nIndex = 0;//点火放到查询状态的地方特殊处理
//        m_SpacTime.start();//做点火检测用
//        for (nIndex = 0; nIndex < 3;nIndex++)
//        {
//            if(((CDriver3*)pDriver1)->Fire_159(m_pro.ModelAddress))
//                break;
//            else
//                usleep(100000);
//        }
    }else if(m_pro.PID == 120)//协调模块，数据采集
    {
        nValue = m_pro.arrParaValue[0];
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->StartGetData_160(m_pro.ModelAddress,nValue))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 122)//协调模块，设置线性升温阶梯
    {
        int nTempt1,nTempt2,nTempt3,nTempt4,nTempt5;
        nTempt1 = nTempt2 = nTempt3 = nTempt4 = nTempt5 = 0;
        //if(ReadIniFile("DescTemp_1",nValue))
        {
            nTempt1 = m_stuInitTempt.DescTemp[0]*10;
        }
        //if(ReadIniFile("DescTemp_2",nValue))
        {
            nTempt2 = m_stuInitTempt.DescTemp[1]*10;
        }
        //if(ReadIniFile("DescTemp_3",nValue))
        {
            nTempt3 = m_stuInitTempt.DescTemp[2]*10;
        }
        //if(ReadIniFile("DescTemp_4",nValue))
        {
            nTempt4 = m_stuInitTempt.DescTemp[3]*10;
        }
        //if(ReadIniFile("DescTemp_5",nValue))
        {
            nTempt5 = m_stuInitTempt.DescTemp[4]*10;
        }

        nValue = m_pro.arrParaValue[0];
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetLineTemptDesc_154(m_pro.ModelAddress,nValue,nTempt1,nTempt2,nTempt3,nTempt4,nTempt5))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 123)//协调模块，设置线性升温斜率
    {
        int nK1,nK2,nK3,nK4,nK5;
        nK1 = nK2 = nK3 = nK4 = nK5 = 0;
        //if(ReadIniFile("TempRate_1",nValue))
        {
            nK1 = m_stuInitTempt.TempRate[0];
        }
        //if(ReadIniFile("TempRate_2",nValue))
        {
            nK2 = m_stuInitTempt.TempRate[1];
        }
        //if(ReadIniFile("TempRate_3",nValue))
        {
            nK3 = m_stuInitTempt.TempRate[2];
        }
        //if(ReadIniFile("TempRate_4",nValue))
        {
            nK4 = m_stuInitTempt.TempRate[3];
        }
        //if(ReadIniFile("TempRate_5",nValue))
        {
            nK5 = m_stuInitTempt.TempRate[4];
        }
        nValue = m_pro.arrParaValue[0];
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetLineTemptK_155(m_pro.ModelAddress,nValue,nK1,nK2,nK3,nK4,nK5))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 124)//协调模块，设置保持时间
    {
        int nRT1,nRT2,nRT3,nRT4,nRT5;
        nRT1=nRT2=nRT3=nRT4=nRT5=0;
        //if(ReadIniFile("TempRTime_1",nValue))
        {
            nRT1 = m_stuInitTempt.TempRTime[0];
        }
        //if(ReadIniFile("TempRTime_2",nValue))
        {
            nRT2 = m_stuInitTempt.TempRTime[1];
        }
        //if(ReadIniFile("TempRTime_3",nValue))
        {
            nRT3 = m_stuInitTempt.TempRTime[2];
        }
        //if(ReadIniFile("TempRTime_4",nValue))
        {
            nRT4 = m_stuInitTempt.TempRTime[3];
        }
        //if(ReadIniFile("TempRTime_5",nValue))
        {
            nRT5 = m_stuInitTempt.TempRTime[4];
        }
        nValue = m_pro.arrParaValue[0];
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->SetLineTemptRTime_156(m_pro.ModelAddress,nValue,nRT1,nRT2,nRT3,nRT4,nRT5))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 126)//协调模块，启动线性升温
    {
        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->StartLineTempt_152(m_pro.ModelAddress,m_pro.arrParaValue[0],m_pro.arrParaValue[1]))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 128)//协调模块，启动单路温度
    {
        if(m_pro.arrParaValue[0] == 114 || m_pro.arrParaValue[0] == 132)//捕集井或富集柱
            nValue = PJJINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 115 || m_pro.arrParaValue[0] == 133)//FID检测器或ECD检测器
            nValue = FIDINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 116)//箱体
            nValue = BOXINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 117)//毛细柱
            nValue = MXZINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 109)//预解析温度,捕集阱
            nValue = PJJINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 111)//烘烤,捕集阱
            nValue = PJJINIT_TEMP;
        else
            nValue = m_pro.arrParaValue[0];


        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if(((CDriver3*)pDriver1)->StartTemptSigle_151(m_pro.ModelAddress,nValue,m_pro.arrParaValue[1]))
                break;
            else
                usleep(100000);
        }
    }
    else if(m_pro.PID == 136)//联动模块，反复抽样
    {
        int nSrcDCF1 = m_pro.arrParaValue[0];
        int nSrcDCF2 = m_pro.arrParaValue[1];
        int nDestDCF =  m_pro.arrParaValue[2];
        int nSpeed =  m_pro.arrParaValue[3];
        int nStep =  m_pro.arrParaValue[4];
        //ReadIniFile("SampleTime",nValue);//挥发性：进样时间(单位:分钟) ，半挥发性则为是抽样次数
        int nTimes =  m_stuInitTempt.SampleTime;

        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if (((CDriver3*)pDriver)->RePollPush_LD(m_pro.ModelAddress,nSrcDCF1,nSrcDCF2,nDestDCF,nSpeed,nStep,nTimes))
                break;
            else
                usleep(100000);
        }
    }else if(m_pro.PID == 135)//联动模块，反复推抽
    {
        int nSrcDCF1 = m_pro.arrParaValue[0];
        int nSrcDCF2 = m_pro.arrParaValue[1];
        int nDestDCF =  m_pro.arrParaValue[2];
        int nSpeed =  m_pro.arrParaValue[3];
        int nStep =  m_pro.arrParaValue[4];
        //ReadIniFile("CSTime",nValue);//挥发性：吹扫时间(单位:分钟) ，半挥发性则为是抽样次数
        int nTimes =  (int)m_stuInitTempt.CSTime;

        for (nIndex = 0; nIndex < 3;nIndex++)
        {
            if (((CDriver3*)pDriver)->RePollPush_LD(m_pro.ModelAddress,nSrcDCF1,nSrcDCF2,nDestDCF,nSpeed,nStep,nTimes))
                break;
            else
                usleep(100000);
        }
    }
    if(nIndex < 0) //不是协调模块的命令
        nRet = CMD_END;
    else if (nIndex == 3) //超时
        nRet = CMD_TIMEOUT;
    else //正常
        nRet = CMD_OK;

    return nRet;
}
//查询当前执行流程的模块
int CProcess3::QueryStatus()
{
   int nResult = MCU_BUSY;
   if(m_pro.PID == 40)
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
        }else if(m_pro.PID == 120)//采集数据,直接忽略
        {
            sleep(1);
            nResult = MCU_IDEL;
        }
        else
        {
            unsigned char bBuff[MAX_SIZE];
            memset(bBuff,0,MAX_SIZE);
            bool bret = false;
             if(m_pro.PID >= 113 && m_pro.PID <= 134)//此PID区域是发给协调模块的命令
             {
//                 if(m_pro.PID == 119)//协调模块，点火
//                 {
//                     if(m_data.nFireTag == 0)//如果是灭火状态,则点火,否则不再重复点火
//                     {
//                         m_nFireTimes++;
//                         if(m_nFireTimes >= 10)//每隔10秒发一次点火命令
//                         {
//                             m_nFireTimes = 0;
//                             ((CDriver3*)pDriver1)->Fire_159(m_pro.ModelAddress);
//                         }
//                         int spac = m_SpacTime.elapsed();
//                         if(spac/1000 >= 60*3)//超过3分钟
//                         {
//                             RecordLog(LOG_WARNING,"点火失败");
//                             if(m_pOnAlarm)
//                             {
//                                 m_pOnAlarm(m_userdata,"点火失败",LOG_WARNING);
//                             }
//                             nResult = SYS_FIRFAULT;
//                         }
//                         else
//                         {
//                            nResult = MCU_BUSY;
//                         }
//                     }
//                     else
//                     {
//                         nResult = MCU_IDEL;
//                     }
//                 }
//                 else
//                 {
//                      nResult = MCU_IDEL;
//                 }
                  nResult = MCU_IDEL;
             }
             else
             {
                  bret = pDriver->ReadModbus(m_pro.ModelAddress,40051,13,bBuff);
                  if(bret)
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
    }
    return nResult;

}
//解析当前模块返回来的数据包
void CProcess3::GetMCUWorkStatus(unsigned char* bBuff)
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
        break;
    case 0x01:
        break;
    default:
        break;
    }
}
//处理当前pid为系统模块的PID
int CProcess3::GetSysModuleStatus()
{
    int nResult = MCU_IDEL;
    bool bRet = true;
    if(m_pro.PID == 121)////等待温度
    {
        int nValue = 0;
        if(m_pro.arrParaValue[0] == 114 || m_pro.arrParaValue[0] == 132)//捕集井或富集柱
            nValue = PJJINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 115 || m_pro.arrParaValue[0] == 133)//FID检测器或ECD检测器
            nValue = FIDINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 116)//箱体
            nValue = BOXINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 117)//毛细柱
            nValue = MXZINIT_TEMP;
        else if(m_pro.arrParaValue[0] == 109/* || m_pro.arrParaValue[0] == 4*/)//预解析温度,4是为了兼容,把流程改了后就删除
            nValue = PJJJX_TEMP;
        else if(m_pro.arrParaValue[0] == 111/* || m_pro.arrParaValue[0] == 5*/)//烘烤,4是为了兼容,把流程改了后就删除
            nValue = PJJHK_TEMP;
        else
        {
            RecordLog(LOG_FAULT,"等待温度参数设置错误");
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"等待温度参数设置错误",LOG_FAULT);
            }
            return SYS_ABORT;
        }

        int nTemp = 0;
        switch(nValue)
        {
        case PJJINIT_TEMP:
            //ReadIniFile("PJJTemp_Init",nTemp);
            nTemp = m_stuInitTempt.PJJTemp_Init;
            break;
        case FIDINIT_TEMP:
            //ReadIniFile("FIDTemp_Init",nTemp);
            nTemp = m_stuInitTempt.FIDTemp_Init;
            break;
        case BOXINIT_TEMP:
            //ReadIniFile("BoxTemp_Init",nTemp);
            nTemp = m_stuInitTempt.BoxTemp_Init;
            break;
        case MXZINIT_TEMP:
            //ReadIniFile("MxzTemp_Init",nTemp);
            nTemp = m_stuInitTempt.MxzTemp_Init;
            break;
        case PJJJX_TEMP:
            //ReadIniFile("PJJTemp_JX",nTemp);
            nTemp = m_stuInitTempt.PJJTemp_Jx;
            break;
        case PJJHK_TEMP:
            //ReadIniFile("PJJTemp_HK",nTemp);
            nTemp = m_stuInitTempt.PJJTemp_Hk;
            break;
        }
        nResult = SysWaitTemplate(nValue,nTemp);
        //报警处理,通知ui
        if(nResult == SYS_TIMEOUT)
        {
            switch(nValue)
            {
            case PJJINIT_TEMP:
                RecordLog(LOG_FAULT,"等待捕集阱温度超时");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"等待捕集阱温度超时",LOG_FAULT);
                }
                break;
            case FIDINIT_TEMP:
                RecordLog(LOG_FAULT,"等待检测器温度超时");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"等待检测器温度超时",LOG_FAULT);
                }
                break;
            case BOXINIT_TEMP:
                RecordLog(LOG_FAULT,"等待箱体温度超时");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"等待箱体温度超时",LOG_FAULT);
                }
                break;
            case MXZINIT_TEMP:
                RecordLog(LOG_FAULT,"等待毛细柱温度超时");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"等待毛细柱温度超时",LOG_FAULT);
                }
                break;
            case PJJJX_TEMP:
                RecordLog(LOG_FAULT,"等待解析温度超时");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"等待解析温度超时",LOG_FAULT);
                }
                break;
            case PJJHK_TEMP:
                 RecordLog(LOG_FAULT,"等待烘烤温度超时");
                 if(m_pOnAlarm)
                 {
                     m_pOnAlarm(m_userdata,"等待烘烤温度超时",LOG_FAULT);
                 }
                break;
            }
        }
    }else if(m_pro.PID == 105)//待机
    {
        if(m_pro.arrParaValue[0] == 120)//等待数据采集
        {
            sleep(1);
            if(m_bSubThread)
                nResult = MCU_BUSY;
            else
                nResult = MCU_IDEL;
        }
        else
        {
            int nValue = 10;
            float fValue = 10.0;
            switch(m_pro.arrParaValue[0])
            {
            case 106://进样时间
                //ReadIniFile("SampleTime",nValue);//进样时间(单位:秒)
                bRet = WaitForTime(m_stuInitTempt.SampleTime);
                break;
            case 107://吹扫时间
                if(CMD_SAMPLEADJUST == m_nCmd)
                    fValue = m_stuInitTempt.CSTime_BY;//ReadIniFile("CSTime_BY",fValue);//吹扫时间(单位:分钟),标样
                else
                    fValue  = m_stuInitTempt.CSTime;//ReadIniFile("CSTime",fValue);//吹扫时间(单位:分钟)

                bRet = WaitForTime(fValue*60);
                break;
            case 108://干吹时间
                //ReadIniFile("GCTime",fValue);//干吹时间(单位:分钟)
                bRet = WaitForTime(m_stuInitTempt.GCTime*60);
                break;
            case 110://解析时间
                //ReadIniFile("PJJTime_jx",nValue);//解析时间(挥发性：秒，半挥发性：分钟)
                if(m_iSubYQType == 1)//半挥发性
                    bRet = WaitForTime(m_stuInitTempt.PJJTime_jx*60);
                else//挥发性
                    bRet = WaitForTime(m_stuInitTempt.PJJTime_jx);
                break;
            case 111://烘烤时间
                //ReadIniFile("PJJTime_hk",fValue);//烘烤时间(单位:分钟)
                bRet = WaitForTime(m_stuInitTempt.PJJTime_hk*60);
                break;
            }
            if(bRet)//正常返回
                 nResult = MCU_IDEL;
            else//手工中止
                 nResult = SYS_ABORT;
        }
    }
    return nResult;
}
//协调模块处理线程
void CProcess3::Work()
{
     /*
    ******************************************************
    ******************************************************
    测试用:算峰高峰面积代码,可能还要用2014.4.17
    m_Qtime =  QDateTime::currentDateTime();
    for(int i=0;i<YQMAXCOUNT;i++)
    {
        if(m_pYQ[i])
        {
            m_pYQ[i]->m_RealInfo.TestTime = m_Qtime;
            m_pYQ[i]->m_RealInfo.TestTime = m_Qtime;
        }
    }
    InitBeginOfTest(0);
    SeachPeak();
    ******************************************************
    ******************************************************
     */
    static int nCheckTimes = 5;//控制系统状态检测
    static int nCommTimes = 0;//与协调模块通信故障次数
    static bool bYQTemptTag = false;//仪器温度记录日志
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    static int16_t nDetecBusy = 0;//协调模块（监测模块）是否空闲
    int16_t wModbus40051 = 0;
    int16_t wModels40052 = 0;
    if(ReadModbus(40051,17,bBuff))
    {
        wModbus40051 = MAKEWORD(bBuff[1],bBuff[0]); //40051	模块名称 0：无，1：继电器模块 2：加热温度控制模块，3：联动模块，
        wModels40052 = MAKEWORD(bBuff[3],bBuff[2]);	//40052 模块的工作状态

        if(wModels40052 != nDetecBusy)
        {
             nDetecBusy = wModels40052;
             //协调模块状态记录处理
             DetectError(nDetecBusy);
        }

        ProcessRealData(bBuff); //处理相关数据，比较各类温度等

        if(nDetecBusy == 1 || nDetecBusy == 2)//忙,2为读基线
        {
            if(!GetbIdle() && !m_bSubThread)
            {
                RunSubThread();//唯一地方启动
            }
        }
        if(nCommTimes > 0)
            nCommTimes = 0;

    }
    else
    {
        nCommTimes++;
        if(nCommTimes >= 10)
        {
            ProcessRealData(bBuff,true);
            nCommTimes = 0;
            DebugInfo("读协调模块温度数据时失败超过10次...");
        }
        cout << "Work() Read Tempture data timeout." << endl;
    }

    //仪器安全温度检测,测试的一级条件必须满足
    if(YQTemptCheck())
    {
        if(bYQTemptTag)//复位标识位
        {
            bYQTemptTag = false;
        }
        //二级条件判断:分空闲状态检测和忙检测两类
       if(GetbIdle())//空闲,检测测试条件
       {
           if(nCheckTimes >= 5)//5秒检测一次
           {
               nCheckTimes = 0;
               if(m_data.nStatus == 0 && ReadyGo())//准备好了,协调模块状态要求为空闲
               {
                   if(m_pOnYQReady)
                   {
                       m_pOnYQReady(m_userdata,true);
                   }
               }
               else//没准备好
               {
                   if(m_pOnYQReady)
                   {
                       m_pOnYQReady(m_userdata,false);
                   }
               }
           }
           else
           {
               nCheckTimes++;
           }
       }
       else//仪器测试,检测相关状态条件,出现问题时测试中止
       {
           if(m_nCmd != CMD_LINESCAN && m_nCmd != CMD_INIT)//读基线不判断
           {
               //1.测试过程,各路关键温度检测
               if(!SelftCheck())
               {
                   RecordLog(LOG_ALARM,"系统发送终止测试命令...");
                   SetThreadExit(true);
               }

               //2.测试过程,点火灭火保护机制
               if(m_nDetector == 0)
               {
                   ReFireProtect();
               }
           }
       }
    }
    else//温度过高
    {
        if(!bYQTemptTag)//防止重复添加日志
        {
            char cLogTxt[255];
            memset(cLogTxt,0,255);
            sprintf(cLogTxt,"仪器内部温度达到%.1f℃,超过限值%.1f℃,终止流程,停止加热",m_data.fYqTemp,m_stuInitTempt.YQTemp_Safe);
            RecordLog(LOG_FAULT,cLogTxt);
            if(m_pOnYQReady)
            {
                m_pOnYQReady(m_userdata,false);
            }
            bYQTemptTag = true;
        }
        if(!GetbIdle())
        {
            SetThreadExit(true);
        }
        CloseAllTempt();//停止所有温度
        RecordLog(LOG_ALARM,"仪器内部温度过高,停止所有温度");

        //FID检测器
        if(m_nDetector == 0)
        {
            CtrlFire(0,false);
            RecordLog(LOG_ALARM,"仪器内部温度过高,关阀熄火");
        }
    }
    //刷新耗材
    RefreshConsumeData();
    return;
}
void CProcess3::ReFireProtect()
{
    if(m_data.nFireTag == 0)//灭火状态
    {
        //流程:
        //灭火->熄火->关阀->等30s->开阀->等30s->点火->等60s->点火->等60s->点火->失败
        if(m_nFireWaitTag == 0)//突然熄火了
        {
            RecordLog(LOG_WARNING,"检测到FID熄火状态,关闭氢气和空气阀");
            CtrlFire(0,false);//m_nFireWaitTag = 2; 关阀标识
        }else if(m_nFireWaitTag == 1)//点火标识
        {
            int spac = m_qtFireTime.elapsed();
            if(spac/1000 >= 60)//超过60秒
            {
                if(m_nFireTimes < 3)//如果小于3次,则继续点火
                {
                     RecordLog(LOG_WARNING,"点火失败,再次打开氢气和空气阀,准备点火");
                     CtrlFire(0,false);//m_nFireWaitTag = 2;//关阀标识
                }
                else
                {
                     CtrlFire(0,false);//m_nFireWaitTag = 2; 关阀标识
                     RecordLog(LOG_FAULT,"点火失败,关闭氢气和空气阀");
                     if(m_pOnAlarm)
                     {
                         m_pOnAlarm(m_userdata,"点火失败,关闭氢气和空气阀",LOG_WARNING);
                     }
                }
            }
        }else if(m_nFireWaitTag == 2)//关阀状态
        {
            int spac = m_qtFireTime.elapsed();
            if(spac/1000 >= 30)//超过30秒,开阀
            {
                if(m_nFireTimes < 3)
                {
                    RecordLog(LOG_WARNING,"打开氢气和空气阀,准备点火");
                    CtrlFire(0,true);//m_nFireWaitTag = 3;//开阀标识
                }
            }
        }else if(m_nFireWaitTag == 3)//开阀标识
        {
            int spac = m_qtFireTime.elapsed();
            if(spac/1000 >= 15)//超过15秒
            {
                if(m_nFireTimes < 3)
                {
                    RecordLog(LOG_WARNING,"尝试点火...");
                    CtrlFire(1,true);// m_nFireWaitTag = 1;//点火标识
                }
            }
        }
    }
    else
    {
        m_nFireTimes = 0;
        m_nFireWaitTag = 0;//点燃状态
    }
    return;
}

void CProcess3::CtrlFire(int nKind,bool bOnOff)
{
    if(nKind == 0)
    {
        if(((CDriver3*)pDriver1)->IOCtrl_LD(m_stuInitTempt.FireAddr,m_stuInitTempt.FireDCF1,bOnOff))
        {
            if(((CDriver3*)pDriver1)->IOCtrl_LD(m_stuInitTempt.FireAddr,m_stuInitTempt.FireDCF2,bOnOff))
            {
                m_qtFireTime.start();
                if(bOnOff)
                {
                    m_nFireWaitTag = 3;//开阀标识
                }
                else
                {
                    m_nFireWaitTag = 2;//关阀标识
                }
            }
        }
    }
    else
    {
        ((CDriver3*)pDriver1)->Fire_159(m_pro.ModelAddress);
         m_qtFireTime.start();
         m_nFireWaitTag = 1;//点火标识
         m_nFireTimes++;
    }
    return;
}

void CProcess3::ProcessRealData(unsigned char *bBuff,bool bFault)
{
    if(bFault)
    {
        m_data.nStatus = -1;
        m_data.fPjjTemp = 0.0;//捕集井温度值(扩大了100倍)
        m_data.fMxzTemp = 0.0;//毛细柱温度值(扩大了100倍)
        m_data.fBoxTemp = 0.0;//箱体温度值(扩大了10倍)
        m_data.fFidTemp = 0.0;//FID检测器温度值(扩大了100倍)
        m_data.fJypTemp = 0.0;//进样瓶温度
        m_data.fYqTemp = 0.0;//仪器温度
        m_data.fBak1Temp = 0.0;//备用1
        m_data.fBak2Temp = 0.0;//备用2
        m_data.TemptModel = 0;
        m_data.TemptModel_2 = 0;
        m_data.fnitrpress = 0.0;
        m_data.fairpress = 0.0;
         m_data.fCSPress = 0;
        m_data.fFidTemp = 0;
    }
    else
    {
        m_data.nStatus = MAKEWORD(bBuff[3],bBuff[2]);
        m_data.fPjjTemp = MAKEWORD(bBuff[7],bBuff[6])/10.0;//捕集井温度值(扩大了100倍)
        m_data.fMxzTemp = MAKEWORD(bBuff[9],bBuff[8])/10.0;//毛细柱温度值(扩大了100倍)
        m_data.fBoxTemp = MAKEWORD(bBuff[11],bBuff[10])/10.0;//箱体温度值(扩大了10倍)
        m_data.fFidTemp = MAKEWORD(bBuff[13],bBuff[12])/10.0;//FID检测器温度值(扩大了100倍)
        m_data.TemptModel = bBuff[15];//温度模块状态

        m_data.fJypTemp = MAKEWORD(bBuff[17],bBuff[16])/10.0;//进样瓶温度(扩大了100倍)
        m_data.fYqTemp = MAKEWORD(bBuff[19],bBuff[18])/10.0;//仪器温度(扩大了100倍)
        m_data.fBak1Temp = MAKEWORD(bBuff[21],bBuff[20])/10.0;//备用1(扩大了100倍)
        m_data.fBak2Temp = MAKEWORD(bBuff[23],bBuff[22])/10.0;//备用2(扩大了100倍)
        m_data.TemptModel_2 = bBuff[25];//温度模块2状态

        m_data.nFireTag =  MAKEWORD(bBuff[27],bBuff[26]);//点火的信息 0未点火 1已点火

        //(LCmax-LCmin)*(AD-ADmin)/(ADmax-ADmin)+LCmin,最大AD=65535
        int nValue = 0;
        nValue = MAKEWORD(bBuff[29],bBuff[28]);
        m_data.fnitrpress = (m_fNitMax-m_fNitMin)*(nValue)/65535;

        nValue = MAKEWORD(bBuff[31],bBuff[30]);
        m_data.fairpress = (m_fAirMax-m_fAirMin)*(nValue)/65535;

        nValue = MAKEWORD(bBuff[33],bBuff[32]);
        m_data.fCSPress = (m_fAirMax-m_fAirMin)*(nValue)/65535;

    }
    if(m_pOnRealData)
    {
        m_pOnRealData(m_userdata,&m_data);
    }
}
//读配置文件
bool CProcess3::ReadIniFile(const char czTeam[], int &nValue)
{
    nValue = 0;
    char czValue[64];memset(czValue,0,64);
    if(GetAfxConfig()->GetIni("VOC_SET",czTeam,czValue))
    {
        nValue = atoi(czValue);
        return true;
    }
    return false;
}
bool CProcess3::ReadIniFile(const char czTeam[], float &fValue)
{
    fValue = 0.0;
    char czValue[64];memset(czValue,0,64);
    if(GetAfxConfig()->GetIni("VOC_SET",czTeam,czValue))
    {
        fValue = atof(czValue);
        return true;
    }
    return false;
}
void CProcess3::ReadData()
{
    DebugInfo("===============开始读数据================",0);
    m_bCalcluteTag = false;
    bool bret = false;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    unsigned long nSumCount = 0;
    u_int16_t nID = 0;
    unsigned int nCurrID = 0;
    unsigned int nLastID = 0;
    unsigned int nVolValue = 0;
    unsigned int nTempValue = 0;
    unsigned int nRemainTime = 0;//起始时间,原有的保存不变
    unsigned int nRealRemainTime = 0;//新增加，用于输出曲线和保存数据
    unsigned int nIncrement = m_nGainDataRate;//豪秒
    double fVolValue = 0.0;
    double fTemptValue = 0.0;
    double fRtSecond = 0.0;
    int nStartReg = 40100;
    int nRegs = 10;
    if(m_nGainDataRate > 0)//每秒钟采集数据个数,取整数
        nRegs = (int)(1000/m_nGainDataRate);


    bool bNoiceOnOff = false;//是否启用去噪声点功能
    int nNoiceValue = 0;//噪声点阀值
    ReadIniFile("DataSrc_Noise",nNoiceValue);
    if(nNoiceValue == 1)
    {
        bNoiceOnOff = true;
        m_deTempRT.clear();
        m_deTempVol.clear();
        m_deTempTp.clear();
    }
    else
        bNoiceOnOff = false;
    ReadIniFile("DataSrc_NoiseValue",nNoiceValue);
    m_nNoiceValue = nNoiceValue;
    if(m_nNoiceValue <= 100)
        m_nNoiceValue = 35000;

    int nEnter = 0;
    int nTimeout = 0;
    QTime Time;
    Time.start();
    int spac = 0;
    if(m_pOnCurveDot)//如果是读基线，则要有开始
    {
        if(m_bReadBaseLine)
            m_pOnCurveDot(m_userdata,0,0,0,-1);
        else
            m_pOnCurveDot(m_userdata,0,0,0,0);
    }
    sleep(1);//留给下位机做准备
    while(m_bSubThread)
    {
        usleep(600000);//留给下位机做准备
        if(ReadModbus(nStartReg,nRegs*4,bBuff))
        {
            ////////////////////////////////////////////////////
            //调试用
//            char debug[255];
//            char tmp[64];
//            memset(debug,0,255);
//            memset(tmp,0,64);
//            strcat(debug,"[");
//            for(int j = 0;j < nRegs*4*2;j++)
//            {
//                sprintf(tmp,"%02x ",bBuff[j]);
//                strcat(debug,tmp);
//            }
//            strcat(debug,"]");
//            DebugInfo(debug);
            ////////////////////////////////////////////////////

            int nPos= 0;
            for(int i = 0;i < nRegs;i++)
            {
                nPos = i*4*2;
                nID = MAKEWORD(bBuff[nPos+1],bBuff[nPos]);
                if(nID == 0xEEEE)//换行
                {
                    nStartReg = 40100;
                    nEnter++;
                    cout << "*************************Enter(EEEE),Reading data id = " << nID << endl;
                    break;
                }else if(nID == 0xFFFF)//结束
                {
                    if(bNoiceOnOff)//是否启用去噪声点,把最后一个点补上
                    {
                        GetPotData(nRealRemainTime,fVolValue,fTemptValue,true);
                        HaveNewData(nCurrID,nRealRemainTime,fVolValue,fTemptValue);
                    }
                    bret = true;
                    cout << "*************************Over(FFFF),Reading data id = " << nID << endl;
                    break;
                }else
                {
                    nCurrID = nID+ MAX_DATABUFFER*nEnter;
                }
                if(nLastID+1 == nCurrID)//编号是否连续
                {
                    nVolValue = MAKEWORD(bBuff[nPos+3],bBuff[nPos+2])<<16;
                    nVolValue += MAKEWORD(bBuff[nPos+5],bBuff[nPos+4]);
                    nTempValue = MAKEWORD(bBuff[nPos+7],bBuff[nPos+6]);
                    nVolValue = nVolValue>>2;
                    fVolValue = (double)nVolValue;

                    fVolValue = fVolValue- 2097152;//2^21   电压值
                    fTemptValue = nTempValue/10.0;//温度:扩大10倍后的值

                    if(bNoiceOnOff)//是否启用去噪声点
                    {
                        PutPotData(nRemainTime,fVolValue,fTemptValue);
                        if(GetPotData(nRealRemainTime,fVolValue,fTemptValue))
                        {
                            HaveNewData(nCurrID,nRealRemainTime,fVolValue,fTemptValue);
                        }
                    }
                    else
                    {
                        HaveNewData(nCurrID,nRemainTime,fVolValue,fTemptValue);
                    }

                    nLastID = nCurrID;
                    nRemainTime +=nIncrement;
                    nSumCount++;
                    nStartReg += 4;

                    cout << "Reading data id = " << nCurrID << endl;
                }
                else
                {
                    char str[64];
                    memset(str,0,64);
                    sprintf(str,"error id = %d,last id = %d",nCurrID,nLastID);
                    DebugInfo(str);
                    cout << "error id = " << nCurrID << endl;
                    break;
                }
            }
            if(bret)//是否结束
            {
                break;
            }
            nTimeout = 0;
        }
        else
        {
//            nTimeout++;
//            if(nTimeout >= 10)
//            {
//                cout << "Error:Read data timeout,abort it...." << endl;
//                DebugInfo("读取AD数据时超时超过10次，中止读数据!");
//                break;
//            }
            DebugInfo("***************读取AD数据超时***************",0);
            cout << "Warning:Read voc data timeout." <<nTimeout<< endl;
        }
        //超时处理
        spac = Time.elapsed();
        if((spac/1000 >= m_nGainDataTime * 60+600) && !m_bReadBaseLine)//如果超过读数据时间的10分钟，且不能为读基线
        {
            DebugInfo("读取AD数据时超时，中止读数据!");
            break;
        }
    }
    if(m_pOnCurveDot)//结束
    {
        m_pOnCurveDot(m_userdata,0,0,0,-2);
    }
    if(bret)
    {
        m_bCalcluteTag = true;
        Calculate();//计算结果
    }
    ExitSubThread();//自已结束
}

void CProcess3::PutPotData(unsigned int nRemainTime, double fVolValue, double fTemptValue)
{
    m_deTempRT.push_back(nRemainTime);
    m_deTempVol.push_back(fVolValue);
    m_deTempTp.push_back(fTemptValue);
}

bool CProcess3::GetPotData(unsigned int &nRemainTime, double &fVolValue, double &fTemptValue,bool bFinish)
{
    static double fFirstValue = 0.0;
    static double fSecondValue = 0.0;
    bool bret = true;
    if(bFinish)//返回最后1个点
    {
        nRemainTime = m_deTempRT.back();
        fVolValue = m_deTempVol.back();
        fTemptValue = m_deTempTp.back();
    }
    else  if(m_deTempRT.size() == 1)//延时1个数
    {
       bret = false;

       fFirstValue = m_deTempVol.front();//取第1个点
    }else if(m_deTempRT.size() == 2)//返回第一个数
    {
        nRemainTime = m_deTempRT.front();
        fVolValue = m_deTempVol.front();
        fTemptValue = m_deTempTp.front();

        fSecondValue = m_deTempVol.back();//取第2个点
    }else//应该对第3个点进行去噪声，然后返回第2个点的数据
    {
        //移走前2个上轮取走的点
        m_deTempVol.pop_front();
        m_deTempRT.pop_front();
        m_deTempTp.pop_front();

        double fDiff = fabs((fabs(fFirstValue) + fabs(fSecondValue))/2.0);//第1点+第2点求平均
        if(fabs(m_deTempVol.back() - fDiff) > m_nNoiceValue)//第3点-平均>噪声
        {
            fVolValue = fDiff;//前两点的平均
            m_deTempVol.pop_back();//把处理过的数据放到里面
            m_deTempVol.push_back(fVolValue);
            cout << "************first[" << fFirstValue << "]" <<"sencond["<<m_deTempVol.front()<<"] fDiff["<<fDiff<<"]"<<"thrid[" <<m_deTempVol.back()<<"]"<<endl;
            DebugInfo("当前点遇到异常值,修正为=前2个点的平均值");
        }
        else//把最近的正确数据更新
        {
            fFirstValue = fSecondValue;
            fSecondValue = m_deTempVol.back();
        }

        fVolValue = m_deTempVol.front();//取第二个点
        nRemainTime = m_deTempRT.front();//取第二个点
        fTemptValue = m_deTempTp.front();//取第二个点

    }
    return bret;
}
//bool CProcess3::GetPotData(unsigned int &nRemainTime, double &fVolValue, double &fTemptValue,bool bFinish)
//{
//    bool bret = true;
//    if(bFinish)//返回最后1个点
//    {
//        nRemainTime = m_deTempRT.back();
//        fVolValue = m_deTempVol.back();
//        fTemptValue = m_deTempTp.back();
//    }
//    else  if(m_deTempRT.size() == 1)//延时1个数
//    {
//       bret = false;
//    }else if(m_deTempRT.size() == 2)//返回第一个数
//    {
//        nRemainTime = m_deTempRT.front();
//        fVolValue = m_deTempVol.front();
//        fTemptValue = m_deTempTp.front();
//    }else//应该对第3个点进行去噪声，然后返回第2个点的数据
//    {
//        float fTempVol_1 = m_deTempVol.front();//第1点
//        m_deTempVol.pop_front();
//        m_deTempRT.pop_front();
//        m_deTempTp.pop_front();

//        double fDiff = fabs((fabs(fTempVol_1) + fabs(m_deTempVol.front()))/2.0);//第1点+第2点求平均
//        if(fabs(m_deTempVol.back() - fDiff) > m_nNoiceValue)//第3点-平均>噪声
//        {
//            fVolValue = fDiff;//前两点的平均
//            m_deTempVol.pop_back();//把处理过的数据放到里面
//            m_deTempVol.push_back(fVolValue);
//            cout << "************first[" << fTempVol_1 << "]" <<"sencond["<<m_deTempVol.front()<<"] fDiff["<<fDiff<<"]"<<"thrid[" <<m_deTempVol.back()<<"]"<<endl;
//            DebugInfo("当前点遇到异常值,修正为=前2个点的平均值");
//        }

//        fVolValue = m_deTempVol.front();//取第二个点
//        nRemainTime = m_deTempRT.front();//取第二个点
//        fTemptValue = m_deTempTp.front();//取第二个点

//    }
//    return bret;
//}
void CProcess3::HaveNewData(unsigned int nId,unsigned int nRemainTime, double fVolValue, double fTemptValue)
{
    double fRtSecond = (double)(nRemainTime/(60*1000.0));
    m_vtVol.push_back(fVolValue);//AD值
    m_vtTp.push_back(fTemptValue);
    m_vtRT.push_back(fRtSecond);
    m_vtNitr.push_back(m_data.fCSPress);
    if(m_pOnCurveDot)
    {
        cout << "send sianal to curve id = [" << nId << "]" <<endl;
        m_pOnCurveDot(m_userdata,(double)nRemainTime,fVolValue,fTemptValue,nId);
        cout << "send sianal to curve id = [" << nId << "] finish" <<endl;
    }
}
void CProcess3::SaveSrcDataToTxt()
{
    char czChildPath[256];memset(czChildPath,0,256);
    char czDataName[256];memset(czDataName,0,256);
    char czDataFile[256];memset(czDataFile,0,256);
    sprintf(czChildPath,"%s/%d/data",DB_ROOT_PATH,this->GetProcessType());
    mkdir_r(czChildPath);
    sprintf(czDataName,"%04d%02d%02d-%02d%02d.txt",m_Qtime.date().year(),m_Qtime.date().month(),m_Qtime.date().day(),m_Qtime.time().hour(),m_Qtime.time().minute());
    sprintf(czDataFile,"%s/%s",czChildPath,czDataName);

    GetAfxConfig()->SetIni("VOC_SET","LastTestSrcData",czDataName);

    QString qsTxtFileName = QString(czDataFile);
    QFile file(qsTxtFileName);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
       QTextStream stream(&file);
       stream << "RTime"<<"\t"<<"Vol"<<"\t"<<"Temp"<<"\t"<<"Nitr"<< "\r\n";
        for ( int i = 0; i < m_vtRT.size(); i++ )
            stream << m_vtRT[i]<<"\t"<<m_vtVol[i]<<"\t"<<m_vtTp[i]<<"\t"<<m_vtNitr[i]<< "\r\n";

        file.close();
    }
}

int CProcess3::ScanFlowFile(const char *filename)
{
    char czItem[64];memset(czItem,0,64);
    char czTmp[64];
    int nSubFlow = 0;
    while (!m_quMyPro.empty())
    {
        m_quMyPro.pop();
    }
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

        m_quMyPro.push(m_pro);
    }
    return m_quMyPro.size();
}

void CProcess3::RunSubThread()
{
    if(!m_bSubThread)
    {
        m_bSubThread = true;
        pthread_create(&m_subthread, NULL, ReadDataThread, this);
    }
}
void CProcess3::ExitSubThread()
{
    if(m_bSubThread)
    {
        m_bSubThread = false;
        pthread_join(m_subthread,0);
        m_subthread = 0;
    }
}

void CProcess3::DoThread()
{
     Work();
     /*
     ******************************************************
     ******************************************************
     测试用:动态曲线，后续可能还要用2014.4.17
     sleep(120);
     if(iss == 0)
          pThis->m_pOnCurveDot(pThis->m_userdata,iss,fabs(10000000*sin(iss)),0,iss);
     else
          pThis->m_pOnCurveDot(pThis->m_userdata,iss,fabs(10000000*sin(iss)),0,iss-1);

     iss++;
     if(iss == 20)
     {
         iss = -1;
         pThis->m_pOnCurveDot(pThis->m_userdata,iss,fabs(10000000*sin(iss)),0,iss);
         iss++;
         sleep(5);
     }
     ******************************************************
     ******************************************************
     */
}

void* CProcess3::ReadDataThread(void* pParam)
{
    CProcess3* pThis = (CProcess3*)pParam;
    while(pThis->m_bSubThread)
    {
        pThis->ReadData();
    }
    return NULL;
}
int CProcess3::SysWaitTemplate(const int nKind,const float fDescValue)
{
    int nMinute = 20;
    if(PJJJX_TEMP == nKind || PJJHK_TEMP == nKind)
         nMinute = 3;
    else
         nMinute = 20;

    int nResult = MCU_BUSY;
    QTime Time;
    Time.start();
    int spac = 0;
    while(true)
    {
        //手动中止测试
        if(m_bExitFlag)
        {
            nResult = SYS_ABORT;
            break;
        }
        //超时处理
        spac = Time.elapsed();
        if(spac/1000 >= nMinute*60)//20分钟
        {
            nResult = SYS_TIMEOUT;
            break;
        }
        //温度
        if(WaitForTemplate(nKind,fDescValue))
        {
            nResult = MCU_IDEL;
            break;
        }
        usleep(100000);
    }
    return nResult;
}
bool CProcess3::WaitForTemplate(const int nKind,const float fDescValue)
{
    bool bRet = false;
    switch(nKind)
    {
    case PJJINIT_TEMP:
        if(m_data.fPjjTemp <= fDescValue)
            bRet = true;
        break;
    case PJJJX_TEMP:
    case PJJHK_TEMP:
        if(m_data.fPjjTemp >= fDescValue-1)
            bRet = true;
        break;
    case FIDINIT_TEMP://正负1
        if(fabs(fDescValue-m_data.fFidTemp) <= 2)
            bRet = true;
        break;
    case BOXINIT_TEMP://正负1
        if(fabs(fDescValue-m_data.fBoxTemp) <= 2)
            bRet = true;
        break;
    case MXZINIT_TEMP://正负0.5
        if(fabs(fDescValue-m_data.fMxzTemp) <= 1)
            bRet = true;
        break;
    case MXZ_UPLIMIT://报警用
        if(m_data.fMxzTemp >= fDescValue)
            bRet = true;
        break;
    case YQ_TEMP:
        if(m_data.fYqTemp >= fDescValue)
            bRet = true;
        break;
    default:
        bRet = false;
        break;
    }
    return bRet;
}
bool CProcess3::WaitForTime(int nTime)
{
    bool bRet = false;
    QTime Time;
    Time.start();
    int spac = 0;
    while(true)
    {
        if(m_bExitFlag)
        {
            break;//手动中止测试
        }
        spac = Time.elapsed();
        if(spac/1000 >= nTime)
        {
            bRet = true;
            break;
        }
        usleep(100000);
    }
    return bRet;
}


bool CProcess3::ReadModbus(int nReg, int nCount,unsigned char* czData)
{
    if(!pDriver1) return false;
    return ((CDriver3*)pDriver1)->ReadComData(m_DetecAddress,nReg,nCount,300,czData);
}

void CProcess3::StopDealEvent(bool bTag)
{
    if(!pDriver1) return;
    if(bTag)
    {
        pDriver->AllModuleReset();
        DebugInfo("复位总线所有模块");
    }
    ((CDriver3*)(pDriver1))->StartTemptSigle_151(m_DetecAddress,PJJINIT_TEMP,0);
    if(m_bSubThread)
    {
        //要求发停止升温
         ((CDriver3*)(pDriver1))->StartLineTempt_152(m_DetecAddress,MXZINIT_TEMP,0);
         usleep(100000);
         if(bTag)
         {
            pDriver1->SingleModuleReset(m_DetecAddress);
            DebugInfo("复位协调模块");
         }
         if(m_bSubThread)
             ((CDriver3*)pDriver1)->StartGetData_160(m_DetecAddress,0);
         usleep(100000);
         ExitSubThread();//中止
    }

}
bool CProcess3::SeachPeak()
{
    bool bret = false;
    if(m_pCalc)
    {
        float BaseLineK,fModifyB,MinPW,MaxPW,Minimum,MinArea;
        ReadIniFile("Search_Baseline",BaseLineK);
        ReadIniFile("Search_ModifyB",fModifyB);

        CDBBusiness*  pDb = new CDBBusiness("yqdata","yq.db");
        MaxPW = pDb->GetIntegral_MaxPW();
        Minimum = pDb->GetIntegral_Minimum();
        MinArea = pDb->GetIntegral_MinArea();
        MinPW = pDb->GetIntegral_MinPW();
        delete pDb;

        m_pCalc->SetData(m_vtVol,m_vtRT);
        m_pCalc->SetSeachParam(BaseLineK,fModifyB,MinPW,MaxPW,Minimum,MinArea);

        if(m_pCalc->Calculate())
        {
            //保存峰组数据
//            for(int i = 0;i < m_pCalc->m_vtnPeark.size();i++)
//            {
//                m_pDB->SaveVocPeaks(m_pCalc->m_vtnPeark[i]);
//            }
            bret = true;
        }
    }

//    ******************************************************
//    ******************************************************
//    测试用:算峰高峰面积代码,可能还要用2014.4.17
//    if(m_pCalc)
//    {
//        sqlite_data datalist;
//        if(m_pDB->GetVocSrcData(datalist))
//        {
//            m_vtTp.clear();
//            m_vtVol.clear();
//            m_pOnCurveDot(m_userdata,0,0,0,0);
//            for(int i = 1; i <= datalist.RowCount(); i++ )
//            {
//                m_vtTp.push_back(QString(datalist.GetData(i,1)).toDouble());
//                if(QString(datalist.GetData(i,2)).toFloat() < 0)
//                    m_vtVol.push_back(0);
//                else
//                    m_vtVol.push_back(QString(datalist.GetData(i,2)).toDouble());
//                cout<<i<<"    RT="<<m_vtTp[i-1] << "   Vol = "<<m_vtVol[i-1]<<endl;
//                //                m_pOnCurveDot(m_userdata,(double)m_RT[i-1],(double)m_Vol[i-1],0,i);

//            }
//            m_pCalc->SetData(m_vtVol,m_vtTp);
//            m_pCalc->SetSeachParam(0,2,0,60,600,0);
//            if(m_pCalc->Calculate())
//            {
//                bret = true;
//                Calculate();
//            }
//        }
//    }
//    ******************************************************
//    ******************************************************
    return bret;
}

void CProcess3::Calculate()
{
    char cLogTxt[255];
    //1.保存源始数据
    SaveSrcDataToTxt();
    //2.寻峰及保存峰
    SeachPeak();

    //读取最新的内标数据,用于计算校准
    float fDiff = 0.0;
    bool bAdjust = false;

    int nTemp = 0;
    ReadIniFile("AdjustOnOff",nTemp);
    bAdjust = (bool)nTemp;
    if(bAdjust)
    {
        float RemainTime,RTime,Conc,RConc;
        if(m_pDB->GetLastVocinternal(RemainTime,RTime,Conc,RConc))
        {
            fDiff = RemainTime - RTime;//两者保留时间表之差

            ReadIniFile("AdjustDiff",nTemp);
            if(fabs(fDiff) > nTemp/60.0)
            {
                DebugInfo("内标保留时间偏大过大,请检查");
                RecordLog(LOG_WARNING,"内标保留时间偏大,请重新校准仪器");
                if(m_pOnAlarm)
                {
                    m_pOnAlarm(m_userdata,"内标保留时间偏大,请重新校准仪器",LOG_WARNING);
                }
                bAdjust = false;
            }
            else
            {
                memset(cLogTxt,0,255);
                sprintf(cLogTxt,"当前校准保存时间为 %d 分钟",fDiff);
                RecordLog(LOG_ALARM,cLogTxt);
            }
        }
    }


    CYQ* pYQ = NULL;
    //3.1 根据峰计算测试结果,已知物质
    for(int i = 0;i < YQMAXCOUNT;i++)
    {
        pYQ = m_pYQ[i];
        if(pYQ && pYQ->GetProcessType() == 3)
        {
            if(strcmp(pYQ->m_sYQ.czID,"UN1") && strcmp(pYQ->m_sYQ.czID,"UN2"))//不是未知物
            {
                struPeak peak;
                int nId = -1;
                if(pYQ->m_CurveInfo.m_VocCurve.Kind == 1)//内标不校内标
                {
                    nId = SeachIndexFromPeaks(pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_CurveInfo.m_VocCurve.WinFactor);
                }
                else
                {
                    if(bAdjust)//校准
                        nId = SeachIndexFromPeaks(pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_CurveInfo.m_VocCurve.WinFactor,fDiff);
                    else
                        nId = SeachIndexFromPeaks(pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_CurveInfo.m_VocCurve.WinFactor);
                }
                if(nId >= 0)//找到物质峰
                {
                    m_pCalc->m_vtnPeark[nId].bCalcTag = true;
                    peak = m_pCalc->m_vtnPeark[nId];
                    float fValue = 0;
                    fValue = pYQ->m_CurveInfo.K * peak.fArea + pYQ->m_CurveInfo.B;
                    if(fValue < 0)
                        fValue = 0;
                    if(pYQ->m_CurveInfo.SampleAdjust != 0)
                        fValue *= pYQ->m_CurveInfo.SampleAdjust;

                    if (CMD_TEST == m_nCmd)
                        pYQ->m_RealInfo.fResult = fValue;
                    else if (CMD_SAMPLETEST == m_nCmd || CMD_SAMPLEADJUST == m_nCmd)
                        pYQ->m_RealInfo.fCheckResult = fValue;
                    else
                        pYQ->m_RealInfo.fResult = fValue;

                    pYQ->m_RealInfo.SampleAdjust =  pYQ->m_CurveInfo.SampleAdjust;
                     if(pYQ->m_CurveInfo.m_VocCurve.Kind == 1)//内标不校内标
                     {
                         pYQ->m_RealInfo.m_vocDataInfo.RemainTime = peak.fRT;
                     }
                     else
                     {
                         if(bAdjust)//校准
                             pYQ->m_RealInfo.m_vocDataInfo.RemainTime = peak.fRT + fDiff;//校准保留时间之后的
                         else
                              pYQ->m_RealInfo.m_vocDataInfo.RemainTime = peak.fRT;
                     }
                    pYQ->m_RealInfo.m_vocDataInfo.PeakHight = peak.fHight;
                    pYQ->m_RealInfo.m_vocDataInfo.Area = peak.fArea;
                    pYQ->m_RealInfo.m_vocDataInfo.K = pYQ->m_CurveInfo.K;
                    pYQ->m_RealInfo.m_vocDataInfo.R = pYQ->m_CurveInfo.R;
                    pYQ->m_RealInfo.m_vocDataInfo.B = pYQ->m_CurveInfo.B;
                    pYQ->m_RealInfo.m_vocDataInfo.m_peak = peak;

                    memset(cLogTxt,0,255);
                    sprintf(cLogTxt,"[%s] %s",pYQ->m_sYQ.czName," 找到保留时间对应的峰");
                    DebugInfo(cLogTxt);
                }
                else
                {
                    if(CMD_TEST == m_nCmd || CMD_SAMPLETEST == m_nCmd)
                    {
                        pYQ->m_RealInfo.m_vocDataInfo.m_peak = peak;
                        memset(pYQ->m_RealInfo.cDataID,0,16);
                        strcpy(pYQ->m_RealInfo.cDataID,"np");

                        memset(cLogTxt,0,255);
                        sprintf(cLogTxt,"[%s] %s",pYQ->m_sYQ.czName," 未找到保留时间对应的峰");
                        RecordLog(LOG_ALARM,cLogTxt);
                    }else if(CMD_SAMPLEADJUST == m_nCmd && pYQ->m_CurveInfo.m_VocCurve.Kind == 1)//内标
                    {
                        pYQ->m_RealInfo.m_vocDataInfo.m_peak = peak;
                        memset(pYQ->m_RealInfo.cDataID,0,16);
                        strcpy(pYQ->m_RealInfo.cDataID,"np");

                        memset(cLogTxt,0,255);
                        sprintf(cLogTxt,"[%s] %s",pYQ->m_sYQ.czName," 未找到保留时间对应的峰");
                        RecordLog(LOG_ALARM,cLogTxt);
                    }
                }
            }
            pYQ = NULL;
        }
    }
    //3.2 根据峰计算测试结果,未知物质
    for(int i = 0;i < YQMAXCOUNT;i++)
    {
        pYQ = m_pYQ[i];
        if(pYQ && pYQ->GetProcessType() == 3)
        {
            if(!(strcmp(pYQ->m_sYQ.czID,"UN1")) || !(strcmp(pYQ->m_sYQ.czID,"UN2")))//未知物
            {
                struPeak peak;
                int nId = -1;
                bool bFind = false;
                float fUnMaxArea = 0.0;
                ReadIniFile("Search_UnMaxArea",fUnMaxArea);
                for(int nIndex = 0;nIndex < (int)m_pCalc->m_vtnPeark.size();nIndex++)
                {
                    peak = m_pCalc->m_vtnPeark[nIndex];
                    if(!peak.bCalcTag)
                    {
                        if(peak.fArea >= fUnMaxArea)
                        {
                            //先判断是不是溶剂峰
                            int nIdId = -1;
                            if(bAdjust)//校准
                                nIdId = SeachIndexFromPeaks(pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_CurveInfo.m_VocCurve.WinFactor,fDiff);
                            else
                                nIdId = SeachIndexFromPeaks(pYQ->m_CurveInfo.m_VocCurve.RemainTime,pYQ->m_CurveInfo.m_VocCurve.WinFactor);

                            if(nIdId < 0)//不是溶剂峰
                            {
                                fUnMaxArea = peak.fArea;
                                nId = nIndex;
                                bFind = true;
                            }
                        }
                    }
                }
                if(bFind)//发现未知物质峰
                {
                    peak = m_pCalc->m_vtnPeark[nId];
                    if (CMD_TEST == m_nCmd)
                        pYQ->m_RealInfo.fResult = 1000;
                    else if (CMD_SAMPLETEST == m_nCmd || CMD_SAMPLEADJUST == m_nCmd)
                        pYQ->m_RealInfo.fCheckResult = 1000;
                    else
                        pYQ->m_RealInfo.fResult = 1000;

                    memset(pYQ->m_RealInfo.cDataID,0,16);
                    strcpy(pYQ->m_RealInfo.cDataID,"yp");

                    pYQ->m_RealInfo.SampleAdjust =  pYQ->m_CurveInfo.SampleAdjust;

                    pYQ->m_RealInfo.m_vocDataInfo.RemainTime = peak.fRT;
                    pYQ->m_RealInfo.m_vocDataInfo.PeakHight = peak.fHight;
                    pYQ->m_RealInfo.m_vocDataInfo.Area = peak.fArea;
                    pYQ->m_RealInfo.m_vocDataInfo.K = 0;
                    pYQ->m_RealInfo.m_vocDataInfo.R = 0;
                    pYQ->m_RealInfo.m_vocDataInfo.B = 0;
                    pYQ->m_RealInfo.m_vocDataInfo.m_peak = peak;

                    memset(cLogTxt,0,255);
                    sprintf(cLogTxt,"[%s]发现未知物质,保留时间为%.4f",pYQ->m_sYQ.czName,peak.fRT);
                    DebugInfo(cLogTxt);
                    RecordLog(LOG_ALARM,cLogTxt);
                }
                else
                {
                     struPeak peak2;
                     pYQ->m_RealInfo.m_vocDataInfo.m_peak = peak2;
                     if (CMD_TEST == m_nCmd)
                         pYQ->m_RealInfo.fResult = 0;
                     else if (CMD_SAMPLETEST == m_nCmd || CMD_SAMPLEADJUST == m_nCmd)
                         pYQ->m_RealInfo.fCheckResult = 0;
                     else
                         pYQ->m_RealInfo.fResult = 0;
                     memset(pYQ->m_RealInfo.cDataID,0,16);
                     strcpy(pYQ->m_RealInfo.cDataID,"np");
                }
                break;
            }
            pYQ = NULL;
        }
    }
    //4.保存数据
    SaveResultToDatabase();
}
void CProcess3::SaveResultToDatabase(int nCodeID)
{
    if(CMD_TEST == m_nCmd || CMD_SAMPLETEST == m_nCmd)
    {
        if(m_pOnTestFinished)
        {
            for(int i=0;i<YQMAXCOUNT;i++)
            {
                if(m_pYQ[i])
                {
                    SYQPARAM sYQ;
                    m_pYQ[i]->GetYQParam(sYQ);
                    //保存测试数据
                    m_pOnTestFinished(m_userdata,8051,sYQ.czID);
                }
            }
        }
    }else if(CMD_SAMPLEADJUST == m_nCmd)
    {
        if(m_pOnTestFinished)
        {
            for(int i=0;i<YQMAXCOUNT;i++)
            {
                if(m_pYQ[i] && m_pYQ[i]->m_CurveInfo.m_VocCurve.Kind == 1)//是内标的
                {
                    SYQPARAM sYQ;
                    m_pYQ[i]->GetYQParam(sYQ);
                    //保存测试数据
                    m_pOnTestFinished(m_userdata,CMD_SAMPLEADJUST,sYQ.czID);
                }
            }
        }
    }
}
//查看相对窗宽范围内最接近保留时间的峰
int CProcess3::SeachIndexFromPeaks(double fRTime,float fWinFactor,float fAdjustDiff)
{
    double fMin = 100;
    double fTemp = 0;
    int nindex = -1;
    float fRT = 0;
    for(int i = 0;i < (int)m_pCalc->m_vtnPeark.size();i++)
    {
        fRT = m_pCalc->m_vtnPeark[i].fRT + fAdjustDiff;//校准后的保留时间
        //相对窗宽范围
        if((fRT < fRTime + fRTime*fWinFactor) && (fRT > fRTime - fRTime*fWinFactor))
        {
            //相对窗宽范围保留时间最接近的
            fTemp = fabs(fRTime - fRT);
            if(fTemp < fMin)
            {
                fMin = fTemp;
                nindex = i;
            }
        }
    }
    return nindex;
}


bool CProcess3::BaseInit_PJJ()
{
    if(!pDriver1) return false;
    int nValue = 0;
    //捕集井初始温度
    //ReadIniFile("PJJTemp_Init",nValue);
    nValue = m_stuInitTempt.PJJTemp_Init*10;
    if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_DetecAddress,PJJINIT_TEMP,nValue))
    {
        usleep(100000);
        if(((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,PJJINIT_TEMP,1))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
bool CProcess3::BaseInit_FID()
{
    if(!pDriver1) return false;
    int nValue = 0;
    //FID温度
    //ReadIniFile("FIDTemp_Init",nValue);
    nValue = m_stuInitTempt.FIDTemp_Init*10;
    if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_DetecAddress,FIDINIT_TEMP,nValue))
    {
        usleep(100000);
        if(((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,FIDINIT_TEMP,1))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
bool CProcess3::BaseInit_BOX()
{
    if(!pDriver1) return false;
    int nValue = 0;
    //设置箱体温度
    //ReadIniFile("BoxTemp_Init",nValue);
    nValue = m_stuInitTempt.BoxTemp_Init*10;
    if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_DetecAddress,BOXINIT_TEMP,nValue))
    {
        usleep(100000);
        if(((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,BOXINIT_TEMP,1))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
bool CProcess3::BaseInit_MXZ()
{
    if(!pDriver1) return false;
    int nValue = 0;
    //设置毛细柱初始温度
    //ReadIniFile("MxzTemp_Init",nValue);
    nValue = m_stuInitTempt.MxzTemp_Init*10;
    if(((CDriver3*)pDriver1)->SetTemptSingle_153(m_DetecAddress,MXZINIT_TEMP,nValue))
    {
        usleep(100000);
        if(((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,MXZINIT_TEMP,1))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
bool CProcess3::MXZJJ_SW()
{
    int nValue = 0;
    //设置线性升温阶梯
    int nTempt1,nTempt2,nTempt3,nTempt4,nTempt5;
    nTempt1=nTempt2=nTempt3=nTempt4=nTempt5=0;
    if(ReadIniFile("DescTemp_1",nValue))
    {
        nTempt1 = nValue*10;
    }
    if(ReadIniFile("DescTemp_2",nValue))
    {
        nTempt2 = nValue*10;
    }
    if(ReadIniFile("DescTemp_3",nValue))
    {
        nTempt3 = nValue*10;
    }
    if(ReadIniFile("DescTemp_4",nValue))
    {
        nTempt4 = nValue*10;
    }
    if(ReadIniFile("DescTemp_5",nValue))
    {
        nTempt5 = nValue*10;
    }
    int nIndex;
    for (nIndex = 0; nIndex < 3;nIndex++)
    {
        if(((CDriver3*)pDriver1)->SetLineTemptDesc_154(m_DetecAddress,MXZINIT_TEMP,nTempt1,nTempt2,nTempt3,nTempt4,nTempt5))
            break;
        else
            usleep(100000);
    }
    if(nIndex == 3)
    {
       return false;
    }
    //设置线性升温斜率
    int nK1,nK2,nK3,nK4,nK5;
    nK1=nK2=nK3=nK4=nK5=0;
    if(ReadIniFile("TempRate_1",nValue))
    {
        nK1 = nValue;
    }
    if(ReadIniFile("TempRate_2",nValue))
    {
        nK2 = nValue;
    }
    if(ReadIniFile("TempRate_3",nValue))
    {
        nK3 = nValue;
    }
    if(ReadIniFile("TempRate_4",nValue))
    {
        nK4 = nValue;
    }
    if(ReadIniFile("TempRate_5",nValue))
    {
        nK5 = nValue;
    }
    for (nIndex = 0; nIndex < 3;nIndex++)
    {
        if(((CDriver3*)pDriver1)->SetLineTemptK_155(m_DetecAddress,MXZINIT_TEMP,nK1,nK2,nK3,nK4,nK5))
            break;
        else
            usleep(100000);
    }
    if(nIndex == 3)
    {
        return false;
    }
    //设置保持时间
    int nRT1,nRT2,nRT3,nRT4,nRT5;
    nRT1=nRT2=nRT3=nRT4=nRT5=0;
    if(ReadIniFile("TempRTime_1",nValue))
    {
        nRT1 = nValue;
    }
    if(ReadIniFile("TempRTime_2",nValue))
    {
        nRT2 = nValue;
    }
    if(ReadIniFile("TempRTime_3",nValue))
    {
        nRT3 = nValue;
    }
    if(ReadIniFile("TempRTime_4",nValue))
    {
        nRT4 = nValue;
    }
    if(ReadIniFile("TempRTime_5",nValue))
    {
        nRT5 = nValue;
    }
    for (nIndex = 0; nIndex < 3;nIndex++)
    {
        if(((CDriver3*)pDriver1)->SetLineTemptRTime_156(m_DetecAddress,MXZINIT_TEMP,nRT1,nRT2,nRT3,nRT4,nRT5))
            break;
        else
            usleep(100000);
    }
    if(nIndex == 3)
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool CProcess3::ReadyGo()
{
    bool bret = false;
    //仪器温度检测
    if(YQTemptCheck())
    {
        bret = true;
        //吹扫压力
        if(m_data.fCSPress < m_stuInitTempt.CSPress)
        {
            bret &= false;
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"吹扫未达到预设压力",LOG_WARNING);
            }
        }
        //氮气压力
        if(m_data.fnitrpress < m_stuInitTempt.Nitrpress)
        {
            bret &= false;
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"氮气压力未达到预设压力",LOG_WARNING);
            }
        }
        //箱体温度
        if(!WaitForTemplate(BOXINIT_TEMP,m_stuInitTempt.BoxTemp_Init))
        {
            bret &= false;
            if(!CheckHWOnOff(BOXINIT_TEMP))
            {
                BaseInit_BOX();
                cout<<"Send Init BoxTempt cmd!"<<endl;
            }
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"箱体未达到预设温度",LOG_WARNING);
            }
        }
        //捕集阱温度
        if(!WaitForTemplate(PJJINIT_TEMP,m_stuInitTempt.PJJTemp_Init))
        {
            bret &= false;
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"捕集阱温度太高",LOG_WARNING);
            }
        }
        //检测器
        if(!WaitForTemplate(FIDINIT_TEMP,m_stuInitTempt.FIDTemp_Init))
        {
            bret &= false;
            if(!CheckHWOnOff(FIDINIT_TEMP))
            {
                BaseInit_FID();
                cout<<"Send Init FIDTempt cmd!"<<endl;
            }
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"检测器未达到预设温度",LOG_WARNING);
            }
        }
        //毛细柱初始温度
        if(!WaitForTemplate(MXZINIT_TEMP,m_stuInitTempt.MxzTemp_Init))
        {
            bret &= false;
            if(!CheckHWOnOff(MXZINIT_TEMP))
            {
                BaseInit_MXZ();
                cout<<"Send Init MXZTempt cmd!"<<endl;
            }
            if(m_pOnAlarm)
            {
                m_pOnAlarm(m_userdata,"毛细柱未达到初始温度",LOG_WARNING);
            }
        }
    }
    return bret;
}

bool CProcess3::SelftCheck()
{
    char cLogTxt[255];
    //箱体温度检测
    if(WaitForTemplate(BOXINIT_TEMP,m_stuInitTempt.BoxTemp_Init))
    {
        return true;
    }
    else
    {
        memset(cLogTxt,0,255);
        sprintf(cLogTxt,"箱体实时温度%.1f℃,预置温度%d℃,超出允许范围,终止流程",m_data.fBoxTemp,m_stuInitTempt.BoxTemp_Init);
        RecordLog(LOG_FAULT,cLogTxt);
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"箱体温度超出允许范围",LOG_FAULT);
        }
        return false;
    }

    //FID检测器温度检测
    if(WaitForTemplate(FIDINIT_TEMP,m_stuInitTempt.FIDTemp_Init))
    {
        return true;
    }
    else
    {
        memset(cLogTxt,0,255);
        sprintf(cLogTxt,"检测器实时温度%.1f℃,预置温度%d℃,超出允许范围,终止流程",m_data.fFidTemp,m_stuInitTempt.FIDTemp_Init);
        RecordLog(LOG_FAULT,cLogTxt);

        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"检测器温度超出允许范围",LOG_FAULT);
        }
        return false;
    }

    //捕集阱限值检测,超过某个温度（建议设置成320℃），流程结束，停止加热并报警；
    if(WaitForTemplate(PJJHK_TEMP,321))
    {
        memset(cLogTxt,0,255);
        sprintf(cLogTxt,"捕集阱实时温度%.1f℃,超过限值320℃,终止流程",m_data.fPjjTemp);
        RecordLog(LOG_FAULT,cLogTxt);

        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,PJJINIT_TEMP,0);//紧急发送停止加热命令

        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"捕集阱温度超过限值320℃",LOG_FAULT);
        }
        return false;
    }

    //毛细柱限值检测,超过某个温度（建议设置成300℃），流程结束，停止加热并报警；
    if(WaitForTemplate(MXZ_UPLIMIT,300))
    {
        memset(cLogTxt,0,255);
        sprintf(cLogTxt,"毛细柱实时温度%.1f℃,超过限值300℃,终止流程",m_data.fMxzTemp);
        RecordLog(LOG_FAULT,cLogTxt);

        ((CDriver3*)pDriver1)->StartLineTempt_152(m_DetecAddress,MXZINIT_TEMP,0);//紧急发送线性升温加热命令
        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,MXZINIT_TEMP,0);//紧急发送停止加热命令

        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"毛细柱温度超过限值300℃",LOG_FAULT);
        }
        return false;
    }
}
bool CProcess3::YQTemptCheck()
{
    //仪器温度检测,超过某个温度（可设置,50℃），流程结束，停止加热并报警；
    if(WaitForTemplate(YQ_TEMP,m_stuInitTempt.YQTemp_Safe))
    {
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,"仪器内部温度过高,系统停止工作",LOG_FAULT);
        }
        return false;
    }
    return true;
}
void CProcess3::CloseAllTempt()
{
    //毛细柱线性升温
    if(CheckLineOnOff(MXZINIT_TEMP))
    {
        ((CDriver3*)pDriver1)->StartLineTempt_152(m_DetecAddress,MXZINIT_TEMP,0);//紧急发送线性升温加热命令
    }
    //毛细柱恒温
    if(CheckHWOnOff(MXZINIT_TEMP))
    {
        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,MXZINIT_TEMP,0);//紧急发送停止加热命令
    }

    //捕集阱
    if(CheckHWOnOff(PJJINIT_TEMP))
    {
        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,PJJINIT_TEMP,0);//紧急发送停止加热命令
    }

    //检测器
    if(CheckHWOnOff(FIDINIT_TEMP))
    {
        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,FIDINIT_TEMP,0);
    }

    //箱体温度
    if(CheckHWOnOff(BOXINIT_TEMP))
    {
        ((CDriver3*)pDriver1)->StartTemptSigle_151(m_DetecAddress,BOXINIT_TEMP,0);
    }
}

void CProcess3::MainWindowDebug(const char *cStr)
{
    char str[64];memset(str,0,64);
    if(m_pOnDebugInfo)
    {
        m_pOnDebugInfo(m_userdata,this->GetProcessType(),cStr);
    }
}
/*
0	表示空闲状态
1	表示正常工作状态(采集数据)
2	表示正常工作状态(烧基线)
3	与上位机通信故障
4	与信号采集模块通信故障
5	点火故障
6	高压输出故障
7	与有机物温度模块通信故障
8	有机物温度模块参数设置故障
9	有机物温度模块控温故障
10	有机物温度模块传感器故障
*/
void CProcess3::DetectError(int nStatus)
{
    switch(nStatus)
    {
    case 0:
        DebugInfo("协调模块状态变化：空闲状态[0]",3);
        break;
    case 1:
        DebugInfo("协调模块状态变化：采集数据[1]",3);
        break;
    case 2:
        DebugInfo("协调模块状态变化：烧基线[2]",3);
        break;
    case 3:
        DebugInfo("协调模块状态变化：与上位机通信故障[3]",3);
        break;
    case 4:
        DebugInfo("协调模块状态变化：与信号采集模块通信故障[4]",3);
        break;
    case 5:
        DebugInfo("协调模块状态变化：点火故障[5]",3);
        break;
    case 6:
        DebugInfo("协调模块状态变化：高压输出故障[6]",3);
        break;
    case 7:
        DebugInfo("协调模块状态变化：与有机物温度模块通信故障[7]",3);
        break;
    case 8:
        DebugInfo("协调模块状态变化：有机物温度模块参数设置故障[8]",3);
        break;
    case 9:
        DebugInfo("协调模块状态变化：有机物温度模块控温故障[9]",3);
        break;
    case 10:
        DebugInfo("协调模块状态变化：有机物温度模块传感器故障[10]",3);
        break;
    }
}
//检查恒温开关是否打开
bool CProcess3::CheckHWOnOff(int nChannal)
{
    int bret = 0;
    switch(nChannal)
    {
    case 0:
        bret = m_data.TemptModel & 0x01;
        break;
    case 1:
        bret = m_data.TemptModel & 0x04;
        break;
    case 2:
        bret = m_data.TemptModel & 0x10;
        break;
    case 3:
        bret = m_data.TemptModel & 0x40;
        break;
    }
    return (bool)bret;
}
//检查线性升级开关是否打开
bool CProcess3::CheckLineOnOff(int nChannal)
{
    int bret = 0;
    switch(nChannal)
    {
    case 0:
        bret = m_data.TemptModel & 0x02;
        break;
    case 1:
        bret = m_data.TemptModel & 0x08;
        break;
    case 2:
        bret = m_data.TemptModel & 0x20;
        break;
    case 3:
        bret = m_data.TemptModel & 0x80;
        break;
    }
    return (bool)bret;
}
/*
挥发性有机物FID：
氮气30ml/min
氢气40ml/mi
空气400ml/min

挥发性有机物ECD：
氮气30ml/min

半挥发性有机物ECD：
氮气30ml/min
萃取头萃取次数*/
void CProcess3::RefreshConsumeData()
{
    int spac = m_qtHcTime.elapsed();
    if(spac/1000 >= 60)
    {
         ConsumeSub(this->GetProcessType(),"HC_P",30);//氮气
         ConsumeSub(this->GetProcessType(),"HC_Q",40);//氢气
         ConsumeSub(this->GetProcessType(),"HC_R",400);//空气
         m_qtHcTime.restart();
    }
}
