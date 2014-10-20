#include "spectrumdevice.h"
#include "../core/cconfig.h"
#include "../core/cprocess.h"
#include <math.h>
spectrumDevice::spectrumDevice(CProcess* proc)
{
    m_proc = proc;
    m_coefs[0] = 152.475425519049;
    m_coefs[1] = 0.448939561030784;
    m_coefs[2] = -3.09039664909139E-5;
    m_coefs[3] = -1.18459983713094E-9;

    Initspec();

    memset(m_czPath,0,256);
    sprintf(m_czPath,"%s/sperom.ini",INI_ROOT_PATH);

    char tmp[64];memset(tmp,0,64);
    GetAfxConfig()->GetIni("Spectrum","IntegrationTime",tmp);
    m_nIntegrationTime = atoi(tmp);

    //初始化
    m_vtnSignal.assign(PIXELNO_2048,0);
    m_vtnDarkSignal.assign(PIXELNO_2048,0);
    m_vtnRefSignal.assign(PIXELNO_2048,0);
    m_vtnTestSignal.assign(PIXELNO_2048,0);
    m_vtfWaveLen.assign(PIXELNO_2048,0);
    m_vtfAbsorbency.assign(PIXELNO_2048,0);
}
spectrumDevice::~spectrumDevice()
{
}

void spectrumDevice::Initspec()
{
    strcpy(m_bwtekUSBInfo.fileHex,"FX2.hex"); //BRC115.img for BRC115, Fx2.hex for BRC112
    m_bwtekUSBInfo.nFXType=0; //0 for BRC112, 1 for BRC115
    m_bwtekUSBInfo.nVID=0x16a3;
    m_bwtekUSBInfo.nPID=0x2ec7; //0x2ec7 for BRC112, 0x2ed0 for BRC115,  0x2ed4 for BRC115P
    m_bwtekUSBInfo.nUnit=0x00;
    m_bwtekUSBInfo.nIntTime=5;//5 for BRC112, Do not change the minimum integration time
}
bool spectrumDevice::ReadCoefsFromRom()
{
    if((bwtekTestUSB(1, PIXELNO_2048, 1, 0, (void *)&m_bwtekUSBInfo)) < 0)
    {
        return false;
    }
    else
    {
        char tmp[255];memset(tmp,0,255);
        bwtekReadEEPROMUSB(m_czPath, 0);
        GetAfxConfig(NULL,"sperom.ini")->GetIni("COMMON","coefs_a0",tmp);
        m_coefs[0] = atof(tmp);
        GetAfxConfig(NULL,"sperom.ini")->GetIni("COMMON","coefs_a1",tmp);
        m_coefs[1] = atof(tmp);
        GetAfxConfig(NULL,"sperom.ini")->GetIni("COMMON","coefs_a2",tmp);
        m_coefs[2] = atof(tmp);
        GetAfxConfig(NULL,"sperom.ini")->GetIni("COMMON","coefs_a3",tmp);
        m_coefs[3] = atof(tmp);
    }
    return true;
}
void spectrumDevice::SetIntegrationTime(int nIntegrationTime)
{
    m_nIntegrationTime = nIntegrationTime;
}

