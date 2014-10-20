#include "cprocess7.h"
#include "../ui/bscomm.h"
#include <algorithm>
#include <math.h>

using namespace std;

CProcess7::CProcess7()
{
    m_iTempValue = 0;
}

CProcess7::~CProcess7()
{

}

//初使化测试
//nKind为1表示手动测试 2表示网络测试 3表示自动测试
int CProcess7::InitBeginOfTest(int nKind)
{
    CProcess::InitBeginOfTest(nKind);
    char czCurveID[64];memset(czCurveID,0,64);
    sqlite_data datalist;
    CYQ* pYQ = NULL;
    for(int i = 0;i<YQMAXCOUNT;i++)
    {
        if( m_pYQ[i])
        {
            pYQ = m_pYQ[i];
            //初使化数据结构对象
            m_nCalcWay = pYQ->m_CurveInfo.CalcMode;
            pYQ->m_RealInfo.TestTime = m_Qtime;//测试时间
            pYQ->m_RealInfo.fResult = 0.0;
            pYQ->m_RealInfo.fCheckResult = 0.0;

            pYQ->m_RealInfo.m_BsDataInfo.BaseValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.SampleValue = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.Absorbency = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.K = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.B = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.R = 0.0;
            pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = 0.0;//空白吸光度
            pYQ->m_RealInfo.CheckChroma = 0.0;//标样浓度
            pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = 0.0;//标样吸光度
            memset(pYQ->m_RealInfo.cDataID,0,16);
            if (1 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"hd",2);
            }
            else if(2 == nKind)
            {
                memcpy(pYQ->m_RealInfo.cDataID,"nt",2);
            }
            char cItem[64];memset(cItem,0,64);
            sprintf(cItem,"WorkCurveID_%s",pYQ->m_sYQ.czID);
            memset(czCurveID,0,64);
            GetAfxConfig()->GetIni("SYSSET",cItem,czCurveID);

            if(strlen(czCurveID) > 0)
            {
                pYQ->m_CurveInfo.CurveID = atoi(czCurveID);
                bool isResult = m_pDB->GetCurveManger(pYQ->m_sYQ.czID,atoi(czCurveID),0,datalist);
                if(isResult)
                {
                    if(!datalist.IsEmpty())
                    {
                        pYQ->m_CurveInfo.R = atof(datalist.GetData(1,5));
                        pYQ->m_CurveInfo.K = atof(datalist.GetData(1,6));
                        pYQ->m_CurveInfo.B = atof(datalist.GetData(1,7));
                        pYQ->m_CurveInfo.BlankAbsorb = atof(datalist.GetData(1,8));
                        pYQ->m_CurveInfo.CheckChroma = atof(datalist.GetData(1,9));
                        pYQ->m_CurveInfo.StdAbsorb = atof(datalist.GetData(1,10));
                    }
                }
            }
        }
    }
    datalist.Free();

    m_vtnBaseSignal.clear();
    m_vtnSampleSignal.clear();
    m_vtnBaseAvg.clear();
    m_vtBsLinear.clear();
    m_vtnSampleAvg.clear();
    m_vtnMinSample.clear();
    dBaseSignalVal = 0;
    dSampleSignalVal = 0;
    //（主）界面显示测试时间,到时去掉
    if(m_pOnBsReadValue)
    {
        m_pOnBsReadValue(m_userdata,1,dBaseSignalVal,m_Qtime);
    }
    return 0;
}

int CProcess7::GetProcessType()
{
    return 7;
}

