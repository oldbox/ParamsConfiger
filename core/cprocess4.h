#ifndef CPROCESS4_H
#define CPROCESS4_H
#include "cprocess.h"
class CProcess4 : public CProcess
{
public:
    CProcess4();
    ~CProcess4();
    int GetProcessType();
    int InitBeginOfTest(int nKind=0);
    int GetSysModuleStatus();

    bool ReadData(int iCMDAdd,int enHgReadCyc,int iReadSignCou);
    int SaveDataToTx(int nKind);
    bool CalcTestValue(int nKey);
    void SaveSrcDataToTxt(int nKind);

private:
    vector<int> m_vtnBaseSignal,m_vtnSampleSignal;//基线值、样品值
    vector<int> m_vtnBaseAvg,m_vtnSampleAvg;
    vector<int> m_vtnMinSample;
    vector<struBsLinear4> m_vtBsLinear; //比色法温度
    double dBaseSignalVal,dSampleSignalVal;
    int m_nCalcWay;
    int m_iTempValue;   //临时值（保存读取基线值的间隔时间或DA值（六价铬）；待调试完程序后可以去掉

private:
    bool ReadADData(int iCmdType,int iReadCout,int iTimeout);
    double CaclWaySum2(int nKind);

    void SaveResultToDatabase(int nCodeID = 0);
};

#endif // CPROCESS4_H
