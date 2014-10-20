#ifndef CPROCESS1_H
#define CPROCESS1_H

#include <unistd.h>
#include <stdlib.h>
#include <QMap>
#include <QTime>
#include "cprocess.h"


class CProcess1 : public CProcess
{
public:
    CProcess1();
    ~CProcess1();

    int GetProcessType();
    int StartTestCmd(int nCmd,int nKind,QDateTime dt);
    int ScanFlowFile(const char *filename);
    STATUS DoCurrentCmd();
    int ReadMCUStatusInfo();
    int EchemModuleStatus();
    void GetMCUWorkStatus(unsigned char* bBuff);
    bool GetModuleDataInfo();
    bool ReadData(int nKind,int nCode);
    void DoProcess();
    bool CalcTestValue(int nCodeID);
    int InitBeginOfTest(int nKind=0);

    void DealReagentsChange();

private:
    void NotifyShowCurve(int nKind);
    void MainWindowDebug(const char *cStr);
    void SaveResultToDatabase(int nCodeID = 0);
    void ScanSubFlowFile(const char *filename);
    void dealRangeSwitch();
    int ReadFlowFile();
    void CaclInitData(int nKind);
    void SetRegisterWorkStatus(unsigned short nStatus);
    void SetRegisterFaultType(unsigned short nType);
    void SetRegisterProcessCode();
    void SaveSrcDataToTxt();
    void SaveScanDataToTxt();
    void SetDataCacl(QVector<double> vtnDataY,QVector<double> vtnDataX);

    bool m_bYQBusy;//仪器流程是否忙
    bool m_bRongChuTag;//容出标记位

    QTime m_tmBeginSleep;//静置开始时间
    float m_fRangeValue;
    bool m_bEndFlag; //正常中止标识,手动中止不保存数据
    int m_nIncrement;
    bool m_bAjustFlag;//空白校准 标样校准标识，用来标识只保存一次校准信息
    vector<stuProc> m_vProcFile;
    struEchemVector m_struEchemVector;
    vector<double> m_vtnSyInitdata,m_vtnByInitdata;



};

#endif // CPROCESS1_H
