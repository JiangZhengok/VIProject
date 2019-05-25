/***Author: Jiang Zheng ******
/***  Time: 2019-03-20  ******
/***rtk串口接收与数据解码头文件******/
#ifndef __RTK__
#define __RTK__

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include <iostream>

//#define TTYUSB "/dev/ttyUSB0"  /*串口设备*/
#define BAUD_RATE 115200      /*波特率 */
#define DATA_BITS 8     /*数据位*/
#define NEVENT 'N'       /*校验 */
#define NSTOP 1         /*停止位*/
#define BUFLEN 512       /*可随意取，但要大于GPGGA_MAX*/
#define GPGGA_MAX 100       /*"$GPGGA……"的最大值，待定*/
#define DBG_GPS

extern int fd1;

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
}date_time; 

typedef struct{
     date_time D;/*时间*/
     char status;       /*接收状态 */
     double latitude;   /*纬度*/
     double longitude;  /*经度 */
     char NS;           /*南北极*/
     char EW;           /*东西 */
     int num;           /*卫星数*/
     double speed;      /*速度 */
     double high;       /*高度*/
}GPS_INFO;

static int GetComma(int num,char *str);
static double get_double_number(char *s);
static int get_int_number(char *s);
static double lat_parse(char *s);
static void UTC2BTC(date_time *GPS);
extern void gps_parse(char *line,GPS_INFO *GPS);
extern void show_gps(GPS_INFO *GPS);

extern int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
extern int Get_GPSData(char* gpd,char* buf,char* tar_value);
extern void port_init(std::string dev);


#endif