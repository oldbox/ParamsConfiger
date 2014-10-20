#ifndef CCOMM_H
#define CCOMM_H

#include "gbtcpclient.h"
#include "tcpserver.h"

class CComm
{
public:
    CComm();
    ~CComm();
    gbtcpclient* m_pGbclient;
    tcpserver* m_tcpServer;

    void SendToAll(char* czData,int iLen);

private:
    char* ip;
    char* pw;
    char* mn;
    int port;
    void initgbclient();
    void unintgbclient();
    static void OnConnected(void* userdata,bool bConnect);
    static void OnClientRecvData(void* userdata,char* szData,int iLen);

    static void OnServerRecvData(void* userdata,int hSocket,char* szData,int iLen);

    void DoData(char* czData,int iLen);
};

#endif // CCOMM_H
