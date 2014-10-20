#include "cdbbusiness.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <QStringList>
#include "cconfig.h"
using namespace std;

CDBBusiness::CDBBusiness(const char* czChildPath, const char *czName)
{
    char czDBFile[256];memset(czDBFile,0,256);
    if(czChildPath)
        sprintf(czDBFile,"%s/%s/%s",DB_ROOT_PATH,czChildPath,czName);
    else
        sprintf(czDBFile,"%s/%s",DB_ROOT_PATH,czName);
    pDB = new CMyDataBase(czDBFile);
}
CDBBusiness::~CDBBusiness()
{
    delete pDB;
}

bool CDBBusiness::GetYQs(vector<SYQPARAM>& sctYQs)
{
    if(pDB->GetYQs(sctYQs))
    {
        int iYQCount = sctYQs.size();
        for(int i=0;i<iYQCount;i++)
        {
            GetYQParamFromINI(sctYQs[i]);
        }
        return true;
    }
    return false;
}
bool CDBBusiness::GetYQCurves(const vector<SYQPARAM> sctYQs,vector<struCurveInfo>& sctCurves)
{
    int iYQCount = sctYQs.size();
    for(int i=0;i<iYQCount;i++)
    {
        struCurveInfo curve;
        memcpy(curve.czID,sctYQs[i].czID,16);
        GetYQCurveFromINI(curve);
        voc_GetYQCurveFromDb(curve);
        sctCurves.push_back(curve);
    }
    return true;
}
void CDBBusiness::voc_GetYQCurveFromDb(struCurveInfo &sYQ)
{
    QString str;
    sqlite_data datalist;
    pDB->GetCurve(datalist,sYQ.czID);

    str = datalist.GetData(1,2);
    sYQ.m_VocCurve.RemainTime = str.toFloat();
    str = datalist.GetData(1,3);
    sYQ.m_VocCurve.Kind = str.toInt();
    str = datalist.GetData(1,4);
    sYQ.K = str.toFloat();
    str = datalist.GetData(1,5);
    sYQ.B = str.toFloat();
    str = datalist.GetData(1,6);
    sYQ.BlankAbsorb = str.toFloat();
    str = datalist.GetData(1,7);
    sYQ.R = str.toFloat();
    str = datalist.GetData(1,8);
    sYQ.m_VocCurve.WinFactor = str.toFloat();
    str = datalist.GetData(1,9);
    sYQ.m_VocCurve.MinPW = str.toInt();
    str = datalist.GetData(1,10);
    sYQ.m_VocCurve.MaxPW = str.toInt();
    str = datalist.GetData(1,11);
    sYQ.m_VocCurve.Minimum = str.toFloat();
    str = datalist.GetData(1,12);
    sYQ.m_VocCurve.MinArea = str.toFloat();
}
void CDBBusiness::GetYQCurveFromINI(struCurveInfo &sYQ)
{
    char czGroup[64];memset(czGroup,0,64);
    char czValue[64];
    sprintf(czGroup,"%s_SET",sYQ.czID);


    //寻峰下限
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"PeakSearchDown",czValue);
    sYQ.m_EchemCurve.PeakSearchDown = atoi(czValue);
    //寻峰上限
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"PeakSearchUp",czValue);
    sYQ.m_EchemCurve.PeakSearchUp = atoi(czValue);

    //光谱仪使用,基线波长
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"BaseWaveLength",czValue);
    sYQ.m_EchemCurve.BaseWaveLength = atoi(czValue);


    //计算方式
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"CalcWay",czValue);
    sYQ.CalcMode = atoi(czValue);

    //标样浓度
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"SampleChroma",czValue);
    sYQ.m_EchemCurve.SouceStandValue = atof(czValue);

    //核查标样浓度
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"CheckChroma",czValue);
    sYQ.CheckChroma = atof(czValue);

    //空白浓度
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"BlankChroma",czValue);
    sYQ.m_EchemCurve.BlankChroma = atof(czValue);
    //标样校准系数
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"SampleAdjust",czValue);
    sYQ.SampleAdjust = atof(czValue);
    //气体积分时间
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"IntegrationTime",czValue);
    sYQ.m_GasCurve.IntegrationTime = atof(czValue);
    //气体积分次数
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"IntegrationNum",czValue);
    sYQ.m_GasCurve.IntegrationNum = atoi(czValue);
    //气体反演系数b
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"ModulusB",czValue);
    sYQ.m_GasCurve.ModulusB = atoi(czValue);
    //气体反演系数a
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"ModulusA",czValue);
    sYQ.m_GasCurve.ModulusA = atoi(czValue);

}

