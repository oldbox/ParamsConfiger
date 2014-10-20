#include "tcpserver.h"
#include <pthread.h>

tcpserver::tcpserver(int iPort)
{
    m_pOnTcpSververRecvData = NULL;
    m_iPort = iPort;
    bExit = false;
    pthread_create(&id_init, NULL, Init, this);
}
tcpserver::~tcpserver()
{
    bExit = true;
    close(listenfd);//关闭服务器监听socket
    for(int i=0;i<FD_SETSIZE;i++)
    {
        shutdown(client[i].fd,SHUT_RDWR);
        close(client[i].fd);
        client[i].fd = -1;
    }
    pthread_join(id_init,0);
}


void* tcpserver::Init(void* pParam)
{
    tcpserver* pThis = (tcpserver*)pParam;
    int i, maxi, maxfd;//, sockfd;
    int nready;
    ssize_t n;
    fd_set rset, allset;        //select所需的文件描述符集合
    int connectfd;    //socket文件描述符
    struct sockaddr_in server;  //服务器地址信息结构体

    char recvbuf[MAXDATASIZE];  //缓冲区
    int sin_size;               //地址信息结构体大小

    if ((pThis->listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {                           //调用socket创建用于监听客户端的socket
        perror("Creating socket failed.");
        return NULL;
    }

    int opt = SO_REUSEADDR;
    setsockopt(pThis->listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  //设置socket属性

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(pThis->m_iPort);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(pThis->listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {                           //调用bind绑定地址
        perror("Bind error.");
        return NULL;
    }

    if (listen(pThis->listenfd, BACKLOG) == -1)
    {                           //调用listen开始监听
        perror("listen() error\n");
        return NULL;
    }

    //初始化select
    maxfd = pThis->listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
    {
        pThis->client[i].fd = -1;
    }
    FD_ZERO(&allset);           //清空
    FD_SET(pThis->listenfd, &allset);  //将监听socket加入select检测的描述符集合

    struct sockaddr_in addr;

    timeval tim;
    tim.tv_sec = 1;   //秒
    tim.tv_usec = 0;  //微秒，请注意不是毫秒。

    while (!pThis->bExit)
    {
        usleep(10000);
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, &tim);    //调用select
        if( 0 == nready)//超时
            continue;        

        if (FD_ISSET(pThis->listenfd, &rset))
        {                       //检测是否有新客户端请求
            printf("Accept a connection.\n");
            //调用accept，返回服务器与客户端连接的socket描述符
            sin_size = sizeof(struct sockaddr_in);
            if ((connectfd =
                 accept(pThis->listenfd, (struct sockaddr *)&addr, (socklen_t *) & sin_size)) == -1)
            {
                perror("Accept() error\n");
                continue;
            }

            //将新客户端的加入数组
            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (pThis->client[i].fd < 0)
                {
                    pThis->client[i].fd = connectfd;   //保存客户端描述符
                    pThis->client[i].addr = addr;
                    break;
                }
            }

            if (i == FD_SETSIZE)
                printf("Too many clients\n");
            FD_SET(connectfd, &allset); //将新socket连接放入select监听集合
            if (connectfd > maxfd)
                maxfd = connectfd;  //确认maxfd是最大描述符
            if (i > maxi)       //数组最大元素值
                maxi = i;
            if (--nready <= 0)
                continue;       //如果没有新客户端连接，继续循环
        }

        for (i = 0; i <= maxi; i++)
        {
            if (pThis->client[i].fd < 0)    //如果客户端描述符小于0，则没有客户端连接，检测下一个
                continue;
            // 有客户连接，检测是否有数据
            if (FD_ISSET(pThis->client[i].fd, &rset))
            {
                printf("Receive from connect fd[%d].\n", i);
                memset(recvbuf,0,MAXDATASIZE);
                if ((n = recv(pThis->client[i].fd, recvbuf, MAXDATASIZE, 0)) <= 0)
                {               //从客户端socket读数据，等于0表示网络中断
                    close(pThis->client[i].fd);  //关闭socket连接
                    //printf("%s closed. User's data: %s\n", pThis->client[i].name, pThis->client[i].data);
                    FD_CLR(pThis->client[i].fd, &allset);    //从监听集合中删除此socket连接
                    pThis->client[i].fd = -1;  //数组元素设初始值，表示没客户端连接
                }
                else
                {
                    pThis->process_client(&pThis->client[i], recvbuf, n); //接收到客户数据，开始处理
                }
                if (--nready <= 0)
                    break;      //如果没有新客户端有数据，跳出for循环回到while循环
            }
        }
    }    
}

void tcpserver::process_client(CLIENT * client, char *recvbuf, int len)
{
    //SendMsg(client->fd, recvbuf, len);
    //SendMsgToAllClient(recvbuf,len);
    if(m_pOnTcpSververRecvData)
    {
        m_pOnTcpSververRecvData(m_userdata,client->fd,recvbuf,len);
    }
}

void tcpserver::SetOnTcpSververRecvData(void* userdata,OnTcpSververRecvData* pOnTcpSververRecvData)
{
    m_userdata = userdata;
    m_pOnTcpSververRecvData = pOnTcpSververRecvData;
}

bool tcpserver::SendMsg(int hSocket,char* data,int len)
{
    return (send(hSocket,data,len, 0) == len);
}

void tcpserver::SendMsgToAllClient(char* data,int len)
{
    for(int i=0;i<FD_SETSIZE;i++)
    {
        if(client[i].fd > 0)
            SendMsg(client[i].fd,data,len);
    }
}
