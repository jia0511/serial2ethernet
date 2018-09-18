#include "config.h"
#include "netcom.h"
#include "seriom.h"

#define COM1 "/dev/sda1"
#define COM2 "/dev/sda1"

#define  	BAUDRATE	115200
#define 	DATABITS	8
#define		PARITY		'N'
#define		STOPBITS	1

struct COM_T
{
    int fd;		
    int length;			
    char data[134];	
}com1, com2;