void CDBBusiness::GetYQParamFromINI(SYQPARAM& sYQ)
{
    char czGroup[64];memset(czGroup,0,64);
    char czValue[64];
    sprintf(czGroup,"%s_SET",sYQ.czID);
    //读取类型
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"ProcessType",czValue);
    sYQ.iProcessType = atoi(czValue);
    //读取参数名称
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"Name",czValue);
    strcpy(sYQ.czName,czValue);

    //仪器内部代码
    char czCodeID[16];
    memset(czCodeID,0,16);
    memcpy(czCodeID,sYQ.czID+1,2);
    sYQ.InTypeID = atoi(czCodeID);

    //Modbus地址
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"SlaveID",czValue);
    sYQ.SlaveID = atoi(czValue);

    //数据精度
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"DataDigits",czValue);
    sYQ.DataDigits = atoi(czValue);

    //浓度数据超上限
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"Max",czValue);
    sYQ.Max = atof(czValue);
    //浓度数据超下限
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"Min",czValue);
    sYQ.Min = atof(czValue);

    //气体单位类型，1表示ppm，2表示mg/m3
    memset(czValue,0,64);
    GetAfxConfig()->GetIni(czGroup,"UnitType",czValue);
    sYQ.UnitType = atoi(czValue);

    //检出上限
     memset(czValue,0,64);
     GetAfxConfig()->GetIni(czGroup,"TopLimit",czValue);
     sYQ.TopLimit = atof(czValue);
    //检出下限
     memset(czValue,0,64);
     GetAfxConfig()->GetIni(czGroup,"LowerLimit",czValue);
     sYQ.LowerLimit = atof(czValue);
}

bool CDBBusiness::InsYQ(SYQPARAM& sYQ)
{
    return pDB->InsYQ(sYQ);
}
bool CDBBusiness::Voc_InsYQ(SYQPARAM& sYQ)
{
    bool bRet = pDB->InsYQ(sYQ);
    if(bRet)
    {
        bRet = pDB->Voc_InitDot(sYQ.czID);
        bRet &= pDB->Voc_InitCurve(sYQ.czID);
    }
    return bRet;
}


bool CDBBusiness::DelYQ(SYQPARAM& sYQ)
{
    return pDB->DelYQ(sYQ);
}

bool CDBBusiness::Voc_DelYQ(SYQPARAM& sYQ)
{
    bool bRet = pDB->DelYQ(sYQ);
    if(bRet)
    {
        bRet = pDB->Voc_DelDot(sYQ.czID);
        bRet &= pDB->Voc_DelCurve(sYQ.czID);
    }
    return bRet;
}
bool CDBBusiness::GetYQCode(sqlite_data &ResultData)
{
    return pDB->GetYQCode(ResultData);
}

bool CDBBusiness::Get3025(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    return pDB->Get3025(czStartTime,czEndTime,qYQIDList,ResultData);
}

bool CDBBusiness::Get3016(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData)
{
    return pDB->Get3016(czStartTime,czEndTime,ResultData);
}

bool CDBBusiness::Get8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    return pDB->Get8051(czStartTime,czEndTime,qYQIDList,ResultData);
}