bool CProcess7::ReadADData(int iCmdType,int iReadCout,int iTimeout)
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    bool bRet = false;
    //命令字
    int16_t wPrm0 = 91;
    int16_t wPrm1 = 1;

    while(iReadCout > 0)
    {
        memset(bBuff,0,MAX_SIZE);
        bBuff[0] = HIBYTE(wPrm0);
        bBuff[1] = LOBYTE(wPrm0);
        bBuff[2] = HIBYTE(wPrm1);
        bBuff[3] = LOBYTE(wPrm1);
        if(pDriver->WriteModbus(6,40001,1+1,bBuff))
        {
            sleep(1);
            memset(bBuff,0,MAX_SIZE);
            if(pDriver->ReadModbus(6,40056,1,bBuff))
            {
                if (iCmdType == 13) //读基线值
                {
                    m_vtnBaseSignal.push_back(MAKEWORD(bBuff[1],bBuff[0]));
                }
                else if (iCmdType == 11)//读样品值
                {
                    m_vtnSampleSignal.push_back(MAKEWORD(bBuff[1],bBuff[0]));
                }
                bRet = true;
                iReadCout -= 1;                
            }
        }
        usleep(1000 * 1000 * (iTimeout - 1));
    }

    //临时加的，给硬件测试用
   if (iCmdType == 13 && m_nCalcWay == 2) //读基线值，而且是六价铬参数时
   {
       memset(bBuff,0,MAX_SIZE);
       if(pDriver->ReadModbus(CMD_LightLight,40054,3,bBuff))
       {
           m_iTempValue = MAKEWORD(bBuff[1],bBuff[0]);
           if(m_pOnBsReadValue)
           {
               m_pOnBsReadValue(m_userdata,20,(float)m_iTempValue,m_Qtime);
           }
       }
   }

   if(iCmdType == 13)
   {
       if(m_vtnBaseSignal.size() == 0)
       {
           //写日志
           char czLog[255];
           memset(czLog,0,255);
           memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
           strcat(czLog," 光敏故障");
           MainWindowDebug("光敏故障");
           bRet = false;
       }
   }
   else if(iCmdType == 11)
   {
       if(m_vtnSampleSignal.size() == 0)
       {
           //写日志
           char czLog[255];
           memset(czLog,0,255);
           memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
           strcat(czLog," 光敏故障");
           MainWindowDebug("光敏故障");
           bRet = false;
       }
   }

    return bRet;
}

bool CProcess7::ReadData(int iCmdType,int iSpaceType,int iReadCou)
{
    unsigned char bBuff[MAX_SIZE];
    memset(bBuff,0,MAX_SIZE);
    int iCount = 0;
    int iRedLenCou = 0;
    int iSpaceTime = 0;
    int iNum = 20;
    char cMsgInfo[64];
    bool bRet = false;
    if(iReadCou > 20000)
        iCount = 20000;
    else
        iCount = iReadCou;
    iSpaceTime = bscomm::GetHgSignSpace(iSpaceType);
    int16_t wPrm0 = 210;
    int16_t wPrm1 = iCount;
    int16_t wPrm2 = iSpaceType;
    bBuff[0] = HIBYTE(wPrm0);
    bBuff[1] = LOBYTE(wPrm0);
    bBuff[2] = HIBYTE(wPrm1);
    bBuff[3] = LOBYTE(wPrm1);
    bBuff[4] = HIBYTE(wPrm2);
    bBuff[5] = LOBYTE(wPrm2);

    m_iTempValue = iSpaceTime;

    int iErrCou = 0;

    int iRedSum = 0;

    if(pDriver->WriteModbus(12,40001,1+2,bBuff))  //先write210命令——》工作状态为空闲——》读取数据长度——》再取数据
    {
        int nStartReg = 40101;

        while(iCount > 0)
        {
            iRedSum ++;

            usleep(iNum * iSpaceTime);
            memset(bBuff,0,MAX_SIZE);
            if(pDriver->ReadModbus(12,40100,1,bBuff))
            {
                iRedLenCou = iRedSum * iNum;
                iRedLenCou = MAKEWORD(bBuff[1],bBuff[0]);
                if(iRedLenCou >= iNum * iRedSum)
                {
                    memset(bBuff,0,MAX_SIZE);
                    if(((pDriver))->ReadModbus(12,nStartReg,iNum,100,bBuff))
                    {

                        int iLx = 0;
                        int iRx = 0;
                        for(int i = 0;i<iNum;i++)
                        {
                            iLx = 2 * i;
                            iRx = 2 * i + 1;


                            if (iCmdType == 13) //读基线值
                            {
                                m_vtnBaseSignal.push_back(MAKEWORD(bBuff[iRx],bBuff[iLx]));                                
                            }
                            else if (iCmdType == 11)//读样品值
                            {                                
                                m_vtnSampleSignal.push_back(MAKEWORD(bBuff[iRx],bBuff[iLx]));                                
                            }
                        }

                        iCount -= iNum;
                        nStartReg += iNum;
                    }
                }
                else
                {
                    iErrCou ++;
                    usleep(iNum * iSpaceTime);
                }
            }
            if(iErrCou == 5)
            {
                iErrCou = 0;
                iCount -= iNum;
                nStartReg += iNum;
            }
        }

        bRet = true;
    }
    else
    {

        usleep(50000);
        memset(cMsgInfo,0,64);
        strcpy(cMsgInfo,"读过程数据失败");
        DebugInfo(cMsgInfo,0);
        printf("1读数据时出现超时...\n");
    }
    if(iCmdType == 13)
    {
        if(m_vtnBaseSignal.size() == 0)
        {
            //写日志
            char czLog[255];
            memset(czLog,0,255);
            memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
            strcat(czLog," 光源故障");
            MainWindowDebug("光源故障");
            bRet = false;
        }
    }
    else if(iCmdType == 11)
    {
        if(m_vtnSampleSignal.size() == 0)
        {
            //写日志
            char czLog[255];
            memset(czLog,0,255);
            memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
            strcat(czLog," 光源故障");
            MainWindowDebug("光源故障");
            bRet = false;
        }
    }
    return bRet;
}

