#include "cprocess6.h"
#include "VocArithmetic.h"
CProcess6::CProcess6()
{
    m_bReadRefValue = false;
    m_bOnlyOnceTag = false;
    m_pCalc = new CVocArithmetic();
    m_bRunThread = true;
}

CProcess6::~CProcess6()
{
    if(m_pCalc)
    {
        delete m_pCalc;
        m_pCalc = NULL;
    }
}

int CProcess6::GetProcessType()
{
    return 6;
}
//初使化测试
//nKind为1表示手动测试 2表示网络测试 3表示自动测试
int CProcess6::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);

    m_bReadRefValue = false;
    m_bOnlyOnceTag = false;
    sqlite_data datalist;
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

            pYQ->m_RealInfo.m_BsDataInfo.BaseValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.SampleValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.Absorbency = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.K = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.B = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.R = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = 0.0;//空白吸光度
            pYQ->m_RealInfo.CheckChroma = 0.0;//标样浓度
            pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = 0.0;//标样吸光度
            memset(pYQ->m_RealInfo.cDataID,0,16);
            if (1 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"hd",2);
            }
            else if(2 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"nt",2);
            }
            char czCurveID[64];memset(czCurveID,0,64);
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
                pYQ->m_CurveInfo.SampleAdjust = 1;
            }

            pYQ->m_RealInfo.m_EchemDataInfo.fLeftBase = pYQ->m_CurveInfo.m_EchemCurve.PeakSearchDown;
            pYQ->m_RealInfo.m_EchemDataInfo.fRighBase = pYQ->m_CurveInfo.m_EchemCurve.PeakSearchUp;
            pYQ->m_RealInfo.nCalWay =  pYQ->m_CurveInfo.CalcMode;


        }
    }
    datalist.Free();
    return 0;
}

//处理当前pid为系统模块的PID
int CProcess6::GetSysModuleStatus()
{
    int nRet = MCU_IDEL;
    //工作流程处理
    switch (m_pro.PID)
    {
    case 51://读暗信号----零点
        if(m_pSpectrum->ReadSpectrumData(DarkSignal) == 0)
        {
            if(m_pOnShowCurve)
                m_pOnShowCurve(m_userdata, m_pSpectrum->m_vtfWaveLen, m_pSpectrum->m_vtnDarkSignal,DarkSignal);
        }
        else
        {
            nRet = SPEC_FAULT;
        }
        break;
    case 13://读参比信号---基线值
        m_bReadRefValue = true;//读了参比信号,则用老的计算方式
        if(m_pSpectrum->ReadSpectrumData(RefSignal) == 0)
        {
            if(m_pOnShowCurve)
                m_pOnShowCurve(m_userdata, m_pSpectrum->m_vtfWaveLen, m_pSpectrum->m_vtnRefSignal,RefSignal);
        }
        else
        {
            nRet = SPEC_FAULT;
        }
        break;
    case 11://读测试信号---样品值
        if(m_pSpectrum->ReadSpectrumData(TestSignal) == 0)
        {
            if(m_pOnShowCurve)
                m_pOnShowCurve(m_userdata, m_pSpectrum->m_vtfWaveLen, m_pSpectrum->m_vtnTestSignal,TestSignal);
        }
        else
        {
            nRet = SPEC_FAULT;
        }
        break;
    case 52://计算结果方式
        Calculate(m_pro.arrParaValue[0],(bool)m_pro.arrParaValue[1]);
        break;
    }
    return nRet;
}

void CProcess6::SaveSrcDataToTxt()
{
    if(m_bOnlyOnceTag)
       return;
    if(m_pSpectrum->m_vtfWaveLen.size() > 0 && m_pSpectrum->m_vtnDarkSignal.size() > 0 && m_pSpectrum->m_vtfWaveLen.size() == m_pSpectrum->m_vtnDarkSignal.size())
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
            if(m_pSpectrum->m_vtnRefSignal.size() > 0)
            {
                stream << "ID"<<"\t"<<"Wave"<<"\t"<<"Dark"<<"\t"<<"Refe"<<"\t"<<"Test"<< "\r\n";
                for ( int i = 0; i < m_pSpectrum->m_vtfWaveLen.size(); i++ )
                    stream <<i+1<<"\t"<< m_pSpectrum->m_vtfWaveLen.at(i)<<"\t"<<m_pSpectrum->m_vtnDarkSignal.at(i)<<"\t"<<m_pSpectrum->m_vtnRefSignal.at(i)<<"\t"<<m_pSpectrum->m_vtnTestSignal.at(i)<< "\r\n";
            }
            else
            {
                stream << "ID"<<"\t"<<"Wave"<<"\t"<<"Dark"<<"\t"<<"Test"<< "\r\n";
                for ( int i = 0; i < m_pSpectrum->m_vtfWaveLen.size(); i++ )
                    stream <<i+1<<"\t"<< m_pSpectrum->m_vtfWaveLen.at(i)<<"\t"<<m_pSpectrum->m_vtnDarkSignal.at(i)<<"\t"<<m_pSpectrum->m_vtnTestSignal.at(i)<< "\r\n";
            }
            file.close();
        }
        m_bOnlyOnceTag = true;
    }
}

