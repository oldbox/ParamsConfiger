#include "cmydatabase.h"
#include "stdio.h"
#include <QStringList>

#include <iostream>
using namespace std;

CMyDataBase::CMyDataBase(char* filename)
{
    memset(m_DBName,0,MAX_DBNAME);
    memcpy(m_DBName,filename,MAX_DBNAME);
    pDB = new sqlite_db(filename);
    //cout << filename << endl;
}
CMyDataBase::~CMyDataBase()
{
    delete pDB;
    //cout << "CMyDataBase::~CMyDataBase()" << endl;
}

bool CMyDataBase::GetYQs(vector<SYQPARAM>& sctYQs)
{
    bool bRT = false;

    // Get the code ID of YQ
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select * from t_yq");
    pDB->Query(czSql);

    //save
    if(pDB->RowCount()>0)
    {
        bRT = true;
        for( int i=1;i<=pDB->RowCount();i++)
        {
            SYQPARAM sYQ;
            strcpy(sYQ.czID, (char*)pDB->Data(i,1));
            sctYQs.push_back(sYQ);

           //读取列名的方法
           //cout << (char*)(pDB->Data(0,1)) << endl;
        }
    }
    return bRT;
}

bool CMyDataBase::InsYQ(SYQPARAM& sYQ)
{
    if(QString(sYQ.czID).length() > 0)
    {
        char czSql[64];memset(czSql,0,64);
        sprintf(czSql,"select YQID from t_yq where YQID = '%s'",sYQ.czID);
        sqlite_data ResultData;
        pDB->Query(ResultData,czSql);
        if(ResultData.IsEmpty())
        {
            memset(czSql,0,64);
            sprintf(czSql,"insert into t_yq(YQID)values('%s')",sYQ.czID);
            return pDB->ExecSQL(czSql);
        }
        else
        {
            ResultData.Free();
            return true;
        }
    }
}

bool CMyDataBase::DelYQ(SYQPARAM &sYQ)
{
    if(0 != strcmp(sYQ.czID,""))
    {
        char czSql[64];memset(czSql,0,64);
        sprintf(czSql,"delete from t_yq where YQID = '%s'",sYQ.czID);
        return pDB->ExecSQL(czSql);
    }
}