bool CDBBusiness::GeGasStatus(const char *czStartTime, const char *czEndTime, QStringList *qYQIDList, sqlite_data &ResultData)
{
    return pDB->GeGasStatus(czStartTime,czEndTime,qYQIDList,ResultData);
}
bool CDBBusiness::GetLinear(const char *czStartTime, const char *czEndTime,sqlite_data &ResultData)
{
    return pDB->GetLinear(czStartTime,czEndTime,ResultData);
}
bool CDBBusiness::Get2091(const char *czStartTime, const char *czEndTime,sqlite_data &ResultData)
{
    return pDB->Get2091(czStartTime,czEndTime,ResultData);
}
bool CDBBusiness::Get2091(const char *czStartTime,const char *czEndTime,const int ProcessType, const char *Code, sqlite_data &ResultData)
{
    return pDB->Get2091(czStartTime,czEndTime,ProcessType,Code,ResultData);
}
bool CDBBusiness::Ins8051(struWaterData& stu8051,int iProcessType)
{   
    //保存浓度
    char czTime[64];memset(czTime,0,64);
    sprintf(czTime,"%04d-%02d-%02d %02d:%02d:%02d",stu8051.TestTime.date().year(),stu8051.TestTime.date().month(),stu8051.TestTime.date().day(),stu8051.TestTime.time().hour(),stu8051.TestTime.time().minute(),stu8051.TestTime.time().second());
    return pDB->Ins8051(czTime,stu8051.czID,stu8051.fResult,stu8051.cDataID);
}
bool CDBBusiness::Ins3016(struWaterData& stu3016)
{
    //保存标样核查浓度
    char czTime[64];memset(czTime,0,64);
    sprintf(czTime,"%04d-%02d-%02d %02d:%02d:%02d",stu3016.TestTime.date().year(),stu3016.TestTime.date().month(),stu3016.TestTime.date().day(),stu3016.TestTime.time().hour(),stu3016.TestTime.time().minute(),stu3016.TestTime.time().second());
    return pDB->Ins3016(czTime,stu3016.czID,stu3016.fCheckResult,stu3016.CheckChroma,stu3016.cDataID);
}
bool CDBBusiness::InsLinear(QDateTime qDataTime,int ProcessType,float fLinearSlope)
{
    char DataTime[64];memset(DataTime,0,64);
    sprintf(DataTime,"%04d-%02d-%02d %02d:%02d:%02d",qDataTime.date().year(),qDataTime.date().month(),qDataTime.date().day(),qDataTime.time().hour(),qDataTime.time().minute(),qDataTime.time().second());
    return pDB->InsLinear(DataTime,ProcessType,fLinearSlope);
}
bool CDBBusiness::InsMaintain(QDateTime qDataTime,const char* czMaintain)
{
    char DataTime[64];memset(DataTime,0,64);
    sprintf(DataTime,"%04d-%02d-%02d %02d:%02d:%02d",qDataTime.date().year(),qDataTime.date().month(),qDataTime.date().day(),qDataTime.time().hour(),qDataTime.time().minute(),qDataTime.time().second());
    return pDB->InsMaintain(DataTime,czMaintain);
}
bool CDBBusiness::InsGasStatus(QDateTime qDataTime,const char* czID,float fResult)
{
    char DataTime[64];memset(DataTime,0,64);
    sprintf(DataTime,"%04d-%02d-%02d %02d:%02d:%02d",qDataTime.date().year(),qDataTime.date().month(),qDataTime.date().day(),qDataTime.time().hour(),qDataTime.time().minute(),qDataTime.time().second());
    return pDB->InsGasStatus(DataTime,czID,fResult);
}
bool CDBBusiness::InsLog(QDateTime qDateTime,const char* Type,const char *Content)
{
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());
    return pDB->InsLog(DateTime,Type,Content);
}
bool CDBBusiness::InsEchemProcessData(const char* Type,float fVoltage,float fElectric)
{
    return pDB->InsEchemProcessData(Type,fVoltage,fElectric);
}
bool CDBBusiness::InsEchemDataInfo(struWaterData& stu8051)
{
    char czTime[64];memset(czTime,0,64);
    sprintf(czTime,"%04d-%02d-%02d %02d:%02d:%02d",stu8051.TestTime.date().year(),stu8051.TestTime.date().month(),stu8051.TestTime.date().day(),stu8051.TestTime.time().hour(),stu8051.TestTime.time().minute(),stu8051.TestTime.time().second());
    return pDB->InsEchemDataInfo(czTime,stu8051.czID,stu8051.m_EchemDataInfo.fHeight1,stu8051.m_EchemDataInfo.fHeight2,stu8051.nCalWay,stu8051.m_EchemDataInfo.fLeftBase,stu8051.m_EchemDataInfo.fRighBase,stu8051.fHeatValue,stu8051.nHeatTime);
}
bool CDBBusiness::InsEchemHh(const char* czID,float fH,float fh,float fResult)
{
    return pDB->InsEchemHh(czID,fH,fh,fResult);
}
//烟气保存光谱信息，如积分时间 次数等
bool CDBBusiness::InsSpectrumInfo(QDateTime qDateTime,float fN02,float fS02,float fN0,float fIntegTime,float fAverageTimes)
{
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());
    return pDB->InsSpectrumInfo(DateTime,fN02,fS02,fN0,fIntegTime,fAverageTimes);
}
//烟气各参数的吸光度信息
bool CDBBusiness::InsGasAbsorbance(QDateTime qDateTime,float fN02,float fS02,float fN0)
{
    char DateTime[64];memset(DateTime,0,64);
    sprintf(DateTime,"%04d-%02d-%02d %02d:%02d:%02d",qDateTime.date().year(),qDateTime.date().month(),qDateTime.date().day(),qDateTime.time().hour(),qDateTime.time().minute(),qDateTime.time().second());
    return pDB->InsGasAbsorbance(DateTime,fN02,fS02,fN0);
}
bool CDBBusiness::Ins2091(const char *DataTime, const int ProcessType, const char* Code, const int Level, const char *Remark)
{
    return pDB->Ins2091(DataTime, ProcessType, Code, Level, Remark);
}
bool CDBBusiness::InsConsume(const char *ID, const char *Name)
{
    return pDB->InsConsume(ID,Name);
}
bool CDBBusiness::InsConsumeInfo(const char *ID, const int ProcessType, const int MaxValue, const int RealValue, const int WaringValue, const int AlarmValue, const int FaultValue, const char *czDateTime)
{
    return pDB->InsConsumeInfo(ID, ProcessType, MaxValue, RealValue, WaringValue, AlarmValue, FaultValue, czDateTime);
}
bool CDBBusiness::InsConsumeUpdate(const char *ID, const char *czUpdateTime, int Quality)
{
    return pDB->InsConsumeUpdate(ID,czUpdateTime,Quality);
}

