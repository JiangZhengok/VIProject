/***Author: Jiang Zheng ******
/***  Time: 2019-03-10  ******
/***DMC串口接收与解码源文件******/

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
#include "../include/dmc.h"
#define _USE_MATH_DEFINES

using namespace std;

int fd; int i;
vector<unsigned char> DataBuff(24);
uint8_t DCMFrame[L];

int open_port(int fd, string &dev) 
{ 
      //const char* devport = str.data();
      cout << dev.data() << endl;
      fd = open( dev.data(), O_RDWR|O_NOCTTY|O_NDELAY); 
      if (-1 == fd)
      { 
            perror("Can't Open Serial Port"); 
            return(-1); 
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
            return true; 
	else
	{
            cout << "check sum failure!" << endl; 
            return false;
      }
}

bool ReceiveChar(void) 
{
	unsigned char BuffOne;
     //int cnt = 0;
     //while(cnt < L)	
     //{
       //   cnt++;
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
               else return 0;
	     }
          else return 0;
    // }
    // return 0;
	
} 

void port_init(string dev)
{
     if( (fd=open_port(fd,dev)) < 0 )
          perror("open_port error");  
     else
          printf("open_port success\n");
     if( (i=set_opt(fd,115200,8,'N',1)) < 0 )
          perror("set_opt error"); 
}

bool DecodeDCMDatagram(DMCData &dat)
{

     dat.TNorth.TPitch = (double)( (int)((signed short)(DCMFrame[3]<<8 | DCMFrame[4])) ) * 0.01;
     dat.TNorth.TYaw   = (double)( (unsigned short)(DCMFrame[5]<<8 | DCMFrame[6])) * 0.01; 
     dat.DNorth.DPitch = (double)( (int)((signed short)(DCMFrame[7]<<8 | DCMFrame[8])) ) * 0.01; 
     dat.DNorth.DYaw   = (double)( (unsigned short)(DCMFrame[9]<<8 | DCMFrame[10])) * 0.01; 

     dat.gyro.x        = (double)( (int)((signed short)(DCMFrame[12]<<8 | DCMFrame[13])) ) * 0.1 * 0.00981007; 
     dat.gyro.y        = (double)( (int)((signed short)(DCMFrame[14]<<8 | DCMFrame[15])) ) * 0.1 * 0.00981007;
     dat.gyro.z        = (double)( (int)((signed short)(DCMFrame[16]<<8 | DCMFrame[17])) ) * 0.1 * 0.00981007; 
     dat.dmc.x         = (double)( (int)((signed short)(DCMFrame[18]<<8 | DCMFrame[19])) ) * 10.0;
     dat.dmc.y         = (double)( (int)((signed short)(DCMFrame[20]<<8 | DCMFrame[21])) ) * 10.0;
     dat.dmc.z         = (double)( (int)((signed short)(DCMFrame[22]<<8 | DCMFrame[23])) ) * 10.0;

     dat.DMC_ST        = DCMFrame[2];
     dat.CheckSum      = DCMFrame[11];
/*
     if( dat.DMC_ST == 0x00 )  cout << "正在寻北..." << endl;
     if( dat.DMC_ST == 0x01 )  cout << "寻北完成，磁北数据有效!" << endl;
     if( dat.DMC_ST == 0x02 )  cout << "寻北完成，真北数据有效!" << endl;
     if( dat.DMC_ST == 0x03 )  cout << "寻北完成，真北磁北数据有效!" << endl;*/
     if( dat.DMC_ST == 0xff )  {cout << "数据错误" << endl; return false;}
     else return true;
}