bool CMyDataBase::GetYQCode(sqlite_data &ResultData)
{
    char czSql[64];memset(czSql,0,64);
    strcpy(czSql,"select * from t_yq");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::Get3025(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    QString szSql;
    szSql += "select distinct c.[DataTime] as DataTime,";
    for(int i=0;i<qYQIDList->length();i++)
    {
        szSql += "(select DataValue ||'&'||DataTag from T_3025  where YQID = '"+qYQIDList->at(i)+"' and DataTime = c.DataTime) as '"+qYQIDList->at(i)+"'";
        szSql += ",";
    }

    if(szSql.length() > 0)
    {
        szSql = szSql.remove(szSql.length() - 1,1);
    }
    szSql += " from T_3025 c ";
    szSql += "where DataTime>='"+QString(czStartTime)+"' and DataTime<'"+QString(czEndTime)+"' ";
    szSql += "order by DataTime desc";
    char czSql[1024];memset(czSql,0,1024);
     strcpy(czSql,szSql.toLatin1().data());
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::Get3016(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select DataTime,YQID,DataValue,StandValue,DataTag from T_3016 where DataTime>='%s' and DataTime<'%s' order by DataTime desc",czStartTime,czEndTime);
    //cout << czSql << endl;
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::GeGasStatus(const char *czStartTime, const char *czEndTime, QStringList *qYQIDList, sqlite_data &ResultData)
{
    QString szSql;
    szSql += "select distinct c.[DataTime] as DataTime,";
    for(int i=0;i<qYQIDList->length();i++)
    {
        szSql += "(select DataValue from t_status  where YQID = '"+qYQIDList->at(i)+"' and DataTime = c.DataTime) as '"+qYQIDList->at(i)+"'";
        szSql += ",";
    }

    if(szSql.length() > 0)
    {
        szSql = szSql.remove(szSql.length() - 1,1);
    }
    szSql += " from t_status c ";
    szSql += "where DataTime>='"+QString(czStartTime)+"' and DataTime<'"+QString(czEndTime)+"' ";
    szSql += "order by DataTime desc";
    char czSql[1024];memset(czSql,0,1024);
    strcpy(czSql,szSql.toLatin1().data());
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetLinear(const char *czStartTime,const char *czEndTime,sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select DataTime,DataValue from t_echemlinear where DataTime>='%s' and DataTime<'%s' order by DataTime desc",czStartTime,czEndTime);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);

}
bool CMyDataBase::Get2091(const char *czStartTime,const char *czEndTime,sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select DataTime,ProcessType,Code,Level,Remark from t_2091 where DataTime>='%s' and DataTime<'%s' order by DataTime desc",czStartTime,czEndTime);
    //qDebug("czSql:%s",czSql);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::Get2091(const char *czStartTime,const char *czEndTime,const int ProcessType, const char *Code, sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select DataTime,ProcessType,Code,Level,Remark from t_2091 where DataTime between '%s' and '%s' and ProcessType = %d and Code = '%s' order by DataTime asc",czStartTime,czEndTime,ProcessType,Code);
    qDebug("czSql:%s",czSql);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::Get8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    QString szSql;
    szSql += "select distinct c.[DataTime] as DataTime,";
    for(int i=0;i<qYQIDList->length();i++)
    {
        szSql += "(select DataValue ||'&'|| DataTag from T_8051  where YQID = '"+qYQIDList->at(i)+"' and DataTime = c.DataTime) as '"+qYQIDList->at(i)+"'";
        szSql += ",";
    }

    if(szSql.length() > 0)
    {
        szSql = szSql.remove(szSql.length() - 1,1);
       // cout << szSql.toStdString()<< endl;
    }
    szSql += " from T_8051 c";
    szSql += " where c.DataTime>='"+QString(czStartTime)+"' and c.DataTime<'"+QString(czEndTime)+"'";
    if(qYQIDList->length() > 0)
    {
        szSql += " and (";
        for(int i=0;i<qYQIDList->length();i++)
        {
            if(i == 0)
                szSql += "c.YQID = '"+qYQIDList->at(i)+"'";
            else
                szSql += " or c.YQID = '"+qYQIDList->at(i)+"'";
        }
        szSql += ")";
    }
    szSql += " order by c.DataTime desc";
    char czSql[4096];memset(czSql,0,4096);
     strcpy(czSql,szSql.toLatin1().data());
     //qDebug("time1:%s",QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
    int iRT = pDB->Query(ResultData,czSql);
    //qDebug("time2:%s",QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
    //qDebug("czSql:%s",czSql);
    return (-1 != iRT);
}
bool CMyDataBase::Ins8051(const char* DataTime,const char* YQID,float DataValue,char* DataTag)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_8051(DataTime,YQID,DataValue,DataTag)values('%s','%s',%.8f,'%s')",DataTime,YQID,DataValue,DataTag);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::Ins3016(const char* DataTime,const char* YQID,float DataValue,float StandValue,const char* DataTag)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_3016(DataTime,YQID,DataValue,StandValue,DataTag)values('%s','%s',%.8f,%.8f,'%s')",DataTime,YQID,DataValue,StandValue,DataTag);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsLinear(const char* DataTime,int ProcessType,float fLinearSlope)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_echemlinear(DataTime,ProcessType,DataValue)values('%s',%d,%.8f)",DataTime,ProcessType,fLinearSlope);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsMaintain(const char* DateTime,const char* czMaintain)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_echemmaintain(DataTime,Content)values('%s','%s')",DateTime,czMaintain);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsGasStatus(const char* DataTime,const char* czID,float DataValue)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_status(DataTime,YQID,DataValue)values('%s','%s',%.4f)",DataTime,czID,DataValue);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsLog(const char* DateTime,const char* Type,const char *Content)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_log(DataTime,wtype,content)values('%s','%s','%s')",DateTime,Type,Content);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsEchemProcessData(const char* Type,float fVoltage,float fElectric)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_processdata(wtype,V,A)values('%s',%.3f,%.3f)",Type,fVoltage,fElectric);
    return pDB->ExecSQL(czSql);
}

bool CMyDataBase::InsEchemDataInfo(const char* DateTime,const char* czID,float fHeight1,float fHeight2,int nCalWay,float fLeftBase,float fRighBase,float fHeatValue,int nHeatTime)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"insert into t_echemhh(DataTime,YQID,H1,H2,KindCal,LeftBase,RightBase,HeatValue,HeatTime)values('%s','%s',%.4f,%.4f,%d,%.4f,%.4f,%.4f,%d)",DateTime,czID,fHeight1,fHeight2,nCalWay,fLeftBase,fRighBase,fHeatValue,nHeatTime);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsEchemHh(const char* czID,float fH,float fh,float fResult)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_hh(YQID,H1,H2,fResult)values('%s',%.4f,%.4f,%.4f)",czID,fH,fh,fResult);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsSpectrumInfo(const char* DateTime,float fN02,float fS02,float fN0,float fIntegTime,float fAverageTimes)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_SpectrumInfo(DataTime,NO2,S02,N0,IntegTime,AverageTimes)values('%s',%.4f,%.4f,%.4f,%.4f,%.4f)",DateTime,fN02,fS02,fN0,fIntegTime,fAverageTimes);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsGasAbsorbance(const char* DateTime,float fN02,float fS02,float fN0)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_Absorbance(DataTime,NO2,S02,N0)values('%s',%.4f,%.4f,%.4f)",DateTime,fN02,fS02,fN0);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::Ins2091(const char *DataTime, const int ProcessType, const char *Code, const int Level, const char *Remark)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_2091(DataTime,ProcessType,Code,Level,Remark)values('%s',%d,'%s',%d,'%s')",DataTime,ProcessType,Code,Level,Remark);
    return pDB->ExecSQL(czSql);
}

