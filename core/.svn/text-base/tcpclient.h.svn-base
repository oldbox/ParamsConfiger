#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

//回调处理数据函数原型
typedef void OnConnected(void* userdata,bool bConnect);

class tcpclient
{
    public:
        tcpclient(char* ip,int port);
        virtual ~tcpclient();
        bool SendMsg(const char* czValue,int iLen);
        void SetOnConnected(void* userdata,OnConnected* pOnConnected);
        void Init();
        void UnInit();

    protected:
        int hSocket;
        virtual void OnReceive(char* czBuf,int iLen) = 0;
        bool bExitRecv;
        bool bConnect;

    private:
        bool Connect();
        static void* ReceiveData(void* pParam);
        pthread_t id_recv;
        char* m_ip;
        int m_port;
        OnConnected* m_pOnConnected;
        void* m_userdata;
};

#endif // TCPCLIENT_H