void CProcess6::SaveAbsorbDataToTxt()
{
    if(m_pSpectrum->m_vtfWaveLen.size() > 0 && m_pSpectrum->m_vtfAbsorbency.size() > 0 && m_pSpectrum->m_vtfWaveLen.size() == m_pSpectrum->m_vtfAbsorbency.size())
    {
        char czChildPath[256];memset(czChildPath,0,256);
        char czDataName[256];memset(czDataName,0,256);
        char czDataFile[256];memset(czDataFile,0,256);
        sprintf(czChildPath,"%s/%d/data",DB_ROOT_PATH,this->GetProcessType());
        mkdir_r(czChildPath);
        sprintf(czDataName,"%04d%02d%02d-%02d%02dAbs.txt",m_Qtime.date().year(),m_Qtime.date().month(),m_Qtime.date().day(),m_Qtime.time().hour(),m_Qtime.time().minute());
        sprintf(czDataFile,"%s/%s",czChildPath,czDataName);

        QString qsTxtFileName = QString(czDataFile);
        QFile file(qsTxtFileName);
        if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream(&file);
            stream << "ID"<<"\t"<<"Wave"<<"\t"<<"Abs"<< "\r\n";
            for ( int i = 0; i < m_pSpectrum->m_vtfWaveLen.size(); i++ )
                stream <<i+1<<"\t"<< m_pSpectrum->m_vtfWaveLen.at(i)<<"\t"<<m_pSpectrum->m_vtfAbsorbency.at(i)<< "\r\n";

            file.close();
        }
    }
}

