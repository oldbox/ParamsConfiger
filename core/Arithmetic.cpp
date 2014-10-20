
#include "Arithmetic.h"
CArithmetic::CArithmetic()
{

}
CArithmetic::~CArithmetic()
{

}
bool CArithmetic::DaoshuJs()
{
    double fValue=0.0;
    int nNumber = (int)m_vtnDataY.size();

    if(nNumber > 6)
    {
        vector<double> vTemp;
        vTemp.clear();
        //平滑处理
        fValue = (double)((69*m_vtnDataY[0]+4*(m_vtnDataY[1]+m_vtnDataY[3])-6*m_vtnDataY[2]-m_vtnDataY[4])/70.0);
        vTemp.push_back(fValue);
        fValue = (double)((2*(m_vtnDataY[0]+m_vtnDataY[4])+27*m_vtnDataY[1]+12*m_vtnDataY[2]-8*m_vtnDataY[3])/35.0);
        vTemp.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            fValue = (double)((-3*m_vtnDataY[i-2]+12*m_vtnDataY[i-1]+17*m_vtnDataY[i]+12*m_vtnDataY[i+1]-3*m_vtnDataY[i+2])/35);
            vTemp.push_back(fValue);
        }

        fValue = (double)((2*(m_vtnDataY[nNumber-1]+m_vtnDataY[nNumber-5])+27*m_vtnDataY[nNumber - 2]+12*m_vtnDataY[nNumber - 3]-8*m_vtnDataY[nNumber-4])/35.0);
        vTemp.push_back(fValue);
        fValue = (double)((69*m_vtnDataY[nNumber-1]+4*(m_vtnDataY[nNumber-2]+m_vtnDataY[nNumber-4])-6*m_vtnDataY[nNumber-3]-m_vtnDataY[nNumber-5])/70.0);
        vTemp.push_back(fValue);

        m_vtnDataY.clear();
        //平滑处理
        fValue = (double)((69*vTemp[0]+4*(vTemp[1]+vTemp[3])-6*vTemp[2]-vTemp[4])/70.0);
        m_vtnDataY.push_back(fValue);
        fValue = (double)((2*(vTemp[0]+vTemp[4])+27*vTemp[1]+12*vTemp[2]-8*vTemp[3])/35.0);
        m_vtnDataY.push_back(fValue);

        for(int i = 2;i < nNumber - 2;i++)
        {
            fValue = (double)((-3*vTemp[i-2]+12*vTemp[i-1]+17*vTemp[i]+12*vTemp[i+1]-3*vTemp[i+2])/35);
            m_vtnDataY.push_back(fValue);
        }

        fValue = (double)((2*(vTemp[nNumber-1]+vTemp[nNumber-5])+27*vTemp[nNumber - 2]+12*vTemp[nNumber - 3]-8*vTemp[nNumber-4])/35.0);
        m_vtnDataY.push_back(fValue);
        fValue = (double)((69*vTemp[nNumber-1]+4*(vTemp[nNumber-2]+vTemp[nNumber-4])-6*vTemp[nNumber-3]-vTemp[nNumber-5])/70.0);
        m_vtnDataY.push_back(fValue);

        vTemp.clear();
        m_vtnDS.clear();

        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);

        //导数计算
        for(int j=3;j<nNumber-3;j++)
        {
            fValue = (double)((22*m_vtnDataY[j-3]-67.0*m_vtnDataY[j-2]-58.0*m_vtnDataY[j-1]+58*m_vtnDataY[j+1]+67*m_vtnDataY[j+2]-22*m_vtnDataY[j+3])/252.0);
            m_vtnDS.push_back(fValue);
        }
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);

        return true;
    }
    return false;
}
bool CArithmetic::AFSDaoshuJs()
{
    double fValue=0.0;
    int nNumber = (int)m_vtnDataY.size();
    if(nNumber > 6)
    {
        m_vtnDS.clear();

        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);

        //导数计算
        for(int j=3;j<nNumber-3;j++)
        {
            fValue = (double)((22*m_vtnDataY[j-3]-67.0*m_vtnDataY[j-2]-58.0*m_vtnDataY[j-1]+58*m_vtnDataY[j+1]+67*m_vtnDataY[j+2]-22*m_vtnDataY[j+3])/252.0);
            m_vtnDS.push_back(fValue);
        }
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);
        m_vtnDS.push_back(0);

        return true;
    }
    return false;
}
void CArithmetic::SetData(vector<double> vtnDataY,vector<double> vtnDataX)
{
    m_vtnDataX.clear();
    m_vtnDataY.clear();

    m_vtnDataX = vtnDataX;
    m_vtnDataY = vtnDataY;
    DaoshuJs();
}
void CArithmetic::SetData(QVector<double> vtnDataY,QVector<double> vtnDataX)
{
    m_vtnDataX.clear();
    m_vtnDataY.clear();

    for(int i=0;i<vtnDataY.size();i++)
    {
        m_vtnDataX.push_back(vtnDataX[i]);
        m_vtnDataY.push_back(vtnDataY[i]);
    }
    AFSDaoshuJs();
}
bool CArithmetic::Calculate(int nCalcMode,int nLeftRange,int nRightRange,double& fPeakHeight,int& nmiddle,int& nmin1,int& nmin2)
{
    bool bRet = false;
    if (m_vtnDataX.size() > 6 && m_vtnDataY.size() > 6)
    {
        fPeakHeight = 0.0;
        int nNumber = (int)m_vtnDataY.size();
        if(nNumber > 6)
        {
            nmiddle = 0;
            double fmax = 0.0;

            int nLowrange = IndexOfValue(nLeftRange/1000.0);
            int nHighrange = IndexOfValue(nRightRange/1000.0,nLowrange);
            if(nLowrange >= 0 && nHighrange > 0)
            {
                while(m_vtnDS[nLowrange]<=0)
                {
                    if(nLowrange + 4 > nHighrange)
                        break;
                    nLowrange = nLowrange + 1;
                }

                int nIndex = nLowrange + 3;
                //找最大值
                for(nIndex;nIndex <= nHighrange-3;nIndex++)
                {
                    if(m_vtnDS[nIndex-1] >= 0 && m_vtnDS[nIndex-2] >= 0 && m_vtnDS[nIndex-3] >= 0 && m_vtnDS[nIndex+1] < 0 && m_vtnDS[nIndex+2] < 0 && m_vtnDS[nIndex+3] < 0)
                    {
                        for(int j=0;j<7;j++)
                        {
                            if(fmax < m_vtnDataY[nIndex+j-3])
                            {
                                fmax = m_vtnDataY[nIndex+j-3];
                                nmiddle = nIndex+j-3;
                            }
                        }
                    }
                }
                //容错判断
                if (nIndex == nHighrange - 2 && nmiddle == 0)
                {
                    for(int n = nLowrange;n <= nHighrange - 1;n++)
                    {
                        if(m_vtnDS[n] * m_vtnDS[n+1] < 0)
                        {
                            for(int j=0;j<2;j++)
                            {
                                if(fmax < m_vtnDataY[n+j])
                                {
                                    fmax = m_vtnDataY[n+j];
                                    nmiddle = n+j;
                                }
                            }
                        }
                    }
                }

                if(nLowrange < nmiddle - 1 && nHighrange > nmiddle)
                {
                    nmin1 = 0;
                    nmin2 = 0;
                    double minderidata1 = 1000.0;
                    double minderidata2 = 1000.0;
                    //左基线
                    for(int n = nLowrange;n < nmiddle-1;n++)
                    {
                        if(minderidata1 > m_vtnDataY[n] /*&& m_vtnDataY[n] > 0*/)
                        {
                            minderidata1 = m_vtnDataY[n];
                            nmin1 = n;
                        }
                    }
                    //右基线
                    for(int n = nmiddle;n <= nHighrange;n++)
                    {
                        if(minderidata2 > m_vtnDataY[n] /*&& m_vtnDataY[n].Y > 0*/)
                        {
                            minderidata2 = m_vtnDataY[n];
                            nmin2 = n;
                        }
                    }

                    if(nCalcMode == 1)//计算峰高
                    {
                        float nK = 0;
                        float nb =0;
                        float nY = 0.0;

                        if(m_vtnDataX[nmin1] == m_vtnDataX[nmin2])
                        {
                            nK = 0;nb = m_vtnDataX[nmin1];
                        }
                        else
                        {
                            nK = (m_vtnDataY[nmin2]-m_vtnDataY[nmin1])/(m_vtnDataX[nmin2]-m_vtnDataX[nmin1]);
                            nb = -nK*m_vtnDataX[nmin1]+m_vtnDataY[nmin1];
                        }
                        nY = nK*m_vtnDataX[nmiddle] + nb;

                        fPeakHeight = m_vtnDataY[nmiddle] - nY;
                    }
                    else//计算峰面积
                    {
                        double Area=0.0;
                        float nK = 0;
                        float nb =0;
                        float nY = 0.0;
                        float fHeight = 0.0;
                        float fWith = 0.0;

                        if(m_vtnDataX[nmin1] == m_vtnDataX[nmin2])
                        {
                            Area = 0.0;
                        }
                        else
                        {
                            nK = (m_vtnDataY[nmin2]-m_vtnDataY[nmin1])/(m_vtnDataX[nmin2]-m_vtnDataX[nmin1]);
                            nb = -nK*m_vtnDataX[nmin1]+m_vtnDataY[nmin1];

                            for(int j = nmin1+1;j < nmin2;j++)
                            {
                                nY = nK*m_vtnDataX[j] + nb;
                                fHeight = m_vtnDataY[j] - nY;
                                fWith = m_vtnDataX[j] - m_vtnDataX[j-1];
                                Area += fHeight*fWith;
                            }
                        }
                        fPeakHeight = Area;
                    }
                    bRet = true;
                }
            }
        }
    }
    return bRet;
}
//num表示平滑次数
bool CArithmetic::Smooth(int num,QVector<double> vtnDataY)
{
    m_vtnSmoothY.clear();
    QVector<double> vtnSignal;
    for(int i=0;i<vtnDataY.size();i++)
    {
        vtnSignal.push_back(vtnDataY[i]);
    }
    int nNumber = (int)vtnSignal.size();
    if(nNumber > 6 && num > 0)
    {
        QVector<double> vTemp;
        double fValue = 0;
        for(int i = 0;i < num;i++)
        {
            vTemp.clear();
            fValue = (double)((69*vtnSignal[0]+4*(vtnSignal[1]+vtnSignal[3])-6*vtnSignal[2]-vtnSignal[4])/70.0);
            vTemp.push_back(fValue);
            fValue = (double)((2*(vtnSignal[0]+vtnSignal[4])+27*vtnSignal[1]+12*vtnSignal[2]-8*vtnSignal[3])/35.0);
            vTemp.push_back(fValue);
            for(int i = 2;i < nNumber - 2;i++)
            {
                fValue = (double)((-3*vtnSignal[i-2]+12*vtnSignal[i-1]+17*vtnSignal[i]+12*vtnSignal[i+1]-3*vtnSignal[i+2])/35);
                vTemp.push_back(fValue);
            }
            fValue = (double)((2*(vtnSignal[nNumber-1]+vtnSignal[nNumber-5])+27*vtnSignal[nNumber - 2]+12*vtnSignal[nNumber - 3]-8*vtnSignal[nNumber-4])/35.0);
            vTemp.push_back(fValue);
            fValue = (double)((69*vtnSignal[nNumber-1]+4*(vtnSignal[nNumber-2]+vtnSignal[nNumber-4])-6*vtnSignal[nNumber-3]-vtnSignal[nNumber-5])/70.0);
            vTemp.push_back(fValue);

            vtnSignal.clear();
            vtnSignal = vTemp;

        }
        m_vtnSmoothY = vTemp;
        return true;
    }
    return false;
}
int CArithmetic::IndexOfValue(double fValue,int nOffset)
{
    double fMin = fabs(m_vtnDataX[nOffset]) - fabs(fValue);
    int nindex = nOffset;
    for(int i = nOffset;i < (int)m_vtnDataX.size();i++)
    {
        if(fabs(fabs(m_vtnDataX[i])-fabs(fValue)) < fabs(fMin))
        {
            fMin = fabs(fabs(m_vtnDataX[i]) - fabs(fValue));
            nindex=i;
        }
    }
    return nindex;
}
bool CArithmetic::AFSCalculate(int nCalcMode,int nLeftRange,int nRightRange,float& fPeakHeight,int& nmiddle,int& nmin1,int& nmin2)
{
    bool bRet = false;
    vector<double> vtnValue;
    vtnValue.clear();
    char cValue[64];
    float fLeftDistance = 0.0;
    float fRightDistance = 0.0;

    memset(cValue,0,64);
    if(GetAfxConfig()->GetIni("Z06_SET","LeftDistance",cValue))
    {
        fLeftDistance = atof(cValue);
    }

    memset(cValue,0,64);
    if(GetAfxConfig()->GetIni("Z06_SET","RightDistance",cValue))
    {
        fRightDistance = atof(cValue);
    }


    if (m_vtnDataX.size() > 6 && m_vtnDataY.size() > 6)
    {
        fPeakHeight = 0.0;
        int nNumber = (int)m_vtnDataY.size();
        if(nNumber > 6)
        {
            nmiddle = 0;
            //找到接近左右寻峰点的索引
            int nLowrange = IndexOfValue(nLeftRange);
            int nHighrange = IndexOfValue(nRightRange,nLowrange);

            if(nLowrange >= 0 && nHighrange > 0)
            {
                //保证是曲线开点寻峰点上坡的，否则一直往后推移
                while(m_vtnDS[nLowrange]<=0)
                {
                    //当小于等于四个点，就返回失败
                    if(nLowrange + 4 > nHighrange)
                    {
                        return bRet;
                        break;
                    }
                    else
                    {
                        nLowrange = nLowrange + 1;
                    }
                }

                //排序计算出峰值
                SortCalcValue(nLowrange,nHighrange,1,nmiddle);

                //从峰高开始，往左fLeftDistance秒开始找基点
                float LeftBase = m_vtnDataX[nmiddle]-fLeftDistance;
                float fCompare = LeftBase - m_vtnDataX[nLowrange];
                int nValue = 0;
                //找到最接点LeftBase的点
                for(int nIndex1 = nLowrange;nIndex1 < nmiddle;nIndex1++)
                {
                    if(fCompare>=fabs(LeftBase - m_vtnDataX[nIndex1]))
                    {
                        fCompare = fabs(LeftBase - m_vtnDataX[nIndex1]);
                        nValue = nIndex1;
                    }
                }
                //排序计算出左基点
                SortCalcValue(nValue,nValue+100,2,nmin1);

                //从峰高开始，往右fRightDistance秒开始找基点
                float RightBase = m_vtnDataX[nmiddle] + fRightDistance;
                fCompare = RightBase - m_vtnDataX[nmiddle];
                //找到与RightBase值最接近的点
                for(int nIndex2 = nmiddle;nIndex2 < nHighrange;nIndex2++)
                {
                    if(fCompare>=fabs(RightBase - m_vtnDataX[nIndex2]))
                    {
                        fCompare = fabs(RightBase - m_vtnDataX[nIndex2]);
                        nValue = nIndex2;
                    }
                }
                //排序计算出右基点
                SortCalcValue(nValue-100,nValue,3,nmin2);
                qDebug("nmin2 %d",nmin2);

                if(nCalcMode == 1)//计算峰高
                {
                    float nK = 0;
                    float nb =0;
                    float nY = 0.0;

                    if(m_vtnDataX[nmin1] == m_vtnDataX[nmin2])
                    {
                        nK = 0;nb = m_vtnDataX[nmin1];
                    }
                    else
                    {
                        nK = (m_vtnDataY[nmin2]-m_vtnDataY[nmin1])/(m_vtnDataX[nmin2]-m_vtnDataX[nmin1]);
                        nb = -nK*m_vtnDataX[nmin1]+m_vtnDataY[nmin1];
                    }
                    nY = nK*m_vtnDataX[nmiddle] + nb;

                    fPeakHeight = m_vtnDataY[nmiddle] - nY;
                }
                else//计算峰面积
                {
                    double Area=0.0;
                    float nK = 0;
                    float nb =0;
                    float nY = 0.0;
                    float fHeight = 0.0;
                    float fWith = 0.0;

                    if(m_vtnDataX[nmin1] == m_vtnDataX[nmin2])
                    {
                        Area = 0.0;
                    }
                    else
                    {
                        nK = (m_vtnDataY[nmin2]-m_vtnDataY[nmin1])/(m_vtnDataX[nmin2]-m_vtnDataX[nmin1]);
                        nb = -nK*m_vtnDataX[nmin1]+m_vtnDataY[nmin1];

                        for(int j = nmin1+1;j < nmin2;j++)
                        {
                            nY = nK*m_vtnDataX[j] + nb;                          
                            fHeight = m_vtnDataY[j] - nY;
                            if(fHeight < 0.0)
                            {
                               fHeight = 0.0;
                            }
                            fWith = m_vtnDataX[j] - m_vtnDataX[j-1];
                            Area += fHeight*fWith;
                        }
                    }
                    fPeakHeight = Area;
                }
                bRet = true;
            }
        }
    }
    return bRet;
}
//nType 1表示求峰高值 2表示求左基点 3表示求右基点
void CArithmetic::SortCalcValue(int nStart,int nEnd,int nType,int& nValue)
{
    double fTemp = 0.0;
    double fAverage = 0.0;
    double fCompare = 0.0;
    double fSum = 0.0;
    int nSize1 = 0;
    int nSize2 = 0;
    vector<double> vtnValue;
    vtnValue.clear();

    int nIndex = nStart;
    //将寻峰范围内的数值放到一个向量中
    for(nIndex;nIndex<nEnd;nIndex++)
    {
        vtnValue.push_back(m_vtnDataY[nIndex]);
    }

    if(1 == nType)
    {
        //冒泡法从大到小排序
        nSize1 = vtnValue.size()-1;
        for(int i=0;i<nSize1;i++)
        {
            nSize2 = vtnValue.size()-i-1;
            for(int j=0;j<nSize2;j++)
            {
                if(vtnValue[j+1]>vtnValue[j])
                {
                    fTemp = vtnValue[j];
                    vtnValue[j] = vtnValue[j+1];
                    vtnValue[j+1] = fTemp;

                }
            }
        }
        //五个最大值，找中间的值为峰高值
        if(nSize1 > 2)
        {
            fAverage = vtnValue[2];
        }
        nIndex = nStart;
        fCompare=fabs(fAverage - m_vtnDataY[nIndex]);
        //找到与平均值最接近的点
        for(nIndex;nIndex < nEnd;nIndex++)
        {
            if(fCompare>=fabs(fAverage - m_vtnDataY[nIndex]))
            {
                fCompare = fabs(fAverage - m_vtnDataY[nIndex]);
                nValue = nIndex;
            }

        }
    }
    else
    {
        //冒泡法从小到大排序
        nSize1 = vtnValue.size()-1;
        for(int i=0;i<nSize1;i++)
        {
            nSize2 = vtnValue.size()-i-1;
            for(int j=0;j<nSize2;j++)
            {
                if(vtnValue[j]>vtnValue[j+1])
                {
                    fTemp = vtnValue[j];
                    vtnValue[j] = vtnValue[j+1];
                    vtnValue[j+1] = fTemp;

                }
            }
        }

        //求最小值和，再平均
        if(nSize1 > 100)
        {
            for(int i=0;i<100;i++)
            {
                fSum += vtnValue[i];

            }
        }
        fAverage = vtnValue[50];//100值找到中间值为基线

        nIndex = nStart;
        fCompare=fabs(fAverage - m_vtnDataY[nIndex]);
        qDebug("fCompare: %f",fCompare);
        //找到与中间值最接近的点，并求出对应的索引
        for(nIndex;nIndex < nEnd;nIndex++)
        {
            if(fCompare>=fabs(fAverage - m_vtnDataY[nIndex]))
            {
                fCompare = fabs(fAverage - m_vtnDataY[nIndex]);
                nValue = nIndex;
            }
        }
    }

}