void CProcess7::SaveSrcDataToTxt(int nKind)
{
    char czChildPath[256];memset(czChildPath,0,256);
    char czDataName[256];memset(czDataName,0,256);
    char czDataFile[256];memset(czDataFile,0,256);
    sprintf(czChildPath,"%s/%d/data",DB_ROOT_PATH,this->GetProcessType());
    mkdir_r(czChildPath);
    sprintf(czDataName,"%04d%02d%02d-%02d%02d.txt",m_Qtime.date().year(),m_Qtime.date().month(),m_Qtime.date().day(),m_Qtime.time().hour(),m_Qtime.time().minute());
    sprintf(czDataFile,"%s/%s",czChildPath,czDataName);

    QString qsTxtFileName = QString(czDataFile);
    QFile file(qsTxtFileName);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) )
    {
       QTextStream stream(&file);
       stream << "TempValue:" << m_iTempValue << "\r\n";

       if(nKind == 13)
       {
           if((int)(m_vtnBaseSignal.size()) > 0)
           {
                stream << "BaseValue:" << "\r\n";
                for ( int i = 0; i < (int)(m_vtnBaseSignal.size()); i++ )
                    stream << m_vtnBaseSignal[i] << "\r\n";
           }
       }

       if(nKind == 14)
       {
           if(m_nCalcWay == 1) //保存基线值计算后的数据
           {
               if((int)(m_vtnBaseAvg.size()) > 0)
               {
                   stream << "BaseSumAvgValue:" << "\r\n";
                   for ( int i = 0; i < (int)(m_vtnBaseAvg.size()); i++ )
                       stream << m_vtnBaseAvg[i] << "\r\n";
               }
           }
       }

       if(nKind == 11)
       {
           if((int)(m_vtnSampleSignal.size()) > 0)
           {
                stream << "SampleValue:" << "\r\n";
                for ( int i = 0; i < (int)(m_vtnSampleSignal.size()); i++ )
                    stream << m_vtnSampleSignal[i]<< "\r\n";
           }

           if((int)(m_vtBsLinear.size()) > 0)
           {
               stream << "LinearValue:" << "r\n";
               for ( int i = 0; i < (int)(m_vtBsLinear.size()); i++ )
               {
                   struBsLinear4 item = m_vtBsLinear[i];
                   stream << item.qDataTime.toString("yyyy-MM-dd HH:mm:ss") << " " << item.fDataValue << "\r\n";
               }
           }
       }

       if(nKind == 12)
       {
           if(m_nCalcWay == 1) //保存样品值计算后的数据
           {
               if((int)(m_vtnSampleAvg.size()) > 0)
               {
                   stream << "SampleSumAvgValue:" << "\r\n";
                   for ( int i = 0; i < (int)(m_vtnSampleAvg.size()); i++ )
                       stream << m_vtnSampleAvg[i] << "\r\n";
               }
           }
       }

       file.close();
    }
}

