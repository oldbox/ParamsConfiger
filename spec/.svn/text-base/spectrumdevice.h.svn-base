#ifndef SPECTRUMDEVICE_H
#define SPECTRUMDEVICE_H
#include "bwtekUSB.h"
#include "../core/vardef.h"
#include <vector>
using namespace std;

const int AVERAGE = 100;
class CProcess;
class spectrumDevice
{
public:
    spectrumDevice(CProcess* proc);
    virtual ~spectrumDevice();

private:
    void Initspec();//初始化光谱仪

    void DataPH(signalKind Mode);//平滑处理

    bool GetTurbidity(int nDescWaveLen,double& dfAbsorb);
    int GetFindWaveLenValue(int nValue);
private:
    struct USBINFO m_bwtekUSBInfo;
    char m_czPath[256]; //配置文件路径
    int m_nIntegrationTime;//积分时间,豪秒
    double m_coefs[4];

     CProcess* m_proc;//保存过程数据的数据库对象
public:
    vector<double> m_vtnSignal;//信号
    vector<double>m_vtfWaveLen;//波长,
    vector<double> m_vtnTestSignal,m_vtnRefSignal,m_vtnDarkSignal;// 测试信号(样品值),参比信号(基线值),暗信号
    vector<double> m_vtfAbsorbency;//吸光度

    void SetIntegrationTime(int nIntegrationTime);
    bool ReadCoefsFromRom();//需要读取波长校准系数
    int ReadSpectrumData(signalKind Mode);//读光谱仪数据
    bool CalcTestValue(const int nCalcMode,const int LeftBasePoint,const int RightBasePoint, double& fAbsorb/*,double& fValueLeft,double& fValueRight*/);

    //计算吸光度bMode:false，则采用老的计算方式，true则采用新的计算方式,则fBaseWaveLength为基点
    int CalcAbsorb(bool bMode = false,float fBaseWaveLength = 0);
};

#endif // SPECTRUMDEVICE_H
