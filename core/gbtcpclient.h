#ifndef GBTCPCLIENT_H
#define GBTCPCLIENT_H

#include "tcpclient.h"

//回调处理数据函数原型
typedef void OnRecvData(void* userdata,char* szData,int iLen);

class gbtcpclient : public tcpclient
{
    public:
        gbtcpclient(char* ip,int port,char* MN,char* PW);
        virtual ~gbtcpclient();
        void SetOnRecvData(void* userdata,OnRecvData* pOnRecvData);
    protected:
    private:
        void OnReceive(char* czBuf,int iLen);
        static void* AutoCheck(void* pParam);
        pthread_t id_autocheck;
        char* m_mn;
        char* m_pw;

        void* m_userdata;
        OnRecvData* m_pOnRecvData;
};

#endif // GBTCPCLIENT_H