bool CMyDataBase::InsConsume(const char *ID, const char *Name)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_Consume(ID,Name)values('%s','%s')",ID,Name);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsConsumeInfo(const char *ID, const int ProcessType, const int MaxValue, const int RealValue, const int WaringValue, const int AlarmValue, const int FaultValue, const char *czDateTime)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_ConsumeInfo(ID,ProcessType,MaxValue,RealValue,WaringValue,AlarmValue,FaultValue,ModifyTime)\
            values('%s',%d,%d,%d,%d,%d,%d,'%s')",ID,ProcessType,MaxValue,RealValue,WaringValue,AlarmValue,FaultValue,czDateTime);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsConsumeUpdate(const char *ID, const char *czUpdateTime, int Quality)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_ConsumeUpdate(ID,UpdateTime,Quality) values('%s','%s',%d)",ID,czUpdateTime,Quality);
    return pDB->ExecSQL(czSql);
}

bool CMyDataBase::InsVocSrcData(double fRT,double fVol,float fTp)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_srcdata(RT,Vol,Tp)values(%f,%f,%f)",fRT,fVol,fTp);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::SaveVocPeaks(struPeak peak)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"insert into t_vocpeaks(PeakId,PeakPos,PeakRTime,LeftPos,LeftRTime,RightPos,\
            RightRTime,PeakWidth,LeftBaseLine,RigtBaseLine,PeakHigh,PeakArea)values \
            (%d,%d,%.6f,%d,%.6f,%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f)",peak.nId,peak.nMidPos,peak.fRT,peak.nLeftPos,\
            peak.fLeftT,peak.nRightPos,peak.fRightT,peak.fWidth,peak.fLeftBaseLine,peak.fRigtBaseLine,\
            peak.fHight,peak.fArea);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::GetVocPeaks(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select PeakId,PeakPos,PeakRTime,LeftPos,LeftRTime,RightPos,\
           RightRTime,PeakWidth,LeftBaseLine,RigtBaseLine,PeakHigh,PeakArea from t_vocpeaks order by PeakId asc");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetVocSrcData(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select RT,Vol,Tp from t_srcdata order by Rt asc");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetEchemProcessData(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select wtype,V,A from t_processdata");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetEchemBaseLine(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select wtype,V,A from t_baseline");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetEchemPeakLine(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select wtype,V,A from t_peakline");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetEchemHh(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select YQID,H1,H2 from t_hh");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetEchemDataInfo(const char* czDateTime,sqlite_data &ProcessData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql, "select DataTime,YQID,H1,H2,KindCal,LeftBase,RightBase,HeatValue,HeatTime from t_echemhh where DataTime = '%s'",czDateTime);
    int iRet = pDB->Query(ProcessData, czSql);
    return (-1 != iRet);
}
bool CMyDataBase::GetLog(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select DataTime,wtype,content from t_log");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::SaveVocDataInfo(const int kind, const char *czTestTime, const char *czID, const struWaterData vocGroup)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_vocdatainfo(DataTime,TestKind,YQID,RemainTime,PeakHight,Area,K,B,R)\
            values('%s',%d,'%s',%.4f,%.4f,%.4f,%.4f,%.4f,%.4f)",czTestTime,kind,czID,vocGroup.m_vocDataInfo.RemainTime,vocGroup.m_vocDataInfo.PeakHight,\
            vocGroup.m_vocDataInfo.Area,vocGroup.m_vocDataInfo.K,vocGroup.m_vocDataInfo.B,vocGroup.m_vocDataInfo.R);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::InsVocinternal(const char *czDateTime, float RemainTime, float RTime, float Conc, float RConc, float Area)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_vocinternal(DataTime,RemainTime,RTime,Conc,RConc,Area)\
            values('%s',%.4f,%.4f,%.4f,%.4f,%.4f)",czDateTime,RemainTime, RTime, Conc,RConc,Area);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::GetLastVocinternal(float &RemainTime, float &RTime, float &Conc, float &RConc)
{
    RemainTime = RTime = Conc = RConc = 0;
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select  RemainTime,RTime,Conc,RConc from t_vocinternal order by ID desc LIMIT 1");
    int iRet = pDB->Query(data, czSql);
    if(iRet == 1)
    {
        RemainTime = QString(data.GetData(1,1)).toFloat();
        RTime = QString(data.GetData(1,2)).toFloat();
        Conc = QString(data.GetData(1,3)).toFloat();
        RConc = QString(data.GetData(1,4)).toFloat();
        return true;
    }
    return false;
}
bool CMyDataBase::GetVocinternal(const char *czStartTime, const char *czEndTime, sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select  DataTime,RemainTime,RTime,Conc,RConc,Area from t_vocinternal where DataTime>='%s' and DataTime<'%s' order by DataTime desc",czStartTime,czEndTime);
//    cout << czSql << endl;
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

bool CMyDataBase::GetVocDataInfo(const char* czDateTime,sqlite_data &ProcessData)
{
    char czSql[256];memset(czSql,0,256);//(注：YQID 必须在第2个位置上)
    sprintf(czSql, "select DataTime,YQID,TestKind,RemainTime,PeakHight,Area,K,B,R from t_vocdatainfo where DataTime = '%s'",czDateTime);
    int iRet = pDB->Query(ProcessData, czSql);
    return (-1 != iRet);
}
bool CMyDataBase::GetVocProcessData(sqlite_data &ProcessData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select RT,Vol,Tp from t_srcdata");
    int iRet = pDB->Query(ProcessData, czSql);
    return (-1 != iRet);
}
float CMyDataBase::GetIntegral_WinFactor()
{
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select min(WinFactor) from t_vocgroup");
    int iRet = pDB->Query(data, czSql);
    if(-1 != iRet)
        return QString(data.GetData(1,1)).toFloat();
    else
        return 0;
}
float CMyDataBase::GetIntegral_MinPW()
{
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select min(MinPW) from t_vocgroup");
    int iRet = pDB->Query(data, czSql);
    if(-1 != iRet)
        return QString(data.GetData(1,1)).toFloat();
    else
        return 0;
}
float CMyDataBase::GetIntegral_MaxPW()
{
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select max(MaxPW) from t_vocgroup");
    int iRet = pDB->Query(data, czSql);
    if(-1 != iRet)
        return QString(data.GetData(1,1)).toFloat();
    else
        return 0;
}
float CMyDataBase::GetIntegral_Minimum()
{
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select min(Minimum) from t_vocgroup");
    int iRet = pDB->Query(data, czSql);
    if(-1 != iRet)
        return QString(data.GetData(1,1)).toFloat();
    else
        return 0;
}
float CMyDataBase::GetIntegral_MinArea()
{
    sqlite_data data;
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql, "select min(MinArea) from t_vocgroup");
    int iRet = pDB->Query(data, czSql);
    if(-1 != iRet)
        return QString(data.GetData(1,1)).toFloat();
    else
        return 0;
}

