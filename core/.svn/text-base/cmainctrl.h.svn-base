#ifndef CMAINCTRL_H
#define CMAINCTRL_H

#include "./cserialport.h"
#include "./ccomm.h"
#include "./cyqs.h"
#include "./cmydatabase.h"
#include "./cconfig.h"
#include <QMessageBox>

class CMainCtrl:public QObject
{
    Q_OBJECT
public:
    CMainCtrl();
    virtual ~CMainCtrl();

    CComm* pComm;//通信对象
    CYQs* pYQs;//仪器对象组

     void Init();//供外面使用

     bool SendCmd(int nCmd,QString qsYQID = "");
     stuMainCtrl m_stMainCtrl;
private:
    void InitIniItem();

    CSerialPort* pSerialPorts[COMCOUNT];
    //串口数据
    static void OnCMainCtrlComData(unsigned long userdata,bool bRead,void* czData,int iLen,int ComX);
    //网络数据
   // static void OnNetData(unsigned long userdata,void* czData,int iLen);

    pthread_t id_autorun;
    bool bExit;
    static void* AutoRun(void* pParam);
    void Run();
    void ExitRun();

    void DoMod1();//周期测试
    void DoMod2();//定点测试
    void DoMod3();//自动电极维护
    void DoMod4();//自动核查，自动标样校准，自动空白校准
    void SendInit();
    void GetIniInit();
    bool m_bTestFlag[24];
    int m_nYQType;
    int m_nLastHour;
    bool m_nLastMod;
    int m_nCount;//二氧化硅记录发命令的次数

signals:
    void NotifyNextTestTime(QDateTime);
    void NotifyComData(bool,void*,int,int);
};

#endif // CMAINCTRL_H