//计算六价铬读取方式
double CProcess7::CaclWaySum2(int nKind)
{
    //sort(m_vtnBaseSignal.begin(), m_vtnBaseSignal.end());
    if (13 == nKind) //保存基线值
    {
        dBaseSignalVal = 0;
        if(m_vtnBaseSignal.size() > 0)
        {
            int iRemoveCou = 0;
            if((int)m_vtnBaseSignal.size() > 0)
               iRemoveCou = (int)m_vtnBaseSignal.size() / 3;

            for(int i = 0;i < iRemoveCou;i++)
            {
                 vector<int>::iterator k = m_vtnBaseSignal.begin();
                 m_vtnBaseSignal.erase(k);//删除第一个元素
                 m_vtnBaseSignal.pop_back();//删除最后一个元素
            }


            for(int i = 0;i < (int)m_vtnBaseSignal.size(); i++)
            {
             dBaseSignalVal += m_vtnBaseSignal[i];
            }

            dBaseSignalVal = qRound(dBaseSignalVal / (int)m_vtnBaseSignal.size());
        }
        return dBaseSignalVal;
    }
    else if(11 == nKind)
    {
        dSampleSignalVal = 0;
        sort(m_vtnSampleSignal.begin(), m_vtnSampleSignal.end());
        if(m_vtnSampleSignal.size() > 0)
        {
            int iRemoveYPCou = 0;
            if(m_vtnSampleSignal.size() > 0)
               iRemoveYPCou = m_vtnSampleSignal.size() / 3;

            for(int i = 0;i<iRemoveYPCou;i++)
            {
                 vector<int>::iterator k = m_vtnSampleSignal.begin();
                 m_vtnSampleSignal.erase(k);//删除第一个元素(删除前面2个数据）
                 m_vtnSampleSignal.pop_back();//删除最后一个元素（删除后面2个数据）
            }
            for(int i = 0;i < (int)(m_vtnSampleSignal.size()); i++)
            {
                dSampleSignalVal += m_vtnSampleSignal[i];
            }

            dSampleSignalVal = qRound(dSampleSignalVal / (int)(m_vtnSampleSignal.size()));
        }
        return dSampleSignalVal;
    }
}