bool CMyDataBase::Voc_InitDot(const char *czID)
{
    bool bRet = true;
    char czSql[256];memset(czSql,0,256);
    for(int i = 0;i < 5;i++)
    {
        sprintf(czSql,"insert into t_vocdots(YQID,Id,Conc,Area) values('%s',%d,0,0)",czID,i);
        bRet &= pDB->ExecSQL(czSql);
        if(!bRet)
            break;
    }
    return bRet;
}
bool CMyDataBase::Voc_DelDot(const char *czID)
{
     char czSql[256];memset(czSql,0,256);
     sprintf(czSql,"delete from t_vocdots where YQID = '%s'",czID);
     return  pDB->ExecSQL(czSql);
}

bool CMyDataBase::Voc_InitCurve(const char *czID)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_vocgroup(YQID,RemainTime,WinFactor,Kind,K,B,KB,R,MinPW,MaxPW,Minimum,MinArea) values('%s',0.0,0.05,0,0,0,0,0,7,70,50000,100)",czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::Voc_DelCurve(const char *czID)
{
     char czSql[256];memset(czSql,0,256);
     sprintf(czSql,"delete from t_vocgroup where YQID = '%s'",czID);
     return  pDB->ExecSQL(czSql);
}
bool CMyDataBase::SetDotConc(const char* czID,int nID,float fConc)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocdots set Conc = %f where YQID='%s' and Id=%d",fConc,czID,nID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::SetDotArea(const char *czID,int nID,float fArea)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocdots set Area = %f where YQID='%s' and Id=%d",fArea,czID,nID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateCurve(const char *czID, double K, double B, double KB,double R)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set K = %f,B = %f,KB = %f, R = %f where YQID='%s'",K,B,KB,R,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateRemaintTime(const char *czID, double fRT)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set RemainTime = %f where YQID='%s'",fRT,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam(const char *czID, float WinFactor,int MinPW,int MaxPW,float Minimum,float MinArea)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set WinFactor = %f,MinPW = %d,MaxPW = %d, Minimum = %f,MinArea = %f where YQID='%s'",WinFactor,MinPW,MaxPW,Minimum,MinArea,czID);
    return pDB->ExecSQL(czSql);
}

bool CMyDataBase::UpdateSearchParam_WinFactor(const char *czID, float WinFactor)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set WinFactor = %f where YQID='%s'",WinFactor,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam_MinPW(const char *czID, int MinPW)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set MinPW = %d where YQID='%s'",MinPW,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam_MaxPW(const char *czID, int MaxPW)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set MaxPW = %d where YQID='%s'",MaxPW,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam_Minimum(const char *czID, float Minimum)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set Minimum = %f where YQID='%s'",Minimum,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam_MinArea(const char *czID, float MinArea)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set MinArea = %f where YQID='%s'",MinArea,czID);
    return pDB->ExecSQL(czSql);
}
bool CMyDataBase::UpdateSearchParam_Kind(const char *czID, int Kind)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_vocgroup set Kind = %d where YQID='%s'",Kind,czID);
    return pDB->ExecSQL(czSql);
}

