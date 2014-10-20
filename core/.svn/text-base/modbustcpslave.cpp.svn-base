#include "modbustcpslave.h"

modbustcpslave::modbustcpslave()
{
    m_pOnModbusTcp03Data = NULL;
    m_pOnModbusTcp16Data = NULL;
    tcp = new tcpserver(5002);
    tcp->SetOnTcpSververRecvData((void*)this,OnTcpSververRecvData);
}
modbustcpslave::~modbustcpslave()
{
    delete tcp;
}


//拆
//buff[4]= (unsigned char)(RegCount >> 8);
//buff[5]= (unsigned char)RegCount;

void modbustcpslave::OnTcpSververRecvData(void* userdata,int hSocket,char* szData,int iLen)
{
    modbustcpslave* pThis = (modbustcpslave*)userdata;
    //网络测试用
    //pThis->tcp->SendMsg(hSocket,szData,iLen);

    MODBUSDATA mbData;
    mbData.stuMBAP.ch0 = szData[0];
    mbData.stuMBAP.ch1 = szData[1];
    mbData.stuMBAP.ch2 = szData[2];
    mbData.stuMBAP.ch3 = szData[3];
    mbData.stuMBAP.ch4 = szData[4];
    mbData.stuMBAP.ch5 = szData[5];
    mbData.stuMBAP.ch6 = szData[6];
    mbData.FC = szData[7];

    //应答包
    char czResData[256];memset(czResData,0,256);
    //前4个字节同步复制
    czResData[0] = mbData.stuMBAP.ch0;
    czResData[1] = mbData.stuMBAP.ch1;
    czResData[2] = mbData.stuMBAP.ch2;
    czResData[3] = mbData.stuMBAP.ch3;
    czResData[6] = mbData.stuMBAP.ch6;
    czResData[7] = mbData.FC;

    int mbLen = 0;
    char czLen[8];memset(czLen,0,8);
    czLen[0] = mbData.stuMBAP.ch5;
    czLen[1] = mbData.stuMBAP.ch4;
    memcpy(&mbLen,czLen,2);
    if(mbLen >=256-10)//超过modbus侦最大长度
    {
        //应答FC+0x80
        int len =  3;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;        
        czResData[7] = 0x80 + mbData.FC;
        czResData[8] = 0x01;//错误码（0x01,0x02,0x03,0x04）
        pThis->tcp->SendMsg(hSocket,czResData,9);
        return;
    }
    switch (mbData.FC)
    {
    case 0x03:
        pThis->Do03Data(hSocket,czResData,szData);
        break;
    case 0x10:
        pThis->Do16Data(hSocket,czResData,szData);
        break;
    default:
        //应答FC+0x80，其它指令不处理，只返回0x01错误码
        int len =  3;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;
        czResData[7] = 0x80 + mbData.FC;
        czResData[8] = 0x01;//错误码（0x01,0x02,0x03,0x04）
        pThis->tcp->SendMsg(hSocket,czResData,9);
        break;
    }

}

void modbustcpslave::Do03Data(int hSocket, char *czResData, char *szData)
{
    char czFirstReg[8];memset(czFirstReg,0,8);
    czFirstReg[0] = szData[9];
    czFirstReg[1] = szData[8];
    int iFirstReg = 0;//首字节
    memcpy(&iFirstReg,czFirstReg,2);
    iFirstReg += 40001;

    char czRegCount[8];memset(czRegCount,0,8);
    czRegCount[0] = szData[11];
    czRegCount[1] = szData[10];
    int iRegCount = 0;//读取数量
    memcpy(&iRegCount,czRegCount,2);
    if(iRegCount > 128)
    {
        int len =  3;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;
        czResData[7] = 0x80 + szData[7];
        czResData[8] = 0x02;//错误码（0x01,0x02,0x03,0x04）
        tcp->SendMsg(hSocket,czResData,9);
    }
    else
    {
        char outData[1024];memset(outData,0,1024);
        int outLen = 0;
        if(m_pOnModbusTcp03Data)
        {
            m_pOnModbusTcp03Data(m_userdata,szData[6],iFirstReg,iRegCount,outData,outLen);
        }
        int len =  outLen + 3;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;
        czResData[8] = outLen;
        memcpy(czResData+9,outData,outLen);
        tcp->SendMsg(hSocket,czResData,outLen+9);
    }
}

void modbustcpslave::Do16Data(int hSocket, char *czResData, char *szData)
{
    char czFirstReg[8];memset(czFirstReg,0,8);
    czFirstReg[0] = szData[9];
    czFirstReg[1] = szData[8];
    int iFirstReg = 0;//首字节
    memcpy(&iFirstReg,czFirstReg,2);
    iFirstReg += 40001;

    char czRegCount[8];memset(czRegCount,0,8);
    czRegCount[0] = szData[11];
    czRegCount[1] = szData[10];
    int iRegCount = 0;//写寄存数量
    memcpy(&iRegCount,czRegCount,2);
    if(iRegCount > 128)
    {
        int len =  3;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;
        czResData[7] = 0x80 + szData[7];
        czResData[8] = 0x03;//错误码（0x01,0x02,0x03,0x04）
        tcp->SendMsg(hSocket,czResData,9);
    }
    else
    {
        int ByteCount = szData[12];
        if(m_pOnModbusTcp16Data)
        {
            m_pOnModbusTcp16Data(m_userdata,szData[6],iFirstReg,iRegCount,szData+13,ByteCount);
        }
        int len =  6;
        czResData[4] = len >> 8;
        czResData[5] = len & 0xff;
        czResData[8] = szData[8];
        czResData[9] = szData[9];
        czResData[10] = szData[10];
        czResData[11] = szData[11];
        tcp->SendMsg(hSocket,czResData,12);
    }
}

void modbustcpslave::SetOnModbusTcp03Data(void* userdata,OnModbusTcp03Data* pOnModbusTcp03Data)
{
    m_userdata = userdata;
    m_pOnModbusTcp03Data = pOnModbusTcp03Data;
}

void modbustcpslave::SetOnModbusTcp16Data(void* userdata,OnModbusTcp16Data* pOnModbusTcp16Data)
{
    m_userdata = userdata;
    m_pOnModbusTcp16Data = pOnModbusTcp16Data;
}
