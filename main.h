#ifndef MAIN_H_
#define MAIN_H_



#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/time.h>
#include <time.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
//#include <iostream.h>


#define COM1 "/dev/sda1"
#define COM2 "/dev/sda1"

#define  	BAUDRATE	115200
#define 	DATABITS	8
#define		PARITY		'N'
#define		STOPBITS	1
#define   MAX_SEND_SIZE  134

#define  UCHAR unsigned char 
using namespace std;

struct COM_T
{
    int fd;		
    int length;			
    UCHAR data[MAX_SEND_SIZE];	
}com1, com2;


struct date
{
	unsigned char  year;
	unsigned char  mon;
	unsigned char  day;
	unsigned char  hour;
	unsigned char  min;
	unsigned char  sec;
}nowTime;
#endif /* MAIN_H_ */
