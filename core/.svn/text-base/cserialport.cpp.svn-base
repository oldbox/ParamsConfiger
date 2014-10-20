/***************************************************************************
 *   Copyright (C) 2006 by 武君贤   *
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

#include "cserialport.h"
#include  <fcntl.h>
#include  <unistd.h>

/*
 * Decription for TIMEOUT_SEC(buflen,baud);
 * baud bits per second, buflen bytes to send.
 * buflen*20 (20 means sending an octect-bit data by use of the maxim bits 20)
 * eg. 9600bps baudrate, buflen=1024B, then TIMEOUT_SEC = 1024*20/9600+1 = 3 
 * don't change the two lines below unless you do know what you are doing.
*/
#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)
#define TIMEOUT_USEC 0

#define BUFFER_LEN	1024    /* sendfile() */

CSerialPort::CSerialPort()
{
    m_pOnComData = NULL;
    pthread_mutex_init(&comlock,NULL);
    printf("new serial port\n");
}

/* Open serial port ComPort at baudrate baud rate. */
int CSerialPort::OpenComPort (int ComPort, int baudrate, int databit,
                              const char *stopbit, char parity)
{
    const char* acComPort;
    int           retval;

    printf("port=%d,baudrate=%d,databit=%d,stopbit=%s,parity=%C\n", ComPort, baudrate, databit,stopbit,parity);
    switch (ComPort)
    {
    case 0:
        acComPort = "/dev/ttyUSB0";
        break;
    case 1:
        acComPort = "/dev/ttycx0";
        break;
    case 2:
        acComPort = "/dev/ttycx1";
        break;
    case 3:
        acComPort = "/dev/ttycx4";
        break;
    case 4:
        acComPort = "/dev/ttycx5";
        break;
    default:
        acComPort = "/dev/ttyUSB0";
        break;
    }
    fd = open (acComPort, O_RDWR | O_NOCTTY |  O_NONBLOCK);
    if (-1 == fd)
    {
       // fprintf (stderr, "can not open port %s\n", acComPort);
        printf ("test can not open port %s\n", acComPort);
        perror("open:");
        return (-1);
    }
    tcgetattr (fd, &termios_old);       /* save old termios value */
    /* 0 on success, -1 on failure */
    retval = SetPortAttr (baudrate, databit, stopbit, parity);
    if (-1 == retval)
    {
        fprintf (stderr, "\nport %s cannot set baudrate at %d\n", acComPort,
                 baudrate);
    }

    return (retval);
}

CSerialPort::~CSerialPort()
{
    CloseComPort();

    //等待锁释放
    Lock();
    UnLock();
    pthread_mutex_destroy(&comlock);
}

void CSerialPort::SetpOnComData(OnComData* pOnComData,unsigned long userdata,int ComX)
{
    m_userdata = userdata;
    m_ComX = ComX;
    m_pOnComData = pOnComData;
}

/* close serial port by use of file descriptor fd */
void CSerialPort::CloseComPort ()
{
    /* flush output data before close and restore old attribute */
    tcsetattr (fd, TCSADRAIN, &termios_old);
    close (fd);
}

int CSerialPort::ReadComPort (void *data, int datalength)
{
    if(-1 == fd)
    {
        printf ("read comport fail \n");
        return 0;

    }
    int           retval = 0;
    FD_ZERO (&fs_read);
    FD_SET (fd, &fs_read);
    tv_timeout.tv_sec = TIMEOUT_SEC (datalength, GetBaudrate ());
    tv_timeout.tv_usec = TIMEOUT_USEC;   
    retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);
    if (retval)
    {
        int iLen = read (fd, data, datalength);
//        if(m_pOnComData)
//        {
//            //将原始包输出
//            m_pOnComData(m_userdata,true,data,datalength,m_ComX);
//        }
        return (iLen);
    }
    else
        return (-1);
}

/*
 * Write datalength bytes in buffer given by unsigned char *data,
 * return value: bytes written
 * Nonblock mode
*/
int CSerialPort::WriteComPort (unsigned char * data, int datalength)
{
    if(-1 == fd)
    {
        printf ("write comport fail \n");
        return 0;

    }
    int           retval, len = 0, total_len = 0;

    FD_ZERO (&fs_write);
    FD_SET (fd, &fs_write);
    tv_timeout.tv_sec = TIMEOUT_SEC (datalength, GetBaudrate ());
    tv_timeout.tv_usec = TIMEOUT_USEC;

    for (total_len = 0, len = 0; total_len < datalength;)
    {
	retval = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);
	if (retval)
	{
	    len = write (fd, &data[total_len], datalength - total_len);
	    if (len > 0)
	    {
		total_len += len;
	    }
	}
	else
	{
        tcflush (fd, TCOFLUSH);     /* flush all output data */
	    break;
	}
    }
