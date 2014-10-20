#include "gbtcpclient.h"
#include <pthread.h>

#include <iostream>
using namespace std;

gbtcpclient::gbtcpclient(char* ip,int port,char* MN,char* PW):tcpclient(ip,port)
{
    //ctor
    m_mn = MN;
    m_pw = PW;
    m_pOnRecvData = NULL;
    pthread_create(&id_autocheck, NULL, AutoCheck, this);
}

gbtcpclient::~gbtcpclient()
{
    //dtor
    pthread_join(id_autocheck,0);
    cout << "gbtcpclient::~gbtcpclient().\n" << endl;
}

 void gbtcpclient::SetOnRecvData(void* userdata,OnRecvData* pOnRecvData)
 {
     m_userdata = userdata;
     m_pOnRecvData = pOnRecvData;
 }

//组装协议包
void gbtcpclient::OnReceive(char* czBuf, int iLen)
{
    if(m_pOnRecvData)
    {
        m_pOnRecvData(m_userdata,czBuf,iLen);
    }
}

void* gbtcpclient::AutoCheck(void* pParam)
{
    gbtcpclient* pThis = (gbtcpclient*)pParam;
    char czCot[1024],czSend[1024];
     sleep(5);
    while(!pThis->bExitRecv)
    {
        if(pThis->bConnect)
        {
            cout << "自检" << endl;
           // ##%04d%s%s\r\n
            memset(czCot,0,1024);
            memset(czSend,0,1024);
            sprintf(czCot,"ST=91;CN=9021;PW=%s;MN=%s;Flag=1;CP=&&&&",pThis->m_pw,pThis->m_mn);
            sprintf(czSend,"##%04d%sFFFF\r\n",strlen(czCot),czCot);
           // printf("%s\n",czSend);
            if( !pThis->SendMsg(czSend,strlen(czSend)))
            {
                shutdown(pThis->hSocket,SHUT_RDWR);
                close(pThis->hSocket);
                cout << "自检掉线了" << endl;
            }
            else
            {
                cout << "自检正常" <<endl;
            }
        }
        for(int i=0;i<60;i++)
        {
            if(!pThis->bExitRecv)
                sleep(1);
            else
                break;
        }
    }
    cout << "gbtcpclient AutoCheck finished.\n" << endl;
    return NULL;
}