//保存数据
int CProcess7::SaveDataToTx(int nKind)
{
    int iFlag = MCU_BUSY;
    int iDataLen = 0;
    if(m_nCalcWay == 1)
        iDataLen = float(10 * float(10 / float(m_iTempValue / 1000)));

     if (13 == nKind) //保存基线值
    {
        if(m_vtnBaseSignal.size() > 0)
        {
             SaveSrcDataToTxt(nKind);

            if(m_nCalcWay == 2)//六价铬方式处理
            {
                CaclWaySum2(13);
            }
            else if(m_nCalcWay == 1)
            {
                if(iDataLen > 0 && (int)(m_vtnBaseSignal.size()) > 0)
                {
                    for(int i = 1;i < (int)(m_vtnBaseSignal.size()) - iDataLen; i++)
                    {
                        if(i + 2 <= ((int)(m_vtnBaseSignal.size()) - iDataLen) && (iDataLen >= 3) && (m_vtnBaseSignal.at(i) == 0 && m_vtnBaseSignal.at(i+1) == 0 && m_vtnBaseSignal.at(i+2) == 0))
                        {
                            iFlag = MCU_FAULT;
                            char czLog[255];//写日志
                            memset(czLog,0,255);
                            memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                            strcat(czLog," 光源故障");
                            return iFlag;
                        }

                        int iBaseTemp = 0;
                        for(int j = 0;j < iDataLen;j++)
                        {
                            iBaseTemp += m_vtnBaseSignal.at(i+j);
                        }
                        if(iBaseTemp != 0)
                            m_vtnBaseAvg.push_back(iBaseTemp / iDataLen);
                    }
                }

                if(m_vtnBaseAvg.size() > 0)
                {
                    SaveSrcDataToTxt(14);
                    int iMidden = m_vtnBaseAvg.size() / 2;
                    if(iMidden >= 40)
                    {
                        int iLefCou = iMidden - 20;
                        int iRigCou = iMidden + 20;
                        int iNum = 0;
                        dBaseSignalVal = 0;
                        for(iLefCou;iLefCou < iRigCou;iLefCou++)
                        {
                               dBaseSignalVal += m_vtnBaseAvg.at(iLefCou);
                               iNum ++;
                        }

                        if(dBaseSignalVal > 0)
                            dBaseSignalVal = qRound(dBaseSignalVal / iNum);
                    }
                }
            }
        }
        //通知显示基线值
        if(m_pOnBsReadValue)
        {
            m_pOnBsReadValue(m_userdata,13,dBaseSignalVal,m_Qtime);
        }
    }
    else if(11 == nKind)  //保存样品值
    {
        if(m_vtnSampleSignal.size() > 0)
        {
             SaveSrcDataToTxt(nKind);

            if(m_nCalcWay == 2)//六价铬方式处理
            {
                CaclWaySum2(11);
            }
            else if(m_nCalcWay == 1) //总汞处理方式
            {
                vector<int> vecTempSampleVal;
                vecTempSampleVal.clear();
                if(iDataLen > 0 && (int)(m_vtnSampleSignal.size()) > 0)
                {
                    for(int i = 1;i < (int)(m_vtnSampleSignal.size()) - iDataLen; i++)
                    {
                        if(i + 2 <= ((int)(m_vtnSampleSignal.size()) - iDataLen) && (iDataLen >= 3) && (m_vtnSampleSignal.at(i) == 0 && m_vtnSampleSignal.at(i+1) == 0 && m_vtnSampleSignal.at(i+2) == 0))
                        {
                            iFlag = MCU_FAULT;
                            char czLog[255];//写日志
                            memset(czLog,0,255);
                            memcpy(czLog,m_pro.cProcName,strlen(m_pro.cProcName));
                            strcat(czLog," 光源故障");
                            return iFlag;
                        }
                        int iSampleTemp = 0;
                        for(int j = 0;j< iDataLen;j++)
                        {
                            iSampleTemp += m_vtnSampleSignal.at(i + j);
                        }
                        if(iSampleTemp > 0)
                        {
                            m_vtnSampleAvg.push_back(iSampleTemp / iDataLen);
                            vecTempSampleVal.push_back(iSampleTemp / iDataLen);
                        }
                    }
                    if(vecTempSampleVal.size() > 0)
                    {
                         SaveSrcDataToTxt(12);
                         float fMinSampleVal;
                         for(int m = 0;m < 20;m++)//移除最前面20个值和最后面20个值，防止取最小数时左边或右边没有20个值
                         {
                             vector<int>::iterator k = vecTempSampleVal.begin();
                             vecTempSampleVal.erase(k);
                             vecTempSampleVal.pop_back();
                         }
                         fMinSampleVal = *min_element(vecTempSampleVal.begin(),vecTempSampleVal.end());//最出这段数据中最小值

                         int iFreeCou = 0;//次数
                         for(int x = 0;x < (int)(m_vtnSampleAvg.size());x++)
                         {
                             if(fMinSampleVal == m_vtnSampleAvg.at(x))
                             {
                                 int nCount = (int)(m_vtnSampleAvg.size());
                                 if(x - 20 <= nCount && nCount >= 0 && x + 20 <= nCount)
                                 {
                                     int iLefCou = x - 20;
                                     int iRigCou = x + 20;
                                     int iNum = 0;
                                     dSampleSignalVal = 0;
                                     for(iLefCou;iLefCou < iRigCou;iLefCou++)
                                     {
                                         dSampleSignalVal += m_vtnSampleAvg.at(iLefCou);
                                         iNum++;
                                     }

                                     if(dSampleSignalVal > 0)
                                         m_vtnMinSample.push_back(dSampleSignalVal / iNum);
                                 }

                                 iFreeCou ++;
                                 if(iFreeCou == 5)
                                 {
                                     break;
                                 }
                             }
                         }
                         dSampleSignalVal = 0;
                         if((int)(m_vtnMinSample.size()) > 0)
                         {
                             for(int i=0;i<(int)(m_vtnMinSample.size());i++)
                             {
                                 dSampleSignalVal += m_vtnMinSample.at(i);
                             }
                             if(dSampleSignalVal > 0)
                                 dSampleSignalVal = qRound(dSampleSignalVal / (int)(m_vtnMinSample.size()));
                         }
                    }
                }
            }
        }

        //通知显示样品值
        if(m_pOnBsReadValue)
        {
            m_pOnBsReadValue(m_userdata,11,dSampleSignalVal,m_Qtime);
        }
    }   

    iFlag = MCU_IDEL;
    return iFlag;
}


