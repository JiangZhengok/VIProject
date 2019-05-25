#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <termios.h> 
#include <stdlib.h> 
 
/**
 * open port
 * @param  fd
 * @param  comport ��Ҫ�򿪵Ĵ��ں�
 * @return  
 */
int open_port(int fd,int comport) 
{ 
	char *dev[]={"/dev/ttyUSB0","/dev/ttyS1","/dev/ttyS2"}; 

	if (comport==1)//����1 
	{
		fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if(comport==2)//����2 
     {     
		fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if (comport==3)//����3 
     { 
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
/*�ָ�����Ϊ����״̬*/ 
     if(fcntl(fd, F_SETFL, 0)<0) 
     		printf("fcntl failed!\n"); 
     else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0)); 
/*�����Ƿ�Ϊ�ն��豸*/ 
     if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
     else 
		printf("isatty success!\n"); 
     printf("fd-open=%d\n",fd); 
     return fd; 
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
     struct termios newtio,oldtio; 
/*����������д��ڲ������ã�������������ںŵȳ���������صĳ�����Ϣ*/ 
     if  ( tcgetattr( fd,&oldtio)  !=  0) {  
      perror("SetupSerial 1");
	printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio)); 
      return -1; 
     } 
     bzero( &newtio, sizeof( newtio ) ); 
/*����һ�������ַ���С*/ 
     newtio.c_cflag  |=  CLOCAL | CREAD;  
     newtio.c_cflag &= ~CSIZE;  
/*����ֹͣλ*/ 
     switch( nBits ) 
     { 
     case 7: 
      newtio.c_cflag |= CS7; 
      break; 
     case 8: 
      newtio.c_cflag |= CS8; 
      break; 
     } 
/*������żУ��λ*/ 
     switch( nEvent ) 
     { 
     case 'o':
     case 'O': //���� 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag |= PARODD; 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      break; 
     case 'e':
     case 'E': //ż�� 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag &= ~PARODD; 
      break;
     case 'n':
     case 'N':  //����żУ��λ 
      newtio.c_cflag &= ~PARENB; 
      break;
     default:
      break;
     } 
     /*���ò�����*/ 
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
     case 460800: 
      cfsetispeed(&newtio, B460800); 
      cfsetospeed(&newtio, B460800); 
      break; 
     default: 
      cfsetispeed(&newtio, B9600); 
      cfsetospeed(&newtio, B9600); 
     break; 
     } 
/*����ֹͣλ*/ 
     if( nStop == 1 ) 
      newtio.c_cflag &=  ~CSTOPB; 
     else if ( nStop == 2 ) 
      newtio.c_cflag |=  CSTOPB; 
/*���õȴ�ʱ�����С�����ַ�*/ 
     newtio.c_cc[VTIME]  = 0; 
     newtio.c_cc[VMIN] = 0; 
/*����δ�����ַ�*/ 
     tcflush(fd,TCIFLUSH); 
/*����������*/ 
if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
     { 
      perror("com set error"); 
      return -1; 
     } 
     printf("set done!\n"); 
     return 0; 
} 

 
int main(void)  
{ 
    int fd; 
    int nread,nwrite,i; 
    char ReciveBuff[24]; 
    char WriteBuff[]={0x24, 0x4E, 0x46, 0x2A};
 
    if((fd=open_port(fd,1))<0){
        perror("open_port error"); 
        return; 
    } else{
		printf("open_port success\n");
	}
    if((i=set_opt(fd,115200,8,'N',1))<0){
        perror("set_opt error"); 
        return; 
    } 
    printf("fd=%d\n",fd); 
    //fd=3; 
     while(1)
     {
          nwrite=write(fd,WriteBuff,4);
          printf("nread=%d,%s\n",nwrite,WriteBuff);
          nread=read(fd,ReciveBuff,24);
          printf("nread=%d,%s\n",nread,ReciveBuff);
          getchar(); 
     }  
    
    close(fd); 
    return; 
} 
