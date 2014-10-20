#ifndef CPROCESS5_H
#define CPROCESS5_H

#include <unistd.h>
#include <stdlib.h>
#include <QMap>
#include <QTime>
#include "cprocess.h"

class CVocArithmetic;
class CProcess5 : public CProcess
{
public:
    CProcess5();
    ~CProcess5();

    int GetProcessType();
    int StartTestCmd(int nCmd,int nKind,QDateTime dt);
    int ScanFlowFile(const char *filename);
    STATUS DoCurrentCmd();
    int ReadMCUStatusInfo();
    bool GetModuleDataInfo();
    void GetMCUWorkStatus(unsigned char* bBuff);
    bool ReadData();
    void DoProcess();
    bool CalcTestValue(int nKey);
    int InitBeginOfTest(int nKind=0);
    void DealReagentsChange();


private:
    void SaveSrcDataToTxt();
    void MainWindowDebug(const char *cStr);
    void SaveResultToDatabase(int nCodeID = 0);
    void ScanSubFlowFile(const char *filename);
    int ReadFlowFile();
    bool VerifyRange(const char* czID,int nCurrentCurve,double dCurrentValue);

    bool m_bYQBusy;//仪器流程是否忙
    QTime m_tmBeginSleep;//静置开始时间
    float m_fRangeValue;
    bool m_bEndFlag; //正常终止标识
    int m_nCount;//采集点数 最大10000
    vector<stuProc> m_vProcFile;
    struAFSVector m_struAFSVector;
    int m_nKind;
    int m_nValueCount;//信号采集数据个数
    int m_nParaRate;//信号采集的频率
    bool m_bFault;
    bool m_bExitRead;//流程是否结束标记
    bool m_bUpSwitchCurve;//向上切曲线标识位
    bool m_bDowSwitchCurve;//向下切曲线标识位;
};

#endif // CPROCESS1_H
