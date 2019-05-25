#include<stdio.h>               /*��׼�����������*/
#include<sys/ioctl.h>
#include<time.h>
#include<unistd.h>             /*UNIX��׼��������*/
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>             /*�ļ����ƶ���*/
#include<stdlib.h>            /*��׼�����ⶨ��*/
#include<termios.h>           /*PPSIX�ն˿��ƶ���*/
#include<errno.h>             /*����Ŷ���*/
#include<sys/select.h>        /*kbhit�����õ���*/
#include<stropts.h>           /*ͬ��*/
#include<string.h>
#define SPEED 115200
#define SIZE 512

unsigned int speed_arr[]={B115200,B57600,B38400,B19200,B9600,B4800,B2400,B1800,B1200,B600,B300,B200,B150,B134,B110,B75,B50,B0};
unsigned int name_arr[]={115200,57600,38400,19200,9600,4800,2400,1800,1200,600,300,200,150,134,100,75,50,0};
char *dev = "/dev/ttyS0";

int open_port(void);
void set_speed(int fd,int speed);
void set_mode(int fd);
void read_port(int fd);
void write_port(int fd);
int kbhit(void);

int main(void)
{
	int fd;
	int i;
	
	fd=open_port();                                             /*�򿪴����豸�ļ�*/  
	//set_speed(fd,SPEED);                                        /*���ô��ڲ�����*/ 
	set_mode(fd);                                               /*��ͨѶ��ʽ��Ϊԭʼģʽ(Raw Mode)*/
	set_speed(fd,SPEED);                                        /*���ô��ڲ�����*/
	
	printf("read\tpress 1\nwrite\tpress 2\n");
	scanf("%d",&i);
	getchar();
	if(i==1)
		read_port(fd);
	else
		write_port(fd);
	
	close(fd);
	
	return 0;

}

/**
 * ��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ(��Ӧ��ֵ)�����򷵻�0
 * @return  [description]
 */
int kbhit(void)
{
	 static const int STDIN = 0;
	 static int initialized=0;
	 if (! initialized)
	 {
	        // Use termios to turn off line buffering
	         struct termios term;
	         tcgetattr(STDIN, &term);
	         term.c_lflag &= ~ICANON;
	         tcsetattr(STDIN, TCSANOW, &term);
	         setbuf(stdin, NULL);
	         initialized = 1;
	 }
	 int bytesWaiting;
	 ioctl(STDIN, FIONREAD, &bytesWaiting);
	
	return bytesWaiting;
}

//�򿪴���
int open_port(void)
{
	int fd;
		
	fd=open(dev,O_RDWR | O_NOCTTY | O_NONBLOCK);
//	printf("fd=%d\n",fd);
	
	if(fd==-1)
	{
		perror("Can't Open SerialPort");
		return (-1);
	}
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

//���ò�����
void set_speed(int fd,int speed)
{
	int i;
	int status;
	struct termios Opt;
	tcgetattr(fd,&Opt);
	for(i=0;i<sizeof(speed_arr)/sizeof(int);i++)
	{
		if(speed==name_arr[i])
		{
			tcflush(fd,TCIOFLUSH);
			cfsetispeed(&Opt,speed_arr[i]);
			cfsetospeed(&Opt,speed_arr[i]);
			status=tcsetattr(fd,TCSANOW,&Opt);
			
			if(status!=0)
			{
				perror("tcsetattr fd");
				return;
			}

			tcflush(fd,TCIOFLUSH);
		}
	}
}
			
//����ģʽ
void set_mode(int fd)
{
	struct termios options;

	if (tcgetattr(fd,&options) != 0)
	{
		perror("SetupSerial Error");
		return;
	}

	 options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);         /*����INPUT*/
	 options.c_oflag &= ~OPOST;                                  /*����OUTPUT*/
	
}

//������
void read_port(int fd)
{
	int nread;
	char buff[SIZE];
	
	while(!kbhit())
	{
		while((nread=read(fd,buff,SIZE))>0)
		{
			printf("\nLen %d\n",nread);
			buff[nread+1]='\0';
			printf("\n%s",buff);

		}
	}
}

//д����
void write_port(int fd)
{
	char buff[SIZE];
	
	while(strcmp(buff,"quit"))
	{
	//	fgets(buff,SIZE,stdin);
		scanf("%s",&buff);
		if(write(fd,buff,strlen(buff))==-1)
		{
			perror("Write SerialPort Error");
		}
	}
}

