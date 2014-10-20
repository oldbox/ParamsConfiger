#ifndef CDBMANAGER_H
#define CDBMANAGER_H
#include "cdatabase.h"

class CDBManager
{
public:
    CDBManager();
    virtual ~CDBManager();

    bool Create_DB_YQ(const char* czChildPath,const char *DBName);
    bool Create_DB_3005(const char* czChildPath,const char *DBName);
    bool Create_DB_3016(const char* czChildPath,const char *DBName);
    bool Create_DB_3025(const char* czChildPath,const char* DBName);
    bool Create_DB_8051(const char* czChildPath,const char* DBName);
    bool Create_DB_Linear(const char* czChildPath,const char* DBName);
    bool Create_DB_Maintain(const char *czChildPath, const char *DBName);
    bool Create_DB_EchemHh(const char *czChildPath, const char *DBName);


    bool Create_DB_Consume(const char* czChildPath,const char *DBName);
    bool Create_DB_ConsumeInfo(const char* czChildPath,const char *DBName);
    bool Create_DB_ConsumeUpdate(const char* czChildPath,const char *DBName);
    bool Create_DB_2091(const char* czChildPath,const char *DBName);



    bool Create_DB_Status(const char *czChildPath, const char *DBName);

    bool Create_1_Hh(const char *czChildPath, const char *DBName);
    bool CreateLog(const char *czChildPath, const char *DBName);
    bool Create_1_ProcessData(const char *czChildPath, const char *DBName);
    bool Create_1_Scan(const char *czPath, const char *DBName);
    bool CreateTempData(const char *czChildPath, const char *DBName);
    bool Create_2_Absorbance(const char *czChildPath, const char *DBName);
    bool Create_2_SpectrumInfo(const char *czChildPath, const char *DBName);


    //有机物数据库表创建
    bool Create_3_SrcData(const char *czChildPath, const char *DBName);//源始数据
    bool Create_DB_VocGroup(const char *czChildPath, const char *DBName);
    bool Create_DB_VocDots(const char *czChildPath, const char *DBName);
    bool Create_DB_VocInternal(const char *czChildPath, const char *DBName);
    bool Create_DB_VocSubDots(const char *czChildPath,const char *DBName);
    bool Create_3_VocPeaks(const char *czChildPath,const char *DBName);
    bool Create_DB_VocDataInfo(const char *czChildPath,const char *DBName);

    // Begin 4. 比色法仪器建立数据表
    //A.主库
    //(1)比色化曲线点信息(主库)
    bool Create_4_BsCurveDotInfo(const char *czChildPath, const char *DBName);
    //(2)比色法曲线管理(主库)
    bool Create_4_BsCurveManger(const char *czChildPath, const char *DBName);
    //(3)比色法详细数据(主库)
    bool Create_4_BsDataInfo(const char *czChildPath, const char *DBName);

    //B.分库
    //bool Create_4_ProcessData(const char *czChildPath, const char *DBName);
    //bool Create_4_SampleVal(const char *czChildPath, const char *DBName);
    //bool Create_4_BaselineVal(const char *czChildPath, const char *DBName);
    //bool Create_4_BaseSampTimeSpace(const char *czChildPath, const char *DBName);
    //比色法实时温度(分库)
    //bool Create_4_BsLinear(const char *czChildPath, const char *DBName);
    // End 4. 比色法仪器建立数据表
    bool Create_5_AFSCurveManger(const char *czChildPath, const char *DBName);
private:
    int mkdir_r(const char *path);
    bool AlterTable(const char* czChildPath, const char *DBName, const char *czSql);
};

#endif // CDBMANAGER_H
