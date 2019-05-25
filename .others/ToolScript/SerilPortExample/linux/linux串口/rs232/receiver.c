#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <termios.h> 
#include <stdlib.h> 

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
	/* 
	设置串口属性： 
	fd: 文件描述符 
	nSpeed: 波特率 
	nBits: 数据位 
	nEvent: 奇偶校验 
	nStop: 停止位 
	*/ 

	struct termios newtio,oldtio; 
	if ( tcgetattr( fd,&oldtio) != 0) 
	{ 
		perror("SetupSerial 1"); 
		return -1; 
	}	 

	bzero( &newtio, sizeof( newtio ) ); 
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 
	
	switch( nBits ) 
	{ 
		case 7: 
			newtio.c_cflag |= CS7;	
			break; 
		case 8: 
			newtio.c_cflag |= CS8; 
			break; 
	} 

	switch( nEvent ) 
	{ 
		case 'O': //奇校验
			newtio.c_cflag |= PARENB; 
			newtio.c_cflag |= PARODD; 
			newtio.c_iflag |= (INPCK | ISTRIP); 
			break; 
		case 'E': //偶校验
			newtio.c_iflag |= (INPCK | ISTRIP); 
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD; 
			break; 
		case 'N': //不校验
			newtio.c_cflag &= ~PARENB; 
			break; 
	}	 

	switch( nSpeed ) 
	{ 
		case 2400: 
			cfsetispeed(&newtio, B2400); 
			cfsetospeed(&newtio, B2400); 
			break;
		case 4800: 
			cfsetispeed(&newtio, B4800); 
			cfsetospeed(&newtio, B4800); 
			break; 
		case 9600: 
			cfsetispeed(&newtio, B9600); 
			cfsetospeed(&newtio, B9600); 
			break; 
		case 115200: 
			cfsetispeed(&newtio, B115200); 
			cfsetospeed(&newtio, B115200); 
			break; 
		default: 
			cfsetispeed(&newtio, B9600); 
			cfsetospeed(&newtio, B9600); 
			break; 
	} 

	if( nStop == 1 ) 
		newtio.c_cflag &= ~CSTOPB; 
	else if ( nStop == 2 ) 
		newtio.c_cflag |= CSTOPB; 

	newtio.c_cc[VTIME] = 0; 
	newtio.c_cc[VMIN] = 0; 
	tcflush(fd,TCIFLUSH); 

	if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
	{ 
		perror("com set error"); 
		return -1; 
	} 

	printf("set done!\n"); 
	return 0; 
} 



int open_port(int fd,int comport) 
{ 
	// char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"}; 
	long vdisable; 

	if (comport==1) 
	{ 
		fd = open( "/dev/ttyS0", O_RDWR); 
		//fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY); 
		// { fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
		else 
		printf("open ttyS0 .....\n"); 
	} 
	else if(comport==2) 
	{ 
		fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
		else 
			printf("open ttyS1 .....\n"); 
	} 
	else if (comport==3) 
	{ 
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{
			perror("Can't Open Serial Port"); 
			return(-1);		
		} 
		else 
			printf("open ttyS2 .....\n"); 	
	} 
	
	if(fcntl(fd, F_SETFL, 0)<0) 
		printf("fcntl failed!\n"); 
	else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0)); 
	
	if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
	else 
		printf("isatty success!\n"); 

	printf("fd-open=%d\n",fd); 
	return fd; 
} 


int main(void) 
{ 
	int fd; 
	int nread,i; 
	unsigned char buff=255;

	if((fd=open_port(fd,1))<0)
	{ 
		perror("open_port error"); 
		return; 
	}	 

	if((i=set_opt(fd,115200,8,'N',1))<0)
	{ 
		perror("set_opt error"); 
		return; 
	} 
	
	printf("fd=%d\n",fd); 
	while(1)
	{
		nread=read(fd,&buff,1); 
		if(nread)
			printf("%d\n",buff); 
	}
	close(fd); 
	return; 
} 
