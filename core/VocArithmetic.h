#ifndef VOCARITHMETIC_H
#define VOCARITHMETIC_H

#include <vector>
#include <math.h>
#include "vardef.h"
using namespace std;

class CVocArithmetic
{
public:
    CVocArithmetic();
    virtual ~CVocArithmetic();

public:
    void SetData(vector<double> vtnSignal,vector<double> vtnRT);
    void SetData(QVector<double> vtnSignal,QVector<double> vtnRT);

    //设置积分参数
    bool SetSeachParam(float BaseLineK,float fModifyB,float MinPW,float MaxPW,float Minimum,float MinArea);
    //开始积分
    bool Calculate();

    vector<struPeak> m_vtnPeark;//峰结果
private:
    //最小二乘移动平滑方法,俗称五点三次
    bool Smooth(int num);
    //求导数
    bool Daoshu();

private:

    vector<double> m_vtnSignal;//源始信号
    vector<double> m_vtnRT;//保留时间
    vector<double> m_vtnDS_1;//一阶导数
    vector<double> m_vtnDS_2;//二阶导数
    vector<double> m_vtnDS_3;//三阶导数


    float m_fBaseLineK;//漂移，即曲线的切线斜率
    float m_fMinPW;//最小窗宽
    float m_fMaxPW;//最大窗宽
    float m_fMinimum;//最小信号值
    float m_fMinArea;//最小面积
    float m_fModifyB;//用于校正基线的K值的阀值
};

#endif // ARITHMETIC_H