void CProcess6::Calculate(int nCode,bool bCalcAbsorb)
{
    CYQ* pYQ = NULL;
    //3.根据峰计算测试结果
    for(int i = 0;i < YQMAXCOUNT;i++)
    {
        if(m_pYQ[i] && m_pYQ[i]->GetProcessType() == 6)
        {
            if(m_pYQ[i]->m_sYQ.InTypeID == nCode)
            {
                 pYQ = m_pYQ[i];
                 break;
            }
        }
    }
    if(!pYQ)
    {
         RecordLog(LOG_FAULT,"计算结果流程设置错误,找到不对应的参数代码");
         return;
    }

    //1.保存源始数据
    SaveSrcDataToTxt();
    //2.计算吸光度等
    int nRet = 0;
    if(bCalcAbsorb)//如果第二个参数是1，则表示计算吸光度等,否则不需要计算,跟前面的相同的吸光度等
    {
        nRet = m_pSpectrum->CalcAbsorb(!m_bReadRefValue,pYQ->m_CurveInfo.m_EchemCurve.BaseWaveLength);
        if(nRet < 0)
            RecordLog(LOG_ALARM,"计算吸度失败,数据未准备好");
        else
            SaveAbsorbDataToTxt();//保存吸光度
       if(nRet == 1)
            RecordLog(LOG_WARNING,"光路异常");
    }
    if(nRet >= 0)
    {
        char czValue[64];memset(czValue,0,64);
        //消解时间
        float fHeatTime = 0.0;
        float fTempValue = 0.0;
        memset(czValue,0,64);
        GetAfxConfig()->GetIni("TempSet","HeatTime",czValue);
        if(strlen(czValue) > 0)
        {
            fHeatTime = atoi(czValue);
        }
        //消解温度
        memset(czValue,0,64);
        GetAfxConfig()->GetIni("TempSet","TempValue",czValue);
        if(strlen(czValue) > 0)
        {
            fTempValue = atof(czValue);
        }

        double fAbsorb = 0.0;
        double fValue = 0.0;
        m_pSpectrum->CalcTestValue(pYQ->m_CurveInfo.CalcMode,pYQ->m_CurveInfo.m_EchemCurve.PeakSearchDown,pYQ->m_CurveInfo.m_EchemCurve.PeakSearchUp,fAbsorb);
        pYQ->m_RealInfo.m_BsDataInfo.Absorbency = fAbsorb;

        if(pYQ->m_CurveInfo.CheckChroma > 0)
        {
            //标样浓度误差 =标样浓度 -( K * (标样吸光度 - 空白吸光度) + B
            //浓度 = K * (X - 空白吸光度) + B +(-) 标样浓度误差
            float fErrSampChroma = 0.0;//标样的误差
            fErrSampChroma = pYQ->m_CurveInfo.CheckChroma - (pYQ->m_CurveInfo.K * (pYQ->m_CurveInfo.StdAbsorb - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B);
            fValue = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B + fErrSampChroma;
        }
        else
        {
            fValue = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B ;
        }
        if(fValue < 0)
            fValue = 0;
        else
            fValue *= pYQ->m_CurveInfo.SampleAdjust;


        pYQ->m_RealInfo.m_BsDataInfo.BaseValue = pYQ->m_CurveInfo.m_EchemCurve.PeakSearchDown;
        pYQ->m_RealInfo.m_BsDataInfo.SampleValue = pYQ->m_CurveInfo.m_EchemCurve.PeakSearchUp;
        pYQ->m_RealInfo.m_BsDataInfo.K = pYQ->m_CurveInfo.K;
        pYQ->m_RealInfo.m_BsDataInfo.B = pYQ->m_CurveInfo.B;
        pYQ->m_RealInfo.m_BsDataInfo.R = pYQ->m_CurveInfo.R;
        pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = pYQ->m_CurveInfo.BlankAbsorb;//空白吸光度
        pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = pYQ->m_CurveInfo.StdAbsorb;//标样吸光度
        pYQ->m_RealInfo.CheckChroma = pYQ->m_CurveInfo.CheckChroma;//标样浓度
        pYQ->m_RealInfo.m_BsDataInfo.CurveID = pYQ->m_CurveInfo.CurveID;

        pYQ->m_RealInfo.nHeatTime = fHeatTime;
        pYQ->m_RealInfo.fHeatValue = fTempValue;
        if(CMD_TEST == m_nCmd)
        {
            pYQ->m_RealInfo.fResult = fValue;
        }else if(CMD_SAMPLETEST == m_nCmd)
        {
            pYQ->m_RealInfo.fCheckResult = fValue;
        }
        //流程正常结束,在此保存结果
        this->SaveResultToDatabase(nCode);

        char str[64];
        memset(str,0,64);
        sprintf(str,"浓度:%0.4f,CurveID=%d,Absorb=%.4f",pYQ->m_RealInfo.fResult,pYQ->m_RealInfo.m_BsDataInfo.CurveID,pYQ->m_RealInfo.m_BsDataInfo.Absorbency);
        DebugInfo(str,3);

        //是否自动切换曲线
        memset(czValue,0,64);
        GetAfxConfig()->GetIni("AutoAdjust_SET","RangeAutoChangeSet",czValue);
        if(!strcmp(czValue,"1") && m_nCmd == CMD_TEST)//仅当是测试命令时才自动切换曲线
        {
            if(VerifyRange(pYQ->m_sYQ.czID,pYQ->m_CurveInfo.CurveID,(double)pYQ->m_RealInfo.fResult))
            {
                ;
            }
            else//不切换曲线,标识位置FALSE
            {
                m_bUpSwitchCurve = false;
                m_bDowSwitchCurve = false;
            }
        }
    }
}
void CProcess6::SaveResultToDatabase(int nCodeID)
{
    if(CMD_TEST == m_nCmd || CMD_SAMPLETEST == m_nCmd)
    {
        CProcess::SaveResultToDatabase(nCodeID);
    }
    else
    {
        if(m_nFinishResult != PRO_OK)//流程异常
            return;

        CYQ* pYQ = NULL;
        for(int i=0;i<YQMAXCOUNT;i++)
        {
            pYQ = m_pYQ[i];
            if(pYQ && (nCodeID == 0 || pYQ->m_sYQ.InTypeID == nCodeID))
            {
                if(m_nCmd == CMD_BANKADJUST)//空白校准
                {
                    if(m_pOnTestFinished)
                    {
                        //这里要计算空白校准的误差率(范围：20%)
                        //空白校准、标样校准测出来的吸光度与当前参数第1个点的吸光度进行比较，
                        //如果误差范围小于20%时，则修正 m_CurveManger4（曲线管理）当前工作曲线空白吸光度值
                        //注意：空白校准比较吸光度，标样校准比较浓度
                        sqlite_data datalist;
                        bool isResult = m_pDB->GetCurveDataInfo(pYQ->m_sYQ.czID,1,0,datalist);
                        if(isResult)
                        {
                            if(!datalist.IsEmpty())
                            {
                                float fAbsorbency = atof(datalist.GetData(1,6));
                                if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency != 0 && fAbsorbency != 0)
                                {
                                    if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency / fAbsorbency * 100 < 20)
                                    {
                                        m_pOnTestFinished(m_userdata,CMD_BANKADJUST,pYQ->m_sYQ.czID);
                                    }
                                }
                            }
                        }
                        datalist.Free();
                    }
                }
                else if(m_nCmd == CMD_SAMPLEADJUST)
                {
                    if(m_pOnTestFinished)
                    {
                        //这里要计算标样校准的误差率(范围：20%)
                        if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency != 0 &&  pYQ->m_CurveInfo.CheckChroma != 0)
                        {
                            if(pYQ->m_RealInfo.fResult / pYQ->m_CurveInfo.CheckChroma * 20 < 20)
                            {
                                m_pOnTestFinished(m_userdata,CMD_SAMPLEADJUST,pYQ->m_sYQ.czID);
                            }
                        }
                    }
                }
                pYQ = NULL;
            }
        }
    }
}
