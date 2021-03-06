#include "Sericom.h"


int open_port(const char *dev)
{
	int fd;
	if( NULL == dev ) 
	{
		return -1;
	}
	//打开串口
	fd = open(dev, O_RDWR|O_NOCTTY|O_NDELAY);
	if ( fd <0 )
	{
		perror("open serial port");
		return -1;
	}
	//恢复串口为堵塞
	if ( fcntl(fd,F_SETFL,0) < 0 ) {
		perror("fcntl F_SETFL");
	}
	
	//测试是否为终端设备
	if ( isatty(STDIN_FILENO) == 0) {
		perror("standard input is not a terminal device.");
	}
	return fd;
}

//设置从
int set_com_config(int fd, int baud_rate, int data_bits, char parity,
		int stop_bits)
{
	struct termios new_cfg, old_cfg;
	int speed;
	//保存并测试现有串口参数设置，在这里如果串口号出错，会有相关的出错信息

	if (tcgetattr(fd, &old_cfg) != 0)
	{
		perror("tcgetattr");
		return -1;
	}

	new_cfg = old_cfg;
	cfmakeraw(&new_cfg); //配置为原始模式
	new_cfg.c_cflag &= ~CSIZE;

	//设置波特率
	switch (baud_rate)
	{
		case 2400:
		{
			speed = B2400;
			break;
		}
		case 4800:
		{
			speed = B4800;
			break;
		}
		case 9600:
		{
			speed = B9600;
			break;
		}
		case 19200:
		{
			speed = B19200;
			break;
		}
		case 38400:
		{
			speed = B38400;
			break;
		}
		case 115200:
		{
			speed = B115200;
			break;
		}

	}
	cfsetispeed(&new_cfg, speed);
	cfsetospeed(&new_cfg, speed);
	//设置数据位

	switch (data_bits)
	{
		case 7:
		{
			new_cfg.c_cflag |= CS7;
			break;
		}

		case 8:
		{
			new_cfg.c_cflag |= CS8;
			break;
		}
	}

	//设置停止位
	switch (stop_bits)
	{
		case 1:
		{
			new_cfg.c_cflag &= ~CSTOPB;
			break;
		}

		case 2:
		{
			new_cfg.c_cflag |= CSTOPB;
			break;
		}
	}

	//设置奇偶校验位
	switch (parity)
	{
		case 'o':
		case 'O':
		{
			new_cfg.c_cflag |= (PARODD | PARENB);
			new_cfg.c_iflag |= (INPCK | ISTRIP);
			break;
		}
		case 'e':
		case 'E':
		{
			new_cfg.c_cflag |= PARENB;
			new_cfg.c_cflag &= ~PARODD;
			new_cfg.c_iflag |= (INPCK | ISTRIP);
			break;
		}
		case 's':
		case 'S':
		{
			new_cfg.c_cflag &= ~PARENB;
			new_cfg.c_cflag &= ~CSTOPB;
			break;
		}

		case 'n':
		case 'N':
		{
			new_cfg.c_cflag &= ~PARENB;
			new_cfg.c_iflag &= ~INPCK;
			break;
		}
	}

	new_cfg.c_cc[VTIME] = 0; //设置接收定时器
	new_cfg.c_cc[VMIN] = 0; //设置read返回的最小字节数
	//处理未接收字符
	tcflush(fd, TCIFLUSH);

	if ((tcsetattr(fd, TCSANOW, &new_cfg)) != 0)
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;
}


