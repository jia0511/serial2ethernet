#include "netcom.h"
#include "config.h"

extern netServer net_server;

int roadfd = -1;//路口句柄

void SetSocketOpt(int sfd)
{
	int keepAlive = 1; // 开启keepalive属性
	int keepIdle = 60; // 如该连接在60秒内没有任何数据往来,则进行探测
	int keepInterval = 5;// 探测时发包的时间间隔为10 秒
	int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发
	int reuseAddr = 1; //允许重复使用本地地址与套接字进行绑定

	//做TCP的探测及绑定本地地址
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
	setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
	setsockopt(sfd, SOL_TCP, TCP_KEEPIDLE, (void*) &keepIdle,sizeof(keepIdle));
	setsockopt(sfd, SOL_TCP, TCP_KEEPINTVL, (void*) &keepInterval,sizeof(keepInterval));
	setsockopt(sfd, SOL_TCP, TCP_KEEPCNT, (void *) &keepCount,sizeof(keepCount));
}


///////////////////////////与路口通信//////////////////
int ConnectRoad_TCP(char* ip,int port)
{
	printf("try TCP-connect Road-server...\n");

	int sfd = 0;
	signal(SIGPIPE,SIG_IGN);

	if((sfd = socket(AF_INET,SOCK_STREAM,0)) ==-1)
	{
		perror("socket error");
		close(sfd);
		sfd= -1;
	}
 	struct sockaddr_in road_addr;
	bzero(&road_addr,sizeof(road_addr));
	road_addr.sin_family = AF_INET;
	road_addr.sin_port = htons(port);
	road_addr.sin_addr.s_addr = inet_addr(ip);

	printf("Road-server Create Sucessfully...\n");
	SetSocketOpt(sfd);//基于TCP
	if(connect(sfd,(struct sockaddr *)&road_addr,sizeof(struct sockaddr))==-1)
	{
		perror("Road  connect error");
		close(sfd);
		sfd= -1;		
	}
	
	return sfd;
}


int KeepRoadConnect()
{
	roadfd = ConnectRoad_TCP (net_server.ip, net_server.port);
	if(roadfd < 0)
	{
		printf("can't connect Road-server host!\n");
		return -2;
	}
	else
	{
		printf("alreay connect Road-server host!\n");
		return 0;
	}
}

int SendtoServer(unsigned char *buf, int len)
{

	if(len == 0) {
		printf("len is 0\n");
		return -1;
	}
	unsigned char sendbuff[SEND_LENGTH];
	memset(sendbuff ,0,sizeof(sendbuff));

	unsigned int  i = 0;
	unsigned int state = 0;

	/*采用纯非阻塞方式,描述符无变化,立即返回*/
	struct timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=0;

	if(roadfd < 0)
    {
		state = KeepRoadConnect();
	}
	else
	{
		fd_set wfds;	
		FD_ZERO(&wfds);
		FD_SET(roadfd, &wfds);

		int ret = select(roadfd+1, NULL,&wfds, NULL, &tv);
		switch(ret)
		{
			case -1:
				perror("send  select error");
				state = -1;
				break;
			case 0:
				perror("send time out!\n");
				state = -1;
				break;
			default:
				if(FD_ISSET(roadfd,&wfds)>0)
				{
					FD_CLR(roadfd, &wfds);
					/*加入报头报尾校验和*/
					sendbuff[0] = SEND_SOH1;
					sendbuff[1] = SEND_SOH2;
					sendbuff[2] = SEND_SOH3;
					sendbuff[3] = SEND_SOH4;
					for(i=0;i<len;i++)
					{
						sendbuff[4+i] = *buf;
						buf++;
					}

					sendbuff[5+len] = SEND_END1;
					//采用TCP模式发送
					state = send(roadfd, sendbuff,5+len,0);
	 				if(state == -1 && errno != EINTR)//如果不为中断错误 
					{	
	 					printf("send error,signal isn't interrupt!\n");
						close(roadfd);
						roadfd = -1;
	 				}
					else
					{
						state = state - 6;
					}
				}//end of 	if
				break;
		}//end of switch
	}//end of else	
	return state;
}


//关闭路口句柄
void CloseRoadfd(void)
{
	close(roadfd);
	roadfd = -1;
}
