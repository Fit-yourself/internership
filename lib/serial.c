#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//初始化串口
#include "serial.h"

//file:传入的串口文件
//peed: 波特率，9600
int serial_init(char *file,int peed)
{
	
	//打开串口文件
	int fd = open(file,O_RDWR);
	if(fd == -1)
	{
		perror("open serial error");
		return -1;
	}
	
	/*定义串口属性结构体*/
	struct termios myserial;
	memset(&myserial,0,sizeof(myserial));//清零
	
	/*设置控制模式 本地连接  使能接收*/
	myserial.c_cflag |= (CLOCAL | CREAD); 
	myserial.c_cflag &=  ~CSIZE; //清空数据位
	myserial.c_cflag &= ~CRTSCTS; //无硬件控制流
	myserial.c_cflag |= CS8; //数据位 8
	myserial.c_cflag &= ~CSTOPB; //停止位 1
	myserial.c_cflag &= ~PARENB; //不要校验

	/*设置波特率*/
	switch(peed)
	{
		case 9600:
			cfsetospeed(&myserial,B9600);
			cfsetispeed(&myserial,B9600);
			break;
		case 57600:
			cfsetospeed(&myserial,B57600);
			cfsetispeed(&myserial,B57600);
			break;
		case 115200:
			cfsetospeed(&myserial,B115200);
			cfsetispeed(&myserial,B115200);
			break;
	}
	
	/*刷新输出队列，清除正接收的数据*/
	tcflush(fd,TCIFLUSH);

	/*更改配置*/
	tcsetattr(fd,TCSANOW,&myserial);
	
	return fd;
}