bool CDBBusiness::InsVocSrcData(double fRT,double fVol,float fTp)
{
    return pDB->InsVocSrcData(fRT,fVol,fTp);
}

bool CDBBusiness::SaveVocPeaks(struPeak peak)
{
    return pDB->SaveVocPeaks(peak);
}
bool CDBBusiness::GetVocPeaks(sqlite_data &ResultData)
{
    return pDB->GetVocPeaks(ResultData);
}
bool CDBBusiness::GetVocSrcData(sqlite_data &ResultData)
{
    return pDB->GetVocSrcData(ResultData);
}
bool CDBBusiness::GetEchemProcessData(sqlite_data &ProcessData)
{
    return pDB->GetEchemProcessData(ProcessData);
}
bool CDBBusiness::GetEchemBaseLine(sqlite_data &ProcessData)
{
    return pDB->GetEchemBaseLine(ProcessData);
}
bool CDBBusiness::GetEchemPeakLine(sqlite_data &ProcessData)
{
    return pDB->GetEchemPeakLine(ProcessData);
}
bool CDBBusiness::GetEchemHh(sqlite_data &ProcessData)
{
    return pDB->GetEchemHh(ProcessData);
}
bool CDBBusiness::GetEchemDataInfo(const char* czDateTime,sqlite_data &ProcessData)
{
    return pDB->GetEchemDataInfo(czDateTime,ProcessData);
}
bool CDBBusiness::GetLog(sqlite_data &ProcessData)
{
    return pDB->GetLog(ProcessData);
}

