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
#ifndef sqlite_dbH
#define sqlite_dbH
//---------------------------------------------------------------------------
#include "sqlite3.h"
#include <string.h>
#include <pthread.h>

//---------------------------------------------------------------------------
#define MAX_DBNAME      255
//---------------------------------------------------------------------------
class  sqlite_db;
struct sqlite_data
{
	public:
        sqlite_data();
		sqlite_data(const sqlite_data &X);
		~sqlite_data();

		void Clone(sqlite_data &X);

		const char* operator()(int Row, int Col);
		const char* GetData(int Row, int Col);

		void Free();
	public:
		int RowCount();
		int ColCount();
		bool Active();
		bool IsEmpty();
	private:
		friend class sqlite_db;

		char** m_data;
     
		int m_RowCount;
		int m_ColCount;

};
//---------------------------------------------------------------------------
class  sqlite_db
{
	public:
		sqlite_db(const char* dbName);
		~sqlite_db();

		bool Open(const char* dbName);
		void Close();
		bool ExecSQL(const char* SQL);
		int Query(sqlite_data &ResultData, const char* SQL);
		int Query(const char* SQL);
	public:
		bool Connected();
		const char* DataBaseName();
		const char* Version();
	public:
		sqlite_data Data;
		void Free();
		bool IsEmpty();
		bool Active();
		int RowCount();
		int ColCount();
	private:
		sqlite3 *m_db;
		char m_dbName[MAX_DBNAME];
//      static int sqlite_callback(void *param,int colcount,char **cols,char **colnames);
        pthread_mutex_t ReadLock;
        pthread_mutex_t WriteLock;

};
//---------------------------------------------------------------------------
static int sqlite_callback(void *param,int colcount,char **cols,char **colnames);
//---------------------------------------------------------------------------
#endif