bool CMyDataBase::GetDots(sqlite_data &ResultData,const char *czID)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select Conc,Area from t_vocdots where YQID='%s' order by Id asc",czID);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetCurve(sqlite_data &ResultData,const char *czID)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select YQID,RemainTime,Kind,K,B,KB,R,WinFactor,MinPW,MaxPW,Minimum,MinArea from t_vocgroup where YQID='%s'",czID);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//=====================================================================//
// Begin 4. 比色法仪器操作数据表
//比色法详细数据(主库)
bool CMyDataBase::SaveBsDataInfo(struWaterData model)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"insert into t_BsDataInfo(DataTime,TestKind,YQID,BaseValue,SampleValue,Absorbency,\
            R,K,B,BlankAbsorb,StdConc,StdAbsorb,DigeTemp,DigeTime,CurveID)values(\
                '%s',%d,'%s',%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d)",\
            model.TestTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.TestKind,model.czID,\
            model.m_BsDataInfo.BaseValue,model.m_BsDataInfo.SampleValue,model.m_BsDataInfo.Absorbency,\
            model.m_BsDataInfo.R,model.m_BsDataInfo.K,model.m_BsDataInfo.B,model.m_BsDataInfo.BlankAbsorb,\
            model.CheckChroma,model.m_BsDataInfo.StdAbsorb,model.fHeatValue,model.nHeatTime,model.m_BsDataInfo.CurveID);
    return pDB->ExecSQL(czSql);
}