bool CDBBusiness::GetVocProcessData(sqlite_data &ProcessData)
{
    return pDB->GetVocProcessData(ProcessData);
}
bool CDBBusiness::GetVocProcessData(const char* czPath,QStringList &ProcessData)
{
    ProcessData.clear();
    bool bRet = false;
    QFile file(czPath);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream stream( &file );
        QString str;
        QStringList list;
        stream.readLine();//标题
        while( !stream.atEnd())
        {
            str = stream.readLine();
            list = str.split("\t");
            ProcessData.push_back(list.at(0));
            ProcessData.push_back(list.at(1));
            ProcessData.push_back(list.at(2));
        }
        file.close();
        bRet = true;
    }
    return  bRet;
}
float CDBBusiness::GetIntegral_WinFactor()
{
    return pDB->GetIntegral_WinFactor();
}
float CDBBusiness::GetIntegral_MinPW()
{
    return pDB->GetIntegral_MinPW();
}
float CDBBusiness::GetIntegral_MaxPW()
{
    return pDB->GetIntegral_MaxPW();
}
float CDBBusiness::GetIntegral_Minimum()
{
    return pDB->GetIntegral_Minimum();
}
float CDBBusiness::GetIntegral_MinArea()
{
    return pDB->GetIntegral_MinArea();
}
bool CDBBusiness::SaveVocDataInfo(const int kind, const QDateTime dt, const char *czID, const struWaterData vocGroup)
{
    char czTime[64];memset(czTime,0,64);
    sprintf(czTime,"%04d-%02d-%02d %02d:%02d:%02d",dt.date().year(),dt.date().month(),dt.date().day(),dt.time().hour(),dt.time().minute(),dt.time().second());
     return pDB->SaveVocDataInfo(kind,czTime,czID,vocGroup);
}
bool CDBBusiness::GetVocDataInfo(const char* czDateTime,sqlite_data &ProcessData)
{
    return pDB->GetVocDataInfo(czDateTime,ProcessData);
}
bool CDBBusiness::InsVocinternal(const QDateTime dt, float RemainTime, float RTime, float Conc, float RConc, float Area)
{
    char czTime[64];memset(czTime,0,64);
    sprintf(czTime,"%04d-%02d-%02d %02d:%02d:%02d",dt.date().year(),dt.date().month(),dt.date().day(),dt.time().hour(),dt.time().minute(),dt.time().second());
    return pDB->InsVocinternal(czTime, RemainTime, RTime, Conc, RConc, Area);
}

bool CDBBusiness::GetLastVocinternal(float& RemainTime,float& RTime,float& Conc,float& RConc)
{
    return pDB->GetLastVocinternal(RemainTime,RTime,Conc,RConc);
}
bool CDBBusiness::GetVocinternal(const char *czStartTime, const char *czEndTime, sqlite_data &ResultData)
{
    return pDB->GetVocinternal(czStartTime,czEndTime,ResultData);
}

bool CDBBusiness::SetDotConc(const char* czID,int nID,float fConc)
{
    return pDB->SetDotConc(czID,nID,fConc);
}
bool CDBBusiness::SetDotArea(const char *czID,int nID,float fArea)
{
    return pDB->SetDotArea(czID,nID,fArea);
}

