#include "VocArithmetic.h"
#include <iostream>
CVocArithmetic::CVocArithmetic()
{
    m_vtnPeark.clear();
    m_vtnSignal.clear();
    m_vtnRT.clear();
    m_vtnDS_1.clear();
    m_vtnDS_2.clear();
    m_vtnDS_3.clear();

    m_fBaseLineK = 0;
    m_fMinPW = 0;
    m_fMaxPW = 60;
    m_fMinimum = 600;
    m_fMinArea = 0;
    m_fModifyB = 4000;
}
CVocArithmetic::~CVocArithmetic()
{

}

bool CVocArithmetic::SetSeachParam(float BaseLineK, float fModifyB, float MinPW, float MaxPW, float Minimum, float MinArea)
{
    m_fBaseLineK = BaseLineK;
    m_fModifyB = fModifyB;
    m_fMinPW = MinPW;
    m_fMaxPW = MaxPW;
    m_fMinimum = Minimum;
    m_fMinArea = MinArea;
}

bool CVocArithmetic::Daoshu()
{
    double fValue=0.0;
    int nNumber = (int)m_vtnSignal.size();

    if(nNumber > 6)
    {
        //一阶导数
        m_vtnDS_1.clear();
        m_vtnDS_1.push_back(0);
        m_vtnDS_1.push_back(0);
        m_vtnDS_1.push_back(0);
        for(int j=3;j<nNumber-3;j++)
        {
            fValue = (double)((22*m_vtnSignal[j-3]-67.0*m_vtnSignal[j-2]-58.0*m_vtnSignal[j-1]+58*m_vtnSignal[j+1]+67*m_vtnSignal[j+2]-22*m_vtnSignal[j+3])/252.0);
            m_vtnDS_1.push_back(fValue);
        }
        m_vtnDS_1.push_back(0);
        m_vtnDS_1.push_back(0);
        m_vtnDS_1.push_back(0);

        /*
        //二阶导数
        m_vtnDS_2.clear();
        m_vtnDS_2.push_back(0);
        m_vtnDS_2.push_back(0);
        m_vtnDS_2.push_back(0);
        for(int j=3;j<nNumber-3;j++)
        {
            fValue = (double)((22*m_vtnDS_1[j-3]-67.0*m_vtnDS_1[j-2]-58.0*m_vtnDS_1[j-1]+58*m_vtnDS_1[j+1]+67*m_vtnDS_1[j+2]-22*m_vtnDS_1[j+3])/252.0);
            m_vtnDS_2.push_back(fValue);
        }
        m_vtnDS_2.push_back(0);
        m_vtnDS_2.push_back(0);
        m_vtnDS_2.push_back(0);

        //三阶导数
        m_vtnDS_3.clear();
        m_vtnDS_3.push_back(0);
        m_vtnDS_3.push_back(0);
        m_vtnDS_3.push_back(0);
        for(int j=3;j<nNumber-3;j++)
        {
            fValue = (double)((22*m_vtnDS_2[j-3]-67.0*m_vtnDS_2[j-2]-58.0*m_vtnDS_2[j-1]+58*m_vtnDS_2[j+1]+67*m_vtnDS_2[j+2]-22*m_vtnDS_2[j+3])/252.0);
            m_vtnDS_2.push_back(fValue);
        }
        m_vtnDS_3.push_back(0);
        m_vtnDS_3.push_back(0);
        m_vtnDS_3.push_back(0);
        */
        return  true;
    }
    return false;
}
void CVocArithmetic::SetData(vector<double> vtnSignal, vector<double> vtnRT)
{
    m_vtnPeark.clear();
    m_vtnSignal.clear();
    m_vtnRT.clear();
    m_vtnDS_1.clear();
    m_vtnDS_2.clear();
    m_vtnDS_3.clear();

    m_vtnSignal = vtnSignal;
    m_vtnRT = vtnRT;
}
void CVocArithmetic::SetData(QVector<double> vtnSignal, QVector<double> vtnRT)
{
    m_vtnPeark.clear();
    m_vtnSignal.clear();
    m_vtnRT.clear();
    m_vtnDS_1.clear();
    m_vtnDS_2.clear();
    m_vtnDS_3.clear();

    for(int i = 0;i < vtnSignal.size();i++)
    {
        m_vtnSignal.push_back(vtnSignal.at(i));
        m_vtnRT.push_back(vtnRT.at(i));
    }
}
bool CVocArithmetic::Smooth(int num)
{
    int nNumber = (int)m_vtnSignal.size();
    if(nNumber > 6 && num > 0)
    {
        vector<double> vTemp;
        double fValue = 0;
        for(int i = 0;i < num;i++)
        {
            vTemp.clear();
            fValue = (double)((69*m_vtnSignal[0]+4*(m_vtnSignal[1]+m_vtnSignal[3])-6*m_vtnSignal[2]-m_vtnSignal[4])/70.0);
            vTemp.push_back(fValue);
            fValue = (double)((2*(m_vtnSignal[0]+m_vtnSignal[4])+27*m_vtnSignal[1]+12*m_vtnSignal[2]-8*m_vtnSignal[3])/35.0);
            vTemp.push_back(fValue);
            for(int i = 2;i < nNumber - 2;i++)
            {
                fValue = (double)((-3*m_vtnSignal[i-2]+12*m_vtnSignal[i-1]+17*m_vtnSignal[i]+12*m_vtnSignal[i+1]-3*m_vtnSignal[i+2])/35);
                vTemp.push_back(fValue);
            }
            fValue = (double)((2*(m_vtnSignal[nNumber-1]+m_vtnSignal[nNumber-5])+27*m_vtnSignal[nNumber - 2]+12*m_vtnSignal[nNumber - 3]-8*m_vtnSignal[nNumber-4])/35.0);
            vTemp.push_back(fValue);
            fValue = (double)((69*m_vtnSignal[nNumber-1]+4*(m_vtnSignal[nNumber-2]+m_vtnSignal[nNumber-4])-6*m_vtnSignal[nNumber-3]-m_vtnSignal[nNumber-5])/70.0);
            vTemp.push_back(fValue);

            m_vtnSignal.clear();
            m_vtnSignal = vTemp;
        }
        return true;
    }
    return false;
}

