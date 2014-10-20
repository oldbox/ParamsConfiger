#include "bscomm.h"
#include <math.h>
#include <QRegExp>

bscomm::bscomm()
{
}

double bscomm::Round(double dbVal, int nPrec /* = 2 */)
{
    const double dbShift = pow(10.0, nPrec);
    return  floor(dbVal * dbShift + 0.5) / dbShift;
}

//用基线值、样品值计算吸光度值
double bscomm::GetCountAbsorbancy(double dBaseSignalVal,double dSampleSignalVal)
{
    if(dBaseSignalVal > 0 && dSampleSignalVal > 0)
    {
        return log10(dBaseSignalVal / dSampleSignalVal);
    }
}

int bscomm::GetHgSignSpace(int iSpaceType)
{
    static int SpaceTime= 250;
    if(0 ==iSpaceType)
        SpaceTime = 250;
    else if(1 ==iSpaceType)
        SpaceTime = 375;
    else if(2 ==iSpaceType)
        SpaceTime = 500;
    else if(3 ==iSpaceType)
        SpaceTime = 750;
    else if(4 ==iSpaceType)
        SpaceTime = 1000;
    else if(5 ==iSpaceType)
        SpaceTime = 1500;
    else if(6 ==iSpaceType)
        SpaceTime = 2000;
    else if(7 ==iSpaceType)
        SpaceTime = 3000;
    else if(8 ==iSpaceType)
        SpaceTime = 4000;
    else if(9 ==iSpaceType)
        SpaceTime = 6000;
    else if(10 ==iSpaceType)
        SpaceTime = 8000;
    else if(11 ==iSpaceType)
        SpaceTime = 12000;
    else if(12 ==iSpaceType)
        SpaceTime = 16000;
    else if(13 ==iSpaceType)
        SpaceTime = 24000;
    else if(14 ==iSpaceType)
        SpaceTime = 32000;
    else if(15 ==iSpaceType)
        SpaceTime = 48000;
    return SpaceTime;
}

//正浮点数（包括整数）
bool bscomm::isNumber(QString qsValue)
 {
//    QRegExp regExp;
//    regExp.setPattern("^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
//    return regExp.exactMatch(qsValue); //返回的值为false，因为s中含有@字符
    return true;
 }


