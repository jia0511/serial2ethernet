#include "main.h"

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
	while(1)
	{
		//一个字节一个字节的读取直到换行
		memset(buff,0,sizeof(buff));
		if(read(com1.fd,buff,1) > 0 )
		{			
			strcat(com1.data,buff);//将读取数据存入缓存
			if(buff[0]=='\n')
			{	
				//查找关键字符
				if(com1.data[0] == 0xEE && com1.data[1] == 0xEE 
				&& com1.data[3] == 0xEE && com1.data[4] == 0xAA)
				{
					//数据处理
					com1.length = com1.data[5];
					//ParseGPS_Data(gpscom1.data);
		
					//数据清空
					//print_info();//打印调试信息
					//memset(&GPS_DATA,0,sizeof(GPS_DATA));
					bzero(com1.data,sizeof(com1.data));
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
void OutPutRoadDirection(void)
{
	while (1)
	{
		//SendtoServer(Direction, 2);
		usleep(1000*100);
	}
}


int main(int argc, char *argv[])
{
	cout<<"hello world"<<endl;

	pthread_t tid;//与TOD通信线程
	pthread_create(&tid, NULL, (void*) OutPutRoadDirection, NULL);

	return 0;
}