bool CDBBusiness::UpdateCurve(const char *czID, double K, double B, double KB,double R)
{
    return pDB->UpdateCurve(czID,K,B,KB,R);
}
bool CDBBusiness::UpdateRemaintTime(const char *czID, double Rt)
{
    return pDB->UpdateRemaintTime(czID,Rt);
}
bool CDBBusiness::UpdateSearchParam(const char *czID, float WinFactor, int MinPW, int MaxPW, float Minimum, float MinArea)
{
    return pDB->UpdateSearchParam(czID,WinFactor,MinPW,MaxPW,Minimum,MinArea);
}
bool CDBBusiness::UpdateSearchParam_WinFactor(const char *czID, float WinFactor)
{
    return pDB->UpdateSearchParam_WinFactor(czID,WinFactor);
}
bool CDBBusiness::UpdateSearchParam_MinPW(const char *czID, int MinPW)
{
    return pDB->UpdateSearchParam_MinPW(czID,MinPW);
}
bool CDBBusiness::UpdateSearchParam_MaxPW(const char *czID, int MaxPW)
{
    return pDB->UpdateSearchParam_MaxPW(czID,MaxPW);
}
bool CDBBusiness::UpdateSearchParam_Minimum(const char *czID, float Minimum)
{
    return pDB->UpdateSearchParam_Minimum(czID,Minimum);
}
bool CDBBusiness::UpdateSearchParam_MinArea(const char *czID, float MinArea)
{
    return pDB->UpdateSearchParam_MinArea(czID,MinArea);
}
bool CDBBusiness::UpdateSearchParam_Kind(const char *czID, int Kind)
{
    return pDB->UpdateSearchParam_Kind(czID,Kind);
}
bool CDBBusiness::GetDots(sqlite_data &ResultData,const char *czID)
{
     return pDB->GetDots(ResultData,czID);
}
bool CDBBusiness::GetCurve(sqlite_data &ResultData, const char *czID)
{
    return pDB->GetCurve(ResultData,czID);
}
//=====================================================================//
// Begin 4. 比色法仪器操作数据表
//比色法详细数据(主库)
bool CDBBusiness::SaveBsDataInfo(struWaterData model)
{
    return pDB->SaveBsDataInfo(model);
}
//插入基线值（临时表）
bool CDBBusiness::InsBsBaseLineVal(const char* Type,float fVoltage,float fElectric)
{
    return pDB->InsBsBaseLineVal(Type,fVoltage,fElectric);
}
//获取基线值（临时表）
bool CDBBusiness::GetBsBaseLineVal(sqlite_data &ResultData)
{
    return pDB->GetBsBaseLineVal(ResultData);
}
//插入样品值（临时表）
bool CDBBusiness::InsBsSampleVal(const char* Type,float fVoltage,float fElectric)
{
    return pDB->InsBsSampleVal(Type,fVoltage,fElectric);
}
//获取样品值（临时表）
bool CDBBusiness::GetBsSampleVal(sqlite_data &ResultData)
{
    return pDB->GetBsSampleVal(ResultData);
}
//插入每次测试时设置基线值、样品值的时间间隔
bool CDBBusiness::InsBsBaseSampTimeSpace(int iDataType,int iSpaceTimeVal)
{
    return pDB->InsBsBaseSampTimeSpace(iDataType,iSpaceTimeVal);
}
//获取每次测试时设置基线值、样品值的时间间隔
bool CDBBusiness::GetBsBaseSampTimeSpace(sqlite_data &ResultData)
{
    return pDB->GetBsBaseSampTimeSpace(ResultData);
}
//测试详细数据(主库)
bool CDBBusiness::GetBsDataInfo(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData)
{
    return pDB->GetBsDataInfo(czStartTime,czEndTime,ResultData);
}
//测试详细数据(主库),获取最后一次测试的数据
bool CDBBusiness::GetTop1BsDataInfo(sqlite_data &ResultData)
{
    return pDB->GetTop1BsDataInfo(ResultData);
}
//获取曲线信息（主库）
bool CDBBusiness::GetCurveDataInfo(BsCurveDotInfo4 model,sqlite_data &ResultData)
{
    return pDB->GetCurveDataInfo(model,ResultData);
}
bool CDBBusiness::GetCurveDataInfo(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData)
{
    return pDB->GetCurveDataInfo(czID,iCurveID,iCalcMode,ResultData);
}

