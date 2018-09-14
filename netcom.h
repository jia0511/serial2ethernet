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

#define     RECV_LENGTH      1024        //接收数据缓冲区长度
#define     SEND_LENGTH      1024         //发送缓冲区长度


#define     INFOFRAM		 0xB0//前后车距离速度帧号
#define     STATIONFRAM		 0xB1//轨旁数据帧号
#define     FEEDBACKFRAM	 0xB2//注册反馈帧号

#define 	MAX_SOCK_FD 	 FD_SETSIZE
#define 	MAX_COUNT		 5

//////////发送路口数据报头/////////
#define     SEND_ROADSOH1     0xEE
#define     SEND_ROADSOH2     0xEE
#define     SEND_ROADSOH3     0x7B
#define     SEND_ROADEND1     0x7C
#define     SEND_ROADEND2     0xDD
#define     SEND_ROADEND3     0xDD

/////////接收路口数据报头/////////
#define     RECV_ROADSOH1     0xAC
#define     RECV_ROADSOH2     0xAC
#define     RECV_ROADSOH3     0x6E
#define     RECV_ROADEND1     0x7D
#define     RECV_ROADEND2     0xBB
#define     RECV_ROADEND3     0xBB

void SetSocketOpt(int sockfd);
void KeepRoadConnect(void);
int SendtoServer(unsigned char *buf, int len);
void CloseRoadfd(void);

#endif
