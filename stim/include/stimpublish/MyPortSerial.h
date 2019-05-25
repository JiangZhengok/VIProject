#ifndef __MYPORTSERIAL__
#define __MYPORTSERIAL__
extern bool PortIint(std::string usbPort, unsigned int baudRate);
extern int Read_abit_fromeSerialPort(unsigned char* buff);
#endif
