#include "tcpclient.h"
#include <pthread.h>
#include <iostream>
using namespace std;

tcpclient::tcpclient(char* ip,int port)
{
    m_pOnConnected = NULL;
    m_ip = ip;
    m_port = port;
    bConnect = false;
    id_recv = 0;
    bExitRecv = false;
}

tcpclient::~tcpclient()
{
}

void tcpclient::Init()
{
    pthread_create(&id_recv, NULL, ReceiveData, this);
}
void tcpclient::UnInit()
{
    bExitRecv = true;
    shutdown(hSocket,SHUT_RDWR);
    close(hSocket);
    cout << "tcpclient::UnInit()\n" << endl;
    pthread_join(id_recv,0);
}

void tcpclient::SetOnConnected(void* userdata,OnConnected* pOnConnected)
{
    m_userdata = userdata;
    m_pOnConnected = pOnConnected;
}

bool tcpclient::Connect()
{
    struct sockaddr_in  servaddr;

    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( hSocket < 0)
    {
        perror("socket error");
        return false;
    }
    //struct timeval interval;
    //int iTimeOut = 10;
    // interval.tv_sec = iTimeOut;
    // interval.tv_usec = (iTimeOut > 0)?0:10;
    // setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, &interval, sizeof(interval));

   // int dwFlag = 0;
   // ioctlscoekt(hSocket,FIONBIO, &dwFlag);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(m_ip);
    servaddr.sin_port = htons (m_port);

    if(bExitRecv)return false;

     if (connect(hSocket, (struct sockaddr *) &servaddr, sizeof(servaddr)) == 0)
     {
        bConnect = true;
        if(m_pOnConnected)
            m_pOnConnected(m_userdata,true);
     }
     else
     {
         close(hSocket);
     }
    return bConnect;
}

bool tcpclient::SendMsg(const char* czValue,int iLen)
{
   // if(bConnect)
    int iRT = send(hSocket,(const char*)czValue,iLen, 0);
    return (iLen == iRT);
   // else
   //     return false;
}

void* tcpclient::ReceiveData( void* pParam)
{
  //  pthread_detach( pthread_self() );

    tcpclient* pThis = (tcpclient*)pParam;

    //首次通知连接是断开的
    if( pThis->m_pOnConnected)
        pThis->m_pOnConnected( pThis->m_userdata,false);

    char* czBuf = new char[1024];
    while(!pThis->bExitRecv)
    {
       // cout << " while(!pThis->bExitRecv).\n" << endl;
        if(pThis->bConnect)
        {
            memset(czBuf,0,1024);
            int iRecvLen = recv(pThis->hSocket,czBuf,1024,0);
            if(iRecvLen <= 0)
            {
                cout << "recv err.\n" << endl;
               // shutdown(pThis->hSocket,SHUT_RDWR);
                close(pThis->hSocket);
                pThis->bConnect = false;
                if(pThis->m_pOnConnected)
                    pThis->m_pOnConnected(pThis->m_userdata,false);
            }
            else
            {
                pThis->OnReceive(czBuf,iRecvLen);
                //cout << " pThis->OnReceive(czBuf).\n" << endl;
            }
        }
        else
        {
            pThis->Connect();
        }
        sleep(1);
    }
    delete []czBuf;
    close(pThis->hSocket);
    cout << "tcpclient::ReceiveData finished.\n" << endl;
    return NULL;
}
