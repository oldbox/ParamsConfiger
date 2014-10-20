#ifndef CMYDATABASE_H
#define CMYDATABASE_H

#include "cdatabase.h"
#include "vardef.h"
#include <vector>


using namespace std;
class CMyDataBase
{
public:
    CMyDataBase(char* filename);
    ~CMyDataBase();

    bool GetYQs(vector<SYQPARAM>& sctYQs);
    bool Get3025(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData);
    bool Get3016(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData);
    bool Get8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData);
    bool GetLinear(const char *czStartTime,const char *czEndTime,sqlite_data &ResultData);
    bool Get2091(const char *czStartTime,const char *czEndTime,sqlite_data &ResultData);
    bool Get2091(const char *czStartTime,const char *czEndTime,const int ProcessType, const char *Code, sqlite_data &ResultData);
    bool Ins8051(const char* DataTime,const char* YQID,float DataValue,char* DataTag);
    bool Ins3016(const char* DataTime,const char* YQID,float DataValue,float StandValue,const char* DataTag);
    bool InsYQ(SYQPARAM& sYQ);
    bool DelYQ(SYQPARAM& sYQ);
    bool GetYQCode(sqlite_data &ResultData);
    bool InsLinear(const char* DataTime,int ProcessType,float fLinearSlope);
    bool InsMaintain(const char* DateTime,const char* czMaintain);
    bool InsGasStatus(const char* DataTime,const char* czID,float DataValue);
    bool InsLog(const char* DateTime,const char* Type,const char *Content);
    bool InsEchemProcessData(const char* Type,float fVoltage,float fElectric);
    bool InsEchemDataInfo(const char* DateTime,const char* czID,float fHeight1,float fHeight2,int nCalWay,float fLeftBase,float fRighBase,float fHeatValue,int nHeatTime);
    bool InsEchemHh(const char* czID,float fH,float fh,float fResult);
    bool InsSpectrumInfo(const char* DateTime,float fN02,float fS02,float fN0,float fIntegTime,float fAverageTimes);
    bool InsGasAbsorbance(const char* DateTime,float fN02,float fS02,float fN0);


    bool Ins2091(const char* DataTime,const int ProcessType,const char* Code,const int Level, const char* Remark);
    bool InsConsume(const char* ID,const char* Name);
    bool InsConsumeInfo(const char* ID,const int ProcessType,const int MaxValue,const int RealValue,const int WaringValue,const int AlarmValue,const int FaultValue,const char* czDateTime);
    bool InsConsumeUpdate(const char* ID,const char* czUpdateTime,int Quality);

    //<有机物数据库操作
    bool InsVocSrcData(double fRT,double fVol,float fTp);
    bool SaveVocPeaks(struPeak peak);
    bool GetVocPeaks(sqlite_data &ResultData);
    bool GetVocSrcData(sqlite_data &ResultData);
    bool GetVocProcessData(sqlite_data &ProcessData);
    float GetIntegral_WinFactor();
    float GetIntegral_MinPW();
    float GetIntegral_MaxPW();
    float GetIntegral_Minimum();
    float GetIntegral_MinArea();
    bool  SaveVocDataInfo(const int kind,const char* czTestTime,const char* czID,const struWaterData vocGroup);
    bool GetVocDataInfo(const char* czDateTime,sqlite_data &ResultData);
    bool InsVocinternal(const char *czDateTime,float RemainTime,float RTime,float Conc,float RConc, float Area);
    bool GetLastVocinternal(float& RemainTime,float& RTime,float& Conc,float& RConc);
    bool GetVocinternal(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData);

    //紫外烟气数据库操作
    bool GeGasStatus(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData);
    //获取重金属过程数据
    bool GetEchemProcessData(sqlite_data &ResultData);
    bool GetEchemBaseLine(sqlite_data &ResultData);
    bool GetEchemPeakLine(sqlite_data &ResultData);
    bool GetEchemHh(sqlite_data &ResultData);
    bool GetEchemDataInfo(const char* czDateTime,sqlite_data &ProcessData);
    bool GetLog(sqlite_data &ResultData);

    //初始化曲线点信息
    bool Voc_InitDot(const char* czID);
    bool Voc_DelDot(const char* czID);
    bool Voc_InitCurve(const char* czID);
    bool Voc_DelCurve(const char* czID);
    bool SetDotConc(const char* czID,int nID,float fConc);
    bool SetDotArea(const char *czID,int nID,float fArea);
    bool UpdateCurve(const char *czID, double K, double B, double KB,double R);
    bool UpdateRemaintTime(const char *czID, double fRT);
    bool UpdateSearchParam(const char *czID, float WinFactor,int MinPW,int MaxPW,float Minimum,float MinArea);
    bool UpdateSearchParam_WinFactor(const char *czID, float WinFactor);
    bool UpdateSearchParam_MinPW(const char *czID, int MinPW);
    bool UpdateSearchParam_MaxPW(const char *czID, int MaxPW);
    bool UpdateSearchParam_Minimum(const char *czID, float Minimum);
    bool UpdateSearchParam_MinArea(const char *czID, float MinArea);
    bool UpdateSearchParam_Kind(const char *czID, int Kind);

    bool GetDots(sqlite_data &ResultData,const char *czID);
    bool GetCurve(sqlite_data &ResultData,const char *czID);

    //----- Begin 4. 比色法仪器操作数据表 -----//
    bool SaveBsDataInfo(struWaterData model);
    //插入基线值（临时表）
    bool InsBsBaseLineVal(const char* Type,float fVoltage,float fElectric);
    //插入样品值（临时表）
    bool InsBsSampleVal(const char* Type,float fVoltage,float fElectric);
    //获取基线值（临时表）
    bool GetBsBaseLineVal(sqlite_data &ResultData);
    //获取样品值（临时表）
    bool GetBsSampleVal(sqlite_data &ResultData);
    //插入每次测试时设置基线值、样品值的时间间隔
    bool InsBsBaseSampTimeSpace(int iDataType,int iSpaceTimeVal);
    //获取每次测试时设置基线值、样品值的时间间隔
    bool GetBsBaseSampTimeSpace(sqlite_data &ResultData);
    //测试详细数据(主库)
    bool GetBsDataInfo(const char* czStartTime,const char* czEndTime,sqlite_data &ResultData);
    //测试详细数据(主库),获取最后一次测试的数据
    bool GetTop1BsDataInfo(sqlite_data &ResultData);
    //获取曲线信息（主库）
    bool GetCurveDataInfo(BsCurveDotInfo4 model,sqlite_data &ResultData);
    bool GetCurveDataInfo(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData);
    //插入曲线信息（主库）
    bool InsCurveDataInfo(BsCurveDotInfo4 model);
    //修改曲线信息（主库）
    bool UpdateCurveDataInfo(BsCurveDotInfo4 model);
    //获取比色法曲线管理(主库)
    bool GetCurveManger(const char *czID,int iCurveID,int iCalcMode,sqlite_data &ResultData);
    //获取比色法曲线管理(主库)
    bool GetCurveManger(const char *czID,int iCalcMode,sqlite_data &ResultData);
    //插入比色法曲线管理(主库)
    //bool InsCurveManger(struBsCurveManger4 model);
    //修改比色法曲线管理(主库)
    bool UpdateCurveManger(struCurveInfo model);
    //修改比色法曲线管理（主库）修改标样校准、空白校准
    bool UpdateCurveManger(emCmdWord eCmdType,struCurveInfo model);
    //初使化曲线管理
    bool InitCurveDataInfo(BsCurveDotInfo4 model1,struCurveInfo model2);

    bool GetBs8051(const char* czStartTime,const char* czEndTime,QStringList* qYQIDList,sqlite_data &ResultData);
    //-----End 4. 比色法仪器操作数据表 -----//
    //耗材类型
    bool GetConsume(sqlite_data &ResultData);
    //修改耗材类型
    bool UpdateConsume(struConsume model);
    //耗材状态
    bool GetConsumeInfo(sqlite_data &ResultData);
    bool GetConsumeInfo(struConsumeInfo *model,sqlite_data &ResultData);
    bool GetConsumeInfo(int iProcessType,sqlite_data &ResultData);
    //添加耗材状态
    bool InsConsumeInfo(struConsumeInfo model);
    //添加耗材更换记录
    bool InsConsumeUpdate(struConsumeUpdate model);
    //获取耗材历史记录
    bool GetConsumeUpdate(const char* cID,sqlite_data &ResultData);
    //删除耗材状态
    bool DelConsumeInfo(struConsumeInfo model);
    //删除耗材更换记录
    bool DelConsumeUpdate(struConsumeUpdate model);
    //修改整条耗材状态
    bool UpdateAllConsumeInfo(struConsumeInfo *model);
    //修改耗材状态
    bool UpdateConsumeInfo(struConsumeInfo model);

   // bool GetBsBaseTempValue(int iType,sqlite_data &ResultData);

    bool Consume_add(const char *ID, const char *DBName,const char *ParentID);
private:
    sqlite_db* pDB;
    char m_DBName[MAX_DBNAME];

};

#endif // CMYDATABASE_H
