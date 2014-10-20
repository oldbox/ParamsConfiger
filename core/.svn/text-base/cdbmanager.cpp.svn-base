#include "cdbmanager.h"
#include <stdio.h>
#include <dirent.h>
#include <cstddef>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

CDBManager::CDBManager()
{
}
CDBManager::~CDBManager()
{
}

int CDBManager::mkdir_r(const char *path) {

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

bool CDBManager::AlterTable(const char* czChildPath,const char* DBName,const char* czSql)
{
    char czPath[256];memset(czPath,0,256);
    sprintf(czPath,"%s/%s",DB_ROOT_PATH,czChildPath);
    mkdir_r(czPath);
    bool bRT = false;
    char czFileName[256];memset(czFileName,0,256);
    sprintf(czFileName,"%s/%s",czPath,DBName);
    sqlite_db* pDB = new sqlite_db(czFileName);
    if(pDB->Connected())
    {
        bRT = pDB->ExecSQL(czSql);
    }
    delete pDB;
    return bRT;
}

bool CDBManager::Create_DB_YQ(const char* czChildPath,const char* DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_YQ( \
            YQID    VARCHAR(8)    NOT NULL, \
            PRIMARY KEY (YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_3005(const char* czChildPath,const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_3005( \
        DataTime           DATETIME         NOT NULL, \
        YQID               VARCHAR(8)       NOT NULL, \
        DataValue          FLOAT(8, 0)      NOT NULL, \
        AddStChroma        DOUBLE(18, 0), \
        AddStVolume        DOUBLE(18, 0), \
        DataTag            CHAR(8)          NOT NULL, \
        T_8051_DataTime    DATETIME         NOT NULL, \
        PRIMARY KEY (DataTime, YQID) \
            );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_3016(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_3016( \
            DataTime      DATETIME         NOT NULL, \
            YQID          VARCHAR(8)       NOT NULL, \
            DataValue     FLOAT(8, 0), \
            StandValue    DOUBLE(18, 0), \
            DataTag       VARCHAR(8)       NOT NULL, \
            PRIMARY KEY (DataTime, YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_3025(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_3025( \
            DataTime     DATETIME       NOT NULL, \
            YQID         VARCHAR(8)     NOT NULL, \
            DataValue    FLOAT(8, 0)    NOT NULL, \
            DataTag      VARCHAR(8)     NOT NULL, \
            PRIMARY KEY (YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_8051(const char* czChildPath,const char* DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_8051( \
            DataTime     DATETIME       NOT NULL, \
            YQID         VARCHAR(8)     NOT NULL, \
            DataValue    FLOAT(8, 0)    NOT NULL, \
            DataTag      VARCHAR(8)     NOT NULL, \
            PRIMARY KEY (DataTime, YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_Linear(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_echemlinear( \
            DataTime       DATETIME       NOT NULL, \
            ProcessType    INT            NOT NULL, \
            DataValue      FLOAT(8, 0)    NOT NULL, \
            PRIMARY KEY (DataTime, ProcessType) \
        ) \
        ;");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_Maintain(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_echemmaintain( \
            DataTime    DATETIME        NOT NULL, \
            content     VARCHAR(256) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_EchemHh(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_EchemHh( \
            DataTime        DATETIME       NOT NULL, \
            YQID            VARCHAR(8)     NOT NULL, \
            H1              FLOAT(8, 0), \
            H2              FLOAT(8, 0), \
            KindCal         INT,\
            LeftBase        FLOAT(8, 0), \
            RightBase       FLOAT(8, 0),\
            HeatValue       FLOAT(8, 0), \
            HeatTime        INT \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_Consume(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Consume( \
            ID           VARCHAR(10)     NOT NULL, \
            Name         VARCHAR(32)    NOT NULL, \
            ParentID     VARCHAR(10)   , \
            PRIMARY KEY (ID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_ConsumeInfo(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_ConsumeInfo( \
            ID           VARCHAR(10)     NOT NULL, \
            ProcessType  INT    NOT NULL, \
            MaxValue     FLOAT(8,2)     ,\
            RealValue    FLOAT(8,2)     ,\
            WaringValue  FLOAT(8,2)     ,\
            AlarmValue   FLOAT(8,2)     ,\
            FaultValue   FLOAT(8,2)     ,\
            QualityDay   INT            ,\
            ModifyTime   DATETIME       ,\
            EmployUnit   INT            ,\
            PRIMARY KEY (ID,ProcessType) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_ConsumeUpdate(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_ConsumeUpdate( \
            [ConsID]  integer PRIMARY KEY autoincrement,\
            ID           VARCHAR(10)     NOT NULL, \
            AddVolume    FLOAT(8,2)     ,\
            UpdateTime   DATETIME    NOT NULL \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_2091(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_2091( \
            DataTime    DATETIME    NOT NULL, \
            ProcessType  INT     NOT NULL, \
            Code     VARCHAR(18)     NOT NULL, \
            Level    INT    ,\
            Remark   VARCHAR(40) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_DB_Status(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Status( \
            DataTime     DATETIME       NOT NULL, \
            YQID         VARCHAR(8)     NOT NULL, \
            DataValue    FLOAT(8, 0)    NOT NULL, \
            PRIMARY KEY (DataTime, YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

bool CDBManager::Create_1_Hh(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Hh( \
            YQID    VARCHAR(8)     NOT NULL, \
            H1      FLOAT(8, 0), \
            H2      FLOAT(8, 0), \
            fResult FLOAT(8, 0), \
            PRIMARY KEY (YQID) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::CreateLog(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Log( \
            DataTime    DATETIME        NOT NULL, \
            wtype       VARCHAR(10), \
            content     VARCHAR(256) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_1_ProcessData(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_ProcessData( \
            wtype    VARCHAR(10), \
            V        FLOAT(8, 0), \
            A        FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_1_Scan(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Scan( \
            A    FLOAT(8, 0), \
            V    FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::CreateTempData(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_TempData( \
            DataTime    DATETIME        NOT NULL, \
            content     VARCHAR(256) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_2_Absorbance(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_Absorbance( \
            DataTime      DATETIME         NOT NULL, \
            NO2           FLOAT(8, 0), \
            S02           FLOAT(8, 0), \
            N0            FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_2_SpectrumInfo(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_SpectrumInfo( \
            DataTime      DATETIME         NOT NULL, \
            NO2           FLOAT(8, 0), \
            S02           FLOAT(8, 0), \
            N0            FLOAT(8, 0), \
            IntegTime     FLOAT(8, 0), \
            AverageTimes  FLOAT(8, 0)  \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_3_SrcData(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_srcdata( \
            RT       DOUBLE(18, 0), \
            Vol      DOUBLE(18, 0), \
            Tp       FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_VocGroup(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocgroup( \
            YQID            VARCHAR(8)     NOT NULL, \
            RemainTime      DOUBLE(18, 0), \
            WinFactor       FLOAT(8, 0), \
            Kind            INT,\
            Multiplier      FLOAT(8, 0), \
            K               FLOAT(8, 0), \
            B               FLOAT(8, 0), \
            KB              FLOAT(8, 0), \
            R               FLOAT(8, 0), \
            MinPW           INT,\
            MaxPW           INT,\
            Minimum         FLOAT(8, 0), \
            MinArea         FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_VocDots(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocdots( \
            YQID            VARCHAR(8)     NOT NULL, \
            Id              INT, \
            Conc            FLOAT(8, 0), \
            Area            FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_VocInternal(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocinternal( \
            ID              integer PRIMARY KEY autoincrement,\
            DataTime        DATETIME       NOT NULL, \
            RemainTime      FLOAT(8, 0), \
            RTime           FLOAT(8, 0),\
            Conc            FLOAT(8, 0),\
            RConc           FLOAT(8, 0),\
            Area            FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
//bool CDBManager::Create_DB_VocInternal(const char *czChildPath, const char *DBName)
//{
//    char czSql[1024];memset(czSql,0,1024);
//    sprintf(czSql,"CREATE TABLE t_vocinternal( \
//            DataTime        DATETIME       NOT NULL, \
//            RemainTime      FLOAT(8, 0), \
//            Conc            FLOAT(8, 0), \
//            Area            FLOAT(8, 0) \
//        );");
//    return AlterTable(czChildPath,DBName,czSql);
//}

bool CDBManager::Create_DB_VocSubDots(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocsubdots( \
            YQID            VARCHAR(8)     NOT NULL, \
            Conc            FLOAT(8, 0), \
            Id              INT, \
            Area            FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_3_VocPeaks(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocpeaks( \
            PeakId          INT     NOT NULL, \
            PeakPos         INT, \
            PeakRTime       FLOAT(8, 0), \
            LeftPos         INT, \
            LeftRTime       FLOAT(8, 0), \
            RightPos        INT, \
            RightRTime      FLOAT(8, 0), \
            PeakWidth       FLOAT(8, 0), \
            LeftBaseLine    FLOAT(8, 0), \
            RigtBaseLine    FLOAT(8, 0), \
            PeakHigh        DOUBLE(18, 0), \
            PeakArea        DOUBLE(18, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
bool CDBManager::Create_DB_VocDataInfo(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE t_vocdatainfo( \
            DataTime        DATETIME       NOT NULL, \
            TestKind        INT, \
            YQID            VARCHAR(8)     NOT NULL, \
            RemainTime      FLOAT(8, 0), \
            PeakHight       FLOAT(8, 0), \
            Area            FLOAT(8, 0), \
            K               FLOAT(8, 0), \
            B               FLOAT(8, 0), \
            R               FLOAT(8, 0) \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

//--------------------------------------------------//
// 4. 比色法仪器建立数据表
//A.主库
//比色化曲线点信息(主库)
bool CDBManager::Create_4_BsCurveDotInfo(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_BsCurveDotInfo( \
            CurveID       INT NOT NULL,\
            YQID          VARCHAR(8) NOT NULL, \
            CalcMode      INT NOT NULL,\
            PointID       INT NOT NULL,\
            Chroma        FLOAT(8, 0), \
            Absorbency    FLOAT(8, 0)  \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
//比色法曲线管理(主库)
bool CDBManager::Create_4_BsCurveManger(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_BsCurveManger( \
            CurveID        INT           NOT NULL,  \
            YQID           VARCHAR(8)    NOT NULL,  \
            CalcMode       INT           NOT NULL,  \
            DataTime       DATETIME      NOT NULL,  \
            R              FLOAT(8, 0),  \
            K              FLOAT(8, 0),  \
            B              FLOAT(8, 0),  \
            BlankAbsorb    FLOAT(8, 0),  \
            StdConc        FLOAT(8, 0),  \
            StdAbsorb      FLOAT(8, 0),  \
            MinValue       FLOAT(8, 0),  \
            MaxValue       FLOAT(8, 0)   \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}
//比色法详细数据(主库)
bool CDBManager::Create_4_BsDataInfo(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_BsDataInfo( \
            DataTime       DATETIME      NOT NULL,  \
            TestKind       INT           NOT NULL,  \
            YQID           VARCHAR(8)    NOT NULL,  \
            BaseValue      FLOAT(8, 0),  \
            SampleValue    FLOAT(8, 0),  \
            Absorbency     FLOAT(8, 0),  \
            R              FLOAT(8, 0),  \
            K              FLOAT(8, 0),  \
            B              FLOAT(8, 0),  \
            BlankAbsorb    FLOAT(8, 0),  \
            StdConc        FLOAT(8, 0),  \
            StdAbsorb      FLOAT(8, 0),  \
            DigeTemp       FLOAT(8, 0),  \
            DigeTime       FLOAT(8, 0),  \
            CurveID        int      NOT NULL  \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

//--------------------------------------------------//
//原子荧光曲线管理(主库)
bool CDBManager::Create_5_AFSCurveManger(const char *czChildPath, const char *DBName)
{
    char czSql[1024];memset(czSql,0,1024);
    sprintf(czSql,"CREATE TABLE T_AFSCurveManger( \
            CurveID        INT           NOT NULL,  \
            YQID           VARCHAR(8)    NOT NULL,  \
            CalcMode       INT           NOT NULL,  \
            R              FLOAT(8, 0),  \
            K              FLOAT(8, 0),  \
            B              FLOAT(8, 0),  \
            BlankAbsorb    FLOAT(8, 0),  \
            StdConc        FLOAT(8, 0),  \
            StdAbsorb      FLOAT(8, 0),  \
            MinValue       FLOAT(8, 0),  \
            MaxValue       FLOAT(8, 0)   \
        );");
    return AlterTable(czChildPath,DBName,czSql);
}

