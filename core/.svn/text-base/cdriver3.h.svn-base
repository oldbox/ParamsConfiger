#ifndef CDRIVER3_H
#define CDRIVER3_H

#include "cdriver.h"
#include "cmodbus.h"
#include <qmutex.h>
class CDriver3 : public CDriver
{
public:
    CDriver3(CSerialPort* pSerialPort);
    ~CDriver3();

    void DriverTest();
    //有机物相关操作
public:
    //联动模块IO控制
    bool IOCtrl_LD(int nAddr,int nChannel,bool bOnOff);
    //联动模块单抽
    bool SinglePoll_LD(int nAddr,int nSource,int nSpeed,int nStep);
    //联动模块单推
    bool SinglePush_LD(int nAddr,int nDes1,int nSpeed,int nDest2);
    //柱塞泵反复抽推
    bool RePollPush_LD(int nAddr,int nSrcDcf1,int nSrcDcf2,int nDest,int nSpeed,int nStep,int nTimes);

    //继电器模块IO控制
    bool IOCtrl_JDQ(int nAddr,int nChannel,bool bOnOff);

    //采样速率设置
    bool SetDataSpeed_150(int nAddr,int nGainDataRate,int nGainDataRTime);
    //启动第N路温度
    bool StartTemptSigle_151(int nAddr,int nChannel,int nOnOff);
    //启动线性升温
    bool StartLineTempt_152(int nAddr,int nChannel,int nOnOff);
    //设置单路温度 0 捕集井温度  1 FID检测器温度 2 箱体温度  3 毛细柱温度
    bool SetTemptSingle_153(int nAddr,int nChannel,int nTempt);
    //设置线性升温阶梯
    bool SetLineTemptDesc_154(int nAddr,int nChannel,int nTempt1,int nTempt2,int nTempt3,int nTempt4,int nTempt5);
    //设置线性升温斜率
    bool SetLineTemptK_155(int nAddr,int nChannel,int nK1,int nK2,int nK3,int nK4,int nK5);
    //设置保持时间
    bool SetLineTemptRTime_156(int nAddr,int nChannel,int nRT1,int nRT2,int nRT3,int nRT4,int nRT5);
    //设置PID参数 第N路编号	P值	I值	D值	限制斜率
    bool SetPIDSet_157(int nAddr,int nChannel,int P,int I,int D,int K);
    //高压输出
    bool GYOupput_158(int nAddr,int nOnOff);
    //点火
    bool Fire_159(int nAddr);
    //数据采集0 停止采集 1 启动测试采集  2 启动基线采集
    bool StartGetData_160(int nAddr,int nKind);
    //设置DA值
    bool SetDAValue_161(int nAddr,int nAD);
    //开关ECD电源
    bool ECDPower_162(int nAddr,int nOnOff);
    //采样速率
    bool DataSpeed_163(int nAddr,int nValue);

public:
    //新增加一个接口,在驱动层添加业务控制
    bool ReadComData(int nAddr,int nReg, int nCount,int ntimeout,unsigned char* czData);
    bool WriteComData(int nAddr,int FisrtRegister,int Regs,unsigned char* czData);
private:
    QMutex m_mutex;
};

#endif // CDRIVER2_H
