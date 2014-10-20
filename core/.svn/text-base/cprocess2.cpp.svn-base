#include "cprocess2.h"
#include <QDateTime>
#include <pthread.h>
#include <math.h>

static double g_dScope[totalPixel];
static double g_dLambda[totalPixel];
CProcess2::CProcess2()
{
    m_NrPixels = 0;
    m_bSaveTag = false;
    m_bLightTag = false;
    m_bBoxTag = false;
    m_bSpecTag = false;
    m_bFanTag1 = false;
    m_bFanTag2 = false;
    m_dScopeLamp = 0.0;
    m_dScopeBk = 0.0;
    m_nCount = 0;
    m_fFlowValue = 0.0;
    m_nCountCali = 0;
    m_nCountCaliFlow = 0;
    m_fResultN0 = 0.0;
    m_fResultN02 = 0.0;
    m_fResultS02 = 0.0;
    m_fResult02 = 0.0;
    m_bProcessFlag = false;
    m_bSpanTag = false;
    m_bScopeSaveFlag = false;
    m_bConsumeFlag =  false;
    m_bStartMaintain = false;
    m_bAutoCali[0] = true;
    m_bAutoCali[1] = false;
    m_bAutoCailTag = false;
    m_bStopFlag = false;
    m_bAutoSpanTag = false;
    m_bMaintainFlag =  false;
    m_nTimes = 0;
    m_bM1Flag = false;
    m_bM2Flag = false;
    m_bM3Flag = false;
    m_bM4Flag = false;
    m_bM5Flag = false;
    m_fOutPutS02 = 0.0;
    m_fOutPutN0 = 0.0;
    m_fOutPut02 = 0.0;
    m_bHighLevel = false;
    m_bRunThread = false;
    RunThread();
    for(int i=0;i<totalPixel;i++)
    {
        g_dScope[i] = 0.0;
        g_dLambda[i] = 0.0;
    }
    for(int i=0;i<9;i++)
    {
        m_StatusValue[i] = 0.0;
    }
    for(int j=0;j<8;j++)
    {
        m_bAlarmFlag[j] = false;
    }
    m_aAnalysis = new analysisData;
    m_aAnalysis->darkSpec = new double[totalPixel];
    m_aAnalysis->lampSpec = new double[totalPixel];
    m_aAnalysis->measSpec = new double[totalPixel];
    m_aAnalysis->specLen = new int [3];
    m_aresult = new resultData;
    m_aresult->concData = new double[3];
    m_aresult->absData = new double[3];
    m_aresult->coefData = new double[5];

    for(int i=0;i<5;i++)
    {
        m_fCaliValueN0[i] = 0.0;
        m_fCaliValueS02[i] = 0.0;
        m_fCaliValue02[i] = 0.0;
        m_fCaliFlowSpan[i] = 0.0;
        m_fCaliFlowZero[i] = 0.0;
    }
    m_struVector.m_vtnLamdba.clear();
    m_struVector.m_vtnScope.clear();
    InitCaliCoef();
}

CProcess2::~CProcess2()
{
    ExitRunThread();
    releaseSpec();
    m_bRun = false;

    delete [] m_aAnalysis->darkSpec;
    delete [] m_aAnalysis->lampSpec;
    delete [] m_aAnalysis->measSpec;
    delete m_aAnalysis;
    delete [] m_aresult->concData;
    delete [] m_aresult->absData;
    delete [] m_aresult->coefData;
    delete m_aresult;
    cout << "void CProcess2::~CProcess2()" << endl;
}

