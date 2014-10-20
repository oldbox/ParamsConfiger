#ifndef MODBUSTCPSLAVE_H
#define MODBUSTCPSLAVE_H

#include "tcpserver.h"

//回调处理数据函数原型
typedef void OnModbusTcp03Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* outData,int& ByteCount);
typedef void OnModbusTcp16Data(void* userdata,int SlaveID,int iFirstReg,int iRegCount,char* czRegContent,int ByteCount);

struct MBAP
{
    char ch0;   //事处处理标只符
    char ch1;   //事务处理标识符
    char ch2;   //协议标识符
    char ch3;   //协议标识符
    char ch4;   //长度
    char ch5;   //长度
    char ch6;   //单元标识符，modbus地址
};
struct MODBUSDATA
{
    //MODBUSDATA(){memset(szData,0,256);};
    MBAP stuMBAP;
    char FC;    //指令代码
    //char szData[256];   //指令内容
};
class modbustcpslave
{
public:
    modbustcpslave();
    ~modbustcpslave();

    void SetOnModbusTcp03Data(void* userdata,OnModbusTcp03Data* pOnModbusTcp03Data);
    void SetOnModbusTcp16Data(void* userdata,OnModbusTcp16Data* pOnModbusTcp16Data);

   // void Send03ToMaster();
private:
    tcpserver* tcp;
    static void OnTcpSververRecvData(void* userdata,int hSocket,char* szData,int iLen);

    void* m_userdata;
    OnModbusTcp03Data* m_pOnModbusTcp03Data;
    OnModbusTcp16Data* m_pOnModbusTcp16Data;

    void Do03Data(int hSocket,char* czResData,char* szData);
    void Do16Data(int hSocket,char* czResData,char* szData);
};

#endif // MODBUSTCPSLAVE_H