//插入曲线信息（主库）
bool CDBBusiness::InsCurveDataInfo(BsCurveDotInfo4 model)
{
    return pDB->InsCurveDataInfo(model);
}
//修改曲线信息（主库）
bool CDBBusiness::UpdateCurveDataInfo(BsCurveDotInfo4 model)
{
    return pDB->UpdateCurveDataInfo(model);
}
//获取比色法曲线管理(主库)
bool CDBBusiness::GetCurveManger(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData)
{
    return pDB->GetCurveManger(czID,iCurveID,iCalcMode,ResultData);
}
//获取比色法曲线管理(主库)
bool CDBBusiness::GetCurveManger(const char *czID,int iCalcMode,sqlite_data &ResultData)
{
    return pDB->GetCurveManger(czID,iCalcMode,ResultData);
}
//插入比色法曲线管理(主库)
//bool CDBBusiness::InsCurveManger(struBsCurveManger4 model)
//{
//     return pDB->InsCurveManger(model);
//}
//修改比色法曲线管理(主库)
bool CDBBusiness::UpdateCurveManger(struCurveInfo model)
{
    return pDB->UpdateCurveManger(model);
}
bool CDBBusiness::UpdateCurveManger(emCmdWord eCmdType,struCurveInfo model)
{
    return pDB->UpdateCurveManger(eCmdType,model);
}

//初使化曲线管理
bool CDBBusiness::InitCurveDataInfo(BsCurveDotInfo4 mod1,struCurveInfo mod2)
{
    return pDB->InitCurveDataInfo(mod1,mod2);
}

bool CDBBusiness::GetBs8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    return pDB->GetBs8051(czStartTime,czEndTime,qYQIDList,ResultData);
}

//bool CDBBusiness::GetBsBaseTempValue(int iType,sqlite_data &ResultData)
//{
//    return pDB->GetBsBaseTempValue(iType,ResultData);
//}

// End 4. 比色法仪器操作数据表
//=====================================================================//
//======================耗材管理 BEGIN ================================//
//耗材类型
bool CDBBusiness::GetConsume(sqlite_data &ResultData)
{
    return pDB->GetConsume(ResultData);
}
//修改耗材类型
bool CDBBusiness:: UpdateConsume(struConsume model)
{
    return pDB->UpdateConsume(model);
}
//耗材状态
bool CDBBusiness::GetConsumeInfo(sqlite_data &ResultData)
{
    return pDB->GetConsumeInfo(ResultData);
}
bool CDBBusiness::GetConsumeInfo(struConsumeInfo *model,sqlite_data &ResultData)
{
    return pDB->GetConsumeInfo(model,ResultData);
}
bool CDBBusiness::GetConsumeInfo(int iProcessType,sqlite_data &ResultData)
{
    return pDB->GetConsumeInfo(iProcessType,ResultData);
}
//添加耗材状态
bool CDBBusiness::InsConsumeInfo(struConsumeInfo model)
{
    return pDB->InsConsumeInfo(model);
}
//添加耗材更换记录
bool CDBBusiness::InsConsumeUpdate(struConsumeUpdate model)
{
    return pDB->InsConsumeUpdate(model);
}
//获取耗材历史记录
bool CDBBusiness::GetConsumeUpdate(const char* cID,sqlite_data &ResultData)
{
    return pDB->GetConsumeUpdate(cID,ResultData);
}
//删除耗材状态
bool CDBBusiness::DelConsumeInfo(struConsumeInfo model)
{
    return pDB->DelConsumeInfo(model);
}
//删除耗材更换记录
bool CDBBusiness::DelConsumeUpdate(struConsumeUpdate model)
{
    return pDB->DelConsumeUpdate(model);
}
//修改耗材状态
bool CDBBusiness::UpdateConsumeInfo(struConsumeInfo model)
{
    return pDB->UpdateConsumeInfo(model);
}
//修改整条耗材状态
bool CDBBusiness::UpdateAllConsumeInfo(struConsumeInfo *model)
{
return pDB->UpdateAllConsumeInfo(model);
}
//======================耗材管理 END ================================//
