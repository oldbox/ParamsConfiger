#include "cmodbus.h"
#include <iostream>
#include <QString>

using namespace std;


CModbus::CModbus(CSerialPort* pCom)
{
    m_pCom = NULL;
    m_pCom = pCom;
}

bool CModbus::Read03Data(int nAddr,int nReg, int nCount)
{
    if(!m_pCom)return false;
    unsigned char pBuf[16];
    unsigned char czData[MAX_SIZE];
    memset(czData,0,MAX_SIZE);
    memset(pBuf,0,16);

    pBuf[0] = nAddr;
    pBuf[1] = 03;
    pBuf[2] = (nReg-40001)>> 8;		//寄存器地址高
    pBuf[3] = (nReg-40001) & 0xff;		//寄存器地址低
    pBuf[4] = nCount >> 8;	//寄存器个数
    pBuf[5] = nCount & 0xff;
    pBuf[6] = LOBYTE(CRC16(pBuf,6));
    pBuf[7] = HIBYTE(CRC16(pBuf,6));

    m_pCom->TCFlush();
    m_pCom->Lock();
    bool bRet = false;
    int nFaild = 0;

    int nLen = 0;

    if(8 == m_pCom->WriteComPort(pBuf,8))
    {
        usleep(30000);
        while(1)
        {
           usleep(1000);
           int nRecv = m_pCom->ReadComPort(czData + nLen, 1);

            if(nRecv > 0)
            {
                nLen += nRecv;
                if(czData[0] == nAddr)
                {
                    if(nLen >= 2)
                    {
                        if(czData[1] == 0x03)
                        {
                            if(nLen == 5+nCount*2)
                            {
                                if(CRC16(czData, 5+nCount*2) == 0x0000)
                                {
                                    memcpy(m_czData03, czData, 5+nCount*2);
                                    nLen = 0;
                                    m_pCom->UnLock();
                                    return true;
                                }
                                else
                                {
                                    memmove(czData, czData+1, nLen-1);
                                    nLen -= 1;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            memmove(czData, czData+1, nLen-1);
                            nLen -= 1;
                        }
                    }
                    else
                    {
                         continue;
                    }
                }
                else
                {
                    memset(czData, 0, MAX_SIZE);
                    nLen = 0;
                }
            }
            else
            {
                //判断超时
                if(nFaild > 3)
                {
                    m_pCom->UnLock();
//                    qDebug("Read03Data 3 times nFaild ");
                    return false;
                }
                nFaild++;
//                qDebug("Read03Data 100000 nFaild ");
                usleep(100000);
            }
        }
    }
    m_pCom->UnLock();
    return bRet;
 }
bool CModbus::Read03Data(int nAddr,int nReg, int nCount,int nTimeout)
{
    if(!m_pCom)return false;
    unsigned char pBuf[16];
    unsigned char czData[MAX_SIZE];
    memset(czData,0,MAX_SIZE);
    memset(pBuf,0,16);

    pBuf[0] = nAddr;
    pBuf[1] = 03;
    pBuf[2] = (nReg-40001)>> 8;		//寄存器地址高
    pBuf[3] = (nReg-40001) & 0xff;		//寄存器地址低
    pBuf[4] = nCount >> 8;	//寄存器个数
    pBuf[5] = nCount & 0xff;
    pBuf[6] = LOBYTE(CRC16(pBuf,6));
    pBuf[7] = HIBYTE(CRC16(pBuf,6));

    m_pCom->TCFlush();
    m_pCom->Lock();
    bool bRet = false;
    int nFaild = 0;

    int nLen = 0;

    if(8 == m_pCom->WriteComPort(pBuf,8))
    {
        usleep(30000);
        while(1)
        {
            usleep(1000);
           int nRecv = m_pCom->ReadComPort(czData + nLen, 1);
            if(nRecv > 0)
            {
                nLen += nRecv;
                if(czData[0] == nAddr)
                {
                    if(nLen >= 2)
                    {
                        if(czData[1] == 0x03)
                        {
                            if(nLen == 5+nCount*2)
                            {
                                if(CRC16(czData, 5+nCount*2) == 0x0000)
                                {
                                    memcpy(m_czData03, czData, 5+nCount*2);
                                    nLen = 0;
                                    m_pCom->UnLock();
                                    return true;
                                }
                                else
                                {
                                    memmove(czData, czData+1, nLen-1);
                                    nLen -= 1;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            memmove(czData, czData+1, nLen-1);
                            nLen -= 1;
                        }
                    }
                    else
                    {
                         continue;
                    }
                }
                else
                {
                    memset(czData, 0, MAX_SIZE);
                    nLen = 0;
                }
            }
            else
            {
                //判断超时
                if(nFaild > 3)
                {
                    m_pCom->UnLock();
//                    qDebug("Read03Data 3 times nFaild ");
                    return false;
                }
                nFaild++;
//                qDebug("Read03Data 500*nTimeout nFaild ");
                usleep(500*nTimeout);
            }
        }
    }
    m_pCom->UnLock();
    return bRet;


 }
/*bool CModbus::Read03Data(int nAddr,int nReg, int nCount,int nTimeout)
{
    if(!m_pCom)return false;
    unsigned char pBuf[16];
    memset(pBuf,0,16);
    pBuf[0] = nAddr;
    pBuf[1] = 03;
    pBuf[2] = (nReg-40001)>> 8;		//寄存器地址高
    pBuf[3] = (nReg-40001) & 0xff;		//寄存器地址低
    pBuf[4] = nCount >> 8;	//寄存器个数
    pBuf[5] = nCount & 0xff;
    pBuf[6] = LOBYTE(CRC16(pBuf,6));
    pBuf[7] = HIBYTE(CRC16(pBuf,6));

    m_pCom->Lock();
    bool bRT = false;
    for (int nIndex = 0; nIndex < 3;nIndex++)
    {
        memset(m_czData03,0,MAX_SIZE);
        if(8 == m_pCom->WriteComPort(pBuf,8))
        {
            usleep(nTimeout*1000);

            if(5+nCount*2 == m_pCom->ReadComPort(m_czData03,5+nCount*2))
            {
                int ByteCount = m_czData03[2];
                if(CRC16(m_czData03,ByteCount+5) == 0x0000)
                {
                    if(m_czData03[0] == nAddr)
                    {
                        bRT = true;
                    }
                    else
                    {
                        bRT=false;
                    }
                    break;
                }
                else
                {
                    printf("Read Received Modbus Fail \n!");
                }
            }
            else
            {
                printf("Read No Data \n!");
                usleep(100000);
            }
        }
        else
        {
            //通信故障
            printf("Read Communication failure\n");
            usleep(100000);
        }
    }
    m_pCom->UnLock();
    return bRT;


 }*/
bool CModbus::Write10Data(int nAddr,int FisrtRegister,int Regs,unsigned char* czData)
{
    if(!m_pCom)return false;
    int Reg = FisrtRegister - 40001;
    unsigned char Data[MAX_SIZE];
    memset(Data,0,MAX_SIZE);
    unsigned char pBuf[MAX_SIZE];

    memset(pBuf,0,MAX_SIZE);
    pBuf[0] = nAddr;
    pBuf[1] = 0x10;
    pBuf[2] = Reg >> 8;
    pBuf[3] = Reg & 0xff;
    pBuf[4] = Regs >> 8;
    pBuf[5] = Regs & 0xff;
    pBuf[6] = Regs * 2;
    for(int i=0;i<Regs*2;i++)
    {
       pBuf[7+i] = czData[i];
    }
    int32_t dCRC = CRC16(pBuf,7+Regs*2);
    pBuf[7+Regs*2] = LOBYTE(dCRC);
    pBuf[8+Regs*2] = HIBYTE(dCRC);

    m_pCom->TCFlush();
    m_pCom->Lock();

    bool bRet = false;
    int nFaild = 0;
    int nLen = 0;

    if(9+Regs*2 == m_pCom->WriteComPort(pBuf,9+Regs*2))
    {
        usleep(30000);
        while(1)
        {
           usleep(1000);
           int nRecv = m_pCom->ReadComPort(Data + nLen, 1);

            if(nRecv > 0)
            {
                nLen += nRecv;
                if(Data[0] == nAddr)
                {
                    if(nLen >= 2)
                    {
                        if(Data[1] == 0x10)
                        {
                            if(nLen == 8)
                            {
                                if(CRC16(Data, 8) == 0x0000)
                                {
                                    memcpy(m_czData03, Data, 8);
                                    nLen = 0;
                                    m_pCom->UnLock();
                                    return true;
                                }
                                else
                                {
                                    memmove(Data, Data+1, nLen-1);
                                    nLen -= 1;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            memmove(Data, Data+1, nLen-1);
                            nLen -= 1;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    memset(Data, 0, MAX_SIZE);
                    nLen = 0;
                }
            }
            else
            {
                //判断超时
                if(nFaild > 3)
                {
                    m_pCom->UnLock();
//                    qDebug("Read03Data 3 times nFaild ");
                    return false;
                }
                nFaild++;
//                qDebug("Read03Data 100000 nFaild ");
                usleep(100000);
            }
        }
    }
    m_pCom->UnLock();
    return bRet;
}
bool CModbus::Write10Data(int nAddr,int FisrtRegister,int Regs,int nTimeout,unsigned char* czData)
{
    if(!m_pCom)return false;
    int Reg = FisrtRegister - 40001;
    unsigned char pBuf[MAX_SIZE];
    unsigned char Data[MAX_SIZE];
    memset(Data,0,MAX_SIZE);

    memset(pBuf,0,MAX_SIZE);
    pBuf[0] = nAddr;
    pBuf[1] = 0x10;
    pBuf[2] = Reg >> 8;
    pBuf[3] = Reg & 0xff;
    pBuf[4] = Regs >> 8;
    pBuf[5] = Regs & 0xff;
    pBuf[6] = Regs * 2;
    for(int i=0;i<Regs*2;i++)
    {
       pBuf[7+i] = czData[i];
    }
    int32_t dCRC = CRC16(pBuf,7+Regs*2);
    pBuf[7+Regs*2] = LOBYTE(dCRC);
    pBuf[8+Regs*2] = HIBYTE(dCRC);

    m_pCom->TCFlush();
    m_pCom->Lock();

    bool bRet = false;
    int nFaild = 0;
    int nLen = 0;

    if(9+Regs*2 == m_pCom->WriteComPort(pBuf,9+Regs*2))
    {
        usleep(30000);
        while(1)
        {
           usleep(1000);
           int nRecv = m_pCom->ReadComPort(Data + nLen, 1);
            if(nRecv > 0)
            {
                nLen += nRecv;
                if(Data[0] == nAddr)
                {
                    if(nLen >= 2)
                    {
                        if(Data[1] == 0x10)
                        {
                            if(nLen == 8)
                            {
                                if(CRC16(Data, 8) == 0x0000)
                                {
                                    memcpy(m_czData03, Data, 8);
                                    nLen = 0;
                                    m_pCom->UnLock();
                                    return true;
                                }
                                else
                                {
                                    memmove(Data, Data+1, nLen-1);
                                    nLen -= 1;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            memmove(Data, Data+1, nLen-1);
                            nLen -= 1;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    memset(Data, 0, MAX_SIZE);
                    nLen = 0;
                }
            }
            else
            {
                //判断超时
                if(nFaild > 3)
                {
                    m_pCom->UnLock();
//                    qDebug("Write10Data 3 times nFaild ");
                    return false;
                }
//                qDebug("Write10Data nTimeout*500 nFaild ");
                nFaild++;
                usleep(nTimeout*500);
            }
        }
     }
     m_pCom->UnLock();
     return bRet;
}
/*bool CModbus::Write10Data(int nAddr,int FisrtRegister,int Regs,int nTimeout,unsigned char* czData)
{
    if(!m_pCom)return false;
    int Reg = FisrtRegister - 40001;
    unsigned char pBuf[MAX_SIZE];
    unsigned char pBuf1[MAX_SIZE];
    int nLen = 0;
    memset(pBuf,0,MAX_SIZE);
    pBuf[0] = nAddr;
    pBuf[1] = 0x10;
    pBuf[2] = Reg >> 8;
    pBuf[3] = Reg & 0xff;
    pBuf[4] = Regs >> 8;
    pBuf[5] = Regs & 0xff;
    pBuf[6] = Regs * 2;
    for(int i=0;i<Regs*2;i++)
    {
       pBuf[7+i] = czData[i];
    }
    int32_t dCRC = CRC16(pBuf,7+Regs*2);
    pBuf[7+Regs*2] = LOBYTE(dCRC);
    pBuf[8+Regs*2] = HIBYTE(dCRC);

    m_pCom->Lock();
    bool bRT = false;
    if(9+Regs*2 == m_pCom->WriteComPort(pBuf,9+Regs*2))
    {
        usleep(nTimeout*1000);
        memset(pBuf1,0,MAX_SIZE);
        nLen = m_pCom->ReadComPort(pBuf1,255);
        if(8 == nLen)
         {
            if(CRC16(pBuf1,8)==0x0000)
            {
                if(pBuf1[0] == nAddr)
                {
                    bRT = true;
                }
                else
                {
                    bRT = false;
                }
             }
             else
             {
                printf("Write Received Modbus Fail \n!");
                usleep(100000);
             }
         }
         else
         {
            printf("Write Communication failure");
            usleep(100000);
         }
     }
     m_pCom->UnLock();
     return bRT;
}*/
bool CModbus::ReadbroadcastData(int nAddr,int nReg, int nCount)
{
    if(!m_pCom)return false;
    unsigned char pBuf[16];
    memset(pBuf,0,16);
    pBuf[0] = nAddr;
    pBuf[1] = 03;
    pBuf[2] = (nReg-40001)>> 8;		//寄存器地址高
    pBuf[3] = (nReg-40001) & 0xff;		//寄存器地址低
    pBuf[4] = nCount >> 8;	//寄存器个数
    pBuf[5] = nCount & 0xff;
    pBuf[6] = LOBYTE(CRC16(pBuf,6));
    pBuf[7] = HIBYTE(CRC16(pBuf,6));

    m_pCom->Lock();
    bool bRT = false;
    if(8 == m_pCom->WriteComPort(pBuf,8))
    {
        sleep(1);
        memset(m_czData03,0,MAX_SIZE);

        if(0 < m_pCom->ReadComPort(m_czData03,255))
        {
            int ByteCount = m_czData03[2];
            if(CRC16(m_czData03,ByteCount+5) == 0x0000)
            {
                if(m_czData03[0] == nAddr)
                    bRT = true;

            }
            else
            {
               printf("broadcast Received Modbus Fail \n!");
            }
        }
        else
        {
            printf("broadcast Communication failure\n!");
        }
    }
    else
    {
       //通信故障
        printf("通信故障\n!");
    }
   m_pCom->UnLock();
    return bRT;
 }
bool CModbus::ReadbroadcastData(int nAddr,int nReg, int nCount,unsigned char* czData)
{
    if(!m_pCom)return false;
    unsigned char pBuf[16];
    memset(pBuf,0,16);
    pBuf[0] = nAddr;
    pBuf[1] = 03;
    pBuf[2] = (nReg-40001)>> 8;		//寄存器地址高
    pBuf[3] = (nReg-40001) & 0xff;		//寄存器地址低
    pBuf[4] = nCount >> 8;	//寄存器个数
    pBuf[5] = nCount & 0xff;
    pBuf[6] = LOBYTE(CRC16(pBuf,6));
    pBuf[7] = HIBYTE(CRC16(pBuf,6));

    m_pCom->Lock();
    bool bRT = false;
    if(8 == m_pCom->WriteComPort(pBuf,8))
    {
        sleep(1);
        memset(czData,0,MAX_SIZE);

        if(0 < m_pCom->ReadComPort(czData,MAX_SIZE))
        {
            int ByteCount = czData[2];
            if(CRC16(czData,ByteCount+5) == 0x0000)
            {
                if(czData[0] == nAddr)
                bRT = true;

            }
            else
            {
               printf("broadcast Received Modbus Fail \n!");
            }
        }
        else
        {
            printf("broadcast Communication failure\n!");
        }
    }
    else
    {
       //通信故障
        printf("通信故障\n!");
    }
   m_pCom->UnLock();
    return bRT;
 }
void CModbus::GetRegisterValue(int nCount,unsigned char* czData)
{
    memcpy(czData,m_czData03+3,nCount*2);
}

int16_t CModbus::CRC16(unsigned char *pBuf,int nLength)
{
   unsigned short wReturn = 0xFFFF;
   for(int nDataLen=0;nDataLen<nLength;nDataLen++)
    {
        wReturn^=(unsigned short)((unsigned char)(pBuf[nDataLen]));
        for(int nIndex=0;nIndex<8;nIndex++)
        {
            if(wReturn&0x0001)
            {
                wReturn>>=1;
                wReturn^=0xA001;
            }
            else
            {
                wReturn>>=1;
            }
        }
    }
    return wReturn;
}
