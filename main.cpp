#include "main.h"
#include "config.h"
#include "netcom.h"
#include "sericom.h"
#include<time.h>

void getNowTime(char* getcurtime)
{
	char current[100];
	memset(current,0x00,sizeof(current));
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    printf("%4d年%02d月%02d日 %02d:%02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	sprintf(current,"%02d%02d%02d%02d%02d%02d",(t->tm_year + 1900) -2000, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	printf("cu:%s\n",current);
	return current;
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
	char buff[2];
	char temp[MAX_SEND_SIZE];
	while(1)
	{
		//一个字节一个字节的读取直到换行
		memset(buff, 0, sizeof(buff));
		memset(temp, 0, sizeof(temp));
		if(read(com1.fd,buff,1) > 0 )
		{			
			strcat(temp,buff);//将读取数据存入缓存
			if(buff[0]=='\n')
			{	
				//查找关键字符
				if(temp[0] == 0xEE && temp[1] == 0xEE 
				&& temp[2] == 0xEE && temp[3] == 0xAA)
				{
					//数据处理
					com1.length = temp[4];
					memset(com1.data, 0x00, MAX_SEND_SIZE);
					memcpy(com1.data,&temp[5],com1.length);
					break;
				}//end of if
			}//end of if = buff
		}//end of if = read
	}//end of if while

	while(1)
	{
		//一个字节一个字节的读取直到换行
		memset(buff,0,sizeof(buff));
		if(read(com2.fd,buff,1) > 0 )
		{			
			strcat(com2.data,buff);//将读取数据存入缓存
			if(buff[0]=='\n')
			{	
				//查找关键字符
				if(com2.data[0] == 0xEE && com2.data[1] == 0xEE 
				&& com2.data[3] == 0xEE && com2.data[4] == 0xAA)
				{
					//数据处理
					com2.length = com2.data[5];
	
					bzero(com2.data,sizeof(com2.data));
					break;
				}//end of if
			}//end of if = buff
		}//end of if = read
	}//end of if while

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
		//SendtoServer(Direction, 2);
		usleep(1000*100);
	}
}


int main(int argc, char *argv[])
{
	//cout<<"hello world"<<endl;
getNowTime();
	pthread_t tid;//与TOD通信线程
//	pthread_create(&tid, NULL, OutPutRoadDirection, NULL);

	return 0;
}

//g++ config.cpp main.cpp netcom.cpp sericom.cpp -o test -lpthread -std=c++11 