int CProcess2::GetProcessType()
{
    return 2;
}
//初始化校准系数
void CProcess2::InitCaliCoef()
{
    char tmp[64];
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","N0SpanCoef",tmp))
    {
        m_fN0SpanCoef = atof(tmp);
    }
    else
    {
        m_fN0SpanCoef = 1.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","NOZeroCoef",tmp))
    {
        m_fNOZeroCoef = atof(tmp);
    }
    else
    {
        m_fNOZeroCoef = 0.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","N02SpanCoef",tmp))
    {
        m_fN02SpanCoef = atof(tmp);
    }
    else
    {
        m_fN02SpanCoef = 1.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","NO2ZeroCoef",tmp))
    {
        m_fNO2ZeroCoef = atof(tmp);
    }
    else
    {
        m_fNO2ZeroCoef = 0.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","S02SpanCoef",tmp))
    {
        m_fS02SpanCoef = atof(tmp);
    }
    else
    {
        m_fS02SpanCoef = 1.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","SO2ZeroCoef",tmp))
    {
        m_fSO2ZeroCoef = atof(tmp);
    }
    else
    {
        m_fSO2ZeroCoef = 0.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","02SpanCoef",tmp))
    {
        m_f02SpanCoef = atof(tmp);
    }
    else
    {
        m_f02SpanCoef = 1.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","O2ZeroCoef",tmp))
    {
        m_fO2ZeroCoef = atof(tmp);
    }
    else
    {
        m_fO2ZeroCoef = 0.0;
    }

    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","FlowSpanCoef",tmp))
    {
        m_fFlowSpanCoef = atof(tmp);
    }
    else
    {
        m_fFlowSpanCoef = 1.0;
    }
    memset(tmp,0,64);
    if(GetAfxConfig()->GetIni("GasCoef","FlowZeroCoef",tmp))
    {
        m_fFlowZeroCoef = atof(tmp);
    }
    else
    {
        m_fFlowZeroCoef = 0.0;
    }

}

int CProcess2::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);
    char czChildPath[MAX_SIZE];
    char czPath[MAX_SIZE];
    char czDBName[MAX_SIZE];

    m_bSaveTag = false;
    QDate qDate = QDate::currentDate();

    getSpecLamdbaPixel(g_dLambda,m_NrPixels);

    memset(czChildPath,0,MAX_SIZE);
    memset(czDBName,0,MAX_SIZE);
    memset(czPath,0,MAX_SIZE);
    sprintf(czChildPath,"%d/data",this->GetProcessType());
    sprintf(czDBName,"%04d%02d%02d.db",qDate.year(),qDate.month(),qDate.day());
    CreateDataBase();
    if(m_pDB)
    {
        delete m_pDB;
        m_pDB = NULL;
    }
    m_pDB = new CDBBusiness(czChildPath,czDBName);
    return 0;
}

void CProcess2::StartGasTestCmd(QDateTime dt)
{
    m_qTestTime = dt;
    m_bRunThread = true;
}

int CProcess2::StartPreHeat()
{
    InitBeginOfTest();
    m_bRun = true;
    RecordLog(LOG_NOMAL,"仪器","开机启动流程");
    return 1;
}
//测试模块通讯是否正常
int CProcess2::StartInitYQ()
{
    char czTmp[MAX_SIZE];
    int nRet1 = TestM1Connect();
    int nRet2 = TestM2Connect();
    int nRet3 = TestM3Connect();
    int nRet4 = TestM4Connect();

    memset(czTmp,0,MAX_SIZE);
    GetAfxConfig()->GetIni("GasSetting","YQKind",czTmp);
    int nRet5 = 0;
    if(atoi(czTmp))//装配4-20mA
    {
        nRet5 = TestM5Connect();
    }
    if((0 == nRet1)&&(0 == nRet2)&&(0 == nRet3)&&(0 == nRet4)&&(0 == nRet5))
    {
        return 1;//所有模块通讯正常
    }
    else if(3 == nRet3)
    {
        return 2;//温度采集模块通讯异常时，则不显示预热界面，直接进入主界面，故需其返回值
    }
    else
    {
        return 3;//其它情况，有模块通讯异常，但没去判断是哪个模块
    }
}
int CProcess2::TestM1Connect()
{
    if(!SendControlCmd(CMD_SMOKEGAS,251,0,1))//主控M1
    {
        int ret = QMessageBox::information(NULL, "提示信息","M1模块通讯失败,请检查机箱内M1的通讯接口是否连接正常.\n"
                                                  "\"Retry\"将对模块重新测试,\"Ignore\"将忽略该模块的连接测试.",
                                               QMessageBox::Retry | QMessageBox::Ignore);
        switch (ret)
        {
          case QMessageBox::Retry:
                TestM1Connect();
              break;
          case QMessageBox::Ignore:
                {
                    int ret = QMessageBox::warning(NULL, "警告","浓度显示将会出现异常",
                                                   QMessageBox::Retry | QMessageBox::Ok);
                    switch (ret)
                    {
                        case QMessageBox::Retry:
                             TestM1Connect();
                             break;
                        case QMessageBox::Ok:
                             m_bM1Flag = true;
                             return 1;
                             break;
                    }

                }
              break;
          default:
                TestM1Connect();
              break;
        }
    }
    return 0;
}
int CProcess2::TestM2Connect()
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    if(!(pDriver1->ReadModbus(CMD_SMOKEGAS,40053,1,bBuff)))//主采M2
    {
        int ret = QMessageBox::information(NULL, "提示信息","M2模块通讯失败,请检查机箱内M2的通讯接口是否连接正常.\n"
                                                  "\"Retry\"将对模块重新测试,\"Ignore\"将忽略该模块的连接测试.",
                                               QMessageBox::Retry | QMessageBox::Ignore);
        switch (ret)
        {
          case QMessageBox::Retry:
                TestM2Connect();
              break;
          case QMessageBox::Ignore:
                {
                   int ret = QMessageBox::warning(NULL, "警告","浓度显示将会出现异常",
                                           QMessageBox::Retry | QMessageBox::Ok);
                    switch (ret)
                    {
                        case QMessageBox::Retry:
                             TestM2Connect();
                             break;
                        case QMessageBox::Ok:
                             m_bM2Flag = true;
                             return 2;
                             break;
                    }

                }
              break;
          default:
                TestM2Connect();
              break;
        }
    }
    return 0;
}
int CProcess2::TestM3Connect()
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    if(!(pDriver1->ReadModbus(CMD_VOCTempGrin,40054,1,bBuff)))//温控M3
    {
        int ret = QMessageBox::information(NULL, "提示信息","M3模块通讯失败,请检查机箱内M3的通讯接口是否连接正常.\n"
                                                  "\"Retry\"将对模块重新测试,\"Ignore\"将忽略该模块的连接测试.",
                                               QMessageBox::Retry | QMessageBox::Ignore);
        switch (ret)
        {
          case QMessageBox::Retry:
                TestM3Connect();
              break;
          case QMessageBox::Ignore:
                {
                    int ret = QMessageBox::warning(NULL, "警告","浓度显示将会出现异常",
                                                   QMessageBox::Retry | QMessageBox::Ok);
                    switch (ret)
                    {
                        case QMessageBox::Retry:
                             TestM3Connect();
                             break;
                        case QMessageBox::Ok:
                             m_bM3Flag = true;
                             return 3;
                             break;
                    }

                }
              break;
          default:
                TestM3Connect();
              break;
        }
    }
    return 0;
}
int CProcess2::TestM4Connect()
{
    if(initSpectrometer())//光谱仪M4
    {
        int ret = QMessageBox::information(NULL, "提示信息","M4模块通讯失败,请检查机箱内M4的通讯接口是否连接正常.\n"
                                                  "\"Retry\"将对模块重新测试,\"Ignore\"将忽略该模块的连接测试.",
                                               QMessageBox::Retry | QMessageBox::Ignore);
        switch (ret)
        {
          case QMessageBox::Retry:
                TestM4Connect();
              break;
          case QMessageBox::Ignore:
                {
                    int ret = QMessageBox::warning(NULL, "警告","浓度显示将会出现异常",
                                                           QMessageBox::Retry | QMessageBox::Ok);
                    switch (ret)
                    {
                        case QMessageBox::Retry:
                             TestM4Connect();
                             break;
                        case QMessageBox::Ok:
                             m_bM4Flag = true;
                             return 4;
                             break;
                    }
                }
              break;
          default:
                TestM4Connect();
              break;
        }
    }
    return 0;
}
int CProcess2::TestM5Connect()
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    if(!(pDriver->ReadModbus(0x11,40050,1,bBuff)))//4-20mA M5
    {
        int ret = QMessageBox::information(NULL, "提示信息","M5模块通讯失败,请检查机箱内M5的通讯接口是否连接正常.\n"
                                                  "\"Retry\"将对模块重新测试,\"Ignore\"将忽略该模块的连接测试.",
                                               QMessageBox::Retry | QMessageBox::Ignore);
        switch (ret)
        {
          case QMessageBox::Retry:
                TestM5Connect();
              break;
          case QMessageBox::Ignore:
                {
                    int ret = QMessageBox::warning(NULL, "警告","浓度输出将会出现异常.\n"
                                                              "建议拔插对应模块的电源和通讯接口或断电重启.",
                                                           QMessageBox::Retry | QMessageBox::Ok);
                    switch (ret)
                    {
                        case QMessageBox::Retry:
                             TestM5Connect();
                             break;
                        case QMessageBox::Ok:
                             m_bM5Flag = true;
                             return 5;
                             break;
                    }

                }
              break;
          default:
                TestM5Connect();
              break;
        }
    }
    return 0;
}
void CProcess2::DoProcess()
{ 
    CreateDataBase();
    ReadStatusData();
    CtrlTempChange();//温控

    if(m_bSpanTag)
    {
        if(m_SpacTime1.elapsed() > 30*1000)//30s输出一次报警
        {
            if(m_bStart)//进入主界面后才启动开关量报警
            {
                DealOutputAlarm();
            }
            m_bSpanTag = false;
        }
    }
    else
    {
        m_SpacTime1.start();
        m_bSpanTag = true;
    }
}

void CProcess2::DoThread()
{
    if(!m_bStartMaintain)//进入维护界面不执行流程
    {
        char cValue[MAX_SIZE];
        int nkind = 0;

        DealProcess1();
        if(m_bAutoCailTag)//正在自动校准中
        {
            m_nTimes++;
            if(m_nTimes >=17)//气体1校准后，需等待再校准气体2
            {
                m_bAutoCailTag = false;
                return;
            }
            memset(cValue,0,MAX_SIZE);
            GetAfxConfig()->GetIni("GasAutoCali","OrderS02",cValue);//气体校准顺序
            if(m_nTimes >= 6)//从第六次开始保存数据，连续保存10个数据
            {
                if((1 == atoi(cValue)&&m_bAutoCali[0])||(2 == atoi(cValue)&&m_bAutoCali[1]))//校准S02情况
                {
                    m_fAutoResult[m_nTimes-6] = m_fResultS02;
                    nkind = 1;
                }
                else
                {
                     m_fAutoResult[m_nTimes-6] = m_fResultN0;
                     nkind = 2;
                }

                if(m_nTimes>=16)
                {
                    m_bAutoCali[0] = !(m_bAutoCali[0]);
                    m_bAutoCali[1] = !(m_bAutoCali[1]);

                    SendControlCmd(CMD_SMOKEGAS,252,8,0);//输出低电平
                    //计算校准系数
                    if(m_bAutoSpanTag)
                    {
                        DealAutoSpanCali(nkind);
                    }
                    else
                    {
                        DealAutoZeroCali(nkind);
                    }
                }
            }
        }
        else
        {
            m_nTimes=0;
        }
        if(m_bCaliTag)//手动校准
        {
            memset(cValue,0,MAX_SIZE);
            GetAfxConfig()->GetIni("GasCaliType","CaliType",cValue);
            if(3 == atoi(cValue))//流速校准
            {
                DealFlowCaliCoef();
            }
            else
            {
                DealCaliCoef(atoi(cValue));
            }
        }
        if(m_bStartMaintain)//可以进入维护界面标志
        {
            if(m_pOnGasRunInfo)
            {
                m_pOnGasRunInfo(m_userdata,0);//通知进入模块维护界面
            }
        }
    }
    ConsumeManage();
}

void CProcess2::DealProcess1()
{
    bool bFaultFlag = false;//烟气主控制模块通讯故障标记
    if(!DealDCFStatus(CMD_SMOKEGAS,250,1,1))//打开电磁阀1
    {
        RecordLog(LOG_ALARM,"仪器","打开电磁阀1失败");
        bFaultFlag = true;
    }
    if(!DealDCFStatus(CMD_SMOKEGAS,250,2,0))//关闭电磁阀2
    {
        RecordLog(LOG_ALARM,"仪器","关闭电磁阀2失败");
        bFaultFlag = true;
    }
    if(!SendControlCmd(CMD_SMOKEGAS,251,1,0))//启动采样泵,关闭遮光片
    {
        RecordLog(LOG_ALARM,"仪器","打开泵或关闭遮光片失败");
        bFaultFlag = true;
    }
    sleep(2);
    getSpecData(g_dScope);//背景谱

    NotifyDrawCurve(2);
    GetCaclScope(2);
    if(!SendControlCmd(CMD_SMOKEGAS,251,1,1))//打开遮光片
    {
        RecordLog(LOG_ALARM,"仪器","打开遮光片失败");
        bFaultFlag = true;
    }
    sleep(1);
    getSpecData(g_dScope);//灯谱
    NotifyDrawCurve(3);
    GetCaclScope(3);
    if(!SendControlCmd(CMD_SMOKEGAS,251,0,0))//关闭采样泵,关闭遮光片
    {
        RecordLog(LOG_ALARM,"仪器","关闭采样泵或遮光片失败");
        bFaultFlag = true;
    }
    if(!DealDCFStatus(CMD_SMOKEGAS,250,2,1))//打开电磁阀2
    {
        RecordLog(LOG_ALARM,"仪器","打开电磁阀2失败");
        bFaultFlag = true;
    }
    if(!DealDCFStatus(CMD_SMOKEGAS,250,1,0))//关闭电磁阀1
    {
        RecordLog(LOG_ALARM,"仪器","关闭电磁阀1失败");
        bFaultFlag = true;
    }
    sleep(14);

    if(!SendControlCmd(CMD_SMOKEGAS,251,0,1))//打开遮光片
    {
        RecordLog(LOG_ALARM,"仪器","打开遮光片失败");
        bFaultFlag = true;
    }
    getSpecData(g_dScope);//测量谱
    NotifyDrawCurve(1);
    GetCaclScope(1);
    CaclTestResult();

    //更新通讯故障标记
    if(bFaultFlag)
    {
        m_bM1Flag = true;
    }
    else
    {
        m_bM1Flag = false;
    }

    //1小时保存一次光谱数据
    if(m_bScopeSaveFlag)
    {
        int spac = m_SpacTime2.elapsed();
        if(spac/1000 >= 3600)
        {
            SaveGasSpecData();
            m_bScopeSaveFlag = false;
        }
    }
    else{
        m_SpacTime2.start();
        m_bScopeSaveFlag = true;
    }
}

void CProcess2::DealProcess2()
{
    InitCaliCoef();
    SendControlCmd(CMD_SMOKEGAS,0xFB,0,1);//打开遮光片
    sleep(7);
    getSpecData(g_dScope);//背景谱
    NotifyDrawCurve(2);
    GetCaclScope(2);
    SendControlCmd(CMD_SMOKEGAS,0xFB,0,0);//关闭遮光片
    sleep(8);
    getSpecData(g_dScope);//测量谱

    NotifyDrawCurve(1);
    GetCaclScope(1);
    CaclTestResult();
    if(m_bScopeSaveFlag)
    {
        int spac = m_SpacTime4.elapsed();
        if(spac/1000 >= 30)
        {
            SaveGasSpecData();
            m_bScopeSaveFlag = false;
        }
    }
    else{
        m_SpacTime4.start();
        m_bScopeSaveFlag = true;
    }
}

void CProcess2::DealProcess3()
{
    getSpecData(g_dScope);//测量谱
    NotifyDrawCurve(1);
    GetCaclScope(1);
    SendControlCmd(CMD_SMOKEGAS,0xFA,1,1);//打开电磁阀
    SendControlCmd(CMD_SMOKEGAS,0xFB,1,1);//启动采样泵,打开遮光片
    sleep(2);
    getSpecData(g_dScope);//背景谱
    NotifyDrawCurve(2);
    GetCaclScope(2);
    SendControlCmd(CMD_SMOKEGAS,0xFB,1,0);//关闭遮光片
    sleep(13);
    getSpecData(g_dScope);//灯谱
    NotifyDrawCurve(3);
    GetCaclScope(3);
    SendControlCmd(CMD_SMOKEGAS,0xFB,0,0);//关闭采样泵
    SendControlCmd(CMD_SMOKEGAS,0xFA,1,0);//关闭电磁阀
    CaclTestResult();
    SaveGasSpecData();

}

void CProcess2::DealProcess4()
{
    SendControlCmd(CMD_SMOKEGAS,0xFB,0,1);//打开遮光片
    sleep(2);
    getSpecData(g_dScope);//背景谱
    NotifyDrawCurve(2);
    GetCaclScope(2);
    SendControlCmd(CMD_SMOKEGAS,0xFB,0,0);//关闭遮光片
    m_fFlowValue = 0.0;

    getSpecData(g_dScope);//灯谱
    NotifyDrawCurve(1);
    GetCaclScope(1);
    CaclTestResult();
    if(m_bScopeSaveFlag)
    {
        int spac = m_SpacTime3.elapsed();
        if(spac/1000 >= 30)
        {
            SaveGasSpecData();
            m_bScopeSaveFlag = false;
        }
    }
    else{
        m_SpacTime3.start();
        m_bScopeSaveFlag = true;
    }
}
void CProcess2::CaclTestResult()
{
    int i=0;
    char czTmp[MAX_SIZE];
    char czValue[MAX_SIZE];
    int ret = getConcenResult(m_aAnalysis,m_aresult);

    if (ret == 0)
    {
        qDebug("Get concentration succeed.");
    }
    else if (ret == 1)
    {
        RecordLog(LOG_FAULT,"仪器","灯谱采集异常");
    }
    else if (ret == 2)
    {
        RecordLog(LOG_FAULT,"仪器","测量谱采集异常");
    }
    else if (ret == 3)
    {
        RecordLog(LOG_WARNING,"仪器","基于上次计算，测量谱采集异常");
    }
    else if (ret == 4)
    {
        RecordLog(LOG_FAULT,"仪器","灯谱和测量谱采集异常");
    }
    else if (ret == 5)
    {
        RecordLog(LOG_FAULT,"仪器","数据长度异常");
    }
    else if (ret == 6)
    {
        RecordLog(LOG_FAULT,"仪器","背景谱采集异常");
    }

    if(!m_bStopFlag)
    {
        m_fResultN02 = m_fN02SpanCoef*(m_aresult->concData[0]-m_fNO2ZeroCoef);
        m_fResultS02 = m_fS02SpanCoef*(m_aresult->concData[1]-m_fSO2ZeroCoef);
        m_fResultN0 = m_fN0SpanCoef*(m_aresult->concData[2]-m_fNOZeroCoef);
        m_fResult02 = m_struTemp.fOxygen;
    }
    else
    {
        m_fResultN02 = 0.0;
        m_fResultS02 = 0.0;
        m_fResultN0 = 0.0;
        m_fResult02 = 0.0;
    }

    memset(czTmp,0,MAX_SIZE);
    GetAfxConfig()->GetIni("GasSetting","YQKind",czTmp);

    memset(czValue,0,MAX_SIZE);
    GetAfxConfig()->GetIni("GasInputSet","InputKeep",czValue);
    if(!(m_bHighLevel&&(1 == atoi(czValue))))//不保持上次输出
    {
        m_fOutPutN0 = m_fResultN0;
        m_fOutPutS02 = m_fResultS02;
        m_fOutPut02 = m_fResult02;
    }
    if(atoi(czTmp))
    {
        DealOutputResult(m_fOutPutS02,m_fOutPutN0,m_fOutPut02);//4-20mA输出
    }
    if(m_pOnGasResult)
    {
        m_pOnGasResult(m_userdata,m_fResultN0,m_fResultN02,m_fResultS02,m_fResult02);
    }
    SaveResultData();//保存测试结果与吸光度
}

void CProcess2::ReadStatusData()
{
    bool bM2Flag = false;//标记主采是否通讯正常
    bool bM3Flag = false;//标记温采是否通讯正常
    unsigned char bBuff[MAX_SIZE];
    float tempValue = 0.0;
     memset(bBuff,0,MAX_SIZE);

     for(int i=0;i<3;i++)
     {
         if(pDriver1->ReadModbus(CMD_VOCTempGrin,40054,4,bBuff))
         {
             u_int16_t nLightTemp = MAKEWORD(bBuff[1],bBuff[0]);
             u_int16_t nSpecTemp = MAKEWORD(bBuff[3],bBuff[2]);
             u_int16_t nBoxTemp = MAKEWORD(bBuff[5],bBuff[4]);
             u_int16_t nHeatingTemp = MAKEWORD(bBuff[7],bBuff[6]);

             m_struTemp.fLightTemp = nLightTemp/10.0;
             m_struTemp.fSpecTemp = nSpecTemp/10.0;
             m_struTemp.fHeatingTemp = nHeatingTemp/10.0;
             m_struTemp.fBoxTemp = nBoxTemp/10.0;
             m_StatusValue[0] = m_struTemp.fLightTemp;
             m_StatusValue[1] = m_struTemp.fSpecTemp;
             m_StatusValue[3] = m_struTemp.fHeatingTemp;
             m_StatusValue[6] = m_struTemp.fBoxTemp;
             bM3Flag = true;
             break;
         }
         else
         {
             qDebug("Read CMD_VOCTempGrin i:%d",i);
         }
     }

     getSpecTemp(tempValue);//获取光谱仪温度,即板载温度
     m_struTemp.fOnboardTemp = tempValue;
     m_StatusValue[2] = m_struTemp.fOnboardTemp;
     for(int j=0;j<3;j++)
     {
         if(pDriver1->ReadModbus(CMD_SMOKEGAS,40053,6,bBuff))
         {
             u_int16_t nGasIn = MAKEWORD(bBuff[1],bBuff[0]);
             u_int16_t nGasOut = MAKEWORD(bBuff[3],bBuff[2]);
             u_int16_t nFlow = MAKEWORD(bBuff[5],bBuff[4]);
             u_int16_t nOxygen = MAKEWORD(bBuff[9],bBuff[8]);

             int nIO1 = bBuff[11] & 0x01; // 最低位
             int nIO2 = ( bBuff[11] & 0x02) >> 1;
             int nIO3 = ( bBuff[11] & 0x04) >> 2;
             int nIO4 = ( bBuff[11] & 0x08) >> 3;
             DealInputStatus(nIO1,nIO2,nIO3,nIO4);//处理输入通道状态

             m_struTemp.fGasIn = nGasIn/100.0;
             m_struTemp.fGasOut = nGasOut/100.0;
             m_struTemp.fFlow = m_fFlowSpanCoef*(0.02963*pow((nFlow/100.0),3.0)-0.19921*pow((nFlow/100.0),2.0)+0.52865*(nFlow/100.0)-0.37459-m_fFlowZeroCoef);//经过校准后


             float fOxygen = (nOxygen/100.0);
             if(fOxygen > 200.0)
             {
                 fOxygen = 0.0;
             }
             m_struTemp.fOxygen = m_f02SpanCoef*(25.0*fOxygen/15.5-m_fO2ZeroCoef);//校准后的值

             if(m_struTemp.fOxygen<0.0)
             {
                 m_struTemp.fOxygen = 0.0;
             }

             if(m_struTemp.fFlow<0.0)
             {
                 m_struTemp.fFlow = 0.0;
             }
             m_fFlowValue += m_struTemp.fFlow;

             m_StatusValue[4] = m_struTemp.fGasIn;
             m_StatusValue[5] = m_struTemp.fGasOut;
             m_StatusValue[7] = m_struTemp.fFlow;
             m_StatusValue[8] = m_struTemp.fOxygen;
             bM2Flag = true;
             break;
         }
         else
         {
             qDebug("Read CMD_SMOKEGAS j:%d",j);
         }
     }
     for(int i=0;i<9;i++)
     {
         qDebug("m_StatusValue[%d]:%f",i,m_StatusValue[i]);
     }
     //启动时一直获取光谱仪数据，目的是为了预热，进入主界面后m_bStart变true
     if(!m_bStart)
     {
         getSpecData(g_dScope);
     }
     if(m_bCaliTag&&m_nCountCaliFlow<5)
     {
        m_fCaliFlowSpan[m_nCountCaliFlow] = m_StatusValue[7];
        m_fCaliFlowZero[m_nCountCaliFlow] = m_StatusValue[7];
        m_nCountCaliFlow++;
     }
     if(m_pOnStatusValue)
     {
         m_pOnStatusValue(m_userdata,&m_struTemp);
     }
     SaveStatusData();
     setStatusValue(m_StatusValue,9);
     //更新通讯是否故障标记
     if(!bM2Flag)
     {
         m_bM2Flag = true;
     }
     else
     {
         m_bM2Flag = false;
     }
     if(!bM3Flag)
     {
          m_bM3Flag = true;
     }
     else
     {
         m_bM3Flag = false;
     }
}

void CProcess2::SaveStatusData()
{
    char czID[16];
    if(m_bSaveTag)
    {
        int spac = m_SaveSpacTime.elapsed();
        if(spac/1000 >= 30)
        {
            m_bSaveTag = false;
            QDateTime dt = QDateTime::currentDateTime();
            for(int i=0;i<YQSTATUSCOUNT-1;i++)//不保存氧含量状态
            {
                memset(czID,0,16);
                sprintf(czID,"G0%d",i);
                m_pDB->InsGasStatus(dt,czID,m_StatusValue[i]);
            }

        }
    }
    else{
        m_SaveSpacTime.start();
        m_bSaveTag = true;
    }
}

void CProcess2::NotifyDrawCurve(int nType)
{
     m_struVector.SpecType = nType;
    if(m_bDrawCurve)//进入曲线界面，才描绘曲线
    {
        m_struVector.m_vtnLamdba.clear();
        m_struVector.m_vtnScope.clear();

        for(int j=0;j<m_NrPixels;j++)
        {
            m_struVector.m_vtnScope.push_back(g_dScope[j]);
            m_struVector.m_vtnLamdba.push_back(g_dLambda[j]);
        }
        if(m_pOnScopeData)
        {
            m_pOnScopeData(m_userdata,&m_struVector,m_struVector.SpecType);
        }
    }
}
//将获取的测量谱 灯谱 背景谱保存至m_aAnalysis中
void CProcess2::GetCaclScope(int nType)
{
    int size = sizeof(g_dScope)/sizeof(g_dScope[0]);
    if(1 == nType)
    {
       m_aAnalysis->specLen[2] = size;

       for(int i=0;i<size;i++)
       {
           m_aAnalysis->measSpec[i] = g_dScope[i]-m_aAnalysis->darkSpec[i];
       }
    }
    else if(2 == nType)
    {
       m_aAnalysis->specLen[0] = size;
       for(int i=0;i<size;i++)
       {
           m_aAnalysis->darkSpec[i] = g_dScope[i];
       }
    }
    else if(3 == nType)
    {
       m_aAnalysis->specLen[1] = size;
       for(int i=0;i<size;i++)
       {
           m_aAnalysis->lampSpec[i] = g_dScope[i]-m_aAnalysis->darkSpec[i];
       }
    }
    else
    {
       //do nothing
    }

}
//进入主界面就不再对关闭风扇进行处理
//进入主界面后m_bStart变true
void CProcess2::CtrlTempChange()
{
    float fLightHigh = 44.5;
    float fLightLow = 43.5;
    float fSpecHigh = 22.5;
    float fSpecLow = 21.5;
//    float fBoxHigh = 25.0;
//    float fBoxLow = 20.0;
    //光源风扇
    if(m_struTemp.fLightTemp>fLightHigh)
    {
        if(!m_bLightTag)
        {
            if(SendControlCmd(CMD_SMOKEGAS,0xFD,1,1))//打开继电器输出2通道
            {
                SendControlCmd(CMD_SMOKEGAS,0xFD,2,1);//打开继电器输出3通道
                m_bLightTag = true;
                RecordLog(LOG_NOMAL,"仪器","打开光源风扇成功");
                cout<< "Open light source fan succeed." <<endl;
            }
            else
            {
                RecordLog(LOG_ALARM,"仪器","打开光源风扇失败");
                cout<< "Open light source fan failed." <<endl;
            }
        }

    }
    if(m_struTemp.fLightTemp<fLightLow)
    {
        if(m_bLightTag&&(!m_bStart))//进入主界面后m_bStart变true
        {
            if(SendControlCmd(CMD_SMOKEGAS,0xFD,1,0))//关闭继电器输出2通道
            {
                SendControlCmd(CMD_SMOKEGAS,0xFD,2,0);//打开继电器输出3通道
                m_bLightTag = false;
                RecordLog(LOG_NOMAL,"仪器","关闭光源风扇成功");
                cout<< "Close light source fan succeed" <<endl;
            }
            else
            {
                RecordLog(LOG_ALARM,"仪器","关闭光源风扇失败");
                cout<< "Close light source fan failed." <<endl;
            }
        }
    }
    //光谱仪风扇
    if(m_struTemp.fSpecTemp>fSpecHigh)
    {
        if(!m_bSpecTag)
        {
            if(SendControlCmd(CMD_SMOKEGAS,0xFD,3,1))//打开继电器输出3通道
            {
                m_bSpecTag = true;
                RecordLog(LOG_NOMAL,"仪器","打开光谱仪风扇成功");
                cout<< "Open spectrometer fan succeed." <<endl;
            }
            else
            {
                RecordLog(LOG_ALARM,"仪器","打开光谱仪风扇失败");
                cout<< "Open spectrometer fan failed." <<endl;
            }
        }

    }
    if(m_struTemp.fSpecTemp<fSpecLow)
    {
        if(m_bSpecTag&&(!m_bStart))//进入主界面后m_bStart变true
        {
            if(SendControlCmd(CMD_SMOKEGAS,0xFD,3,0))//关闭继电器输出3通道
            {
                m_bSpecTag = false;
                RecordLog(LOG_NOMAL,"仪器","关闭光谱仪风扇成功");
                cout<< "Close spectrometer fan succeed." <<endl;
            }
            else
            {
                RecordLog(LOG_ALARM,"仪器","关闭光谱仪风扇失败");
                cout<< "Close spectrometer fan failed." <<endl;
            }
        }
    }
//    //机箱风扇
//    if(m_struTemp.fBoxTemp>fBoxHigh)
//    {
//        if(!m_bBoxTag)
//        {
//            if(SendControlCmd(CMD_SMOKEGAS,0xFD,3,1))//打开继电器输出3通道
//            {
//                m_bBoxTag = true;
//                RecordLog(LOG_NOMAL,"仪器","打开机箱风扇成功");
//            }
//            else
//            {
//                RecordLog(LOG_ALARM,"仪器","打开机箱风扇失败");
//            }
//        }

//    }
//    if(m_struTemp.fBoxTemp<fBoxLow)
//    {
//        if(m_bBoxTag&&(!m_bStart))//进入主界面后m_bStart变true
//        {
//            if(SendControlCmd(CMD_SMOKEGAS,0xFD,3,0))//关闭继电器输出3通道
//            {
//                m_bBoxTag = false;
//                RecordLog(LOG_NOMAL,"仪器","关闭机箱风扇成功");
//            }
//            else
//            {
//                RecordLog(LOG_ALARM,"仪器","关闭机箱风扇失败");
//            }
//        }
//    }
}
//第10通道表示第一二三四路温控同时启动
bool CProcess2::SendControlTemp(int nAdd,int cmd,int nIO,int nOnOff)
{
    int16_t wValue = 0;
    bool bRet = false;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = cmd; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);
    wValue = nIO;//通道
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = nOnOff;
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    for(int i=0;i<5;i++)
    {
        if (pDriver1->WriteModbus(nAdd,40001,3,bBuff))
        {
            bRet = true;
            break;
        }
        else
        {
            //RecordLog(LOG_WARNING,"仪器","写命令超时");
        }
    }
    return bRet;
}
bool CProcess2::SendControlCmd(int nAdd,int cmd,int nIO,int nOnOff)
{
    int16_t wValue = 0;
    bool bRet = false;
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    wValue = cmd; //命令字
    bBuff[0] = HIBYTE(wValue);
    bBuff[1] = LOBYTE(wValue);
    wValue = nIO;//通道
    bBuff[2] = HIBYTE(wValue);
    bBuff[3] = LOBYTE(wValue);
    wValue = nOnOff;//开关或4-20mA输出值
    bBuff[4] = HIBYTE(wValue);
    bBuff[5] = LOBYTE(wValue);
    for(int i=0;i<5;i++)
    {
        if (pDriver->WriteModbus(nAdd,40001,3,bBuff))
        {
            bRet = true;
            break;
        }
        else
        {
            //RecordLog(LOG_WARNING,"仪器","写命令超时");
        }
    }
    return bRet;
}
bool CProcess2::DealDCFStatus(int nAdd,int cmd,int nIO,int nOnOff)
{
    bool bRet = false;
    for(int i=0;i<3;i++)
    {
        if(SendControlCmd(nAdd,cmd,nIO,nOnOff))
        {
            if(ReadDCFStatus(nIO,nOnOff))
            {
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}
bool CProcess2::ReadDCFStatus(int nIO,int nOnOff)
{
    unsigned char bBuff[MAX_SIZE];
    int nStatus[3];
    bool bRet = false;
    for(int i=0;i<3;i++)
    {
        usleep(30000);
        memset(bBuff,0,MAX_SIZE);
        if (pDriver->ReadModbus(CMD_SMOKEGAS,40056,1,bBuff))
        {
            //获取电磁阀状态,当为1时相应的电磁阀打开，0时为关闭
            nStatus[0] = bBuff[1] & 0x01; //电磁阀1状态
            nStatus[1] = ( bBuff[1] & 0x02) >> 1;//电磁阀2状态
            nStatus[2] = ( bBuff[1] & 0x04) >> 2;//电磁阀3状态
            if(nStatus[nIO-1] == nOnOff)//读到的状态与写的状态比较
            {
               bRet = true;
               break;
            }
            else
            {
                if(nOnOff)
                {
                    RecordLog(LOG_ALARM,"仪器","电磁阀打开失败");
                }
                else
                {
                    RecordLog(LOG_ALARM,"仪器","电磁阀关闭失败");
                }
            }
        }
    }
    return bRet;
}
void CProcess2::SaveGasSpecData()
{
    QDateTime qTime = QDateTime::currentDateTime();

    char czChildPath[256];memset(czChildPath,0,256);
    char czDataName[256];memset(czDataName,0,256);
    char czDataFile[256];memset(czDataFile,0,256);
    sprintf(czChildPath,"%s/%d/specdata",DB_ROOT_PATH,this->GetProcessType());
    mkdir_r(czChildPath);
    sprintf(czDataName,"%04d%02d%02d-%02d.txt",qTime.date().year(),qTime.date().month(),qTime.date().day(),qTime.time().hour());
    sprintf(czDataFile,"%s/%s",czChildPath,czDataName);

    QString qsTxtFileName = QString(czDataFile);
    QFile file(qsTxtFileName);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
       QTextStream stream(&file);
       stream << "Lambda"<<"\t"<<"darkSpec"<<"\t"<<"lampSpec"<<"\t"<<"measSpec"<<"\r\n";
       for(int i = 0;i < totalPixel; i++)
       {
           stream <<g_dLambda[i]<<"\t"<<m_aAnalysis->darkSpec[i]<<"\t"<<m_aAnalysis->lampSpec[i]<<"\t"<<m_aAnalysis->measSpec[i]<<"\r\n";
       }
       file.close();
    }
}

void CProcess2::CreateDataBase()
{
    char czChildPath[MAX_SIZE];
    char czRmName[MAX_SIZE];
    char czPath[MAX_SIZE];
    char czDBName[MAX_SIZE];
    CDBManager db;

    memset(czChildPath,0,MAX_SIZE);
    memset(czDBName,0,MAX_SIZE);
    memset(czPath,0,MAX_SIZE);
    sprintf(czChildPath,"%d/data",this->GetProcessType());
    QDate qDate = QDate::currentDate();
    sprintf(czDBName,"%04d%02d%02d.db",qDate.year(),qDate.month(),qDate.day());
    sprintf(czPath,"%s/%s/%s",DB_ROOT_PATH,czChildPath,czDBName);
    QString qsPath = QString(czPath);
    if(!QFile::exists(qsPath))
    {

        QDir *createfile     = new QDir;
        memset(czRmName,0,MAX_SIZE);
        memset(czPath,0,MAX_SIZE);
        QDateTime qTime = m_qTestTime.addDays(-30);
        sprintf(czRmName,"%04d%02d%02d.db",qTime.date().year(),qTime.date().month(),qTime.date().day());
        sprintf(czPath,"%s/%s/%s",DB_ROOT_PATH,czChildPath,czRmName);
        QString qsRmPath = QString(czPath);
        createfile->remove(qsRmPath);

        db.Create_2_Absorbance(czChildPath,czDBName);//创建吸光度表
        db.Create_DB_2091(czChildPath,czDBName);//日志
        db.Create_DB_Status(czChildPath,czDBName);//状态
        db.Create_DB_8051(czChildPath,czDBName);//结果数据
        db.Create_2_SpectrumInfo(czChildPath,czDBName);//系数与积分时间 次数
        delete createfile;

        if(m_pDB)
        {
            delete m_pDB;
            m_pDB = NULL;
        }
        m_pDB = new CDBBusiness(czChildPath,czDBName);
    }
}
//处理校准系数
void CProcess2::DealCaliCoef(int nCaliKind)
{
    char cValue[64];
    float fCoef = 0.0;
    float fAverageN0 = 0.0;
    float fAverageS02 = 0.0;
    float fAverage02 = 0.0;
    float fSumN0 = 0.0;
    float fSumS02 = 0.0;
    float fSum02 = 0.0;
    float fValue = 0.0;
    float fabsValue = 0.0;

    if(m_nCountCali < 5)
    {
        m_fCaliValueN0[m_nCountCali] = m_fResultN0;
        m_fCaliValueS02[m_nCountCali] = m_fResultS02;
        m_fCaliValue02[m_nCountCali] = m_fResult02;
        m_nCountCali++;
    }
    if(m_nCountCali >= 5)//保存五组数据后
    {
        //取得五组数据，再求平均
        m_nCountCali = 0;
        for(int i=0;i<5;i++)
        {
            fSumN0 += m_fCaliValueN0[i];
            fSumS02 += m_fCaliValueS02[i];
            fSum02 += m_fCaliValue02[i];
        }
        fAverageN0 = fSumN0/5;
        fAverageS02 = fSumS02/5;
        fAverage02 = fSum02/5;

        if(1 == nCaliKind)//跨度校准
        {
            if(0 == m_nType)
            {
                fValue = ChangeChromaToPPM(1);
                fabsValue = fabs(fAverageN0 - fValue);
                if(fabsValue>=100.0)
                {
                    //do nothing
                }
                else if(fabsValue<=50.0)
                {
                    m_fN0SpanCoef = (fValue/fAverageN0)*m_fN0SpanCoef;//atof(cValue)/m_fResultN0值与上次校准系数相乘，得到新的校准系数
                    fCoef = m_fN0SpanCoef;
                    setCurrentState('2');//通知计算库，一氧化氮校准
                }
                else if(fabsValue>50.0&&fabsValue<100.0)
                {                  
                    int ret = QMessageBox::information(NULL, "提示信息",""
                                                              "请确认标气的浓度值、单位与设定信息是否一致?",
                                                           QMessageBox::Yes | QMessageBox::No);
                    switch (ret)
                    {
                      case QMessageBox::Yes:
                           {
                                m_fN0SpanCoef = (fValue/fAverageN0)*m_fN0SpanCoef;//atof(cValue)/m_fResultN0值与上次校准系数相乘，得到新的校准系数
                                fCoef = m_fN0SpanCoef;
                                setCurrentState('2');//通知计算库，一氧化氮校准
                            }
                          break;
                      case QMessageBox::No:
                          break;
                      default:
                          break;
                    }
                }

            }
            else if(1 == m_nType)
            {
                fValue = ChangeChromaToPPM(2);
                fabsValue = fabs(fAverageS02 - fValue);
                if(fabsValue>=100.0)
                {
                    QMessageBox::information(NULL, "提示信息","校准失败!",QMessageBox::Ok);
                }
                else if(fabsValue<=50.0)
                {
                    m_fS02SpanCoef = (atof(cValue)/fAverageS02)*m_fS02SpanCoef;
                    fCoef = m_fS02SpanCoef;
                    setCurrentState('1');//通知计算库，二氧化硫校准
                }
                else if(fabsValue>50.0&&fabsValue<100.0)
                {                  
                    int ret = QMessageBox::information(NULL, "提示信息","请确认标气的浓度值、单位与设定信息是否一致?",
                                                           QMessageBox::Yes | QMessageBox::No);
                    switch (ret)
                    {
                      case QMessageBox::Yes:
                           {
                                m_fS02SpanCoef = (atof(cValue)/fAverageS02)*m_fS02SpanCoef;
                                fCoef = m_fS02SpanCoef;
                                setCurrentState('1');//通知计算库，二氧化硫校准
                            }
                          break;
                      case QMessageBox::No:
                          break;
                      default:
                          break;
                    }
                }
            }
            else if(2 == m_nType)
            {
                memset(cValue,0,64);
                GetAfxConfig()->GetIni("GasHandCali","Conc02",cValue);
                fValue = atof(cValue);
                fabsValue = fabs(fAverage02 - fValue);
                if(fabsValue>=100.0)
                {
                    //do nothing
                }
                else if(fabsValue<=50.0)
                {
                    m_f02SpanCoef = (atof(cValue)/fAverage02)*m_f02SpanCoef;
                    fCoef= m_f02SpanCoef;
                }
                else if(fabsValue>50.0&&fabsValue<100.0)
                {               
                    int ret = QMessageBox::information(NULL, "提示信息","请确认标气的浓度值、单位与设定信息是否一致?",
                                                           QMessageBox::Yes | QMessageBox::No);
                    switch (ret)
                    {
                      case QMessageBox::Yes:
                           {
                                m_f02SpanCoef = (atof(cValue)/fAverage02)*m_f02SpanCoef;
                                fCoef= m_f02SpanCoef;
                                qDebug("m_f02SpanCoef %f",m_f02SpanCoef);
                            }
                          break;
                      case QMessageBox::No:
                          break;
                      default:
                          break;
                    }
                }
            }
        }
        else if(2 == nCaliKind)//零点校准
        {
            if(0 == m_nType)
            {
                m_fNOZeroCoef += fAverageN0;//m_fResultN0值与上次零点校准系数累加，得到新的校准系数
                m_fNOZeroCoef = m_fNOZeroCoef/m_fN0SpanCoef;
                fCoef = m_fNOZeroCoef;
            }
            else if(1 == m_nType)
            {
                m_fSO2ZeroCoef += fAverageS02;
                m_fSO2ZeroCoef = m_fSO2ZeroCoef/m_fS02SpanCoef;
                fCoef = m_fSO2ZeroCoef;
            }
            else if(2 == m_nType)
            {
                m_fO2ZeroCoef += fAverage02;
                m_fO2ZeroCoef = m_fO2ZeroCoef/m_f02SpanCoef;
                fCoef = m_fO2ZeroCoef;
            }
        }
        if(m_pOnGasCoef)
        {
            m_pOnGasCoef(m_userdata,fCoef);
        }
        m_bCaliTag = false;
    }
}
//处理流速校准系数
void CProcess2::DealFlowCaliCoef()
{
    char cValue[64];
    float fCoef = 0.0;
    float fAverageSpan = 0.0;
    float fAverageZero = 0.0;
    float fSumSpan = 0.0;
    float fSumZero = 0.0;
    float fValue = 0.0;
    float fabsValue = 0.0;

    memset(cValue,0,64);
    GetAfxConfig()->GetIni("GasFlowCali","SettingFlow",cValue);
    fValue = atof(cValue);
    if(0 == fValue)//设定流速为0时,系统默认为零点校准
    {
        //执行流速零点校准流程中
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,1,0))//关闭电磁阀1
        {
            RecordLog(LOG_ALARM,"仪器","关闭电磁阀1失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,2,0))//关闭电磁阀2
        {
            RecordLog(LOG_ALARM,"仪器","关闭电磁阀2失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFB,0,0))//关闭采样泵,关闭遮光片
        {
            RecordLog(LOG_ALARM,"仪器","关闭泵或遮光片失败");
        }
        m_nCountCaliFlow = 0;
        //等侍读取流速五组数据完成
        while(m_nCountCaliFlow>=5)
        {
            //取得五组数据，再求平均
            for(int i=0;i<5;i++)
            {
                fSumZero += m_fCaliFlowZero[i];
            }
            fAverageZero = fSumZero/5;
            fabsValue = fabs(fAverageZero - fValue);
            if(fabsValue < 0.5)
            {
                m_fFlowZeroCoef += fAverageZero;
                m_fFlowZeroCoef = m_fFlowZeroCoef/m_fFlowSpanCoef;
                fCoef = m_fFlowZeroCoef;
            }
            else
            {           
                QMessageBox::information(NULL, "提示信息","校准失败!",QMessageBox::Ok);
            }
            break;
        }

        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,1,1))//打开电磁阀1
        {
            RecordLog(LOG_ALARM,"仪器","打开电磁阀1失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,2,1))//打开电磁阀2
        {
            RecordLog(LOG_ALARM,"仪器","打开电磁阀2失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFB,1,0))//打开采样泵,关闭遮光片
        {
            RecordLog(LOG_ALARM,"仪器","打开泵或遮光片失败");
        }
    }
    else
    {
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,1,1))//打开电磁阀1
        {
            RecordLog(LOG_ALARM,"仪器","打开电磁阀1失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,2,1))//打开电磁阀2
        {
            RecordLog(LOG_ALARM,"仪器","打开电磁阀2失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFB,0,0))//关闭采样泵,关闭遮光片
        {
            RecordLog(LOG_ALARM,"仪器","关闭泵或遮光片失败");
        }
        m_nCountCaliFlow = 0;
        while(m_nCountCaliFlow>=5)
        {
            //取得五组数据，再求平均
            for(int i=0;i<5;i++)
            {
                fSumSpan += m_fCaliFlowSpan[i];
            }
            fAverageSpan = fSumSpan/5;       
            if(fabsValue < 0.5)
            {
                m_fFlowSpanCoef = (atof(cValue)/fAverageSpan)*m_fFlowSpanCoef;
                fCoef = m_fFlowSpanCoef;
            }
            else
            {
                QMessageBox::information(NULL, "提示信息","校准失败!",QMessageBox::Ok);
            }
            break;
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,1,0))//关闭电磁阀1
        {
            RecordLog(LOG_ALARM,"仪器","关闭电磁阀1失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFA,2,0))//关闭电磁阀2
        {
            RecordLog(LOG_ALARM,"仪器","关闭电磁阀2失败");
        }
        if(!SendControlCmd(CMD_SMOKEGAS,0xFB,1,0))//打开采样泵,关闭遮光片
        {
            RecordLog(LOG_ALARM,"仪器","打开泵或遮光片失败");
        }
    }

    if(m_pOnGasCoef)
    {
        m_pOnGasCoef(m_userdata,fCoef);
    }
    m_bCaliTag = false;
}
//将单位转化为ppm
float CProcess2::ChangeChromaToPPM(int nKind)
{
    float fValue = 0.0;
    char cValue[MAX_SIZE];
    if(1 == nKind)
    {
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasHandCali","ConcN0",cValue);
        fValue = atof(cValue);
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasHandCali","UnitN0",cValue);
        if(atoi(cValue))
        {
            fValue = fValue*(22.4/30);
        }

    }
    else if(2 == nKind)
    {
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasHandCali","ConcS02",cValue);
        fValue = atof(cValue);
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasHandCali","UnitS02",cValue);
        if(atoi(cValue))
        {
            fValue = fValue*(22.4/64);
        }
    }
    return fValue;
}
void CProcess2::RecordLog(LOG_LEVEL Level,const char *Type,const char *Content)
{
    QDateTime qDateTime = QDateTime::currentDateTime();
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());

    if(m_pDB)
    {
        m_pDB->Ins2091(DateTime,this->GetProcessType(),Type,(int)Level, Content);
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
//保存测试结果，吸光度及系数
void CProcess2::SaveResultData()
{
    QDateTime qCurrTime = QDateTime::currentDateTime();
    for(int k=0;k<YQMAXCOUNT;k++)
    {
        if(m_pYQ[k])
        {

            memset(m_pYQ[k]->m_RealInfo.cDataID,0,16);
            if(m_bM1Flag||m_bM2Flag||m_bM3Flag||m_bM4Flag||m_bM5Flag)
            {
                strcpy(m_pYQ[k]->m_RealInfo.cDataID,"E");
            }
            if(m_bMaintainFlag)//仪器维护
            {
                strcat(m_pYQ[k]->m_RealInfo.cDataID,"M");
            }
            if(m_bCaliTag||m_bAutoCailTag)//仪器校准
            {
                strcat(m_pYQ[k]->m_RealInfo.cDataID,"C");
            }
            memcpy(m_pYQ[k]->m_RealInfo.czID,m_pYQ[k]->m_sYQ.czID,strlen(m_pYQ[k]->m_sYQ.czID));
            m_pYQ[k]->m_RealInfo.TestTime = qCurrTime;
            if(!strcmp(m_pYQ[k]->m_sYQ.czID,"ZA0"))
            {
                m_pYQ[k]->m_RealInfo.fResult = m_fResultN0;

            }else if(!strcmp(m_pYQ[k]->m_sYQ.czID,"ZA1"))
            {
                m_pYQ[k]->m_RealInfo.fResult = m_fResultN02;
            }else if(!strcmp(m_pYQ[k]->m_sYQ.czID,"ZB2"))
            {
                m_pYQ[k]->m_RealInfo.fResult = m_fResultS02;
            }else if(!strcmp(m_pYQ[k]->m_sYQ.czID,"ZB4"))
            {
                m_pYQ[k]->m_RealInfo.fResult = m_fResult02;
            }
            //将测试数据及标识，保存至输出寄存器
            m_pYQ[k]->Set8051(m_pYQ[k]->m_RealInfo.fResult);
            m_pYQ[k]->SetWaterTag(m_pYQ[k]->m_RealInfo.cDataID,strlen(m_pYQ[k]->m_RealInfo.cDataID));

            m_pDB->Ins8051(m_pYQ[k]->m_RealInfo,this->GetProcessType());//保存结果数据
        }
    }
    //qDebug("SaveResultData:absData[0]:%f,absData[1]:%f,absData[2]:%f",m_aresult->absData[0],m_aresult->absData[1],m_aresult->absData[2]);
    //qDebug("SaveResultData:coefData[0]:%f,coefData[1]:%f,coefData[3]:%f,coefData[4]:%f,coefData[5]:%f",m_aresult->coefData[0],m_aresult->coefData[1],m_aresult->coefData[2],m_aresult->coefData[3],m_aresult->coefData[4]);
    m_pDB->InsGasAbsorbance(qCurrTime,m_aresult->absData[0],m_aresult->absData[1],m_aresult->absData[2]);//保存吸光度
    m_pDB->InsSpectrumInfo(qCurrTime,m_aresult->coefData[0],m_aresult->coefData[1],m_aresult->coefData[2],m_aresult->coefData[3],m_aresult->coefData[4]);//保存系数
}
void CProcess2::SpanCaliRestoreFactorySetting()
{
    m_fN0SpanCoef = 1.0;
    GetAfxConfig()->SetIni("GasCoef","N0SpanCoef","1.0");

    m_fN02SpanCoef = 1.0;
    GetAfxConfig()->SetIni("GasCoef","N02SpanCoef","1.0");

    m_fS02SpanCoef = 1.0;
    GetAfxConfig()->SetIni("GasCoef","S02SpanCoef","1.0");

    m_f02SpanCoef = 1.0;
    GetAfxConfig()->SetIni("GasCoef","02SpanCoef","1.0");

    setCurrentState('3');//通知计算库，恢复出厂设置
}
void CProcess2::ZeroCaliRestoreFactorySetting()
{
    m_fNOZeroCoef = 0.0;
    GetAfxConfig()->SetIni("GasCoef","NOZeroCoef","0.0");

    m_fNO2ZeroCoef = 0.0;
    GetAfxConfig()->SetIni("GasCoef","NO2ZeroCoef","0.0");

    m_fSO2ZeroCoef = 0.0;
    GetAfxConfig()->SetIni("GasCoef","SO2ZeroCoef","0.0");

    m_fO2ZeroCoef = 0.0;
    GetAfxConfig()->SetIni("GasCoef","O2ZeroCoef","0.0");

    setCurrentState('3');//通知计算库，恢复出厂设置
}
void CProcess2::FlowCaliRestoreFactorySetting()
{
    m_fFlowSpanCoef = 1.0;
    GetAfxConfig()->SetIni("GasCoef","FlowSpanCoef","1.0");

    m_fFlowZeroCoef = 0.0;
    GetAfxConfig()->SetIni("GasCoef","FlowZeroCoef","0.0");

    setCurrentState('3');//通知计算库，恢复出厂设置
}
//界面调用此接口获取光谱仪光谱数据
void CProcess2::GetScopeData(struGasVector* pScope)
{
    pScope->SpecType = m_struVector.SpecType;
    for(int j=0;j<m_NrPixels;j++)
    {
         pScope->m_vtnScope.push_back(g_dScope[j]);
         pScope->m_vtnLamdba.push_back(g_dLambda[j]);
    }
}
//处理开关量输入状态
void CProcess2::DealInputStatus(int nIO1, int nIO2, int nIO3, int nIO4)
{
    char czValue[MAX_SIZE];
    char czGroup[MAX_SIZE];
    char czTemp[MAX_SIZE];

    int nIO[5];
    if(nIO1||nIO2||nIO3||nIO4)//有高电平输入
    {
        m_bHighLevel = true;
    }
    else
    {
        m_bHighLevel = false;
    }

    nIO[0] = 0;
    nIO[1] = nIO1;
    nIO[2] = nIO2;
    nIO[3] = nIO3;
    nIO[4] = nIO4;
    if(!m_bAutoCailTag)//未执行自动校准
    {
        for(int i=1;i<5;i++)//4个通道循环
        {
            memset(czTemp,0,MAX_SIZE);
            memset(czGroup,0,MAX_SIZE);

            strcpy(czGroup,"InputIO");
            sprintf(czTemp,"%d",i);
            strcat(czGroup,czTemp);

            GetAfxConfig()->GetIni("GasInputSet",czGroup,czValue);
            if(0 == atoi(czValue))//系统维护
            {
                if(nIO[i])
                {
                    m_bMaintainFlag = true;
                }
                else
                {
                    m_bMaintainFlag = false;
                }
            }
            else if(1 == atoi(czValue))//排放停炉
            {
                if(nIO[i])//i通道
                {
                    m_bStopFlag = true;
                }
                else
                {
                    m_bStopFlag = false;
                }
            }
            else if(2 == atoi(czValue))//跨度校准
            {
                if(nIO[i])
                {
                    SendControlCmd(CMD_SMOKEGAS,252,8,1);
                    m_bAutoCailTag = true;
                    m_bAutoSpanTag = true;
                    m_nTimes = 0;
                }
            }
            else if(3 == atoi(czValue))//零点校准
            {
                if(nIO[i])
                {
                    SendControlCmd(CMD_SMOKEGAS,252,8,1);
                    m_bAutoCailTag = true;
                    m_bAutoSpanTag = false;//标记零点校准
                    m_nTimes = 0;
                }
            }
            else if(4 == atoi(czValue))//保留
            {
                //do nothing
            }
        }
    }
}
bool CProcess2::DealAutoSpanCali(int nKind)
{
    float fSum = 0.0;
    float fAverage = 0.0;
    float fValue = 0.0;
    char cValue[MAX_SIZE];
    char cUint[MAX_SIZE];
    char tmp[MAX_SIZE];

    if(1 == nKind)
    {
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","WaveS02",cValue);
        for(int i=0;i<10;i++)
        {
            //分析10次数据，每两个相邻数据间的差值同时不超过波动值
            if(m_fAutoResult[i] - m_fAutoResult[i+1]>atof(cValue))
            {
                return false;
            }
            fSum = +m_fAutoResult[i];
        }
        fAverage = fSum/10;
        memset(cUint,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","UnitS02",cUint);
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","ChromaS02",cValue);
        fValue = atof(cValue);
        if(1 == atoi(cUint))
        {
            fValue = fValue*(22.4/64);
        }

        m_fS02SpanCoef = (fValue/fAverage)*m_fS02SpanCoef;
        //保存校准系数
        memset(tmp,0,MAX_SIZE);
        sprintf(tmp,"%f",m_fS02SpanCoef);
        GetAfxConfig()->SetIni("GasCoef","S02SpanCoef",tmp);
    }
    else if(2 == nKind)
    {
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","WaveN0",cValue);
        for(int i=0;i<10;i++)
        {
            //分析10次数据，每两个相邻数据间的差值同时不超过波动值
            if(m_fAutoResult[i] - m_fAutoResult[i+1]>atof(cValue))
            {
                return false;
            }
            fSum = +m_fAutoResult[i];
        }
        fAverage = fSum/10;

        memset(cUint,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","UnitN0",cUint);
        memset(cValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasAutoCali","ChromaN0",cValue);
        fValue = atof(cValue);
        if(1 == atoi(cUint))
        {
            fValue = fValue*(22.4/30);
        }
        m_fN0SpanCoef = (fValue/fAverage)*m_fN0SpanCoef;
        //保存校准系数
        memset(tmp,0,MAX_SIZE);
        sprintf(tmp,"%f",m_fN0SpanCoef);
        GetAfxConfig()->SetIni("GasCoef","N0SpanCoef",tmp);
    }
}
bool CProcess2::DealAutoZeroCali(int nKind)
{
    float fSum = 0.0;
    float fAverage = 0.0;
    char tmp[MAX_SIZE];

    for(int i=0;i<10;i++)
    {
        fSum = +m_fAutoResult[i];
    }

    fAverage = fSum/10;
    if(1 == nKind)//二氧化硫
    {
        m_fSO2ZeroCoef += fAverage;
        m_fSO2ZeroCoef = m_fSO2ZeroCoef/m_fS02SpanCoef;
        //保存校准系数
        memset(tmp,0,MAX_SIZE);
        sprintf(tmp,"%f",m_fSO2ZeroCoef);
        GetAfxConfig()->SetIni("GasCoef","SO2ZeroCoef",tmp);

    }
    else if(2 == nKind)//一氧化氮
    {
        m_fNOZeroCoef += fAverage;//m_fResultN0值与上次零点校准系数累加，得到新的校准系数
        m_fNOZeroCoef = m_fNOZeroCoef/m_fN0SpanCoef;
        //保存校准系数
        memset(tmp,0,MAX_SIZE);
        sprintf(tmp,"%f",m_fNOZeroCoef);
        GetAfxConfig()->SetIni("GasCoef","NOZeroCoef",tmp);
    }

}
//4-20mA输出
void CProcess2::DealOutputResult(float fS02,float fN0,float f02)
{
    char czValue[MAX_SIZE];
    char czGroup[MAX_SIZE];
    char czTemp[MAX_SIZE];
    int ADValue = 0;
    int LCMin = 0;
    int LCMax = 0;
    int ADMin = 65535/5;
    int ADMax = 65535;
    bool bFaultFlag = false;//通讯故障标记

    int nIO[5];

    nIO[0] = 0;
    nIO[1] = 0;
    nIO[2] = 1;
    nIO[3] = 2;
    nIO[4] = 3;
    for(int i=1;i<5;i++)//4个通道循环
    {
        memset(czTemp,0,MAX_SIZE);
        memset(czGroup,0,MAX_SIZE);

        strcpy(czGroup,"OutputIO");
        sprintf(czTemp,"%d",i);
        strcat(czGroup,czTemp);

        memset(czValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasOutputSet",czGroup,czValue);
        if(0 == atoi(czValue))//二氧化硫
        {
            LCMax = GetDataLCMax(1);
            ADValue = (ADMax - ADMin)*(fS02 - LCMin)/(LCMax - LCMin) - ADMin;
            if(!SendControlCmd(17,310,nIO[i],ADValue))
            {
                RecordLog(LOG_ALARM,"仪器","二氧化硫AD值输出失败");
                bFaultFlag = true;
            }
        }
        else if(1 == atoi(czValue))//一氧化氮
        {
            LCMax = GetDataLCMax(2);
            ADValue = (ADMax - ADMin)*(fN0 - LCMin)/(LCMax - LCMin) - ADMin;
            if(!SendControlCmd(17,310,nIO[i],ADValue))
            {
                RecordLog(LOG_ALARM,"仪器","一氧化氮AD值输出失败");
                bFaultFlag = true;
            }
        }
        else if(2 == atoi(czValue))//氧含量
        {
             LCMax = 25;
             ADValue = (ADMax - ADMin)*(f02 - LCMin)/(LCMax - LCMin) - ADMin;
             if(!SendControlCmd(17,310,nIO[i],ADValue))
             {
                 RecordLog(LOG_ALARM,"仪器","氧含量AD值输出失败");
                 bFaultFlag = true;
             }
        }
        else if(3 == atoi(czValue))//保留
        {
               //do nothing
        }
    }
    //记录通讯故障
    if(bFaultFlag)
    {
        m_bM5Flag = true;
    }
    else
    {
        m_bM5Flag = false;
    }
}
//输出配置报警(状态量 超量程 故障报警)
void CProcess2::DealOutputAlarm()
{
    bool bStatus = false;
    char czValue[MAX_SIZE];
    char czTmp[MAX_SIZE];

    float fGasAverge = (m_StatusValue[4] + m_StatusValue[5])/2.0;
    if(fGasAverge>148.0||fGasAverge<142.0)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",fGasAverge);
        strcpy(czValue,"气室温度异常:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        bStatus = true;
    }
    if(m_StatusValue[0]>60.0||m_StatusValue[0]<50.0)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_StatusValue[0]);
        strcpy(czValue,"光源温度异常:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        bStatus = true;
    }
    if(m_StatusValue[2]>33.0||m_StatusValue[2]<23.0)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_StatusValue[1]);
        strcpy(czValue,"探测器温度异常:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        bStatus = true;
    }
    if(m_StatusValue[6]>45.0||m_StatusValue[6]<20.0)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_StatusValue[6]);
        strcpy(czValue,"机箱温度异常:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        bStatus = true;
    }
    if(m_StatusValue[7]>1.5||m_StatusValue[7]<0.5)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_StatusValue[7]);
        strcpy(czValue,"流速异常:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        bStatus = true;
    }
    if(bStatus)
    {
        if(!m_bAlarmFlag[0])
        {
            SendControlCmd(CMD_SMOKEGAS,252,1,1);//输出高电平
            m_bAlarmFlag[0] = true;
        }
    }
    else
    {
        if(m_bAlarmFlag[0])
        {
            SendControlCmd(CMD_SMOKEGAS,252,1,0);//输出高电平
            m_bAlarmFlag[0] = false;
        }
    }

    if(m_fResultS02>GetDataLCMax(1))
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_fResultS02);
        strcpy(czValue,"二氧化硫超量程:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        if(!m_bAlarmFlag[1])
        {
            SendControlCmd(CMD_SMOKEGAS,252,2,1);//输出高电平
            m_bAlarmFlag[1] = true;
        }
    }
    else
    {
        if(m_bAlarmFlag[1])
        {
            SendControlCmd(CMD_SMOKEGAS,252,2,0);//输出高电平
            m_bAlarmFlag[1] = false;
        }
    }

    if(m_fResultN0>GetDataLCMax(2))
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_fResultN0);
        strcpy(czValue,"一氧化氮超量程:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        if(!m_bAlarmFlag[2])
        {
            SendControlCmd(CMD_SMOKEGAS,252,3,1);//输出高电平
            m_bAlarmFlag[2] = true;
        }
    }
    else
    {
        if(m_bAlarmFlag[3])
        {
            SendControlCmd(CMD_SMOKEGAS,252,3,0);//输出高电平
            m_bAlarmFlag[3] = false;
        }
    }

    if(m_fResult02 > 25.0)
    {
        memset(czValue,0,MAX_SIZE);
        memset(czTmp,0,MAX_SIZE);
        sprintf(czTmp,"%0.2f",m_fResult02);
        strcpy(czValue,"氧含量超量程:");
        strcat(czValue,czTmp);
        RecordLog(LOG_ALARM,"仪器",czValue);
        if(!m_bAlarmFlag[4])
        {
            SendControlCmd(CMD_SMOKEGAS,252,4,1);//输出高电平
            m_bAlarmFlag[4] = true;
        }
    }
    else
    {
        if(m_bAlarmFlag[4])
        {
            SendControlCmd(CMD_SMOKEGAS,252,4,0);//输出高电平
            m_bAlarmFlag[4] = false;
        }
    }

    if(m_bM1Flag)
    {
        if(!m_bAlarmFlag[7])
        {
            SendControlCmd(CMD_SMOKEGAS,252,7,1);//输出高电平
            m_bAlarmFlag[7] = true;
        }
    }
    else
    {
        if(m_bAlarmFlag[7])
        {
            SendControlCmd(CMD_SMOKEGAS,252,7,0);//输出高电平
            m_bAlarmFlag[7] = false;
        }
    }
}
//获取量程最大值
int CProcess2::GetDataLCMax(int nKind)
{
    char czValue[MAX_SIZE];
    memset(czValue,0,MAX_SIZE);
    int LCMax = 250;
    if(1 == nKind)
    {
        GetAfxConfig()->GetIni("GasSetting","S02Range",czValue);
        if(0 == atoi(czValue))
        {
            LCMax = 250;
        }
        else
        {
            LCMax = 500;
        }
    }
    else
    {
        memset(czValue,0,MAX_SIZE);
        GetAfxConfig()->GetIni("GasSetting","N0Range",czValue);
        if(0 == atoi(czValue))
        {
            LCMax = 250;
        }
        else
        {
            LCMax = 500;
        }
    }
    return LCMax;
}
//耗材管理
void CProcess2::ConsumeManage()
{
    //1小时保存一次光谱数据
    if(m_bConsumeFlag)
    {
        int spac = m_SpacConsume.elapsed();
        if(spac/1000 >= 3600)
        {
            ConsumeSub(this->GetProcessType(),"HC_J",-1);//光源
            ConsumeSub(this->GetProcessType(),"HC_K",-1);//氧传感器
            ConsumeSub(this->GetProcessType(),"HC_L",-0.25);//电机
            ConsumeSub(this->GetProcessType(),"HC_M",-0.25);//采样泵
            ConsumeSub(this->GetProcessType(),"HC_N",-1);//风扇
            ConsumeSub(this->GetProcessType(),"HC_O",-1);//制冷片
            m_bConsumeFlag = false;
        }
    }
    else{
            m_SpacConsume.start();
            m_bConsumeFlag = true;
    }
}
void CProcess2::NotifyAlarmInfo()
{
    QString qsContent;
    if(m_bM1Flag||m_bM2Flag||m_bM3Flag||m_bM4Flag||m_bM5Flag)
    {
        if(m_bM1Flag)
        {
            qsContent.append("M1");
        }
        if(m_bM2Flag)
        {
            qsContent.append("M2");
        }
        if(m_bM3Flag)
        {
            qsContent.append("M3");
        }
        if(m_bM4Flag)
        {
            qsContent.append("M4");
        }
        if(m_bM5Flag)
        {
            qsContent.append("M5");
        }
        qsContent.append("模块通讯故障");
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,qsContent.toLatin1().data(),4);
        }
    }
    else
    {
        if(m_pOnAlarm)
        {
            m_pOnAlarm(m_userdata,qsContent.toLatin1().data(),LOG_NOMAL);
        }
    }
}
