#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
//#include "ros/ros.h"

using namespace std;
int FD_WES;
char nameofPort_1[] = "/dev/ttyUSB0";
char nameofPort_2[] = "/dev/ttyUSB1";


bool PortIint(string usbPort, unsigned int baudRate)
{
	struct termios options;
	FD_WES = open(usbPort.data(), O_RDWR|O_NONBLOCK|O_NOCTTY|O_NDELAY);
	if(FD_WES == -1)
	{
		cout << "can not open the COM1!" << endl; 
		FD_WES = open(nameofPort_2, O_RDONLY|O_NONBLOCK|O_NOCTTY|O_NDELAY);
		if(FD_WES == -1)
		{
			cout << "can not open the COM2!" << endl; 
			return false;
		}
		else
			cout << "open COM2 ok!" << endl;
	}
	else
		cout << "open COM1 ok!" << endl;
	
	bzero(&options,sizeof(options));  //?
	
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE; //the bitmask
	
	//Set the baudrate
	switch(baudRate)
	{
		case 115200:
			cfsetispeed(&options, B115200);
			cfsetospeed(&options, B115200);
			break;
		case 460800:
			cfsetispeed(&options, B460800);
			cfsetospeed(&options, B460800);
			break;
		case 921600:
			cfsetispeed(&options, B921600);
			cfsetospeed(&options, B921600);
			break;
		default:
			cfsetispeed(&options, B460800);
			cfsetospeed(&options, B460800);
	}
	options.c_cflag |= CS8; //8 data bits
	
	options.c_cflag &= ~PARENB;
	options.c_iflag &= ~INPCK;//no parity check
	
	options.c_cflag &= ~CSTOPB; //one stop bit
	
	options.c_cc[VTIME] = 0;  //waiting time for reading every bit
	options.c_cc[VMIN] = 0;   //minimum bit(s) to read
	
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	
	tcflush(0, TCIOFLUSH);
	
	//activate the configuration
	if((tcsetattr(FD_WES, TCSANOW, &options)) != 0)
	{
		cout << "com setup error!" << endl;
		return false;
	}
	else
	{
		cout << "com setup succeeded!" << endl;
		return true;
	}
}
int Read_abit_fromeSerialPort(unsigned char* buff)
{
	return read(FD_WES , buff , 1);
}
