#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <termios.h> 
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <vector>
#define _USE_MATH_DEFINES

#define L (24)
#define IDEN1 (0xAA)
#define IDEN2 (0x55)

using namespace std;

int fd, i;
vector<unsigned char> DataBuff(24);
uint8_t DCMFrame[L];

/********DMC数据格式**************/
struct TrueNorth{
	double TPitch; //真北 
     double TYaw; //真北
};

struct DmcNorth{
	double DPitch; //磁北
	double DYaw; //磁北
};

struct axes{
	double x;
	double y;
	double z;
};

typedef struct dmcdata{
     struct TrueNorth TNorth;
     struct DmcNorth  DNorth;

	struct axes gyro;
	struct axes dmc;

	unsigned char DMC_ST;	//状态
	unsigned char CheckSum;  //校验和
} DMCData;
/**
 * open port
 * @param  fd
 * @param  comport 
 * @return  
 */
int open_port(int fd,int comport) 
{ 
	if (comport==0)
	{
		fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if(comport==1)
     {     
		fd = open( "/dev/ttyUSB1", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if (comport==2)
     { 
		fd = open( "/dev/ttyUSB2", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
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

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
     struct termios newtio,oldtio; 

     if  ( tcgetattr( fd,&oldtio)  !=  0) {  
      perror("SetupSerial 1");
	printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio)); 
      return -1; 
     } 
     bzero( &newtio, sizeof( newtio ) ); 

     newtio.c_cflag  |=  CLOCAL | CREAD;  
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
     case 'o':
     case 'O': 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag |= PARODD; 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      break; 
     case 'e':
     case 'E': 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag &= ~PARODD; 
      break;
     case 'n':
     case 'N':  
      newtio.c_cflag &= ~PARENB; 
      break;
     default:
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
     case 460800: 
      cfsetispeed(&newtio, B460800); 
      cfsetospeed(&newtio, B460800); 
      break; 
     default: 
      cfsetispeed(&newtio, B9600); 
      cfsetospeed(&newtio, B9600); 
     break; 
     } 

     if( nStop == 1 ) 
      newtio.c_cflag &=  ~CSTOPB; 
     else if ( nStop == 2 ) 
      newtio.c_cflag |=  CSTOPB; 
 
     newtio.c_cc[VTIME]  = 0; 
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

/*unsigned char CodeConv(const unsigned char &a) 
{
     unsigned char b = a;
     //printf("传入值： %x\n", b);
     if( (b & 0x80) == 0 )
     {
          //printf("补码是： %x\n", b); 
          //getchar(); 
          return  b;
     }    
     else
     {
          b ^= 0x7f;
         // printf("取反： %x\n", b);
          b+=1;
          //printf("加1： %x\n", b);
          b |= 0x80;
          //printf("补码是： %x\n", b); getchar();
          return b;
     }
}*/

bool CheckData(const unsigned char* frame)
{
	uint8_t datagram[L];
	unsigned int Checksum = 0;
	for(int i = 0; i < L; i++)
		datagram[i] = frame[i];
	Checksum = datagram[10] + datagram[9] + datagram[8] + datagram[7] + datagram[6] + 
                datagram[5] + datagram[4] + datagram[3] + datagram[2];
     Checksum &= 0xff;
     //printf("%d %d\n", Checksum, datagram[11]);
	if( datagram[11] == Checksum)
		{cout << "check sum successful!" << endl; return true; }
	else
		{cout << "check sum failure!" << endl; return false;}
}

bool ReceiveChar(void) 
{
	unsigned char BuffOne;
     int cnt = 0;
     while(cnt < L)	
     {
          cnt++;
          if( i = read(fd , &BuffOne , 1) > 0 )
	     {
               DataBuff.push_back(BuffOne);
               DataBuff.erase( DataBuff.begin() );
               if( DataBuff[0] == IDEN1 && DataBuff[1] == IDEN2 )
               {
                    if(CheckData(&DataBuff[0]))
                    {
                         for(int i = 0; i < L; i++)
                              DCMFrame[i] = DataBuff[i];
                         return true;
                    }
                    else 
                         return 0;          
               }
	     }
          else return 0;
     }
     return 0;
	
} 

void port_init()
{
     if( (fd=open_port(fd,0)) < 0 )
          perror("open_port error");  
     else
          printf("open_port success\n");
     if( (i=set_opt(fd,115200,8,'N',1)) < 0 )
          perror("set_opt error"); 
}

bool DecodeDCMDatagram(const unsigned char (&frame)[L], DMCData &dat)
{

     dat.TNorth.TPitch = (double)( (int)((signed short)(frame[3]<<8 | frame[4])) ) * 0.01;
     dat.TNorth.TYaw   = (double)( (unsigned int)(frame[5]<<8 | frame[6])) * 0.01; 
     dat.DNorth.DPitch = (double)( (int)((signed short)(frame[7]<<8 | frame[8])) ) * 0.01; 
     dat.DNorth.DYaw   = (double)( (unsigned int)(frame[9]<<8 | frame[10])) * 0.01; 

     dat.gyro.x        = (double)( (int)((signed short)(frame[12]<<8 | frame[13])) ) * 0.1; 
     dat.gyro.y        = (double)( (int)((signed short)(frame[14]<<8 | frame[15])) ) * 0.1;
     dat.gyro.z        = (double)( (int)((signed short)(frame[16]<<8 | frame[17])) ) * 0.1; 
     dat.dmc.x         = (double)( (int)((signed short)(frame[18]<<8 | frame[19])) ) * 10.0;
     dat.dmc.y         = (double)( (int)((signed short)(frame[20]<<8 | frame[21])) ) * 10.0;
     dat.dmc.z         = (double)( (int)((signed short)(frame[22]<<8 | frame[23])) ) * 10.0;

     dat.DMC_ST        = frame[2];
     dat.CheckSum      = frame[11];

     if( dat.DMC_ST == 0x00 )  cout << "正在寻北..." << endl;
     if( dat.DMC_ST == 0x01 )  cout << "寻北完成，磁北数据有效!" << endl;
     if( dat.DMC_ST == 0x02 )  cout << "寻北完成，真北数据有效!" << endl;
     if( dat.DMC_ST == 0x03 )  cout << "寻北完成，真北磁北数据有效!" << endl;
     if( dat.DMC_ST == 0xff )  {cout << "数据错误" << endl; return false;}
     else return true;
}

int main(void)  
{ 
     int nread=0, nwrite=0, sn;
     DMCData MyDmc; 
     char WriteBuff[4] = {0x24, 0x4E, 0x46, 0x2A}; // DMC读数指令
     char GpsPose[] = "$GPGGA,014154.00,3957.8229620,N,11618.3032507,E,1,7,2.1,53.259,M,-8.698,M,,,3.0*74";
     port_init();  //串口初始化
     sn = sizeof(GpsPose) / sizeof(char);
     nwrite = write(fd, GpsPose, sn);

     while(1)
     {
          nwrite=write(fd,WriteBuff,4);
          usleep(500);
          getchar(); 
         
          if( ReceiveChar())  //是否完整的接收到一帧数据
          {
               for(int i = 0; i < L; i++)  //打印当前帧数据
                    printf("%x ", DCMFrame[i]);
               cout << endl;
               cout << "开始解码..." << endl;
               if( DecodeDCMDatagram(DCMFrame, MyDmc) )
               {
                    cout << "解码数据如下： " << endl;
                    cout << "TNorth.TPitch = " << MyDmc.TNorth.TPitch << endl;
                    cout << "TNorth.TYaw = "   << MyDmc.TNorth.TYaw << endl;
                    cout << "DNorth.DPitch = " << MyDmc.DNorth.DPitch << endl;
                    cout << "DNorth.DYaw = "   << MyDmc.DNorth.DYaw << endl;
                    cout << "gyro = ( " << MyDmc.gyro.x << ", " << MyDmc.gyro.y << ", " << MyDmc.gyro.z << " )" << endl;
                    cout << "dmc  = ( " << MyDmc.dmc.x << ", " << MyDmc.dmc.y << ", " << MyDmc.dmc.z << " )" << endl;
                    printf("DMC_ST = %x\n", MyDmc.DMC_ST);
                    printf("CheckSum = %x\n", MyDmc.CheckSum);
               }
               else  cout << "解码失败！" << endl;                    
          }
     }      
     close(fd); 
     return 0; 
} 
