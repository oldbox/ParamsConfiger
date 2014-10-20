/***************************************************************************
 *   Copyright (C) 2007 by 武君贤   *
 *   tech@lhren.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#pragma hdrstop

#include "cdatabase.h"
#include "stdio.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
sqlite_data::sqlite_data()
{
    m_data = NULL;
    m_RowCount = 0;
    m_ColCount = 0;
}
//---------------------------------------------------------------------------
//拷贝构造不能复制,可以用Clone方法复制
sqlite_data::sqlite_data(const sqlite_data &X)
{
    m_data = NULL;
    m_RowCount = 0;
    m_ColCount = 0;
}
//---------------------------------------------------------------------------
sqlite_data::~sqlite_data()
{
    Free();
}
//---------------------------------------------------------------------------
//复制一个结果集，待完善
void sqlite_data::Clone(sqlite_data &X)
{
    m_RowCount = X.m_RowCount;
    m_ColCount = X.m_ColCount;
    
    //    m_data
    
}
//---------------------------------------------------------------------------
//重载(),取得结果
const char* sqlite_data::operator()(int Row, int Col)
{
    return GetData(Row, Col);
}
//---------------------------------------------------------------------------
//取得某行某列结果，不存在则返回NULL
const char* sqlite_data::GetData(int Row, int Col)
{
    if(Row > m_RowCount || Col > m_ColCount || Col == 0)
	return NULL;
    else
	return m_data[m_ColCount*Row+Col-1];
}
//---------------------------------------------------------------------------
//释放结果集
void sqlite_data::Free()
{
    if(NULL != m_data)
    {
	sqlite3_free_table(m_data);
	m_data = NULL;
	m_RowCount = 0;
	m_ColCount = 0;
    }
}
//---------------------------------------------------------------------------
//返回行数
int sqlite_data::RowCount()
{
    return m_RowCount;
}
//---------------------------------------------------------------------------
//返回列数
int sqlite_data::ColCount()
{
    return m_ColCount;
}
//---------------------------------------------------------------------------
//结果集是否打开
bool sqlite_data::Active()
{
    return (m_data == NULL);
}
//---------------------------------------------------------------------------
//结果集是否为空
bool sqlite_data::IsEmpty()
{
    return (m_RowCount == 0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
sqlite_db::sqlite_db(const char* dbName)
{
    pthread_mutex_init(&ReadLock,NULL);
    pthread_mutex_init(&WriteLock,NULL);
    m_db = NULL;
    Open(dbName);
}
//---------------------------------------------------------------------------
sqlite_db::~sqlite_db()
{
    Close();
    pthread_mutex_destroy(&ReadLock);
    pthread_mutex_destroy(&WriteLock);
}
//---------------------------------------------------------------------------
//返回SQLite的版本
const char* sqlite_db::Version()
{
    return SQLITE_VERSION;
}
//---------------------------------------------------------------------------
//返回打开的数据库名
const char* sqlite_db::DataBaseName()
{
    return m_dbName;
}
//---------------------------------------------------------------------------
//打开数据库
bool sqlite_db::Open(const char* dbName)
{
    if(Connected())
    {
	Close();
    }
    if(NULL == dbName)
    {
	return false;
    }
    if(strlen(dbName) >= MAX_DBNAME)
    {
	return false;
    }
    strcpy(m_dbName, dbName);
    int r = sqlite3_open(m_dbName,&m_db);
    if(SQLITE_OK == r)
    {
	return true;
    }
    else
    {
	Close();
	return false;
    }
    
}
//---------------------------------------------------------------------------
//关闭数据库
void sqlite_db::Close()
{
    Free();
    if(NULL != m_db)
    {
	sqlite3_close(m_db);
	m_db = NULL;
	memset(m_dbName, 0, MAX_DBNAME);
    }
}
//---------------------------------------------------------------------------
//是否打开数据库状态
bool sqlite_db::Connected()
{
    return (NULL != m_db);
}
//---------------------------------------------------------------------------
//执行查询
int sqlite_db::Query(const char* SQL)
{
    return  Query(Data, SQL);
}
//---------------------------------------------------------------------------
//执行查询
int sqlite_db::Query(sqlite_data &ResultData, const char* SQL)
{
    if(!Connected())
    {
	return -1;
    }
    if(NULL == SQL)
    {
	return -1;
    }
    
    ResultData.Free();
    
    char *pcErrMsg;          /* 返回错误信息*/
    pthread_mutex_lock(&ReadLock);
    int r = sqlite3_get_table(m_db, SQL, &ResultData.m_data, &ResultData.m_RowCount, &ResultData.m_ColCount, &pcErrMsg);
    pthread_mutex_unlock(&ReadLock);
    if(SQLITE_OK == r)
    {
	return ResultData.RowCount();
    }
    else
    {
	sqlite3_free(pcErrMsg);
	ResultData.Free();
	return  -1;
    }
}
//---------------------------------------------------------------------------
//执行SQL命令
bool sqlite_db::ExecSQL(const char* SQL)
{
    if(!Connected())
    {
	return false;
    }
    if(NULL == SQL)
    {
	return false;
    }
    
    char *pcErrMsg = NULL;
    int Row = 0;
    
    pthread_mutex_lock(&WriteLock);
    int r = sqlite3_exec(m_db, SQL, sqlite_callback, (void*)&Row, &pcErrMsg);
    pthread_mutex_unlock(&WriteLock);
    if(SQLITE_OK == r)
    {
	//return Row;
	return true;
    }
    else
    {
	sqlite3_free(pcErrMsg);
	return false;
    }
    
}
//---------------------------------------------------------------------------
//回调函数
int sqlite_callback(void *param,int colcount,char **cols,char **colnames)
{
    return 0;
}
//---------------------------------------------------------------------------
void sqlite_db::Free()
{
    Data.Free();
}
//---------------------------------------------------------------------------
bool sqlite_db::IsEmpty()
{
    return Data.IsEmpty();
}
//---------------------------------------------------------------------------
bool sqlite_db::Active()
{
    return Data.Active();
}
//---------------------------------------------------------------------------
int sqlite_db::RowCount()
{
    return Data.RowCount();
}
//---------------------------------------------------------------------------
int sqlite_db::ColCount()
{
    return Data.ColCount();
}