int spectrumDevice::ReadSpectrumData(signalKind Mode)
{
    //需要读取波长校准系数
    if(!ReadCoefsFromRom())
    {
        //提示，光谱仪连接失败
        m_proc->RecordLog(LOG_FAULT,"光谱仪连接失败,请确保驱动程序是否存在");
        cout<<"Maybe the spectrum driver can't find......"<<endl;
         return 1;
    }
    if(bwtekSetTimeUSB(m_nIntegrationTime ,0) != m_nIntegrationTime)
    {
        //提示，设置积分时间失败
        m_proc->RecordLog(LOG_FAULT,"设置积分时间失败");
        return 2;
    }

    unsigned short nDataArray[PIXELNO_2048];
    long mid[PIXELNO_2048];

    if(bwtekDataReadUSB(0, nDataArray, 0) != PIXELNO_2048)
    {
        //提示，读取光谱数据失败
        m_proc->RecordLog(LOG_FAULT,"读取光谱数据失败");
        return 3;
    }

    //初始化
    m_vtnSignal.assign(PIXELNO_2048,0);
    if(Mode == DarkSignal)
        m_vtnDarkSignal.assign(PIXELNO_2048,0);
    else if(Mode == RefSignal)
        m_vtnRefSignal.assign(PIXELNO_2048,0);
    else if(Mode == TestSignal)
        m_vtnTestSignal.assign(PIXELNO_2048,0);

    for (int i = 0; i < PIXELNO_2048; i++)
    {
        mid[i] = 0;
        nDataArray[i] = 0;
    }

    for(int i = 0;i < AVERAGE;i++)
    {
        if(bwtekDataReadUSB(0,nDataArray,0) != PIXELNO_2048)
        {
            //提示，读取光谱数据失败
             m_proc->RecordLog(LOG_FAULT,"读取光谱数据失败");
            return 3;
        }
        for (int j = 0; j < PIXELNO_2048; j++)
        {
            mid[j] += nDataArray[PIXELNO_2048 - j - 1];
        }
    }
    if(bwtekCloseUSB(0) < 0)
    {
        //提示，关闭光谱仪失败
    }

    for (int i = 0; i < PIXELNO_2048; i++)
    {
        m_vtnSignal[i] = (mid[i] / AVERAGE);
        if(Mode == DarkSignal)
        {
            m_vtnDarkSignal[i] = m_vtnSignal[i];
        }
        else if(Mode == RefSignal)
        {
            m_vtnRefSignal[i] = m_vtnSignal[i];
        }
        else if(Mode == TestSignal)
        {
            m_vtnTestSignal[i] = m_vtnSignal[i];
        }
    }
    //清空
    m_vtfWaveLen.clear();
    m_vtfWaveLen.assign(PIXELNO_2048,0);

    //像素转换成波长,最后258个点没有数据的
    for (int i = 0; i < PIXELNO_2048; i++)
    {
        m_vtfWaveLen[i] = m_coefs[0] + m_coefs[1]*i + m_coefs[2]*i*i + m_coefs[3]*i*i*i;
    }

    DataPH(Mode);
    return 0;
}
void spectrumDevice::DataPH(signalKind Mode)
{
    int nNumber = PIXELNO_2048;
    vector<unsigned short> vTemp;
    double fValue = 0;
    if(Mode == DarkSignal)
    {
        fValue = (double)((69*m_vtnDarkSignal[0]+4*(m_vtnDarkSignal[1]+m_vtnDarkSignal[3])-6*m_vtnDarkSignal[2]-m_vtnDarkSignal[4])/70.0);
        vTemp.push_back(fValue);
        fValue = (double)((2*(m_vtnDarkSignal[0]+m_vtnDarkSignal[4])+27*m_vtnDarkSignal[1]+12*m_vtnDarkSignal[2]-8*m_vtnDarkSignal[3])/35.0);
        vTemp.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //一次五点法平滑拟合公式
            fValue = (double)((-3*m_vtnDarkSignal[i-2]+12*m_vtnDarkSignal[i-1]+17*m_vtnDarkSignal[i]+12*m_vtnDarkSignal[i+1]-3*m_vtnDarkSignal[i+2])/35);
            vTemp.push_back((unsigned short)fValue);
        }

        fValue = (double)((2*(m_vtnDarkSignal[nNumber-1]+m_vtnDarkSignal[nNumber-5])+27*m_vtnDarkSignal[nNumber - 2]+12*m_vtnDarkSignal[nNumber - 3]-8*m_vtnDarkSignal[nNumber-4])/35.0);
        vTemp.push_back(fValue);
        fValue = (double)((69*m_vtnDarkSignal[nNumber-1]+4*(m_vtnDarkSignal[nNumber-2]+m_vtnDarkSignal[nNumber-4])-6*m_vtnDarkSignal[nNumber-3]-m_vtnDarkSignal[nNumber-5])/70.0);
        vTemp.push_back(fValue);

        m_vtnDarkSignal.clear();

        fValue = (double)((69*vTemp[0]+4*(vTemp[1]+vTemp[3])-6*vTemp[2]-vTemp[4])/70.0);
        m_vtnDarkSignal.push_back(fValue);
        fValue = (double)((2*(vTemp[0]+vTemp[4])+27*vTemp[1]+12*vTemp[2]-8*vTemp[3])/35.0);
        m_vtnDarkSignal.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //二次五点法平滑拟合公式
            fValue = (double)((-3*vTemp[i-2]+12*vTemp[i-1]+17*vTemp[i]+12*vTemp[i+1]-3*vTemp[i+2])/35);
            m_vtnDarkSignal.push_back((unsigned short)fValue);
        }

        fValue = (double)((2*(vTemp[nNumber-1]+vTemp[nNumber-5])+27*vTemp[nNumber - 2]+12*vTemp[nNumber - 3]-8*vTemp[nNumber-4])/35.0);
        m_vtnDarkSignal.push_back(fValue);
        fValue = (double)((69*vTemp[nNumber-1]+4*(vTemp[nNumber-2]+vTemp[nNumber-4])-6*vTemp[nNumber-3]-vTemp[nNumber-5])/70.0);
        m_vtnDarkSignal.push_back(fValue);

        vTemp.clear();

    }else if(Mode == RefSignal)
    {
        fValue = (double)((69*m_vtnRefSignal[0]+4*(m_vtnRefSignal[1]+m_vtnRefSignal[3])-6*m_vtnRefSignal[2]-m_vtnRefSignal[4])/70.0);
        vTemp.push_back(fValue);
        fValue = (double)((2*(m_vtnRefSignal[0]+m_vtnRefSignal[4])+27*m_vtnRefSignal[1]+12*m_vtnRefSignal[2]-8*m_vtnRefSignal[3])/35.0);
        vTemp.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //一次五点法平滑拟合公式
            fValue = (double)((-3*m_vtnRefSignal[i-2]+12*m_vtnRefSignal[i-1]+17*m_vtnRefSignal[i]+12*m_vtnRefSignal[i+1]-3*m_vtnRefSignal[i+2])/35);
            vTemp.push_back((unsigned short)fValue);
        }
        fValue = (double)((2*(m_vtnRefSignal[nNumber-1]+m_vtnRefSignal[nNumber-5])+27*m_vtnRefSignal[nNumber - 2]+12*m_vtnRefSignal[nNumber - 3]-8*m_vtnRefSignal[nNumber-4])/35.0);
        vTemp.push_back(fValue);
        fValue = (double)((69*m_vtnRefSignal[nNumber-1]+4*(m_vtnRefSignal[nNumber-2]+m_vtnRefSignal[nNumber-4])-6*m_vtnRefSignal[nNumber-3]-m_vtnRefSignal[nNumber-5])/70.0);
        vTemp.push_back(fValue);

        m_vtnRefSignal.clear();

        fValue = (double)((69*vTemp[0]+4*(vTemp[1]+vTemp[3])-6*vTemp[2]-vTemp[4])/70.0);
        m_vtnRefSignal.push_back(fValue);
        fValue = (double)((2*(vTemp[0]+vTemp[4])+27*vTemp[1]+12*vTemp[2]-8*vTemp[3])/35.0);
        m_vtnRefSignal.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //二次五点法平滑拟合公式
            fValue = (double)((-3*vTemp[i-2]+12*vTemp[i-1]+17*vTemp[i]+12*vTemp[i+1]-3*vTemp[i+2])/35);
            m_vtnRefSignal.push_back((unsigned short)fValue);
        }

        fValue = (double)((2*(vTemp[nNumber-1]+vTemp[nNumber-5])+27*vTemp[nNumber - 2]+12*vTemp[nNumber - 3]-8*vTemp[nNumber-4])/35.0);
        m_vtnRefSignal.push_back(fValue);
        fValue = (double)((69*vTemp[nNumber-1]+4*(vTemp[nNumber-2]+vTemp[nNumber-4])-6*vTemp[nNumber-3]-vTemp[nNumber-5])/70.0);
        m_vtnRefSignal.push_back(fValue);

        vTemp.clear();
    }else if(Mode == TestSignal)
    {
        //b(1) = (69*a(1) +4*(a(2) +a(4)) -6*a(3) -a(5)) /70;
        fValue = (double)((69*m_vtnTestSignal[0]+4*(m_vtnTestSignal[1]+m_vtnTestSignal[3])-6*m_vtnTestSignal[2]-m_vtnTestSignal[4])/70.0);
        vTemp.push_back(fValue);
        //b(2) = (2* (a(1) +a(5)) +27*a(2) +12*a(3) -8*a(4)) /35;
        fValue = (double)((2*(m_vtnTestSignal[0]+m_vtnTestSignal[4])+27*m_vtnTestSignal[1]+12*m_vtnTestSignal[2]-8*m_vtnTestSignal[3])/35.0);
        vTemp.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //一次五点法平滑拟合公式
            //b (j) = (-3*(a(j-2) +a(j+2)) +12*(a(j-1) +a(j+1)) +17*a(j)) /35;
            fValue = (double)((-3*m_vtnTestSignal[i-2]+12*m_vtnTestSignal[i-1]+17*m_vtnTestSignal[i]+12*m_vtnTestSignal[i+1]-3*m_vtnTestSignal[i+2])/35.0);
            vTemp.push_back((unsigned short)fValue);
        }

        //b (n-1) = (2*(a(n) +a(n-4)) +27*a(n-1) +12*a(n-2) -8*a(n-3)) /35;
        fValue = (double)((2*(m_vtnTestSignal[nNumber-1]+m_vtnTestSignal[nNumber-5])+27*m_vtnTestSignal[nNumber - 2]+12*m_vtnTestSignal[nNumber - 3]-8*m_vtnTestSignal[nNumber-4])/35.0);
        vTemp.push_back(fValue);

        //b (n) = (69*a(n) +4* (a(n-1) +a(n-3)) -6*a(n-2) -a(n-4)) /70;
        fValue = (double)((69*m_vtnTestSignal[nNumber-1]+4*(m_vtnTestSignal[nNumber-2]+m_vtnTestSignal[nNumber-4])-6*m_vtnTestSignal[nNumber-3]-m_vtnTestSignal[nNumber-5])/70.0);
        vTemp.push_back(fValue);

        m_vtnTestSignal.clear();

        fValue = (double)((69*vTemp[0]+4*(vTemp[1]+vTemp[3])-6*vTemp[2]-vTemp[4])/70.0);
        m_vtnTestSignal.push_back(fValue);
        fValue = (double)((2*(vTemp[0]+vTemp[4])+27*vTemp[1]+12*vTemp[2]-8*vTemp[3])/35.0);
        m_vtnTestSignal.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //二次五点法平滑拟合公式
            fValue = (double)((-3*vTemp[i-2]+12*vTemp[i-1]+17*vTemp[i]+12*vTemp[i+1]-3*vTemp[i+2])/35);
            m_vtnTestSignal.push_back(fValue);
        }

        fValue = (double)((2*(vTemp[nNumber-1]+vTemp[nNumber-5])+27*vTemp[nNumber - 2]+12*vTemp[nNumber - 3]-8*vTemp[nNumber-4])/35.0);
        m_vtnTestSignal.push_back(fValue);
        fValue = (double)((69*vTemp[nNumber-1]+4*(vTemp[nNumber-2]+vTemp[nNumber-4])-6*vTemp[nNumber-3]-vTemp[nNumber-5])/70.0);
        m_vtnTestSignal.push_back(fValue);

        vTemp.clear();

    }else if(Mode == Absorb)
    {
        vector<double> fvtTemp;
        fValue = (double)((69*m_vtfAbsorbency[0]+4*(m_vtfAbsorbency[1]+m_vtfAbsorbency[3])-6*m_vtfAbsorbency[2]-m_vtfAbsorbency[4])/70.0);
        fvtTemp.push_back(fValue);
        fValue = (double)((2*(m_vtfAbsorbency[0]+m_vtfAbsorbency[4])+27*m_vtfAbsorbency[1]+12*m_vtfAbsorbency[2]-8*m_vtfAbsorbency[3])/35.0);
        fvtTemp.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //一次五点法平滑拟合公式
            fValue = (double)((-3*m_vtfAbsorbency[i-2]+12*m_vtfAbsorbency[i-1]+17*m_vtfAbsorbency[i]+12*m_vtfAbsorbency[i+1]-3*m_vtfAbsorbency[i+2])/35);
            fvtTemp.push_back(fValue);
        }


        fValue = (double)((2*(m_vtfAbsorbency[nNumber-1]+m_vtfAbsorbency[nNumber-5])+27*m_vtfAbsorbency[nNumber - 2]+12*m_vtfAbsorbency[nNumber - 3]-8*m_vtfAbsorbency[nNumber-4])/35.0);
        fvtTemp.push_back(fValue);
        fValue = (double)((69*m_vtfAbsorbency[nNumber-1]+4*(m_vtfAbsorbency[nNumber-2]+m_vtfAbsorbency[nNumber-4])-6*m_vtfAbsorbency[nNumber-3]-m_vtfAbsorbency[nNumber-5])/70.0);
        fvtTemp.push_back(fValue);

        m_vtfAbsorbency.clear();

        fValue = (double)((69*fvtTemp[0]+4*(fvtTemp[1]+fvtTemp[3])-6*fvtTemp[2]-fvtTemp[4])/70.0);
        m_vtfAbsorbency.push_back(fValue);
        fValue = (double)((2*(fvtTemp[0]+fvtTemp[4])+27*fvtTemp[1]+12*fvtTemp[2]-8*fvtTemp[3])/35.0);
        m_vtfAbsorbency.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            //二次五点法平滑拟合公式
            fValue = (double)((-3*fvtTemp[i-2]+12*fvtTemp[i-1]+17*fvtTemp[i]+12*fvtTemp[i+1]-3*fvtTemp[i+2])/35);
            m_vtfAbsorbency.push_back(fValue);
        }
        fValue = (double)((2*(fvtTemp[nNumber-1]+fvtTemp[nNumber-5])+27*fvtTemp[nNumber - 2]+12*fvtTemp[nNumber - 3]-8*fvtTemp[nNumber-4])/35.0);
        m_vtfAbsorbency.push_back(fValue);
        fValue = (double)((69*fvtTemp[nNumber-1]+4*(fvtTemp[nNumber-2]+fvtTemp[nNumber-4])-6*fvtTemp[nNumber-3]-fvtTemp[nNumber-5])/70.0);
        m_vtfAbsorbency.push_back(fValue);
        fvtTemp.clear();
    }
}

