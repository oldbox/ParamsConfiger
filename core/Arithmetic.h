#ifndef ARITHMETIC_H
#define ARITHMETIC_H 

#include <vector>
#include <math.h>
#include "vardef.h"
#include "./cconfig.h"

using namespace std;

class CArithmetic
{
public:
    CArithmetic();
    virtual ~CArithmetic();


public:
    void SetData(vector<double> vtnDataY,vector<double> vtnDataX);
    void SetData(QVector<double> vtnDataY,QVector<double> vtnDataX);
    bool Calculate(int nCalcMode,int nLeftRange,int nRightRange,double& fPeakHeight,int& nmiddle,int& nmin1,int& nmin2);
    bool AFSCalculate(int nCalcMode,int nLeftRange,int nRightRange,float& fPeakHeight,int& nmiddle,int& nmin1,int& nmin2);
    bool Smooth(int num,QVector<double> vtnDataY);

private:
    bool DaoshuJs();
    bool AFSDaoshuJs();
    int IndexOfValue(double fValue,int nOffset = 0);
    void SortCalcValue(int nStart,int nEnd,int nType,int& nValue);
public:
    QVector<double> m_vtnSmoothY;
private:
    vector<double> m_vtnDataX,m_vtnDataY;
    vector<double> m_vtnDS;
};

#endif // ARITHMETIC_H