//插入基线值（临时表）
bool CMyDataBase::InsBsBaseLineVal(const char* Type,float fVoltage,float fElectric)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_baselineval(wtype,V,A)values('%s',%.3f,%.3f)",Type,fVoltage,fElectric);
    return pDB->ExecSQL(czSql);
}
//插入样品值（临时表）
bool CMyDataBase::InsBsSampleVal(const char* Type,float fVoltage,float fElectric)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_sampleval(wtype,V,A)values('%s',%.3f,%.3f)",Type,fVoltage,fElectric);
    return pDB->ExecSQL(czSql);
}
//获取基线值（临时表）
bool CMyDataBase::GetBsBaseLineVal(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select * from t_baselineval");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//获取样品值（临时表）
bool CMyDataBase::GetBsSampleVal(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select * from t_sampleval");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//插入每次测试时设置基线值、样品值的时间间隔
bool CMyDataBase::InsBsBaseSampTimeSpace(int iDataType,int iSpaceTimeVal)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_BaseSampTimeSpace(wtype,A)values(%d,%d)",iDataType,iSpaceTimeVal);
    return pDB->ExecSQL(czSql);
}
//获取每次测试时设置基线值、样品值的时间间隔
bool CMyDataBase::GetBsBaseSampTimeSpace(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select * from T_BaseSampTimeSpace");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//测试详细数据(主库),获取一个时段的数据(注：YQID 必须在第2个位置上)
bool CMyDataBase::GetBsDataInfo(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select DataTime,YQID,TestKind,BaseValue,SampleValue,Absorbency,K,B,R,BlankAbsorb,StdConc,StdAbsorb,DigeTemp,DigeTime,CurveID from T_BsDataInfo where DataTime between '%s' and '%s' order by DataTime desc",czStartTime,czEndTime);
    //qDebug("czSql:%s",czSql);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//测试详细数据(主库),获取最后一次测试的数据
bool CMyDataBase::GetTop1BsDataInfo(sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select top 1 DataTime,TestKind,YQID,BaseValue,SampleValue,Absorbency,R,K,B,BlankAbsorb,StdConc,StdAbsorb,DigeTemp,DigeTime from T_BsDataInfo order by DataTime desc");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

//获取曲线信息（主库）
bool CMyDataBase::GetCurveDataInfo(BsCurveDotInfo4 model,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select CurveID,YQID,CalcMode,PointID,Chroma,Absorbency from T_BsCurveDotInfo where CalcMode = %d and CurveID = %d and YQID = '%s' and PointID = %d",model.iCalcMode,model.iCurveID,model.czID,model.iPointID);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
bool CMyDataBase::GetCurveDataInfo(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select CurveID,YQID,CalcMode,PointID,Chroma,Absorbency from T_BsCurveDotInfo where CalcMode = %d and CurveID = %d and YQID = '%s'",iCalcMode,iCurveID,czID);
    //qDebug("czSql:%s",czSql);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//初使化曲线管理
bool CMyDataBase::InitCurveDataInfo(BsCurveDotInfo4 mod1,struCurveInfo mod2)
{
    bool bRet = true;
    char czSql[256];
    for(int x = 1;x <= 5;x++)
    {
        memset(czSql,0,256);
        sprintf(czSql,"select * from t_bscurvemanger where CalcMode = %d and CurveID = %d and YQID = '%s'",mod2.CalcMode,x,mod2.czID);
        int iRT = pDB->Query(czSql);
        if(iRT == 0)
        {
            memset(czSql,0,256);
            sprintf(czSql,"insert into t_bscurvemanger(CurveID,YQID,CalcMode,DataTime,R,K,B,BlankAbsorb,StdConc,StdAbsorb,MinValue,MaxValue)values(%d,'%s',%d,'%s',%f,%f,%f,%f,%f,%f,%f,%f)",
                    x,mod2.czID,mod2.CalcMode,mod2.DataTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),mod2.R,mod2.K,mod2.B,mod2.BlankAbsorb,mod2.CheckChroma,mod2.StdAbsorb,mod2.MinValue,mod2.MaxValue);
            //qDebug("czSql:%s",czSql);
            bRet &= pDB->ExecSQL(czSql);
        }
        if(iRT == 0 && bRet)
        {
            for(int y = 1;y <= 5;y++)
            {
                memset(czSql,0,256);
                sprintf(czSql,"select * from T_BsCurveDotInfo where CalcMode = %d and CurveID = %d and YQID = '%s' and PointID = %d",mod2.CalcMode,x,mod2.czID,y);
                int iYT = pDB->Query(czSql);
                if(iYT == 0)
                {
                    memset(czSql,0,256);
                    sprintf(czSql,"insert into T_BsCurveDotInfo(CurveID,YQID,CalcMode,PointID,Chroma,Absorbency)values(%d,'%s',%d,%d,%f,%f)",x,mod1.czID,mod2.CalcMode,y,mod1.fChroma,mod1.fAbsorbency);
                    bRet &= pDB->ExecSQL(czSql);
                }
            }
        }
    }

     return bRet;
}


//插入曲线信息（主库）
bool CMyDataBase::InsCurveDataInfo(BsCurveDotInfo4 model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_BsCurveDotInfo(CurveID,YQID,CalcMode,PointID,Chroma,Absorbency)values(%d,'%s',%d,%d,%f,%f)",model.iCurveID,model.czID,model.iCalcMode,model.iPointID,model.fChroma,model.fAbsorbency);
    return pDB->ExecSQL(czSql);
}
//修改曲线信息（主库）
bool CMyDataBase::UpdateCurveDataInfo(BsCurveDotInfo4 model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update T_BsCurveDotInfo set Chroma = %f,Absorbency=%f where CalcMode = %d and CurveID = %d and YQID = '%s' and PointID = %d",model.fChroma,model.fAbsorbency,model.iCalcMode,model.iCurveID,model.czID,model.iPointID);
    return pDB->ExecSQL(czSql);
}
//获取比色法曲线管理(主库)
bool CMyDataBase::GetCurveManger(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select * from t_bscurvemanger where CalcMode = %d and CurveID = %d and YQID = '%s'",iCalcMode,iCurveID,czID);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//获取比色法曲线管理(主库)
bool CMyDataBase::GetCurveManger(const char *czID,int iCalcMode,sqlite_data &ResultData)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"select * from t_bscurvemanger where YQID = '%s' and CalcMode = %d ",czID,iCalcMode);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
////插入比色法曲线管理(主库)
//bool CMyDataBase::InsCurveManger(struBsCurveManger4 model)
//{
//    char czSql[512];memset(czSql,0,512);
//    sprintf(czSql,"insert into t_bscurvemanger(CurveID,YQID,CalcMode,DataTime,R,K,B,BlankAbsorb,StdConc,StdAbsorb,MinValue,MaxValue)values(%d,'%s',0,'%s',%f,%f,%f,%f,%f,%f,%f,%f)",
//            model.CurveID,model.czID,model.DataTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.R,model.K,model.B,model.BlankAbsorb,model.StdConc,model.StdAbsorb,model.MinValue,model.MaxValue);
//    return pDB->ExecSQL(czSql);
//}
//修改比色法曲线管理(主库)
bool CMyDataBase::UpdateCurveManger(struCurveInfo model)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"update t_bscurvemanger set DataTime = '%s',R = %f,K = %f,B = %f,BlankAbsorb = %f,StdConc = %f,StdAbsorb = %f,MinValue = %f,MaxValue = %f where CurveID = %d and YQID = '%s' and CalcMode = %d",
            model.DataTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.R,model.K,model.B,model.BlankAbsorb,model.CheckChroma,model.StdAbsorb,model.MinValue,model.MaxValue,model.CurveID,model.czID,model.CalcMode);
    return pDB->ExecSQL(czSql);
}
//修改标样校准（主库）
bool CMyDataBase::UpdateCurveManger(emCmdWord eCmdType,struCurveInfo model)
{
    char czSql[512];memset(czSql,0,512);
    if(eCmdType == CMD_SAMPLEADJUST) //更新标样校准数据
    {
        sprintf(czSql,"update t_bscurvemanger set DataTime = '%s',StdConc = %f,StdAbsorb = %f where CalcMode = %d and CurveID = %d and YQID = '%s'",model.DataTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.CheckChroma,model.StdAbsorb,model.CalcMode,model.CurveID,model.czID);
    }
    else if(eCmdType == CMD_BANKADJUST) //空白校准数据
    {
        sprintf(czSql,"update t_bscurvemanger set DataTime = '%s',BlankAbsorb = %f where CalcMode = %d and CurveID = %d and YQID = '%s'",model.DataTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.BlankAbsorb,model.CalcMode,model.CurveID,model.czID);
    }
    //qDebug("czSql:%s",czSql);
    return pDB->ExecSQL(czSql);
}

//比色法查询数据
bool CMyDataBase::GetBs8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData)
{
    QString szSql;
    szSql += "select b.DataTime,b.YQID,a.DataValue ||'&'||DataTag,b.Absorbency,b.BaseValue,b.SampleValue,b.CurveID from T_BsDataInfo b ";
    szSql += "inner join T_8051 a on a.DataTime = b.DataTime and a.YQID = b.YQID ";
    szSql += "where b.DataTime>='"+QString(czStartTime)+"' and b.DataTime<'"+QString(czEndTime)+"' ";
    szSql += "and a.YQID = '"+qYQIDList->at(0)+"' ";
    szSql += "order by b.DataTime desc";

//    szSql += "select xx.*,b.Absorbency,b.BaseValue,b.SampleValue,b.CurveID from (";
//    szSql += "select distinct c.[DataTime] as DataTime,";
//    for(int i=0;i<qYQIDList->length();i++)
//    {
//        szSql += "(select DataValue ||'&'||DataTag from T_8051  where YQID = '"+qYQIDList->at(i)+"' and DataTime = c.DataTime) as '"+qYQIDList->at(i)+"'";
//        szSql += ",";
//    }

//    if(szSql.length() > 0)
//    {
//        szSql = szSql.remove(szSql.length() - 1,1);
//    }
//    szSql += " from T_8051 c ";
//    szSql += "where DataTime>='"+QString(czStartTime)+"' and DataTime<'"+QString(czEndTime)+"' ";
//    szSql += "order by DataTime desc";
//    szSql += ") xx left join T_BsDataInfo b on datetime(xx.DataTime) = datetime(b.DataTime) order by xx.DataTime desc";

    char czSql[4096];memset(czSql,0,4096);
    strcpy(czSql,szSql.toLatin1().data());
    //qDebug("czSql:%s",czSql);
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

// End 4. 比色法仪器操作数据表
//=====================================================================//

bool CMyDataBase::Consume_add(const char *ID, const char *DBName,const char *ParentID)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into T_Consume(ID,Name,ParentID) VALUES('%s','%s','%s')",ID,DBName,ParentID);
    return pDB->ExecSQL(czSql);
}
//耗材类型
bool CMyDataBase::GetConsume(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select ID,Name,ParentID from t_consume");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//修改耗材类型
bool CMyDataBase::UpdateConsume(struConsume model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"update t_consume set Name = '%s' where ID = '%s'",model.Name,model.cID);
     return pDB->ExecSQL(czSql);
}
//耗材状态
bool CMyDataBase::GetConsumeInfo(sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    strcpy(czSql,"select a.ID,a.ProcessType,b.Name,a.MaxValue,a.RealValue,a.WaringValue,a.AlarmValue,a.FaultValue,a.QualityDay,a.ModifyTime,a.EmployUnit,b.ParentID from t_consumeinfo a inner join t_consume b on a.ID = b.ID order by a.ProcessType asc,a.ID asc");
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//获取当前仪器试剂列表
bool CMyDataBase::GetConsumeInfo(int iProcessType,sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select a.ID,b.Name from t_consumeinfo a inner join t_consume b on a.ID = b.ID where b.ParentID = 'SJ' and a.ProcessType = %d",iProcessType);
    //cout << czSql << endl;
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}

//耗材状态
bool CMyDataBase::GetConsumeInfo(struConsumeInfo *model,sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    char czTemp[256];
    strcpy(czSql,"select a.ID,a.ProcessType,b.Name,a.MaxValue,a.RealValue,a.WaringValue,a.AlarmValue,a.FaultValue,a.QualityDay,a.ModifyTime,a.EmployUnit from t_consumeinfo a inner join t_consume b on a.ID = b.ID where 1 = 1");
    if(strlen(model->cID) > 0)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," and a.ID = '%s'",model->cID);
        strcat(czSql,czTemp);
    }
    if(model->ProcessType != 0)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," and a.ProcessType = %d",model->ProcessType);
        strcat(czSql,czTemp);
    }
    cout << czSql << endl;
    int iRT = pDB->Query(ResultData,czSql);
    return (-1 != iRT);
}
//添加耗材状态
bool CMyDataBase::InsConsumeInfo(struConsumeInfo model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_consumeinfo(ID,ProcessType,MaxValue,RealValue,WaringValue,AlarmValue,FaultValue,QualityDay,ModifyTime,EmployUnit)values('%s',%d,%f,%f,%f,%f,%f,%d,'%s',%d)",
        model.cID,model.ProcessType,model.MaxValue,model.RealValue,model.WaringValue,model.AlarmValue,model.FaultValue,model.QualityDay,model.ModifyTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model.iEmployUnit);
