#ifndef NETCOM_H_
#define NETCOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define     RECV_LENGTH      1024        //接收数据缓冲区长度
#define     SEND_LENGTH      1024         //发送缓冲区长度


#define     INFOFRAM		 0xB0//前后车距离速度帧号
#define     STATIONFRAM		 0xB1//轨旁数据帧号
#define     FEEDBACKFRAM	 0xB2//注册反馈帧号

#define 	MAX_SOCK_FD 	 FD_SETSIZE
#define 	MAX_COUNT		 5

//////////发送路口数据报头/////////
#define     SEND_SOH1     0xEE
#define     SEND_SOH2     0xEE
#define     SEND_SOH3     0xEE
#define     SEND_SOH4     0xFE

#define     SEND_END1     0xFE


int ConnectRoad_TCP(char* ip,int port);
void SetSocketOpt(int sockfd);
int KeepRoadConnect(void);
int SendtoServer(unsigned char *buf, int len);
void CloseRoadfd(void);

#endif

