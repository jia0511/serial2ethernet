#include "config.h"

netServer net_server;

//解析处理提取数据
void extract_data(char *line, char *temp)
{
	int i = 0, j = 0;

	for (; line[i] != '='; i++)
		;
	i++;
	for (; i < strlen(line); i++)
	{
		if (line[i] == ' ')
			continue;
		temp[j++] = line[i];
	}
	temp[j] = '\0';
}

//配置读取内容        
void load_file(FILE *s_fp)
{
	char line[MAXLINE], temp[MAXDATA];
	int len = 0;
	while (fgets(line, MAXLINE, s_fp) != NULL)
	{
		if (strstr(line, "serip") != NULL)
		{
			memset(temp, 0, sizeof(temp));
			extract_data(line, temp);
			len = strlen(temp);
			strncpy(net_server.ip, temp, len - 2); //减去"/n","/0"
		}
		if (strstr(line, "serport") != NULL)
		{
			memset(temp, 0, sizeof(temp));
			extract_data(line, temp);
			net_server.port = atoi(temp);
		}
	} //end of while
}

//调试信息打印
void debug_config_print(void)
{
	printf("netServerip : %s\n", net_server.ip);
	printf("netServerport : %d\n", net_server.port);
}

//打开配置文件
void ReadConfigData(void)
{
	FILE *s_fp;

	if ((s_fp = fopen("./option.ini", "r+")) == NULL)
	{
		printf("[-] open option.ini failed.\n");
	}

	load_file(s_fp);
	fclose(s_fp);

	debug_config_print();
	printf("[+] done.\n\n");
}





