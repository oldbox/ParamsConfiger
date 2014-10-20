#ifndef TCPSERVER_H
#define TCPSERVER_H


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>

#define BACKLOG 5               //listen队列中等待的连接数
#define MAXDATASIZE 1024        //缓冲区大小

//回调处理数据函数原型
typedef void OnTcpSververRecvData(void* userdata,int hSocket,char* szData,int iLen);

typedef struct _CLIENT
{
    int fd;                     //客户端socket描述符
   // char name[20];              //客户端名称
    struct sockaddr_in addr;    //客户端地址信息结构体
    char data[MAXDATASIZE];     //客户端私有数据指针
} CLIENT;

class tcpserver
{
public:
    tcpserver(int iPort);
    ~tcpserver();

    bool SendMsg(int hSocket,char* data,int len);
    void SendMsgToAllClient(char* data,int len);
    void SetOnTcpSververRecvData(void* userdata,OnTcpSververRecvData* pOnTcpSververRecvData);

private:
    static void* Init(void* pParam);
    void process_client(CLIENT * client, char *recvbuf, int len);

    pthread_t id_init;
    bool bExit;
    int listenfd;
    CLIENT client[FD_SETSIZE];  //FD_SETSIZE为select函数支持的最大描述符个数
    int m_iPort;
    OnTcpSververRecvData* m_pOnTcpSververRecvData;
    void* m_userdata;
};

#endif // TCPSERVER_H
