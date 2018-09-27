#include "main.h"
#include "config.h"
#include "netcom.h"
#include "sericom.h"
#include<time.h>

void getNowTime()
{	
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
 
    printf("%4d年%02d月%02d日 %02d:%02d:%02d\n", 
	t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	nowTime.year = (t->tm_year + 1900) -2000;
	nowTime.mon = t->tm_mon + 1;
	nowTime.day = t->tm_mday;
	nowTime.hour = t->tm_hour;
	nowTime.min = t->tm_min;
	nowTime.sec = t->tm_sec;
}

void SetSysTimeForWeb(char* datetime)
{
	if(NULL == datetime ) {
		return ;
	}
	//获取系统时间
	time_t now_time;
	now_time=time(NULL);
	//时间同步到硬件
	strftime(datetime,sizeof(datetime),
	"date %m%d%H%M20%y",
	localtime(&now_time));
	system(datetime);
	system("hwclock -w");
	printf("systemtime success!\n");
}

void SerialCOMInit(void)
{
	com1.fd = open_port(COM1);
	if(set_com_config(com1.fd,BAUDRATE,DATABITS,PARITY,STOPBITS) < 0) {
		perror("sync COM1 init err!");
	} else {
		printf("Init COM1 OK!\n");
	}  
	com2.fd = open_port(COM2);
	if(set_com_config(com2.fd,BAUDRATE,DATABITS,PARITY,STOPBITS) < 0) {
		perror("sync COM2 init err!");
	} else {
		printf("Init COM2 OK!\n");
	}
}


void ReadSerialCOM(void) 
{
	unsigned char buff[5];
	//一个字节一个字节的读取直到换行
	memset(buff, 0, sizeof(buff));
	if(read(com1.fd,buff,5) > 0 )
	{
		//查找关键字符
		if(buff[0] == 0xEE && buff[1] == 0xEE 
		&& buff[2] == 0xEE && buff[3] == 0xAA)
		{
			//数据处理
			com1.length = buff[4];
			memset(com1.data, 0, MAX_SEND_SIZE);
			if(read(com1.fd, com1.data, com1.length +1) > 0 )
			{
				if(com1.data[com1.length] == 0xFE )
				{
					getNowTime();//update now time
					com1.data[com1.length] = nowTime.year;
					com1.data[com1.length +1] = nowTime.mon;
					com1.data[com1.length +2] = nowTime.day;
					com1.data[com1.length +3] = nowTime.hour;
					com1.data[com1.length +4] = nowTime.min;
					com1.data[com1.length +5] = nowTime.sec;
				}
			}
		}//end of if
	}//end of if = read
/*
	while(1)
	{
		//一个字节一个字节的读取直到换行
		memset(buff, 0, sizeof(buff));
		memset(temp, 0, sizeof(temp));
		if(read(temp,buff,1) > 0 )
		{			
			strcat(temp,buff);//将读取数据存入缓存
			if(buff[0]=='\n')
			{	
				//查找关键字符
				if(temp[0] == 0xEE && temp[1] == 0xEE 
				&& temp[2] == 0xEE && temp[3] == 0xAA)
				{
					//数据处理
					com2.length = temp[4];
					memset(com1.data, 0x00, MAX_SEND_SIZE);
					memcpy(com2.data,&temp[5],com2.length);
					//strcat(com2.data,current);
					break;
				}//end of if
			}//end of if = buff
		}//end of if = read
	}//end of if while
*/
}

void initFunction()
{

	SerialCOMInit();
	ReadConfigData(); //读取配置文件
}

//根据公里标输出路口方向
void* OutPutRoadDirection(void* )
{
	while (1)
	{
		SendtoServer(com1.data, com1.length + 6);
		//SendtoServer(com2.data, com2.length + 6);
		usleep(1000*10);
	}
}


int main(int argc, char *argv[])
{
	printf("hello world\n");
	pthread_t tid;//与TOD通信线程
	pthread_create(&tid, NULL, OutPutRoadDirection, NULL);

	while(1){

	 	ReadSerialCOM();
		usleep(1000*10); 
	}
	return 0;
}

//g++ config.cpp main.cpp netcom.cpp sericom.cpp -o test -lpthread -std=c++11 

