#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix标准函数定义*/
#include     <sys/types.h>  /**/
#include     <sys/stat.h>   /**/
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX终端控制定义*/
#include     <errno.h>      /*错误号定义*/

/***@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void*/

int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
	    B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400,  19200,  9600,  4800,  2400,  1200,  300,
	    38400,  19200,  9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed)
{
  int   i;
  int   status;
  struct termios   Opt;
  tcgetattr(fd, &Opt);
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
   {
   	if  (speed == name_arr[i])
   	{
   	    tcflush(fd, TCIOFLUSH);
    	cfsetispeed(&Opt, speed_arr[i]);
    	cfsetospeed(&Opt, speed_arr[i]);
    	status = tcsetattr(fd, TCSANOW, &Opt);
    	if  (status != 0)
            perror("tcsetattr fd1");
     	return;
     	}
   tcflush(fd,TCIOFLUSH);
   }
}
/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄*
*@param  databits 类型  int 数据位   取值 为 7 或者8*
*@param  stopbits 类型  int 停止位   取值为 1 或者2*
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
int set_Parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
 if  ( tcgetattr( fd,&options)  !=  0)
  {
  	perror("SetupSerial 1");
  	return 0;
  }
  options.c_cflag &= ~CSIZE;
  switch (databits) /*设置数据位数*/
  {
  	case 7:
  		options.c_cflag |= CS7;
  		break;
  	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return 0;
	}
  switch (parity)
  	{
  	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);  /* 设置为奇效验*/ 
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/  
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return 0;
		}
  /* 设置停止位*/   
  switch (stopbits)
  	{
  	case 1:
  		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return 0;
	}
  /* Set input parity option */
  if (parity != 'n')
  		options.c_iflag |= INPCK;
    options.c_cc[VTIME] = 150; // 15 seconds
    options.c_cc[VMIN] = 0;

  tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
  if (tcsetattr(fd,TCSANOW,&options) != 0)
  	{
  		perror("SetupSerial 3");
		return 0;
	}
  return 1;
 }
/**
*@breif 打开串口
fd = open("/dev/tttyS0",O_RDWR | O_NOCTTY);//以阻塞模式打开串口  
fd = open("/dev/tttyS0",O_RDWR | O_NOCTTY | O_NDELAY);//以非阻塞模式打开串口  //O_NDELAY 等价于 O_NOBLOCK
阻塞模式设置超时：有读到数据时返回字节数，没有数据和一般超时一样返回0 
非阻塞模式: read没有读到数据立即返回-1
*/
int OpenDev(char *Dev)
{
int	fd = open( Dev, O_RDWR | O_NOCTTY | O_NDELAY);         //| O_NOCTTY | O_NDELAY  ....  O_RDWR | O_NOCTTY | O_NDELAY
	if (-1 == fd)
		{ /*设置数据位数*/
			perror("Can't Open Serial Port");
			return -1;
		}
	else
	{
		struct termios options;
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
		options.c_oflag &= ~OPOST; /*Output*/
		return fd;
	}
		

}
int read_n_bytes(int FD, char* buf, int n)
{
	int read_len = 0, len = 0;
	char* read_buf = buf;
	while(len < n)
	{
		if((read_len = read(FD, read_buf, 1)) > 0)
		{
			len += read_len;
			read_buf += read_len;
			read_len = 0;
		}
		printf("%d\n", read_len);
		perror("read");
		getchar();
		usleep(5);
	}
	return n;
}
/**
*@breif 	main()
*/
int main(int argc, char **argv)
{
	int fd; fd_set rd;
	int nread=0, nwrite=0, ReadCnt=0, WriteCnt=0;
	char buff[24];   char buf[1]={0};
	char comm[4] = {0x24, 0x4E, 0x46, 0x2A};
	char *dev ="/dev/ttyUSB0";
	fd = OpenDev(dev);
	printf("%d\n", fd);
	if (fd>0)
        set_speed(fd,115200);
	else
	{
		printf("Can't Open Serial Port!\n");
		exit(0);
	}
	if (set_Parity(fd,8,1,'N')== 0)
	{
		printf("Set Parity Error\n");
		exit(1);
	}
 	usleep(200000);
	tcflush(fd, TCIOFLUSH);
	while(1)
	{
		FD_ZERO(&rd);
		FD_SET(fd,&rd);
		while(FD_ISSET(fd,&rd))
		{
			if(select(fd+1,&rd,NULL,NULL,NULL) < 0)
				perror("select error!\n");
			else
			{
				while((nread = read(fd,buff,24))>0)
				{printf("nread = %d,%s\n",nread,buff);}
			}
		}
		/*printf("%s\n","start" );
		getchar();//("写入命令/n");
		nwrite = write(fd,comm,4);
		perror("write");
       
        nread = read(fd,buff,24);
		perror("read");
		printf("%x\n",buff);
		printf("%d\n", nread);
	/*	if(nwrite!=4)
			printf("write error\n"); 
		printf("%s%d\n","WriteByte: ", nwrite); 
		if(nwrite==4)
		{
			WriteCnt++;
			printf("%s%d\n","WriteTime: ",WriteCnt);
			nwrite = 0;
			nread = read(fd,buff,24);
			printf("%d\n", nread);
			if(nread > 0)
			{
				ReadCnt++;
				printf("%s%d\n","ReadTime: ",ReadCnt);
				printf("Len %d\n",nread);
				buff[nread+1]='\0';
				printf("%x\n",buff);
				nread = 0;
			}
		}*/
		//tcflush(fd, TCIOFLUSH);
		
		
  	}
    //close(fd);
    //exit(0);
}
