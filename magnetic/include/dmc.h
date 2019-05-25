/***Author: Jiang Zheng ******
/***  Time: 2019-03-10  ******
/***DMC串口接收与解码头文件******/

#ifndef __DMC__
#define __DMC__

#include <iostream>
#include <math.h>
#include <vector>
#define _USE_MATH_DEFINES

#define L (24)
#define IDEN1 (0xAA)
#define IDEN2 (0x55)

extern uint8_t DCMFrame[L];
extern int fd;

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

extern int open_port(int fd,int comport);
extern int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
extern bool CheckData(const unsigned char* frame);
extern bool ReceiveChar(void);
extern void port_init(std::string dev);
extern bool DecodeDCMDatagram(DMCData &dat);

#endif