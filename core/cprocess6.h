#ifndef CPROCESS6_H
#define CPROCESS6_H
#include "cprocess.h"
class CVocArithmetic;
class CProcess6 : public CProcess
{
public:
    CProcess6();
    virtual ~CProcess6();
    int GetProcessType();

private:
   //测试之前初始化
   int InitBeginOfTest(int nKind=0);
   //保存源始数据
   void SaveSrcDataToTxt();
   //保存吸光度
   void SaveAbsorbDataToTxt();
   //计算结果
   void Calculate(int nCode,bool bCalcAbsorb = true);
   //处理系统PID
   int GetSysModuleStatus();

    void SaveResultToDatabase(int nCodeID = 0);
private:
   CVocArithmetic* m_pCalc;
   bool m_bReadRefValue;//流程中是否有 读参比信号(13)---基线值
   bool m_bOnlyOnceTag;//保存源始数据只允许一次

};

#endif // CPROCESS6_H
