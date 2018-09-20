/*
 * config.h
 *
 *  创建于: Mar 22, 2013
 *      作者:jiajunjie
 *		内容：读取配置文件代码			
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define MAXLINE     1024
#define MAXDATA     200

typedef struct NETSERVER
{
	char ip[20];//配置IP
	int port;
}netServer;

void ReadConfigData( void );


#endif
