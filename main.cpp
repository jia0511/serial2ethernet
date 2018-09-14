#include "main.h"


//根据公里标输出路口方向
void OutPutRoadDirection(void)
{
	while (1)
	{
		SendtoServer(Direction, 2);
		sleep(1);
	}
}


int main(int argc, char *argv[])
{
	cout<<"hello world"<<endl;

	pthread_t tid;//与TOD通信线程
	pthread_create(&tid, NULL, (void*) OutPutRoadDirection, NULL);

	return 0;
}
