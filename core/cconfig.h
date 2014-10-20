#ifndef CCONFIG_H
#define CCONFIG_H

#include "vardef.h"
#include "minIni.h"
//#include "IniFile.h"
#include "keyboard/myinputpanelcontext.h"
#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */

//#include "vardef.h"             /* int, INT16, INT8, Uint, UINT16, UINT8 */
#include <termios.h>            /* tcgetattr, tcsetattr */
//#include <sys/signal.h>
#include <pthread.h>
#include <QDesktopWidget>

//#define INIOBJCOUT 16
class CConfig
{
public:
    CConfig(const char* childPath,const char* filename);
    ~CConfig();

    void SetIni(const char* group,const char* item,const char* value);
    bool GetIni(const char* group,const char* item,char* value);
    bool DelIni(const char* group);
    bool DelItem(const char* group,const char* item);//删除组中的项
    char m_ChilePath[256];
    char m_FileName[256];
    void FlushFile();
    static void setAfxUserType(int nUserType);
    static int GetAfxUserType();

private:
   // INIFile ini;
    minIni* pINI;
    char m_PathName[256];
    pthread_mutex_t lock;
    int mkdir_r(const char *path);
    static int m_nUserType;
};

#define CF_MAX 16
static CConfig* pConfig[CF_MAX];


static void InitAfxConfig()
{
     for(int i=0;i<CF_MAX;i++)
     {
         pConfig[i] = NULL;
     }
}

static CConfig* GetAfxConfig(const char* childPath=NULL,const char* filename=ININAME)
{
    for(int i=0;i<CF_MAX;i++)
    {
        if(NULL == pConfig[i])
        {
            pConfig[i] = new CConfig(childPath,filename);
            return pConfig[i];
        }
        else
        {
            if(childPath)
            {
                if(strcmp(pConfig[i]->m_ChilePath,childPath)==0 && strcmp(pConfig[i]->m_FileName,filename)==0)
                {
                    return pConfig[i];
                }
            }
            else
            {
                if( strcmp(pConfig[i]->m_FileName,filename)==0)
                {
                    return pConfig[i];
                }
            }
        }
    }
    return NULL;
}

static void UnInitAfxConfig()
{
    for(int i=0;i<CF_MAX;i++)
    {
        if(pConfig[i])
            delete pConfig[i];
    }
}

static int GetAfxYQType()
{
    char tmp[64];memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("INFO","YQType",tmp)) GetAfxConfig()->SetIni("INFO","YQType","1");
    return atoi(tmp);
}
//仪器子类型,比如:挥发性有机物(0),和半挥发性(1)
////0:普通,氨氮(分光光度法)1:COD,高氯  总磷,活性磷 氨氮(电极法) 其它参数为污染源
static int GetAfxYQSubType()
{
    char tmp[64];memset(tmp,0,64);
    if(!GetAfxConfig()->GetIni("INFO","YQSubType",tmp)) GetAfxConfig()->SetIni("INFO","YQSubType","0");
    return atoi(tmp);
}

#include <iconv.h>
/* iInLen的长度不包括\0，应该用strlen。返回值是处理后的sOut长度 */
static int Utf8ToGb2312(char *sOut, int iMaxOutLen, const char *sIn, int iInLen)
{
    char *pIn = (char *)sIn;
    char *pOut = sOut;
    size_t ret;
    size_t iLeftLen=iMaxOutLen;
    iconv_t cd;

    cd = iconv_open("gb2312", "utf-8");
    if (cd == (iconv_t) - 1)
    {
        return -1;
    }
    size_t iSrcLen=iInLen;
    ret = iconv(cd, &pIn,&iSrcLen, &pOut,&iLeftLen);
    if (ret == (size_t) - 1)
    {
        iconv_close(cd);
        return -1;
    }

    iconv_close(cd);

    return (iMaxOutLen - iLeftLen);
}

#endif // CCONFIG_H