//    cout << model.ModifyTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data() << endl;
   // cout << czSql << endl;
    return pDB->ExecSQL(czSql);
}
//添加耗材更换记录
bool CMyDataBase::InsConsumeUpdate(struConsumeUpdate model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"insert into t_consumeupdate(ID,AddVolume,UpdateTime)values('%s',%f,'%s')",model.cID,model.AddVolume,model.UpdateTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
    return pDB->ExecSQL(czSql);
}
//删除耗材更换记录
bool CMyDataBase::DelConsumeUpdate(struConsumeUpdate model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"delete from t_consumeupdate where ID = '%s'",model.cID);
    return pDB->ExecSQL(czSql);
}
//耗材历史记录
bool CMyDataBase::GetConsumeUpdate(const char* cID,sqlite_data &ResultData)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"select a.ConsID,a.ID,b.Name,a.AddVolume,a.UpdateTime from t_consumeupdate a inner join T_Consume b on a.ID = b.ID where a.ID = '%s' order by a.ConsID desc",cID);
    int iRT = pDB->Query(ResultData,czSql);
    //qDebug("czSql:%s",czSql);
    return (-1 != iRT);
}
//删除耗材状态
bool CMyDataBase::DelConsumeInfo(struConsumeInfo model)
{
    char czSql[256];memset(czSql,0,256);
    sprintf(czSql,"delete from t_consumeinfo where ID = '%s'",model.cID);
    return pDB->ExecSQL(czSql);
}
//修改整条耗材状态
bool CMyDataBase::UpdateAllConsumeInfo(struConsumeInfo *model)
{
    char czSql[512];memset(czSql,0,512);
    sprintf(czSql,"update t_consumeinfo set RealValue = %f,WaringValue = %f,AlarmValue = %f,FaultValue = %f,MaxValue = %f,QualityDay = %d,ModifyTime = '%s',EmployUnit = %d where ID = '%s' and ProcessType = %d",
            model->RealValue,model->WaringValue,model->AlarmValue,model->FaultValue,model->MaxValue,model->QualityDay,model->ModifyTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data(),model->iEmployUnit,model->cID,model->ProcessType);
     return pDB->ExecSQL(czSql);
}
//修改耗材状态
bool CMyDataBase::UpdateConsumeInfo(struConsumeInfo model)
{
    char czSql[256];memset(czSql,0,256);
    char czTemp[256];
    strcpy(czSql,"update t_consumeinfo set ");
    if(model.iEditType == 0)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," RealValue = %f",model.RealValue);
        strcat(czSql,czTemp);
    }
    else if(model.iEditType == 1)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," WaringValue = %f",model.WaringValue);
        strcat(czSql,czTemp);
    }
    else if(model.iEditType == 2)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," AlarmValue = %f",model.AlarmValue);
        strcat(czSql,czTemp);
    }
    else if(model.iEditType == 3)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," FaultValue = %f",model.FaultValue);
        strcat(czSql,czTemp);
    }
    else if(model.iEditType == 4)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," MaxValue = %f",model.MaxValue);
        strcat(czSql,czTemp);
    }
    else if(model.iEditType == 5)
    {
        memset(czTemp,0,256);
        sprintf(czTemp," QualityDay = %d",model.QualityDay);
        strcat(czSql,czTemp);
    }
    if(model.iEditType >= 0)
    {
        memset(czTemp,0,256);
        sprintf(czTemp,",ModifyTime = '%s'",model.ModifyTime.toString("yyyy-MM-dd HH:mm:ss").toLatin1().data());
        strcat(czSql,czTemp);
    }

    memset(czTemp,0,256);
    sprintf(czTemp," where ID = '%s' and ProcessType = %d",model.cID,model.ProcessType);
    strcat(czSql,czTemp);
    return pDB->ExecSQL(czSql);
}

//bool CMyDataBase::GetBsBaseTempValue(int iType,sqlite_data &ResultData)
//{
//    char czSql[256];memset(czSql,0,256);
//    sprintf(czSql,"select * from t_bsbasetempvalue where DataType = %d",iType);
//    int iRT = pDB->Query(ResultData,czSql);
//    //qDebug("czSql:%s",czSql);
//    return (-1 != iRT);
//}