bool CVocArithmetic::Calculate()
{
    bool bRet =  Smooth(1) && Daoshu();
    if(!bRet)
        return false;
    float fDefaultBaseK = 0.0;// 建立默认基线<<色谱定性与定量>>P239
    float fDefaultBaseb =0.0;
    m_vtnPeark.clear();
    int nNumber = (int)m_vtnSignal.size();
    int nIndex = 0;
    while(nIndex < nNumber - 2)
    {
        //寻找斜率小于设定值
        if(m_vtnDS_1[nIndex] > m_fBaseLineK && m_vtnDS_1[nIndex+1] > m_fBaseLineK)//连续2个点
        {
            if(fDefaultBaseK <= 0.000001)//建立默认基线,基于开始时信息水平和结束时信号水平建立基线
            {
                fDefaultBaseK = (m_vtnSignal[nNumber-2]-m_vtnSignal[nIndex/2])/(m_vtnRT[nNumber-2]-m_vtnRT[nIndex/2]);
                fDefaultBaseb = -fDefaultBaseK*m_vtnRT[nIndex/2]+m_vtnSignal[nIndex/2];
            }

            //<<找到一个峰的左边基线,视为基线,修正基线建立<<色谱定性与定量>>P240
            bool bLeft = false;
            bool bRight = false;
            struPeak peak;
            peak.bCalcTag = false;
            peak.fLeftBaseLine = m_vtnSignal[nIndex];
            peak.nLeftPos = nIndex;
            peak.fLeftT = m_vtnRT[nIndex];

            //开始找拐点,即一阶导数为0的点
            while(nIndex < nNumber - 2)
            {
                if(m_vtnDS_1[nIndex] > 0)//一阶导数>0,曲线上升
                {
                    nIndex++;
                }
                else if(m_vtnDS_1[nIndex] <= 0 && m_vtnDS_1[nIndex+1] < 0 && m_vtnDS_1[nIndex+2] < 0)//连续3个点一阶导数<0,曲线下降,找到拐点
                {
                    peak.nMidPos = nIndex - 1;//峰高点，导数为正的点
                    peak.fRT = m_vtnRT[nIndex - 1];//保存峰高保留时间
                    bLeft = true;
                    break;//找到拐点，结束
                }
                else//可能有1个点异常，忽略
                {
                    nIndex++;
                }
            }
            //找峰的右边基线,视为修正基线>>
            while(nIndex < nNumber - 2)
            {
                if(m_vtnDS_1[nIndex] < 0)//一阶导数<0,曲线下降
                {
                    nIndex++;
                }
                else if(m_vtnDS_1[nIndex] >= 0 && m_vtnDS_1[nIndex+1] > 0 && m_vtnDS_1[nIndex+2] > 0)//连续3个点一阶导数>0,曲线上升,找到基线或拐点（可能有BV,或VV峰）
                {
                    peak.nRightPos = nIndex;//导数为正的点
                    peak.fRigtBaseLine = m_vtnSignal[nIndex];
                    peak.fRightT = m_vtnRT[nIndex];
                    bRight = true;
                    break;//找到基线或拐点，结束
                }
                else//可能有1个点异常，忽略
                {
                    nIndex++;
                }
            }
            float fK = 0.0;
            float fb =0.0;
            if(bLeft && bRight)//找到一个峰,计算峰面积和峰高等
            {
                //double temp = (fabs(m_vtnSignal[peak.nRightPos]) - fabs(m_vtnSignal[peak.nLeftPos])) * (m_vtnRT[peak.nMidPos]-m_vtnRT[peak.nLeftPos]) / (m_vtnRT[peak.nRightPos] - m_vtnRT[peak.nLeftPos]) + fabs(m_vtnSignal[peak.nLeftPos]);
                //计算峰高,暂时先用此计算方法，方法同电化学
                /*原来的计算方式
                double a = fabs(fabs(m_vtnSignal[peak.nRightPos]) - fabs(m_vtnSignal[peak.nLeftPos]));
                double b =  (m_vtnRT[peak.nMidPos]-m_vtnRT[peak.nLeftPos]);
                double c = (m_vtnRT[peak.nRightPos] - m_vtnRT[peak.nLeftPos]);
                double d = a*b/c;
                double e = m_vtnSignal[peak.nMidPos];
                double temp = fabs(m_vtnSignal[peak.nMidPos]) - d - fabs(m_vtnSignal[peak.nLeftPos]);
                */
                double temp = 0.0;
                float fY = 0.0;

                if(m_vtnRT[peak.nLeftPos] == m_vtnRT[peak.nRightPos])
                {
                    fK = 0;
                    fb = m_vtnRT[peak.nLeftPos];
                }
                else
                {
                    fK = (m_vtnSignal[peak.nRightPos]-m_vtnSignal[peak.nLeftPos])/(m_vtnRT[peak.nRightPos]-m_vtnRT[peak.nLeftPos]);
                    fb = -fK*m_vtnRT[peak.nLeftPos]+m_vtnSignal[peak.nLeftPos];
                }

                //检测是否有重叠峰,如果有重叠峰,则使用默认基线计算峰高
                if(fabs(fK) > m_fModifyB)
                {
                    fK = fDefaultBaseK;
                    fb = fDefaultBaseb;
                }

                fY = fK*m_vtnRT[peak.nMidPos] + fb;
                temp = m_vtnSignal[peak.nMidPos] - fY;

                if (temp < 0)
                    temp= 0;
                peak.fHight = temp;

                //计算峰宽
                double fMidHigh = peak.fHight / 2.0+(peak.fLeftBaseLine+peak.fRigtBaseLine)/2;
                double fLeft = 0;
                double fRight = 0;
                temp = fMidHigh;
                for(int i= peak.nLeftPos;i < peak.nMidPos;i++)
                {
                    if(fabs(fMidHigh - fabs(m_vtnSignal[i])) < temp)
                    {
                        temp  = fabs(fMidHigh - fabs(m_vtnSignal[i]));
                        fLeft = m_vtnRT[i];
                    }
                }
                temp = fMidHigh;
                for(int i= peak.nMidPos;i < peak.nRightPos;i++)
                {
                    if(fabs(fMidHigh - fabs(m_vtnSignal[i])) < temp)
                    {
                        temp = fabs(fMidHigh - fabs(m_vtnSignal[i]));
                        fRight = m_vtnRT[i];
                    }
                }
                peak.fWidth = fRight - fLeft;

                double hi = 0;
                double wi = 0;
                //计算峰面积
                /*原来的计算方式
                for(int j = peak.nLeftPos+1;j < peak.nRightPos;j++)
                {
                    //temp += (m_vtnRT[j] - m_vtnRT[j-1])*(fabs(m_vtnSignal[j]) - (fabs(fabs(m_vtnSignal[peak.nRightPos]) - fabs(m_vtnSignal[peak.nLeftPos])) * (m_vtnRT[j] - m_vtnRT[peak.nLeftPos]) / (m_vtnRT[peak.nRightPos] - m_vtnRT[peak.nLeftPos]) + fabs(m_vtnSignal[peak.nLeftPos])));
                    a = fabs(fabs(m_vtnSignal[peak.nRightPos]) - fabs(m_vtnSignal[peak.nLeftPos]));
                    b = m_vtnRT[j] - m_vtnRT[peak.nLeftPos];
                    c = m_vtnRT[peak.nRightPos] - m_vtnRT[peak.nLeftPos];
                    d = a*b/c;
                    if(m_vtnSignal[peak.nLeftPos] < m_vtnSignal[peak.nRightPos])
                        e = fabs(m_vtnSignal[peak.nLeftPos])+d;
                    else
                        e = fabs(m_vtnSignal[peak.nRightPos])+d;
                    hi = fabs(m_vtnSignal[j]) - e;
                    wi = ((m_vtnRT[j] - m_vtnRT[j-1])*60.0);
                    temp += fabs(hi * wi);
                }*/
                if(m_vtnRT[peak.nLeftPos] == m_vtnRT[peak.nRightPos])
                {
                    temp = 0.0;
                }
                else
                {
                    fK = (m_vtnSignal[peak.nRightPos]-m_vtnSignal[peak.nLeftPos])/(m_vtnRT[peak.nRightPos]-m_vtnRT[peak.nLeftPos]);
                    fb = -fK*m_vtnRT[peak.nLeftPos]+m_vtnSignal[peak.nLeftPos];

                    //检测是否有重叠峰,如果有重叠峰,则使用默认基线计算峰面积
                    if(fabs(fK) > m_fModifyB)
                    {
                        fK = fDefaultBaseK;
                        fb = fDefaultBaseb;
                    }

                    for(int j = peak.nLeftPos+1;j < peak.nRightPos;j++)
                    {
                        fY = fK*m_vtnRT[j] + fb;
                        hi = m_vtnSignal[j] - fY;
                        wi = (m_vtnRT[j] - m_vtnRT[j-1])*60.0;
                        temp += fabs(hi * wi);
                    }
                }
                peak.fArea = temp;
            }
            if(peak.fWidth < m_fMinPW || peak.fWidth > m_fMaxPW || peak.fHight < m_fMinimum || peak.fArea < m_fMinArea)//峰小于最小阀值或面积小于最小面积,小于最小峰宽或大于最大峰宽
            {
                ;
            }
            else
            {
                peak.nId = m_vtnPeark.size()+1;
                m_vtnPeark.push_back(peak);
                cout<<"peak id="<<peak.nId<<",  pos="<<peak.nMidPos<< ",    Rt="<<peak.fRT<<",  LeftT="<<peak.fLeftT<<",    RightT="<<peak.fRightT<<",  hight="<<peak.fHight<<",    Area="<<peak.fArea<</*",  fLeftBaseLine="<<peak.fLeftBaseLine<<",  fRigtBaseLine="<<peak.fRigtBaseLine<<*/",  K="<<fK<<",  B="<<fb<<endl;
            }
        }
        else
        {
            nIndex++;
        }
    }
    if(m_vtnPeark.size() > 0)
        bRet = true;
    else
        bRet = false;
    return bRet;
}