//0是单波长，1是计算峰高，2是计算峰面积，3是双波长
bool spectrumDevice::CalcTestValue(const int nCalcMode,const int LeftBasePoint,const int RightBasePoint, double& fAbsorb/*,double& fValueLeft,double& fValueRight*/)
{
    double fValueLeft,fValueRight;
    fAbsorb = fValueLeft = fValueRight = 0.0;
    int nIndex = 0;
    if (nCalcMode == 0)//单波长
    {
        for (vector<double>::iterator iter = m_vtfWaveLen.begin();iter != m_vtfWaveLen.end();iter++,nIndex++)
        {
            if(int(*iter) == LeftBasePoint)
            {
                fAbsorb = m_vtfAbsorbency[nIndex];
                break;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        //如果开启了抗浊度功能
        char tmp[64];memset(tmp,0,64);
        GetAfxConfig()->GetIni("Spectrum","Turbidity",tmp);
        if (atoi(tmp) == 1)
        {
            double fTbAbsorb = 0.0;
            if(GetTurbidity(LeftBasePoint,fTbAbsorb))
            {
                if (fAbsorb + 0.05 < fTbAbsorb)
                {
                     m_proc->RecordLog(LOG_ALARM,"浊度吸光度大于目标吸光度,异常");
                }
                else
                {
                    fAbsorb -= fTbAbsorb;
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////
    }else if(nCalcMode == 3)//双波长
    {
        bool bTag = true;

        for (vector<double>::iterator iter = m_vtfWaveLen.begin();iter != m_vtfWaveLen.end();iter++,nIndex++)
        {
            if(int(*iter) == LeftBasePoint && bTag)
            {
                fValueLeft = m_vtfAbsorbency[nIndex];
                bTag = FALSE;

                //////////////////////////////////////////////////////////////////////////
                //如果开启了抗浊度功能
                char tmp[64];memset(tmp,0,64);
                GetAfxConfig()->GetIni("Spectrum","Turbidity",tmp);
                if (atoi(tmp) == 1)
                {
                    double fTbAbsorb = 0.0;
                    if(GetTurbidity(LeftBasePoint,fTbAbsorb))
                    {
                        if (fValueLeft + 0.05 < fTbAbsorb)
                        {
                            m_proc->RecordLog(LOG_ALARM,"浊度吸光度大于目标吸光度,异常");
                        }
                        else
                        {
                            fValueLeft -= fTbAbsorb;
                        }
                    }
                }
                //////////////////////////////////////////////////////////////////////////
            }
            if (!bTag)
            {
                if(int(*iter) == RightBasePoint)
                {
                    fValueRight = m_vtfAbsorbency[nIndex];

                    //////////////////////////////////////////////////////////////////////////
                    //如果开启了抗浊度功能
                    char tmp[64];memset(tmp,0,64);
                    GetAfxConfig()->GetIni("Spectrum","Turbidity",tmp);
                    if (atoi(tmp) == 1)
                    {
                        double fTbAbsorb = 0.0;
                        if(GetTurbidity(RightBasePoint,fTbAbsorb))
                        {
                            if (fValueRight + 0.05 < fTbAbsorb)
                            {
                                m_proc->RecordLog(LOG_ALARM,"浊度吸光度大于目标吸光度,异常");
                            }
                            else
                            {
                                fValueRight -= fTbAbsorb;
                            }
                        }
                    }
                    //////////////////////////////////////////////////////////////////////////
                    break;
                }
            }
        }
        fAbsorb = fValueLeft - 2*fValueRight;
    }else if(nCalcMode == 1 || nCalcMode == 2)//1是计算峰高，2是计算峰面积
    {
        if(m_vtfAbsorbency.size() > 6)
        {
            double minderidata1 = 10000.0; //寻峰范围内左边
            double minderidata2 = 10000.0; //寻峰范围内右边
            double minX = m_vtfWaveLen[0];//波长最小值
            double maxX = m_vtfWaveLen[m_vtfWaveLen.size()-1];//波长最大值
            int nLowrange = 0;
            int nHighrange = 0;
            int LowWaveLen = LeftBasePoint;
            int HighWaveLen = RightBasePoint;
            double max = -2.0;
            int min1 = 0;
            int min2 = 0;
            int middle = 0;

            if(LowWaveLen <= (int)minX+30)//去掉头30个点
                LowWaveLen = (int)minX+30;
            if(LowWaveLen >= (int)maxX)
                LowWaveLen = (int)minX+30;
            if(HighWaveLen >= (int)maxX)
                HighWaveLen = (int)maxX;
            if(HighWaveLen <= (int)minX)
                HighWaveLen = (int)maxX;

            nLowrange = GetFindWaveLenValue(LowWaveLen);
            nHighrange = GetFindWaveLenValue(HighWaveLen);
            if(nLowrange>=0 && nHighrange>=0)
            {
                //找出最大值所在的点
                //先保证往上爬坡

                for(int n = nLowrange + 3; n <= nHighrange - 3;n++)
                {
                    if(max < m_vtfAbsorbency[n-1])
                    {
                        max = m_vtfAbsorbency[n-1];
                        middle = n - 1;
                    }

                }
                //如果中间值没有边界值重合
                if(nLowrange < middle - 1 && nHighrange > middle)
                {
                    minderidata1 = 1000.0;
                    minderidata2 = 1000.0;
                    //找出左边最小值对应的点
                    for(int n = nLowrange; n < middle - 1; n++)
                    {
                        if(minderidata1 > m_vtfAbsorbency[n])
                        {
                            minderidata1 = m_vtfAbsorbency[n];
                            min1 = n;
                        }
                    }
                    /*		找出右边最小值对应的点*/
                    for(int n = middle;n <= nHighrange; n++)
                    {
                        if(minderidata2 > m_vtfAbsorbency[n])
                        {
                            minderidata2 = m_vtfAbsorbency[n];
                            min2 = n;
                        }
                    }
                    double temp = 0.0;
                    if (nCalcMode == 1)//计算峰高
                    {
                        temp=(m_vtfAbsorbency[min2] - m_vtfAbsorbency[min1]) * (m_vtfWaveLen[middle] - m_vtfWaveLen[min1]) / (m_vtfWaveLen[min2] - m_vtfWaveLen[min1]) + m_vtfAbsorbency[min1];
                        fAbsorb = m_vtfAbsorbency[middle] - temp;//样品峰高
                    }
                    else
                    {
                        for(int j = min1;j <= min2; j++)//计算峰面积
                        {
                            temp += (m_vtfAbsorbency[j]-((m_vtfAbsorbency[min2] - m_vtfAbsorbency[min1]) * (m_vtfWaveLen[j] - m_vtfWaveLen[min1]) / (m_vtfWaveLen[min2] - m_vtfWaveLen[min1]) + m_vtfAbsorbency[min1])) * (m_vtfWaveLen[j] - m_vtfWaveLen[j-1]);
                        }
                        if(temp<0)
                            temp=0;
                        fAbsorb=temp;
                    }
                }
                else
                {
                    fAbsorb = 0;
                }
            }
        }
    }
    return true;
}
bool spectrumDevice::GetTurbidity(int nDescWaveLen,double& dfAbsorb)
{
    dfAbsorb = 0.0;
    double fAbs[3];
    double fWaveLen[3];
    char tmp[64];memset(tmp,0,64);
    GetAfxConfig()->GetIni("Spectrum","Turb1",tmp);
    fWaveLen[0] = atof(tmp);
    fAbs[0] = 0.0;
    memset(tmp,0,64);
    GetAfxConfig()->GetIni("Spectrum","Turb2",tmp);
    fWaveLen[1] = atof(tmp);
    fAbs[1] = 0.0;
    memset(tmp,0,64);
    GetAfxConfig()->GetIni("Spectrum","Turb3",tmp);
    fWaveLen[2] = atof(tmp);
    fAbs[2] = 0.0;

    int nIndex = 0;
    int nNumber = 0;
    for (vector<double>::iterator iter = m_vtfWaveLen.begin();iter != m_vtfWaveLen.end();iter++,nIndex++)
    {
        if (nNumber == 0)//找第一点的波长
        {
            if(int(*iter) == fWaveLen[0])
            {
                fAbs[0] = m_vtfAbsorbency[nIndex];
                nNumber = 1;
                if (fAbs[0] < 0.05/*theApp.m_Config->GetTurbidityFisrtPoint()*/)//第一点必须大于设定值才考虑扣除浊度干扰
                {
                    m_proc->RecordLog(LOG_ALARM,"抗浊度第一个点吸光度必须大于设定值,本次抗干扰取消");
                    break;
                }
            }
        }else if (nNumber == 1)
        {
            if(int(*iter) == fWaveLen[1])
            {
                fAbs[1] = m_vtfAbsorbency[nIndex];
                nNumber = 2;
            }
        }else if (fWaveLen[2] > 0 && nNumber == 2)
        {
            if(int(*iter) == fWaveLen[2])
            {
                fAbs[2] = m_vtfAbsorbency[nIndex];
                nNumber = 3;
                break;
            }
        }
    }
    bool bTag = true;
    double dfK = 0,dfB = 0,dfR = 0;
    if(nNumber >= 2)//至少要两点
    {
        double KSum =0,BSum = 0,KBSum = 0,KSquareSum = 0,BSquareSum = 0;
        for(int i = 0 ; i < nNumber ; i++)
        {
            KSum += fAbs[i];			//吸光度和
            BSum += fWaveLen[i];			//波长和
            KBSum += fAbs[i] * fWaveLen[i];		//波长乘浓度的和
            KSquareSum += fAbs[i] * fAbs[i];	//波长的平方和
            BSquareSum += fWaveLen[i] * fWaveLen[i];	//波长的平方和
        }
        double ftmp = nNumber * KSquareSum - KSum * KSum;
        if(ftmp != 0)
        {
            dfK = (nNumber * KBSum - KSum * BSum) / ftmp;
            dfB = (BSum - dfK * KSum) / nNumber;
        }
        else
        {
            m_proc->RecordLog(LOG_ALARM,"拟合抗浊度曲线时除数为0,本次抗干扰取消,请检查三点配置值");
            bTag =  false;
        }
        ftmp = (nNumber * KSquareSum - KSum * KSum) * (nNumber * BSquareSum - BSum * BSum);
        if(ftmp > 0)
        {
            dfR = (nNumber * KBSum - KSum * BSum) / sqrt(ftmp);
        }
        else
        {
             m_proc->RecordLog(LOG_ALARM,"拟合抗浊度曲线时,取开方小于0,本次抗干扰取消,请检查三点配置值");
            bTag =  false;
        }
    }
    else
    {
        //theApp.DBConn->AddYQLog("SYS","只找到一点，取消曲线拟合！",ALARM);
        bTag = false;
    }
    if (bTag)
    {
        if (dfK > 0)
        {
            bTag = false;
            //浊度补偿异常
            m_proc->RecordLog(LOG_ALARM,"抗浊度曲线K值大于0,本次抗干扰取消,请检查三点配置值");
        }else if(fabs(dfR) < 0.9)
        {
            //浊度补偿不满足要求，浊度曲线<0.99
            m_proc->RecordLog(LOG_ALARM,"抗浊度曲线浊度补偿不满足要求,浊度曲线<0.9,本次抗干扰取消,请检查三点配置值");
            bTag = false;
        }else
        {
            dfAbsorb = (nDescWaveLen-dfB)/dfK;

            char stmp[255];
            memset(stmp,0,255);
            sprintf(stmp,"本次抗浊度吸光度A=[x(%d)-b(%.4f)]/K(%.4f)=%.4f相关系数R=%.4f",nDescWaveLen,dfB,dfK,dfAbsorb,dfR);
            m_proc->RecordLog(LOG_NOMAL,stmp);
        }
    }
    return bTag;
}
int spectrumDevice::GetFindWaveLenValue(int nValue)
{
    int nIndex =0;
    for (vector<double>::iterator iter = m_vtfWaveLen.begin();iter != m_vtfWaveLen.end(); iter++)
    {
        if ((int)(*iter - nValue) == 0)
            break;
        else
            nIndex++;
    }
    return nIndex;
}
int spectrumDevice::CalcAbsorb(bool bMode,float fBaseWaveLength)
{
    int dataCount = m_vtnTestSignal.size();
    if(dataCount <= 0)
        return -1;

    int nRet = 0;
    double dfA,dfB;
    if(bMode)//新的算法
    {
        int nTheIndex = GetFindWaveLenValue(fBaseWaveLength);
        for (int i = 0;i < dataCount; i++)
        {
            if(m_vtnTestSignal[i] == m_vtnDarkSignal[i])
            {
                m_vtfAbsorbency[i] = 0;
            }
            else
            {
                dfA = (double)(m_vtnTestSignal[nTheIndex] - m_vtnDarkSignal[i]);
                dfB = (double)(m_vtnTestSignal[i] - m_vtnDarkSignal[i]);
                if (dfA < 0 || dfB < 0)
                {
                    m_vtfAbsorbency[i]  = 0.0;
                    if (!(i < 50 || i > dataCount - 50))//前后50点忽略
                        nRet = 1;
                }
                else
                {
                    m_vtfAbsorbency[i] = log10(dfA / dfB);
                }
            }
        }
    }
    else//老的算法
    {
        for (int i = 0;i < dataCount; i++)
        {
            if(m_vtnRefSignal.at(i) == m_vtnDarkSignal.at(i) || m_vtnTestSignal.at(i) == m_vtnDarkSignal.at(i))
            {
                m_vtfAbsorbency[i] = 0;
            }
            else
            {
                dfA = (double)(m_vtnRefSignal.at(i) - m_vtnDarkSignal.at(i));
                dfB = (double)(m_vtnTestSignal.at(i) - m_vtnDarkSignal.at(i));
                if (dfA < 0 || dfB < 0)
                {
                    m_vtfAbsorbency[i]  = 0.0;
                    if (!(i < 50 || i > dataCount - 50))//前后50点忽略
                        nRet = 1;

                }
                else
                {
                    m_vtfAbsorbency[i] = log10(dfA / dfB);
                }
            }
        }
    }
    DataPH(Absorb);
    return nRet;
}
