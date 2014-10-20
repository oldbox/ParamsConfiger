#include "cmainctrl.h"
#include "vardef.h"

CMainCtrl::CMainCtrl()
{
    bExit = false;
    m_nCount = 1;
    InitIniItem(); //初始化配置文件

    for(int j=0;j<24;j++)
    {
        m_bTestFlag[j] = true;
    }
    m_nYQType = GetAfxYQType();
    int nCurrHour = QDateTime::currentDateTime().time().hour();
    m_bTestFlag[nCurrHour] = false;
    m_nLastHour = nCurrHour;
    m_nLastMod = 0;
    GetIniInit();

    //测试所有已配置串口的连接状态
    for(int i=0;i<COMCOUNT;i++)
    {
        char czGroup[64];memset(czGroup,0,64);
        char cz_baudrate[64];memset(cz_baudrate,0,64);
        char cz_databit[64];memset(cz_databit,0,64);
        char cz_stopbit[64];memset(cz_stopbit,0,64);
        char cz_parity[64];memset(cz_parity,0,64);
        sprintf(czGroup,"comport_%d",i);

        GetAfxConfig()->GetIni(czGroup,"baudrate",cz_baudrate);
        GetAfxConfig()->GetIni(czGroup,"databit",cz_databit);
        GetAfxConfig()->GetIni(czGroup,"stopbit",cz_stopbit);
        GetAfxConfig()->GetIni(czGroup,"parity",cz_parity);

        CSerialPort* port = new CSerialPort();
        if(port->OpenComPort(i,atoi(cz_baudrate),atoi(cz_databit),cz_stopbit,cz_parity[0]) < 0)
        {
            delete port;
            pSerialPorts[i] = NULL;
        }
        else
        {
            pSerialPorts[i] = port;
            pSerialPorts[i]->SetpOnComData(OnCMainCtrlComData,(unsigned long)this,i);
        }
    }

    pComm = new CComm();
    pYQs = new CYQs(pSerialPorts,pComm);

    Run();
}

CMainCtrl::~CMainCtrl()
{
    ExitRun();

    delete pYQs;
    delete pComm;

    for(int i=0;i<COMCOUNT;i++)
    {
       if(pSerialPorts[i])
       {
           pSerialPorts[i]->CloseComPort();
           delete pSerialPorts[i];
           pSerialPorts[i] = NULL;
       }
    }
}

void CMainCtrl::Run()
{
    if(!bExit)
    pthread_create(&id_autorun, NULL, AutoRun, this);
}
void CMainCtrl::ExitRun()
{
    bExit = true;
    pthread_join(id_autorun,0);
}

