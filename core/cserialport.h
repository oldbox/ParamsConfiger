/***************************************************************************
 *   Copyright (C) 2006 by   *
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
#ifndef CSERIALPORT_H
#define CSERIALPORT_H

/**

*/
typedef void OnComData(unsigned long userdata,bool bRead,void* czData,int iLen,int ComX);
#include <stdio.h>              /* perror, printf, puts, fprintf, fputs */
#include <unistd.h>             /* read, write, close */
#include <fcntl.h>              /* open */

#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */

//#include "vardef.h"             /* int, INT16, INT8, Uint, UINT16, UINT8 */
#include <termios.h>            /* tcgetattr, tcsetattr */
//#include <sys/signal.h>
#include <pthread.h>

class CSerialPort
{
public:
    CSerialPort();
    ~CSerialPort();

    int OpenComPort(int ComPort, int baudrate, int databit,
                    const char *stopbit, char parity);
    void CloseComPort (void);
    int ReadComPort (void *data, int datalength);
    int WriteComPort (unsigned char * data, int datalength);
    //int SendFile (const char *pathname);

    void Lock();
    void UnLock();

    void SetpOnComData(OnComData* pOnComData,unsigned long userdata,int ComX);
    void TCFlush();
    

private:
    int		fd;             //File descriptor for the port
    struct	termios termios_old, termios_new;
    fd_set	fs_read, fs_write;
    struct	timeval tv_timeout;
    //struct	sigaction sigaction_io;   /* definition of signal action */

    //int		SetPortSig ();
    // static void	signal_handler_IO (int status);
    void	SetBaudrate (int);
    int		GetBaudrate ();
    void	SetDataBit (int databit);
    int		BAUDRATE (int baudrate);
    int		_BAUDRATE (int baudrate);
    int		SetPortAttr (int baudrate, int databit,
                      const char *stopbit, char parity);
    void	SetStopBit (const char *stopbit);
    void	SetParityCheck (char parity);

    pthread_mutex_t comlock;
    OnComData* m_pOnComData;
    unsigned long m_userdata;
    int m_ComX;
    
};

#endif