//计算结果值
bool CProcess7::CalcTestValue(int nKey)
{
    char czValue[64];

    //计算结果
    if (nKey >= 0)
    {
        CYQ* pYQ = NULL;
        pYQ = m_pYQ[nKey];
        if(pYQ)
        {
            pYQ->m_RealInfo.TestTime = m_pYQ[nKey]->m_RealInfo.TestTime;
            pYQ->m_RealInfo.m_BsDataInfo.BaseValue = dBaseSignalVal;
            pYQ->m_RealInfo.m_BsDataInfo.SampleValue = dSampleSignalVal;
            pYQ->m_RealInfo.m_BsDataInfo.K = pYQ->m_CurveInfo.K;
            pYQ->m_RealInfo.m_BsDataInfo.B = pYQ->m_CurveInfo.B;
            pYQ->m_RealInfo.m_BsDataInfo.R = pYQ->m_CurveInfo.R;
            pYQ->m_RealInfo.m_BsDataInfo.BlankAbsorb = pYQ->m_CurveInfo.BlankAbsorb;//空白吸光度
            pYQ->m_RealInfo.m_BsDataInfo.StdAbsorb = pYQ->m_CurveInfo.StdAbsorb;//标样吸光度
            pYQ->m_RealInfo.CheckChroma = pYQ->m_CurveInfo.CheckChroma;//标样浓度
            pYQ->m_RealInfo.m_BsDataInfo.CurveID = pYQ->m_CurveInfo.CurveID;

            //获取消解温度与消解时间
            //消解时间
            memset(czValue,0,64);
            GetAfxConfig()->GetIni("TempSet","HeatTime",czValue);
            if(strlen(czValue) > 0)
            {
                pYQ->m_RealInfo.nHeatTime = atoi(czValue);
            }

            //消解温度
            memset(czValue,0,64);
            GetAfxConfig()->GetIni("TempSet","TempValue",czValue);
            if(strlen(czValue) > 0)
            {
                pYQ->m_RealInfo.fHeatValue = atof(czValue);
            }

            double fAbsorb = 0.0;
            double fValue = 0.0;

            if(dBaseSignalVal > 0 && dSampleSignalVal > 0)
            {
                fAbsorb = log10(dBaseSignalVal / dSampleSignalVal);
                pYQ->m_RealInfo.m_BsDataInfo.Absorbency = fAbsorb;

                if(pYQ->m_CurveInfo.CheckChroma > 0)
                {
                   // cout << m_pYQ[nKey]->m_CurveManger4.StdConc << endl;
                    //标样浓度误差 =标样浓度 -( K * (标样吸光度 - 空白吸光度) + B
                    //浓度 = K * (X - 空白吸光度) + B +(-) 标样浓度误差

                    float fErrSampChroma = 0.0;//标样的误差
                    fErrSampChroma = pYQ->m_CurveInfo.CheckChroma - (pYQ->m_CurveInfo.K * (pYQ->m_CurveInfo.StdAbsorb - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B);
                    fValue = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B + fErrSampChroma;
                }
                else
                {
                    fValue = pYQ->m_CurveInfo.K * (pYQ->m_RealInfo.m_BsDataInfo.Absorbency - pYQ->m_CurveInfo.BlankAbsorb) + pYQ->m_CurveInfo.B ;
                }

                if(fValue < 0)
                    fValue = 0;

                memset(czValue,0,64);
                sprintf(czValue,"计算浓度结果,浓度:%.4f,吸光度:%.4f",fValue,fAbsorb);
                DebugInfo(czValue,3);
            }
            else
            {
                fValue = 0;
            }
            if(CMD_TEST == m_nCmd)
            {
                pYQ->m_RealInfo.fResult = fValue;
            }
            else if(CMD_SAMPLETEST == m_nCmd) //标样核查
            {
                pYQ->m_RealInfo.fCheckResult = fValue;
            }

            //流程正常结束,在此保存结果
            this->SaveResultToDatabase();

            //通知界面测试结果
            if(m_pOnBsReadValue)
            {
                m_pOnBsReadValue(m_userdata,15,pYQ->m_RealInfo.fResult,m_Qtime);
            }

            //是否自动切换曲线
            memset(czValue,0,64);
            GetAfxConfig()->GetIni("AutoAdjust_SET","RangeAutoChangeSet",czValue);
            if(!strcmp(czValue,"1") && m_nCmd == CMD_TEST)//仅当是测试命令时才自动切换曲线
            {
                if(VerifyRange(pYQ->m_sYQ.czID,pYQ->m_CurveInfo.CurveID,(double)pYQ->m_RealInfo.fResult))
                {
                }
                else//不切换曲线,标识位置FALSE
                {
                    m_bUpSwitchCurve = false;
                    m_bDowSwitchCurve = false;
                }
            }
            pYQ = NULL;
        }
    }
    return true;
}
void CProcess7::SaveResultToDatabase(int nCodeID)
{
    if(CMD_TEST == m_nCmd || CMD_SAMPLETEST == m_nCmd)
    {
        CProcess::SaveResultToDatabase(nCodeID);
    }
    else
    {
        if(m_nFinishResult != PRO_OK)//流程异常
            return;

        CYQ* pYQ = NULL;
        for(int i=0;i<YQMAXCOUNT;i++)
        {
            pYQ = m_pYQ[i];
            if(pYQ && (nCodeID == 0 || pYQ->m_sYQ.InTypeID == nCodeID))
            {
                if(m_nCmd == CMD_BANKADJUST)//空白校准
                {
                    if(m_pOnTestFinished)
                    {
                        //这里要计算空白校准的误差率(范围：20%)
                        //空白校准、标样校准测出来的吸光度与当前参数第1个点的吸光度进行比较，
                        //如果误差范围小于20%时，则修正 m_CurveManger4（曲线管理）当前工作曲线空白吸光度值
                        //注意：空白校准比较吸光度，标样校准比较浓度
                        sqlite_data datalist;
                        bool isResult = m_pDB->GetCurveDataInfo(pYQ->m_sYQ.czID,1,0,datalist);
                        if(isResult)
                        {
                            if(!datalist.IsEmpty())
                            {
                                float fAbsorbency = atof(datalist.GetData(1,6));
                                if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency != 0 && fAbsorbency != 0)
                                {
                                    if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency / fAbsorbency * 100 < 20)
                                    {
                                        m_pOnTestFinished(m_userdata,CMD_BANKADJUST,pYQ->m_sYQ.czID);
                                    }
                                }
                            }
                        }
                        datalist.Free();
                    }
                }
                else if(m_nCmd == CMD_SAMPLEADJUST)
                {
                    if(m_pOnTestFinished)
                    {
                        //这里要计算标样校准的误差率(范围：20%)
                        if(pYQ->m_RealInfo.m_BsDataInfo.Absorbency != 0 &&  pYQ->m_CurveInfo.CheckChroma != 0)
                        {
                            if(pYQ->m_RealInfo.fResult / pYQ->m_CurveInfo.CheckChroma * 20 < 20)
                            {
                                m_pOnTestFinished(m_userdata,CMD_SAMPLEADJUST,pYQ->m_sYQ.czID);
                            }
                        }
                    }
                }
                pYQ = NULL;
            }
        }
    }
}
//读取Modble数据信息min_element
int CProcess7::GetSysModuleStatus()
{
    int bRet = MCU_BUSY;
    switch(m_pro.PID)
    {
    case 13:  //基线值
    {
        //m_pro.arrParaValue[0]  读取类型(1-15) m_pro.arrParaValue[1] 读取数据个数
        if(m_nCalcWay == 1)
        {
            if(ReadData(13,m_pro.arrParaValue[0],m_pro.arrParaValue[1]))
            {
                if((CMD_TEST == m_nCmd)||(CMD_SAMPLETEST == m_nCmd)||(CMD_SAMPLEADJUST == m_nCmd)||(CMD_BANKADJUST == m_nCmd) || (CMD_CURVECALIBRATION == m_nCmd))
                {
                    bRet = SaveDataToTx(13);
                }
            }
            else
            {
                bRet = MCU_FAULT;
            }
        }
        else if(m_nCalcWay == 2)
        {
            if(ReadADData(13,m_pro.arrParaValue[0],m_pro.arrParaValue[1]))
            {
                if((CMD_TEST == m_nCmd)||(CMD_SAMPLETEST == m_nCmd)||(CMD_SAMPLEADJUST == m_nCmd)||(CMD_BANKADJUST == m_nCmd) || (CMD_CURVECALIBRATION == m_nCmd))
                {
                    bRet = SaveDataToTx(13);
                }
            }
        }
        break;
    }
    case 11:  //读取样品值  m_pro.arrParaValue[0]  读取类型(1-15) m_pro.arrParaValue[1] 读取数据个数
    {
        if(m_nCalcWay == 1)
        {
            if(ReadData(11,m_pro.arrParaValue[0],m_pro.arrParaValue[1]))
            {
                bRet = SaveDataToTx(11);
            }
            else
            {
                bRet = MCU_FAULT;
            }
        }
        else if(m_nCalcWay == 2)
        {
            if(ReadADData(11,m_pro.arrParaValue[0],m_pro.arrParaValue[1]))
            {
                bRet = SaveDataToTx(11);
            }
        }
        break;
    }
    case 52:
        if(m_pro.arrParaValue[0] == 0)//所有仪器
        {
            for(int t1=0;t1<YQMAXCOUNT;t1++)
            {
                if(m_pYQ[t1] && m_pYQ[t1]->GetProcessType() == 4)
                {
                    CalcTestValue(t1);
                }
            }
        }
        else//单个仪器
        {
            for(int t1=0;t1<YQMAXCOUNT;t1++)
            {
                if((m_pYQ[t1]) && (m_pro.arrParaValue[0] == m_pYQ[t1]->m_sYQ.InTypeID) && m_pYQ[t1]->GetProcessType() == 4)
                {
                    CalcTestValue(t1);
                }
            }
        }
        bRet = MCU_IDEL;
        break;
    default:
        bRet = MCU_IDEL;
        break;
    }
    return bRet;
}