void* CMainCtrl::AutoRun(void* pParam)
{
    CMainCtrl* pThis = (CMainCtrl*)pParam;
    char czValue[MAX_SIZE];memset(czValue,0,MAX_SIZE);
    sleep(1);
    pThis->SendInit();
    while(!pThis->bExit)
    {
        switch (pThis->m_stMainCtrl.RunMod)
        {
        case 1://周期模式
            pThis->DoMod1();
            if((1 == pThis->m_stMainCtrl.AutoStdAbsorb||1 == pThis->m_stMainCtrl.AutoBlankAbsorb||1 == pThis->m_stMainCtrl.AutoSampLetest)&&(1 != pThis->m_nYQType&&2 != pThis->m_nYQType&&3 != pThis->m_nYQType))
            {
                pThis->DoMod4();
            }
            break;
        case 2://定点模式
            pThis->DoMod2();
        default:
            break;
        }
        //电化学自动电极维护
        if((1 == pThis->m_stMainCtrl.MaintainMod)&&(1 == pThis->m_nYQType))
        {
            pThis->DoMod3();
        }
        pThis->m_nLastMod = pThis->m_stMainCtrl.RunMod;
        sleep(1);
    }
    return NULL;
}
//周期模式
void CMainCtrl::DoMod1()
{
    char czValue[MAX_SIZE];
    memset(czValue,0,MAX_SIZE);
    QDateTime currDT = QDateTime::currentDateTime();

    if(m_stMainCtrl.NextTestTime.secsTo(currDT) > 0 )
    {
        bool bTag = false;
        //发测试命令
        for(int i=0;i<pYQs->GetYQCount();i++)
        {
            SYQPARAM sYQ;
            pYQs->pYQ[i]->GetYQParam(sYQ);
            if(pYQs->GetYQbIdle(sYQ.czID))
            {
                if(7 == sYQ.iProcessType)
                {
                    if(!bTag)
                    {
                        bTag = true;
                        //最临近的一个周期再发测试命令
                        if(m_stMainCtrl.NextTestTime.secsTo(currDT) < m_stMainCtrl.TestCYC*60)
                        {
                            if(sYQ.Priority == m_nCount)
                            {
                                m_nCount++;
                                int iRt = pYQs->SendCmd(sYQ.czID,CMD_TEST,3,m_stMainCtrl.NextTestTime);
                                qDebug("cmd rt=%d",iRt);
                            }
                            else if(sYQ.Priority  == m_nCount)
                            {
                                m_nCount++;
                                int iRt = pYQs->SendCmd("Z59_2",CMD_TEST,3,m_stMainCtrl.NextTestTime);
                                qDebug("cmd rt=%d",iRt);
                            }
                            if(m_nCount >= pYQs->GetYQCount())
                            {
                                m_nCount = 1;
                                //改变下次测试时间
                                m_stMainCtrl.NextTestTime = m_stMainCtrl.NextTestTime.addSecs(m_stMainCtrl.TestCYC*60);
                                GetAfxConfig()->SetIni("SYSSET","8051NextTime",m_stMainCtrl.NextTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                            }
                        }
                        else
                        {
                            //改变下次测试时间
                            m_stMainCtrl.NextTestTime = m_stMainCtrl.NextTestTime.addSecs(m_stMainCtrl.TestCYC*60);
                            GetAfxConfig()->SetIni("SYSSET","8051NextTime",m_stMainCtrl.NextTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                        }


                     }
                }
                else if(sYQ.iProcessType == 3 || sYQ.iProcessType == 6)//有机物只发一次命令
                {
                    if(!bTag)
                    {
                        bTag = true;
                        //最临近的一个周期再发测试命令
                        if(m_stMainCtrl.NextTestTime.secsTo(currDT) < m_stMainCtrl.TestCYC*60)
                        {
                            int iRt = pYQs->SendCmd(sYQ.czID,CMD_TEST,3,m_stMainCtrl.NextTestTime);
                            qDebug("cmd rt=%d",iRt);
                        }

                        //改变下次测试时间
                        m_stMainCtrl.NextTestTime = m_stMainCtrl.NextTestTime.addSecs(m_stMainCtrl.TestCYC*60);
                        GetAfxConfig()->SetIni("SYSSET","8051NextTime",m_stMainCtrl.NextTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                     }
                }
                else
                {
                    //最临近的一个周期再发测试命令
                    if(m_stMainCtrl.NextTestTime.secsTo(currDT) < m_stMainCtrl.TestCYC*60)
                    {
                        int iRt = pYQs->SendCmd(sYQ.czID,CMD_TEST,3,m_stMainCtrl.NextTestTime);
                        qDebug("cmd rt=%d",iRt);
                    }
                    //改变下次测试时间
                    m_stMainCtrl.NextTestTime = m_stMainCtrl.NextTestTime.addSecs(m_stMainCtrl.TestCYC*60);
                    GetAfxConfig()->SetIni("SYSSET","8051NextTime",m_stMainCtrl.NextTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                 }
            }
        }
    }
     emit NotifyNextTestTime(m_stMainCtrl.NextTestTime);
}
//定点模式
void CMainCtrl::DoMod2()
{
    char czValue[MAX_SIZE];
    char czStr[16];
    QString dt;
    QString qstime;
    QDateTime NextTime;
    QDateTime TestTime;

    qstime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:00:00");
    TestTime = QDateTime::fromString(qstime, "yyyy-MM-dd hh:mm:ss");
    int nCurrHour = QDateTime::currentDateTime().time().hour();
    if(0 == nCurrHour)
    {
        m_bTestFlag[23] = true;
    }
    else
    {
        m_bTestFlag[nCurrHour-1] = true;
    }
    if(m_nLastHour != nCurrHour)
    {
        memset(czStr,0,16);
        memset(czValue,0,MAX_SIZE);
        sprintf(czStr,"%d",nCurrHour);
        GetAfxConfig()->GetIni("SYSSET",czStr,czValue);
    }
    m_nLastHour = nCurrHour;
    if((m_bTestFlag[nCurrHour]))
    {
        if(!strcmp(czValue,"8051"))
        {
            //发送测试命令
            for(int i=0;i<pYQs->GetYQCount();i++)
            {
                SYQPARAM sYQ;
                pYQs->pYQ[i]->GetYQParam(sYQ);
                if(pYQs->GetYQbIdle(sYQ.czID))
                {
                    int iRt = pYQs->SendCmd(sYQ.czID,CMD_TEST,3,TestTime);
                    m_bTestFlag[nCurrHour] = false;
                    qDebug("cmd rt=%d",iRt);
                    break;
                }

            }
        }
        else if(!strcmp(czValue,"3016"))
        {
            //发送标样核查命令
            for(int i=0;i<pYQs->GetYQCount();i++)
            {
                SYQPARAM sYQ;
                pYQs->pYQ[i]->GetYQParam(sYQ);
                if(pYQs->GetYQbIdle(sYQ.czID))
                {
                    int iRt = pYQs->SendCmd(sYQ.czID,CMD_SAMPLETEST,3,TestTime);
                    qDebug("cmd rt=%d",iRt);
                    break;
                }
            }
        }
        else if (!strcmp(czValue,"3021"))
        {
            for(int i=0;i<pYQs->GetYQCount();i++)
            {
                SYQPARAM sYQ;
                pYQs->pYQ[i]->GetYQParam(sYQ);
                if(pYQs->GetYQbIdle(sYQ.czID))
                {
                    int iRt = pYQs->SendCmd(sYQ.czID,CMD_SAMPLEADJUST,3,TestTime);
                    qDebug("cmd rt=%d",iRt);
                    break;
                }
            }
        }
        else if(!strcmp(czValue,"3011"))
        {
            for(int i=0;i<pYQs->GetYQCount();i++)
            {
                SYQPARAM sYQ;
                pYQs->pYQ[i]->GetYQParam(sYQ);
                if(pYQs->GetYQbIdle(sYQ.czID))
                {
                    int iRt = pYQs->SendCmd(sYQ.czID,CMD_BANKADJUST,3,TestTime);
                    qDebug("cmd rt=%d",iRt);
                    break;
                }
            }
        }
        else
        {
            //do nothing
        }
    }

    //发测试命令或运行模式变化后，改变下次测试时间
    if(m_bTestFlag[nCurrHour]||(m_nLastMod != m_stMainCtrl.RunMod))
    {
        m_bTestFlag[nCurrHour] = false;
        for(int i = 1;i < 24;i++)
        {
            if(i+nCurrHour < 24)
            {
                memset(czStr,0,16);
                memset(czValue,0,MAX_SIZE);
                sprintf(czStr,"%d",i+nCurrHour);
                GetAfxConfig()->GetIni("SYSSET",czStr,czValue);
                if((!strcmp(czValue,"8051"))||(!strcmp(czValue,"3016"))||(!strcmp(czValue,"3011"))||(!strcmp(czValue,"3021")))
                {
                    dt = QDateTime::currentDateTime().addSecs(i*60*60).toString("yyyy-MM-dd HH:00:00");
                    NextTime = QDateTime::fromString(dt, "yyyy-MM-dd hh:mm:ss");
                    emit NotifyNextTestTime(NextTime);
                    break;
                }
            }
            else
            {
                int j = (i+nCurrHour)%24;
                memset(czStr,0,16);
                memset(czValue,0,MAX_SIZE);
                sprintf(czStr,"%d",j);
                GetAfxConfig()->GetIni("SYSSET",czStr,czValue);
                if((!strcmp(czValue,"8051"))||(!strcmp(czValue,"3016"))||(!strcmp(czValue,"3011"))||(!strcmp(czValue,"3021")))
                {
                    dt = QDateTime::currentDateTime().addDays(1).toString("yyyy-MM-dd 00:00:00");
                    NextTime = QDateTime::fromString(dt,"yyyy-MM-dd hh:mm:ss").addSecs(j*60*60);
                    emit NotifyNextTestTime(NextTime);
                    break;
                }
            }
        }
    }
}
//电化学自动电极维护
void CMainCtrl::DoMod3()
{
    QDateTime currDT = QDateTime::currentDateTime();
    if(m_stMainCtrl.MaintainTime.secsTo(currDT) >= 0 )
    {
         //最临近的一个周期再发测试命令
        if(m_stMainCtrl.MaintainTime.secsTo(currDT) < m_stMainCtrl.MaintainCYC*60*60)
        {
            //发测试命令
            for(int i=0;i<pYQs->GetYQCount();i++)
            {
                SYQPARAM sYQ;
                pYQs->pYQ[i]->GetYQParam(sYQ);
                int iRt = pYQs->SendCmd(sYQ.czID,CMD_ELECMAINTAIN,3,m_stMainCtrl.MaintainTime);

                if(MCU_BUSY != iRt)
                {
                    //改变下次测试时间
                    m_stMainCtrl.MaintainTime = m_stMainCtrl.MaintainTime.addSecs(m_stMainCtrl.MaintainCYC*60*60);
                    GetAfxConfig()->SetIni("SYSSET","MaintainTime",m_stMainCtrl.MaintainTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                    break;
                }

            }
        }
        else{
            //改变下次测试时间
            m_stMainCtrl.MaintainTime = m_stMainCtrl.MaintainTime.addSecs(m_stMainCtrl.MaintainCYC*60*60);
            GetAfxConfig()->SetIni("SYSSET","MaintainTime",m_stMainCtrl.MaintainTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
        }
    }
}
//自动核查，自动标样校准，自动空白校准
void CMainCtrl::DoMod4()
{
    QDateTime dtTestTime;
    bool b_AutoStdAbsorb = false,b_AutoBlankAbsorb = false,b_AutoSampLetest = false;
    QDateTime AutoStdAbsorbTime,AutoBlankAbsorbTime,AutoSampLetestTime;
    int iStdAbsorbCyc = 0,iBlankAbsorbCyc = 0,iSampLetestCyc = 0;
    //自动标样校准

    b_AutoStdAbsorb = m_stMainCtrl.AutoStdAbsorb == 1 ? true : false;
    AutoStdAbsorbTime= m_stMainCtrl.AutoStdAbsorbTime;
    iStdAbsorbCyc = m_stMainCtrl.StdAbsorbCyc;

    b_AutoBlankAbsorb = m_stMainCtrl.AutoBlankAbsorb == 1 ? true : false;
    AutoBlankAbsorbTime = m_stMainCtrl.AutoBlankAbsorbTime;
    iBlankAbsorbCyc = m_stMainCtrl.BlankAbsorbCyc;


    //自动标样核查
    b_AutoSampLetest = m_stMainCtrl.AutoSampLetest == 1 ? true : false;
    AutoSampLetestTime = m_stMainCtrl.AutoSampLetestTime;
    iSampLetestCyc = m_stMainCtrl.SampLetestCyc;

    int iAutoCyc = 0;
    int iCMDType = 0;
    QDateTime currDT = QDateTime::currentDateTime();
    if(b_AutoStdAbsorb)
    {
        if(AutoStdAbsorbTime.secsTo(currDT) >= 0 ) //标样校准
        {
            dtTestTime = AutoStdAbsorbTime;
            iCMDType = CMD_SAMPLEADJUST;
            iAutoCyc = iStdAbsorbCyc;
        }
    }
    else if(b_AutoBlankAbsorb)
    {
        if(AutoBlankAbsorbTime.secsTo(currDT) >= 0) //空白校准
        {
            dtTestTime = AutoBlankAbsorbTime;
            iCMDType = CMD_BANKADJUST;
            iAutoCyc = iBlankAbsorbCyc;
        }
    }
    else if(b_AutoSampLetest)
    {
        if(AutoSampLetestTime.secsTo(currDT) >= 0)//核查
        {
            dtTestTime = AutoSampLetestTime;
            iCMDType = CMD_SAMPLETEST;
            iAutoCyc = iSampLetestCyc;
        }
    }

    if(iCMDType != 0 && iAutoCyc > 0)
    {
        //最临近的一个周期再发测试命令
       if(dtTestTime.secsTo(currDT) < iAutoCyc*60)
       {
           //发测试命令
           for(int i=0;i<pYQs->GetYQCount();i++)
           {
               SYQPARAM sYQ;
               pYQs->pYQ[i]->GetYQParam(sYQ);
               if(pYQs->GetYQbIdle(sYQ.czID))//空闲时发
               {
                   int iRt = pYQs->SendCmd(sYQ.czID,iCMDType,4,dtTestTime);

                   if(MCU_BUSY != iRt)
                   {
                       //改变下次测试时间
                       dtTestTime = dtTestTime.addSecs(iAutoCyc*60);
                       if(iCMDType == CMD_SAMPLEADJUST)
                       {
                           GetAfxConfig()->SetIni("AutoAdjust_SET","AutoStdAbsorbTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                       }
                       else if(iCMDType == CMD_BANKADJUST)
                       {
                           GetAfxConfig()->SetIni("AutoAdjust_SET","AutoBlankAbsorbTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                       }
                       else if(iCMDType == CMD_SAMPLETEST)
                       {
                            GetAfxConfig()->SetIni("AutoAdjust_SET","AutoSampLetestTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
                       }
                       break;
                   }
               }
           }
       }
       else
       {
           //改变下次测试时间
           dtTestTime = dtTestTime.addSecs(iAutoCyc*60);
           if(iCMDType == CMD_SAMPLEADJUST)
           {
               GetAfxConfig()->SetIni("AutoAdjust_SET","AutoStdAbsorbTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
           }
           else if(iCMDType == CMD_BANKADJUST)
           {
               GetAfxConfig()->SetIni("AutoAdjust_SET","AutoBlankAbsorbTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
           }
           else if(iCMDType == CMD_SAMPLETEST)
           {
                GetAfxConfig()->SetIni("AutoAdjust_SET","AutoSampLetestTime",dtTestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
           }
       }
    }
}

void CMainCtrl::OnCMainCtrlComData(unsigned long userdata,bool bRead,void* czData,int iLen,int ComX)
{
    CMainCtrl* pThis = (CMainCtrl*)userdata;
    emit pThis->NotifyComData(bRead,czData,iLen,ComX);
}
void CMainCtrl::Init()
{
    char czValue[MAX_SIZE];
    memset(czValue,0,MAX_SIZE);
    if(!GetAfxConfig()->GetIni("SYSSET","8051NextTime",czValue)) GetAfxConfig()->SetIni("SYSSET","8051NextTime","2013-11-01 00:00:00");
    QDateTime dt8051 = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");
    emit NotifyNextTestTime(dt8051);
}
void CMainCtrl::InitIniItem()
{
    int nYQType = GetAfxYQType();
    char tmp[64];memset(tmp,0,64);

    if(!GetAfxConfig()->GetIni(NETGROUP,GBIP,tmp)) GetAfxConfig()->SetIni(NETGROUP,GBIP,"127.0.0.1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni(NETGROUP,GBMN,tmp)) GetAfxConfig()->SetIni(NETGROUP,GBMN,"12345678901234");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni(NETGROUP,GBPW,tmp)) GetAfxConfig()->SetIni(NETGROUP,GBPW,"123456");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni(NETGROUP,GBPORT,tmp)) GetAfxConfig()->SetIni(NETGROUP,GBPORT,"8100");

    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_0","baudrate",tmp)) GetAfxConfig()->SetIni("comport_0","baudrate","9600");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_0","databit",tmp)) GetAfxConfig()->SetIni("comport_0","databit","8");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_0","stopbit",tmp)) GetAfxConfig()->SetIni("comport_0","stopbit","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_0","parity",tmp)) GetAfxConfig()->SetIni("comport_0","parity","N");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_1","baudrate",tmp)) GetAfxConfig()->SetIni("comport_1","baudrate","9600");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_1","databit",tmp)) GetAfxConfig()->SetIni("comport_1","databit","8");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_1","stopbit",tmp)) GetAfxConfig()->SetIni("comport_1","stopbit","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_1","parity",tmp)) GetAfxConfig()->SetIni("comport_1","parity","N");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_2","baudrate",tmp)) GetAfxConfig()->SetIni("comport_2","baudrate","9600");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_2","databit",tmp)) GetAfxConfig()->SetIni("comport_2","databit","8");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_2","stopbit",tmp)) GetAfxConfig()->SetIni("comport_2","stopbit","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_2","parity",tmp)) GetAfxConfig()->SetIni("comport_2","parity","N");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_3","baudrate",tmp)) GetAfxConfig()->SetIni("comport_3","baudrate","9600");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_3","databit",tmp)) GetAfxConfig()->SetIni("comport_3","databit","8");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_3","stopbit",tmp)) GetAfxConfig()->SetIni("comport_3","stopbit","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_3","parity",tmp)) GetAfxConfig()->SetIni("comport_3","parity","N");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_4","baudrate",tmp)) GetAfxConfig()->SetIni("comport_4","baudrate","9600");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_4","databit",tmp)) GetAfxConfig()->SetIni("comport_4","databit","8");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_4","stopbit",tmp)) GetAfxConfig()->SetIni("comport_4","stopbit","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("comport_4","parity",tmp)) GetAfxConfig()->SetIni("comport_4","parity","N");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_0","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_0","comport","0");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_0","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_0","Driver","0");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_1","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_1","comport","2");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_1","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_1","Driver","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_2","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_2","comport","2");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_2","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_2","Driver","2");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_3","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_3","comport","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_3","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_3","Driver","3");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_4","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_4","comport","1");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_4","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_4","Driver","4");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_5","comport",tmp)) GetAfxConfig()->SetIni("ProcessType_5","comport","2");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ProcessType_5","Driver",tmp)) GetAfxConfig()->SetIni("ProcessType_5","Driver","5");

    //用户密码
    //普通用户
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("UserType_1","passwd",tmp)) GetAfxConfig()->SetIni("UserType_1","passwd","911456");
    //管理员
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("UserType_2","passwd",tmp)) GetAfxConfig()->SetIni("UserType_2","passwd","lhsoft");
    //超级用户
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("UserType_3","passwd",tmp)) GetAfxConfig()->SetIni("UserType_3","passwd","2021036965");
    //软件倒计时
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("SYSSET","CountDown",tmp)) GetAfxConfig()->SetIni("SYSSET","CountDown","60");

    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("SYSSET","8051NextTime",tmp)) GetAfxConfig()->SetIni("SYSSET","8051NextTime","2014-11-01 00:00:00");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("SYSSET","8051CYC",tmp)) GetAfxConfig()->SetIni("SYSSET","8051CYC","60");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("SYSSET","MOD",tmp)) GetAfxConfig()->SetIni("SYSSET","MOD","1");

    //进样阀编号
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ValveNO","SampleValveNO",tmp)) GetAfxConfig()->SetIni("ValveNO","SampleValveNO","3");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ValveNO","WaterValveNO",tmp)) GetAfxConfig()->SetIni("ValveNO","WaterValveNO","8");
    //0:水样管进样 1:标样管进样
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ValveNO","ByOrSy",tmp)) GetAfxConfig()->SetIni("ValveNO","ByOrSy","0");
    //蒸馏水阀编号
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ValveNO","DistilledValveNO",tmp)) GetAfxConfig()->SetIni("ValveNO","DistilledValveNO","1");
    //源电磁阀2,多参数光谱仪用到
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("ValveNO","DCFSource2",tmp)) GetAfxConfig()->SetIni("ValveNO","DCFSource2","0");


    //1.自动标样校准开关
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoStdAbsorb",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoStdAbsorb","0");
    //1.自动标样校准周期
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","StdAbsorbCyc",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","StdAbsorbCyc","60");

    //2.自动空白校准开关
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoBlankAbsorb",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoBlankAbsorb","0");
    //2.自动空白校准周期
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","BlankAbsorbCyc",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","BlankAbsorbCyc","60");

    //3.自动核查校准开关
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoSampLetest",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoSampLetest","0");
    //3.自动核查校准周期
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("AutoAdjust_SET","AutoHcCyc",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","AutoHcCyc","60");

    if(1 == nYQType)
    {
        //自动电极维护,1开启自动电极维护,0关闭自动电极维护
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("SYSSET","Maintain",tmp)) GetAfxConfig()->SetIni("SYSSET","Maintain","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("SYSSET","MaintainCYC",tmp)) GetAfxConfig()->SetIni("SYSSET","MaintainCYC","24");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("SYSSET","MaintainTime",tmp)) GetAfxConfig()->SetIni("SYSSET","MaintainTime","2014-11-01 00:00:00");
        //量程设置
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("RangeSet","Range1Value",tmp)) GetAfxConfig()->SetIni("RangeSet","Range1Value","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("RangeSet","Range2Value",tmp)) GetAfxConfig()->SetIni("RangeSet","Range2Value","5.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("RangeSet","xiaojieValue",tmp)) GetAfxConfig()->SetIni("RangeSet","xiaojieValue","1.1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("RangeSet","RangeWay",tmp)) GetAfxConfig()->SetIni("RangeSet","RangeWay","0");
        //量程是否自动切换
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("RangeSet","RangeSwitch",tmp)) GetAfxConfig()->SetIni("RangeSet","RangeSwitch","0");

        //温度设置
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","TempValue",tmp)) GetAfxConfig()->SetIni("TempSet","TempValue","80");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","HeatTime",tmp)) GetAfxConfig()->SetIni("TempSet","HeatTime","2");

        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","TempValue_2",tmp)) GetAfxConfig()->SetIni("TempSet","TempValue_2","80");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","HeatTime_2",tmp)) GetAfxConfig()->SetIni("TempSet","HeatTime_2","2");

        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","TempValue_3",tmp)) GetAfxConfig()->SetIni("TempSet","TempValue_3","80");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("TempSet","HeatTime_3",tmp)) GetAfxConfig()->SetIni("TempSet","HeatTime_3","2");

        //原始数据计算取值
         memset(tmp,0,64);
        GetAfxConfig()->SetIni("INITDATA","CaclFlag","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z07A",tmp)) GetAfxConfig()->SetIni("INITDATA","Z07A","5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z07B",tmp)) GetAfxConfig()->SetIni("INITDATA","Z07B","16");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z08A",tmp)) GetAfxConfig()->SetIni("INITDATA","Z08A","5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z08B",tmp)) GetAfxConfig()->SetIni("INITDATA","Z08B","16");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z24A",tmp)) GetAfxConfig()->SetIni("INITDATA","Z24A","5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z24B",tmp)) GetAfxConfig()->SetIni("INITDATA","Z24B","16");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z27A",tmp)) GetAfxConfig()->SetIni("INITDATA","Z27A","5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INITDATA","Z24B",tmp)) GetAfxConfig()->SetIni("INITDATA","Z24B","16");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INFO","TitNumID",tmp)) GetAfxConfig()->SetIni("INFO","TitNumID","0");
    }
    else if(3 == nYQType)
    {
        //检测器类型FID(0),ECD(1)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("INFO","Detector",tmp)) GetAfxConfig()->SetIni("INFO","Detector","0");

        //VOC配置项目
        //nSampleTime:进样时间(单位:秒)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","SampleTime",tmp)) GetAfxConfig()->SetIni("VOC_SET","SampleTime","30");
        //nCSTime:吹扫时间(单位:分钟)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","CSTime",tmp)) GetAfxConfig()->SetIni("VOC_SET","CSTime","5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","CSTime_BY",tmp)) GetAfxConfig()->SetIni("VOC_SET","CSTime_BY","2");
        //nGCTime:干扫时间(单位:分钟)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","GCTime",tmp)) GetAfxConfig()->SetIni("VOC_SET","GCTime","5");

        //吹扫压力,测试时最低吹扫压力
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","CSPress",tmp)) GetAfxConfig()->SetIni("VOC_SET","CSPress","0.0");
        //氮气压力,测试时最低吹扫压力
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Nitrpress",tmp)) GetAfxConfig()->SetIni("VOC_SET","Nitrpress","0.0");

        //测试完后是否关火
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","FireOnOff",tmp)) GetAfxConfig()->SetIni("VOC_SET","FireOnOff","1");

        //nGainDataRate: 采集数据速率(单位:毫秒)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","GainDataRate",tmp)) GetAfxConfig()->SetIni("VOC_SET","GainDataRate","200");
        //nGainDataRTime: 采集数据时间(单位:分钟)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","GainDataRTime",tmp)) GetAfxConfig()->SetIni("VOC_SET","GainDataRTime","10");

        //捕集井初始温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","PJJTemp_Init",tmp)) GetAfxConfig()->SetIni("VOC_SET","PJJTemp_Init","50");
        //FID初始温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","FIDTemp_Init",tmp)) GetAfxConfig()->SetIni("VOC_SET","FIDTemp_Init","250");
        //箱体初始温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","BoxTemp_Init",tmp)) GetAfxConfig()->SetIni("VOC_SET","BoxTemp_Init","50");
        //毛细柱初始温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","MxzTemp_Init",tmp)) GetAfxConfig()->SetIni("VOC_SET","MxzTemp_Init","50");

        //仪器安全温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","YQTemp_Safe",tmp)) GetAfxConfig()->SetIni("VOC_SET","YQTemp_Safe","45");


        //捕集井解析温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","PJJTemp_JX",tmp)) GetAfxConfig()->SetIni("VOC_SET","PJJTemp_JX","40");
        //捕集井解析时间(单位:分钟)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","PJJTime_jx",tmp)) GetAfxConfig()->SetIni("VOC_SET","PJJTime_jx","30");
        //捕集井烘烤温度
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","PJJTemp_HK",tmp)) GetAfxConfig()->SetIni("VOC_SET","PJJTemp_HK","40");
        //捕集井烘烤时间(单位:分钟)
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","PJJTime_hk",tmp)) GetAfxConfig()->SetIni("VOC_SET","PJJTime_hk","5");

        char team[64];
        for(int i = 1; i <= 20;i++)
        {
            //nTempRate升温速率   (单位:min/℃)
            memset(tmp,0,64);
            memset(team,0,64);
            sprintf(team,"TempRate_%d",i);
            if(!GetAfxConfig()->GetIni("VOC_SET",team,tmp)) GetAfxConfig()->SetIni("VOC_SET",team,"0");

            //nDescTemp毛细柱温度
            memset(tmp,0,64);
            memset(team,0,64);
            sprintf(team,"DescTemp_%d",i);
            if(!GetAfxConfig()->GetIni("VOC_SET",team,tmp)) GetAfxConfig()->SetIni("VOC_SET",team,"0");

            //nTempRTime保持时间  (单位:分钟)
            memset(tmp,0,64);
            memset(team,0,64);
            sprintf(team,"TempRTime_%d",i);
            if(!GetAfxConfig()->GetIni("VOC_SET",team,tmp)) GetAfxConfig()->SetIni("VOC_SET",team,"0");
        }

        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Y1axis_Max",tmp)) GetAfxConfig()->SetIni("VOC_SET","Y1axis_Max","100000");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Y1axis_Min",tmp)) GetAfxConfig()->SetIni("VOC_SET","Y1axis_Min","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","X1axis_Max",tmp)) GetAfxConfig()->SetIni("VOC_SET","X1axis_Max","20");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","X1axis_Min",tmp)) GetAfxConfig()->SetIni("VOC_SET","X1axis_Min","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Y2axis_Max",tmp)) GetAfxConfig()->SetIni("VOC_SET","Y2axis_Max","300");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Y2axis_Min",tmp)) GetAfxConfig()->SetIni("VOC_SET","Y2axis_Min","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","X2axis_Max",tmp)) GetAfxConfig()->SetIni("VOC_SET","X2axis_Max","20");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","X2axis_Min",tmp)) GetAfxConfig()->SetIni("VOC_SET","X2axis_Min","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","Y1Multiple",tmp)) GetAfxConfig()->SetIni("VOC_SET","Y1Multiple","10000");

        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","LC_NitMin",tmp)) GetAfxConfig()->SetIni("VOC_SET","LC_NitMin","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","LC_NitMax",tmp)) GetAfxConfig()->SetIni("VOC_SET","LC_NitMax","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","LC_AirMin",tmp)) GetAfxConfig()->SetIni("VOC_SET","LC_AirMin","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","LC_AirMax",tmp)) GetAfxConfig()->SetIni("VOC_SET","LC_AirMax","1");

        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","AdjustOnOff",tmp)) GetAfxConfig()->SetIni("VOC_SET","AdjustOnOff","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","AdjustMode",tmp)) GetAfxConfig()->SetIni("VOC_SET","AdjustMode","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("VOC_SET","AdjustDiff",tmp)) GetAfxConfig()->SetIni("VOC_SET","AdjustDiff","60");

         memset(tmp,0,64);
         if(!GetAfxConfig()->GetIni("VOC_SET","Search_Baseline",tmp)) GetAfxConfig()->SetIni("VOC_SET","Search_Baseline","6");
         memset(tmp,0,64);
         if(!GetAfxConfig()->GetIni("VOC_SET","Search_ModifyB",tmp)) GetAfxConfig()->SetIni("VOC_SET","Search_ModifyB","4000");

         memset(tmp,0,64);
         if(!GetAfxConfig()->GetIni("VOC_SET","Search_UnMaxArea",tmp)) GetAfxConfig()->SetIni("VOC_SET","Search_UnMaxArea","20000");
    }
    else if(2 == nYQType)
    {
        //紫外烟气校准类型  1跨度校准 2零点 3流速
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCaliType","CaliType",tmp)) GetAfxConfig()->SetIni("GasCaliType","CaliType","1");

        //紫外烟气手动校准
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","ConcN0",tmp)) GetAfxConfig()->SetIni("GasHandCali","ConcN0","250.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","ConcN02",tmp)) GetAfxConfig()->SetIni("GasHandCali","ConcN02","250.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","ConcS02",tmp)) GetAfxConfig()->SetIni("GasHandCali","ConcS02","250.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","Conc02",tmp)) GetAfxConfig()->SetIni("GasHandCali","Conc02","20.7");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","UnitN0",tmp)) GetAfxConfig()->SetIni("GasHandCali","UnitN0","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","UnitN02",tmp)) GetAfxConfig()->SetIni("GasHandCali","UnitN02","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","UnitS02",tmp)) GetAfxConfig()->SetIni("GasHandCali","UnitS02","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasHandCali","Unit02",tmp)) GetAfxConfig()->SetIni("GasHandCali","Unit02","2");
        //零点校准
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasZeroCali","UnitN0",tmp)) GetAfxConfig()->SetIni("GasZeroCali","UnitN0","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasZeroCali","UnitN02",tmp)) GetAfxConfig()->SetIni("GasZeroCali","UnitN02","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasZeroCali","UnitS02",tmp)) GetAfxConfig()->SetIni("GasZeroCali","UnitS02","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasZeroCali","Unit02",tmp)) GetAfxConfig()->SetIni("GasZeroCali","Unit02","2");
         //紫外烟气流速校准
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasFlowCali","SettingFlow",tmp)) GetAfxConfig()->SetIni("GasFlowCali","SettingFlow","1");
        //烟气校准系数
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","N0SpanCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","N0SpanCoef","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","NOZeroCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","NOZeroCoef","0.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","N02SpanCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","N02SpanCoef","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","NO2ZeroCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","NO2ZeroCoef","0.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","S02SpanCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","S02SpanCoef","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","SO2ZeroCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","SO2ZeroCoef","0.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","02SpanCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","02SpanCoef","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","O2ZeroCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","O2ZeroCoef","0.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","FlowSpanCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","FlowSpanCoef","1.0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCoef","FlowZeroCoef",tmp)) GetAfxConfig()->SetIni("GasCoef","FlowZeroCoef","0.0");
        //光谱仪参数
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("SpecParam","IntegrationTime",tmp)) GetAfxConfig()->SetIni("SpecParam","IntegrationTime","1.5");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("SpecParam","NrAverages",tmp)) GetAfxConfig()->SetIni("SpecParam","NrAverages","100");
        //输入通道设置
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasInputSet","InputIO1",tmp)) GetAfxConfig()->SetIni("GasInputSet","InputIO1","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasInputSet","InputIO2",tmp)) GetAfxConfig()->SetIni("GasInputSet","InputIO2","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasInputSet","InputIO3",tmp)) GetAfxConfig()->SetIni("GasInputSet","InputIO3","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasInputSet","InputIO4",tmp)) GetAfxConfig()->SetIni("GasInputSet","InputIO4","3");
        //烟气测试参数
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZA0_SET","ProcessType",tmp)) GetAfxConfig()->SetIni("ZA0_SET","ProcessType","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZA0_SET","Name",tmp)) GetAfxConfig()->SetIni("ZA0_SET","Name","NO");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZA1_SET","ProcessType",tmp)) GetAfxConfig()->SetIni("ZA1_SET","ProcessType","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZA1_SET","Name",tmp)) GetAfxConfig()->SetIni("ZA1_SET","Name","NO2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZB2_SET","ProcessType",tmp)) GetAfxConfig()->SetIni("ZB2_SET","ProcessType","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZB2_SET","Name",tmp)) GetAfxConfig()->SetIni("ZB2_SET","Name","SO2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZB4_SET","ProcessType",tmp)) GetAfxConfig()->SetIni("ZB4_SET","ProcessType","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("ZB4_SET","Name",tmp)) GetAfxConfig()->SetIni("ZB4_SET","Name","O2");
        //显示浓度单位设置
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasSetting","ChromaUnit",tmp))GetAfxConfig()->SetIni("GasSetting","ChromaUnit","0");
        //是否显示负值
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasSetting","ShowNegative",tmp))GetAfxConfig()->SetIni("GasSetting","ShowNegative","0");
        //设置仪器种类,当为1时,则表示装配4-20mA,0表示不装配4-20mA
        if(!GetAfxConfig()->GetIni("GasSetting","YQKind",tmp))GetAfxConfig()->SetIni("GasSetting","YQKind","1");
        //曲线界面，显示数据，需设置波长范围，查看范围内的值
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasSetting","LamdbaMax",tmp))GetAfxConfig()->SetIni("GasSetting","LamdbaMax","420");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasSetting","LamdbaMin",tmp))GetAfxConfig()->SetIni("GasSetting","LamdbaMin","185");

        //4~20mA通道初始化
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasOutputSet","OutputIO1",tmp))GetAfxConfig()->SetIni("GasOutputSet","OutputIO1","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasOutputSet","OutputIO2",tmp))GetAfxConfig()->SetIni("GasOutputSet","OutputIO2","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasOutputSet","OutputIO3",tmp))GetAfxConfig()->SetIni("GasOutputSet","OutputIO3","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasOutputSet","OutputIO4",tmp))GetAfxConfig()->SetIni("GasOutputSet","OutputIO4","3");
        //通讯设置初始化
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCommunication","baudrate",tmp))GetAfxConfig()->SetIni("GasCommunication","baudrate","9600");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCommunication","databit",tmp))GetAfxConfig()->SetIni("GasCommunication","databit","8");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCommunication","stopbit",tmp))GetAfxConfig()->SetIni("GasCommunication","stopbit","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCommunication","parity",tmp))GetAfxConfig()->SetIni("GasCommunication","parity","N");
        //1表示启用TCP通讯，2表示启用RS232通讯
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasCommunication","TCPFlag",tmp))GetAfxConfig()->SetIni("GasCommunication","TCPFlag","1");
        //自动校准波动值等信息
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","WaveS02",tmp))GetAfxConfig()->SetIni("GasAutoCali","WaveS02","3");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","UnitS02",tmp))GetAfxConfig()->SetIni("GasAutoCali","UnitS02","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","OrderS02",tmp))GetAfxConfig()->SetIni("GasAutoCali","OrderS02","1");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","ChromaS02",tmp))GetAfxConfig()->SetIni("GasAutoCali","ChromaS02","250");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","WaveN0",tmp))GetAfxConfig()->SetIni("GasAutoCali","WaveN0","3");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","UnitN0",tmp))GetAfxConfig()->SetIni("GasAutoCali","UnitN0","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","OrderN0",tmp))GetAfxConfig()->SetIni("GasAutoCali","OrderN0","2");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("GasAutoCali","ChromaN0",tmp))GetAfxConfig()->SetIni("GasAutoCali","ChromaN0","250");
    }
    else if(4 == nYQType)
    {
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("AutoAdjust_SET","AutoAdjustSet",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","AutoAdjustSet","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoStdAbsorb",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoStdAbsorb","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoBlankAbsorb",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoBlankAbsorb","0");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoSampLetest",tmp)) GetAfxConfig()->SetIni("AutoAdjust_SET","IsAutoSampLetest","0");

    }
    else if(5 == nYQType)
    {
        //原子荧光设置基点至峰高的距离
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("Z06_SET","LeftDistance",tmp)) GetAfxConfig()->SetIni("Z06_SET","LeftDistance","3");
        memset(tmp,0,64);
        if(!GetAfxConfig()->GetIni("Z06_SET","RightDistance",tmp)) GetAfxConfig()->SetIni("Z06_SET","RightDistance","5");
    }

    //多参数协议类型
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName1",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName1","LH_MODBUS");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName2",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName2","科泽仪器(单参数)");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName3",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName3","Seres2000");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName4",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName4","力合老协议");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName5",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName5","新普惠");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName6",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName6","YD8000电压");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName7",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName7","TOC4100");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName8",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName8","IQ2020");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName9",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName9","HACH/MODBUS");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName10",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName10","聚光COD");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName11",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName11","力合温控模块");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName12",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName12","WTW Other");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName13",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName13","聚光烟气");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName14",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName14","TOC-X");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName15",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName15","ModbusInt");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName16",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName16","ESA");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName17",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName17","HM29");
    memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("InterAgreeType","AgreeName18",tmp)) GetAfxConfig()->SetIni("InterAgreeType","AgreeName18","力合流量");




}
bool CMainCtrl::SendCmd(int nCmd, QString qsYQID)
{
    int iRet = 0;
    SYQPARAM sYQ;
    if(qsYQID.length() > 0)
    {
        //发测试命令
        for(int i=0;i<pYQs->GetYQCount();i++)
        {
            pYQs->pYQ[i]->GetYQParam(sYQ);
            if(qsYQID == QString(sYQ.czID))
            {
                iRet = pYQs->SendCmd(sYQ.czID,nCmd,1);
                break;
            }
        }
    }
    else
    {
        bool bTag = false;
        QDateTime dt = QDateTime::currentDateTime();
        //发测试命令
        for(int i=0;i<pYQs->GetYQCount();i++)
        {
            pYQs->pYQ[i]->GetYQParam(sYQ);
            if(sYQ.iProcessType == 3)
            {
                if(!bTag)//有机物只需要发一次命令
                {
                    iRet = pYQs->SendCmd(sYQ.czID,nCmd,1,dt);
                    bTag = true;
                }
            }
            else
            {
                iRet = pYQs->SendCmd(sYQ.czID,nCmd,1,dt);
            }
        }
    }
    return iRet;
}
//仪器启动时，判断仪器上次是否异常结束测试，如异常结束测试则发送初始化命令
void CMainCtrl::SendInit()
{
    char czValue[MAX_SIZE];memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","IdleFlag",czValue);
    if(1 == atoi(czValue))
    {
        for(int i=0;i<pYQs->GetYQCount();i++)
        {
            SYQPARAM sYQ;
            pYQs->pYQ[i]->GetYQParam(sYQ);
            if(pYQs->GetYQbIdle(sYQ.czID))
            {
                int iRt = pYQs->SendCmd(sYQ.czID,CMD_INIT,3,QDateTime::currentDateTime());
                qDebug("cmd rt=%d",iRt);
                break;
            }
        }
    }
}
void CMainCtrl::GetIniInit()
{
    char czValue[MAX_SIZE];

    //获取运行模式
    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","MOD",czValue);
    m_stMainCtrl.RunMod = atoi(czValue);

    //获取电化学自动电级维护
    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","Maintain",czValue);
    m_stMainCtrl.MaintainMod = atoi(czValue);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","8051NextTime",czValue);
    m_stMainCtrl.NextTestTime = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","8051CYC",czValue);
    m_stMainCtrl.TestCYC = atoi(czValue);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","MaintainTime",czValue);
    m_stMainCtrl.MaintainTime = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("SYSSET","MaintainCYC",czValue);
    m_stMainCtrl.MaintainCYC = atoi(czValue);

    //自动标样校准
    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoStdAbsorb",czValue);
    m_stMainCtrl.AutoStdAbsorb = atoi(czValue);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","AutoStdAbsorbTime",czValue);
    m_stMainCtrl.AutoStdAbsorbTime = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","StdAbsorbCyc",czValue);
    m_stMainCtrl.StdAbsorbCyc = atoi(czValue);

    //自动空白校准
    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoBlankAbsorb",czValue);
    m_stMainCtrl.AutoBlankAbsorb = atoi(czValue);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","AutoBlankAbsorbTime",czValue);
    m_stMainCtrl.AutoBlankAbsorbTime = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","BlankAbsorbCyc",czValue);
    m_stMainCtrl.BlankAbsorbCyc = atoi(czValue);

    //自动核查校准
    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","IsAutoSampLetest",czValue);
    m_stMainCtrl.AutoSampLetest = atoi(czValue);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","AutoSampLetestTime",czValue);
    m_stMainCtrl.AutoSampLetestTime = QDateTime::fromString(QString::fromLatin1(czValue), "yyyy-MM-dd HH:mm:ss");

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("AutoAdjust_SET","AutoHcCyc",czValue);
    m_stMainCtrl.SampLetestCyc = atoi(czValue);

}