//    if(m_pOnComData)
//    {
//        //将原始包输出
//        m_pOnComData(m_userdata,false,data,datalength,m_ComX);
//    }
    return (total_len);
}
void CSerialPort::TCFlush()
{
     tcflush (fd, TCIOFLUSH);     /* flush all output data */
}
/* get serial port baudrate */
int CSerialPort::GetBaudrate ()
{
    return (_BAUDRATE (cfgetospeed (&termios_new)));
}

/* set serial port baudrate by use of file descriptor fd */
void CSerialPort::SetBaudrate (int baudrate)
{
    termios_new.c_cflag = BAUDRATE (baudrate);  /* set baudrate */
}

void CSerialPort::SetDataBit (int databit)
{
    termios_new.c_cc[VMIN]     = 1;  
    
    termios_new.c_cflag &= ~CSIZE;
    switch (databit)
    {
    case 8:
        termios_new.c_cflag |= CS8;
        break;
    case 7:
        termios_new.c_cflag |= CS7;
        break;
    case 6:
        termios_new.c_cflag |= CS6;
        break;
    case 5:
        termios_new.c_cflag |= CS5;
        break;
    default:
        termios_new.c_cflag |= CS8;

        break;
    }
}

void CSerialPort::SetStopBit (const char *stopbit)
{
    if (0 == strcmp (stopbit, "1"))
    {
        termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
    }
    else if (0 == strcmp (stopbit, "1.5"))
    {
        termios_new.c_cflag &= ~CSTOPB; /* 1.5 stop bits */
    }
    else if (0 == strcmp (stopbit, "2"))
    {
        termios_new.c_cflag |= CSTOPB;  /* 2 stop bits */
    }
    else
    {
        termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
    }
}

void CSerialPort::SetParityCheck (char parity)
{
    switch (parity)
    {
    case 'N':                  /* no parity check */
        termios_new.c_cflag &= ~PARENB;
        break;
    case 'E':                  /* even */
        termios_new.c_cflag |= PARENB;
        termios_new.c_cflag &= ~PARODD;
        break;
    case 'O':                  /* odd */
        termios_new.c_cflag |= PARENB;
        termios_new.c_cflag |= ~PARODD;
        break;
    default:                   /* no parity check */
        termios_new.c_cflag &= ~PARENB;
        break;
    }
}

int CSerialPort::SetPortAttr (int baudrate,
                              int databit, const char *stopbit, char parity)
{
    bzero (&termios_new, sizeof (termios_new));
    cfmakeraw (&termios_new);
    SetBaudrate (baudrate);
    termios_new.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */
    SetDataBit (databit);
    SetParityCheck (parity);
    SetStopBit (stopbit);
    termios_new.c_oflag = 0;
    termios_new.c_lflag |= 0;
    termios_new.c_oflag &= ~OPOST;
    termios_new.c_cc[VTIME] = 1;        /* unit: 1/10 second. */
    termios_new.c_cc[VMIN] = 1; /* minimal characters for reading */
    tcflush (fd, TCIFLUSH);
    return (tcsetattr (fd, TCSANOW, &termios_new));
}

int CSerialPort::BAUDRATE (int baudrate)
{
    switch (baudrate)
    {
    case 0:
        return (B0);
    case 50:
        return (B50);
    case 75:
        return (B75);
    case 110:
        return (B110);
    case 134:
        return (B134);
    case 150:
        return (B150);
    case 200:
        return (B200);
    case 300:
        return (B300);
    case 600:
        return (B600);
    case 1200:
        return (B1200);
    case 2400:
        return (B2400);
    case 9600:
        return (B9600);
    case 19200:
        return (B19200);
    case 38400:
        return (B38400);
    case 57600:
        return (B57600);
    case 115200:
        return (B115200);
    default:
        return (B9600);
    }
}

int CSerialPort::_BAUDRATE (int baudrate)
{
    /* reverse baudrate */
    switch (baudrate)
    {
    case B0:
        return (0);
    case B50:
        return (50);
    case B75:
        return (75);
    case B110:
        return (110);
    case B134:
        return (134);
    case B150:
        return (150);
    case B200:
        return (200);
    case B300:
        return (300);
    case B600:
        return (600);
    case B1200:
        return (1200);
    case B2400:
        return (2400);
    case B9600:
        return (9600);
    case B19200:
        return (19200);
    case B38400:
        return (38400);
    case B57600:
        return (57600);
    case B115200:
        return (115200);
    default:
        return (9600);
    }
}

void CSerialPort::Lock()
{
    pthread_mutex_lock(&comlock);
}
void CSerialPort::UnLock()
{
    pthread_mutex_unlock(&comlock);
}
