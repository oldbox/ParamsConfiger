#include "ccomm.h"
#include <iostream>
#include "cconfig.h"
using namespace std;

CComm::CComm()
{
    initgbclient();
    m_tcpServer = new tcpserver(8100);
    m_tcpServer->SetOnTcpSververRecvData((void*)this,OnServerRecvData);
}
CComm::~CComm()
{
    delete m_tcpServer;
    unintgbclient();
}

void CComm::initgbclient()
{
    ip = new char[64];memset(ip,0,64);
    mn = new char[64];memset(mn,0,64);
    pw = new char[64];memset(pw,0,64);
    char* czPort = new char[64];memset(czPort,0,64);

    GetAfxConfig()->GetIni(NETGROUP,GBIP,ip);
    GetAfxConfig()->GetIni(NETGROUP,GBMN,mn);
    GetAfxConfig()->GetIni(NETGROUP,GBPW,pw);
    GetAfxConfig()->GetIni(NETGROUP,GBPORT,czPort);
    port = atoi(czPort);

    m_pGbclient = new gbtcpclient(ip,port,mn,pw);
    m_pGbclient->SetOnConnected((void*)this,OnConnected);
    m_pGbclient->SetOnRecvData((void*)this,OnClientRecvData);
    m_pGbclient->Init();
}

void CComm::unintgbclient()
{
    m_pGbclient->UnInit();
    delete m_pGbclient;
    delete []ip;
    delete []pw;
    delete []mn;
}

void CComm::OnConnected( void* userdata,bool bConnect)
{
  CComm* pThis = (CComm*)userdata;
   if(!bConnect)cout << "断开" << endl;
   else cout << "连接" << endl;
}

void CComm::OnClientRecvData(void* userdata,char* szData,int iLen)
{
    CComm* pThis = (CComm*)userdata;
    pThis->DoData(szData,iLen);
}
void CComm::OnServerRecvData(void* userdata,int hSocket,char* szData,int iLen)
{
    CComm* pThis = (CComm*)userdata;
    pThis->DoData(szData,iLen);
}
void CComm::SendToAll(char* czData,int iLen)
{
    m_pGbclient->SendMsg(czData,iLen);
    m_tcpServer->SendMsgToAllClient(czData,iLen);
}

//处理客户端或服务器来的指令
void CComm::DoData(char *czData, int iLen)
{
    cout << czData << endl;
}
