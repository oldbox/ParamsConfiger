#include "cconfig.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstddef>
#include <stdlib.h>

int CConfig::m_nUserType = 0;
CConfig::CConfig(const char* childPath,const char* filename)
{
    pthread_mutex_init(&lock,NULL);
    //将两个参数保存起来
    memset(m_ChilePath,0,256);
    if(childPath)strcpy(m_ChilePath,childPath);
    memset(m_FileName,0,256);
    strcpy(m_FileName,filename);

    //格式化路径
    char czPath[256];memset(czPath,0,256);
    if(childPath)
        sprintf(czPath,"%s/%s",INI_ROOT_PATH,childPath);
    else
        sprintf(czPath,"%s",INI_ROOT_PATH);
    //级联创建路径
    mkdir_r(czPath);
    memset(m_PathName,0,256);
    sprintf(m_PathName,"%s/%s",czPath,filename);
    //ini.Create(m_PathName);
    pINI = new minIni(m_PathName);
}
CConfig::~CConfig()
{
    pthread_mutex_destroy(&lock);
    delete pINI;
    FlushFile();

}
void CConfig::FlushFile()
{
    //写入flush
    FILE *pFile = NULL;
    int iFd;

    pFile = fopen(m_PathName, "ab");
    if (NULL == pFile)
    {
            return;
    }
    iFd = fileno(pFile);
    fsync(iFd);
    fclose(pFile);
}

void CConfig::SetIni(const char* group,const char* item,const char* value)
{

    pthread_mutex_lock(&lock);
    pINI->put(group,item,value);
   // ini.SetVar(group,item,value);
    //ini.Save(m_PathName);
    //write_profile_string(group,item,value,m_PathName);
    //IniFile::write_profile_string(group,item,value,m_PathName);
  //  ini_puts(group,item,value,m_PathName);
    pthread_mutex_unlock(&lock);
    //FlushFile();
}

bool CConfig::GetIni(const char* group,const char* item,char* value)
{
    pthread_mutex_lock(&lock);
    strcpy(value,pINI->gets(group,item).c_str());
    pthread_mutex_unlock(&lock);
    if(strlen(value)>0)
        return true;
    return false;

//    string str;
//    bool brt = ini.GetVar(group,item,str);
//    strcpy(value,str.c_str());
//    return brt;

    //return read_profile_string(group,item,value,64,"0",m_PathName);
   // return IniFile::read_profile_string(group,item,value,64,"0",m_PathName);
   // return ini_gets(group,item,"0",value,64,m_PathName);
   // return false;

}
bool CConfig::DelIni(const char* group)
{
    if(pINI->del(group))
       return true;
    return false;
}

//删除ini组中的项
bool CConfig::DelItem(const char* group,const char* item)
{
    if(pINI->del(group,item))
       return true;
    return false;
}

int CConfig::mkdir_r(const char *path) {

        if (path == NULL) {

                return -1;

        }

        char *temp = strdup(path);

        char *pos = temp;



        /* 去掉开头的 './' 或 '/' */

        if (strncmp(temp, "/", 1) == 0) {

                pos += 1;

        } else if (strncmp(temp, "./", 2) == 0) {

                pos += 2;

        }



        /* 循环创建目录 */

        for ( ; *pos != '\0'; ++ pos) {

                if (*pos == '/') {

                        *pos = '\0';

                        mkdir(temp, 0644);

                        //printf("for %s\n", temp);

                        *pos = '/';

                }

        }



        /* 如果最后一级目录不是以'/'结尾，

        遇到'\0'就中止循环，

        不会创建最后一级目录 */

        if (*(pos - 1) != '/') {

                //printf("if %s\n", temp);

                mkdir(temp, 0644);

        }

        free(temp);

        return 0;

}
int CConfig::GetAfxUserType()
{
    return m_nUserType;
}
//0游客 1普通用户 2管理员 3超级用户
void CConfig::setAfxUserType(int nUserType)
{
    m_nUserType = nUserType;
}
