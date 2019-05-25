/***Author: Jiang Zheng ******
/***  Time: 2019-03-20  ******
/***rtk串口接收与数据解码源文件******/

#include "../include/rtk.h"

using namespace std;
#define _USE_MATH_DEFINES

int fd1;

static int GetComma(int num,char *str)
{
    int i,j=0;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {
        if(str[i]==',')
            j++;
        if(j==num)
            return i+1;   /*返回当前找到的逗号位置的下一个位置*/
    }
    return 0;
}

static double get_double_number(char *s)
{
    char buf[128];
    int i;
    double rev;
    i=GetComma(1,s);    /*得到数据长度 */
    strncpy(buf,s,i);
    buf[i]=0;           /*加字符串结束标志*/
    rev=atof(buf);      /*字符串转float */
    return rev;
}

static int get_int_number(char *s)
{
    char buf[128];
    int i;
    double rev;
    i=GetComma(1,s);    /*得到数据长度*/
    strncpy(buf,s,i);
    buf[i]=0;           /*加字符串结束标志 */
    rev=atoi(buf);      /*字符串转int */
    return rev;
}

static void UTC2BTC(date_time *GPS)
{
/*如果秒号先出,再出时间数据,则将时间数据+1秒 */
    GPS->second++; /*加一秒*/
    if(GPS->second>59)
    {
        GPS->second=0;
        GPS->minute++;
        if(GPS->minute>59)
        {
            GPS->minute=0;
            GPS->hour++;
        }  

    }
    GPS->hour+=8;        /*北京时间跟UTC时间相隔8小时 */
    if(GPS->hour>23)
    {
        GPS->hour-=24;
        GPS->day+=1;
        if(GPS->month==2 ||GPS->month==4 ||GPS->month==6 ||GPS->month==9 ||GPS->month==11 )
        {
            if(GPS->day>30)
            {          /*上述几个月份是30天每月，2月份还不足30*/
                GPS->day=1;
                GPS->month++;
            }
        }  

        else
        {
            if(GPS->day>31)
            { /*剩下的几个月份都是31天每月 */
                GPS->day=1;
                GPS->month++;
            }
        }
        if(GPS->year % 4 == 0 )
        {
            if(GPS->day > 29 && GPS->month ==2)
            {       /*闰年的二月是29天*/
                GPS->day=1;
                GPS->month++;
            }
        }
        else
        {
            if(GPS->day>28 &&GPS->month ==2)
            { /*其他的二月是28天每月*/
                GPS->day=1;
                GPS->month++;
            }
        }
        if(GPS->month>12)
        {
            GPS->month-=12;
            GPS->year++;
        }
    }
}

static double lat_parse(char *s)
{
    char buf[128], du[2];
    int i;
    double rev;
    i=GetComma(1,s);    /*得到数据长度 */
    strncpy(buf,s,i);
    buf[i]=0;           /*加字符串结束标志*/
    char fen[i-1];

    strncpy(du, buf, 2);
    strncpy(fen,buf+2, i-1);
    rev = atof(du)+atof(fen) / 60;
    //rev=atof(buf);      /*字符串转float */
    return rev;
}

static double long_parse(char *s)
{
    char buf[128], du[3];
    int i;
    double rev;
    i=GetComma(1,s);    /*得到数据长度 */
    strncpy(buf,s,i);
    buf[i]=0;           /*加字符串结束标志*/
    char fen[i-2];

    strncpy(du, buf, 3);
    strncpy(fen,buf+3, i-2);
    rev = atof(du)+atof(fen) / 60;
    //rev=atof(buf);      /*字符串转float */
    return rev;
}

void gps_parse(char *line,GPS_INFO *GPS)
{
    int tmp;
    char c;
    char* buf=line;
    c=buf[5];
    if(c=='C')/* "GPRMC" */
    {
        GPS->D.hour   =(buf[7]-'0')*10+(buf[8]-'0');
        GPS->D.minute =(buf[9]-'0')*10+(buf[10]-'0');
        GPS->D.second =(buf[11]-'0')*10+(buf[12]-'0');
        tmp = GetComma(9,buf);      /*得到第9个逗号的下一字符序号*/
        GPS->D.day    =(buf[tmp+0]-'0')*10+(buf[tmp+1]-'0');
        GPS->D.month  =(buf[tmp+2]-'0')*10+(buf[tmp+3]-'0');
        GPS->D.year   =(buf[tmp+4]-'0')*10+(buf[tmp+5]-'0')+2000;
        /*********************************************************/
        GPS->status   =buf[GetComma(2,buf)];     /*状态*/
        GPS->latitude =lat_parse(&buf[GetComma(3,buf)]); /*纬度*/

        GPS->NS       =buf[GetComma(4,buf)];             /*南北纬 */
        GPS->longitude=long_parse(&buf[GetComma(5,buf)]); /*经度*/
        GPS->EW       =buf[GetComma(6,buf)];             /*东西经 */
        UTC2BTC(&GPS->D);                        /*转北京时间*/
    }  

    if(c=='A') /*"$GPGGA" */
    {
        GPS->high =get_double_number(&buf[GetComma(9,buf)]);
        GPS->num =get_int_number(&buf[GetComma(7,buf)]);
    }
}

void show_gps(GPS_INFO *GPS)
{
     printf("年份     : %ld-%02d-%02d\n",GPS->D.year,GPS->D.month,GPS->D.day);
     printf("时间     : %02d:%02d:%02d\n",GPS->D.hour,GPS->D.minute,GPS->D.second);
     printf("纬度     : %s %10.4f\n",(GPS->NS=='N')?"北纬":"南纬",GPS->latitude);
     printf("经度     : %s %10.4f\n",(GPS->EW=='W')?"西经":"东经",GPS->longitude);
     printf("卫星数   : %02d\n",GPS->num);
     printf("高度     : %.4f\n",GPS->high);
     printf("状态     : %s\n",(GPS->status=='A')?"定位":"导航");
     printf("--------------------\n");
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0)
    {
        perror("SetupSerial 1");
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
            
        break;
        case 's':
        case 'S':
            newtio.c_cflag &= ~PARENB;
            newtio.c_cflag &= ~CSTOPB;
        break;
        default:
            printf("Unsupported parity\n"); 
        return -1; 
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

    newtio.c_cc[VTIME]  = 0;/*超时 重要*/
    newtio.c_cc[VMIN] = 100;/*返回的最小值  重要*/
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    return 0;
}

int Get_GPSData(char* gpd,char* buf,char* tar_value)
{
    char *pos = NULL;
    int cur = 0,counter=1,i =0;
    pos = strstr(buf,gpd);
    if(pos == NULL)
    {
        return counter;
    }

    cur = pos - buf;
    tar_value[0]='$';
    for(i = cur; i < BUFLEN; i++)
    {
        if(buf[i]!='$' && buf[i]!='\n' )
        {
            tar_value[counter]=buf[i];
            counter++;
        } 
        else
        {
            tar_value[counter]='\0';
            break;
        }
    }
    return counter;
}

void port_init(string dev)
{
    int nset1;
    fd1 = open( dev.data(), O_RDWR|O_NOCTTY|O_NDELAY );/*打开串口*/
    
    if (fd1 == -1)
    {
        printf("%s\n", "打开串口失败！");
        exit(1);  
    }
    else
    {
        nset1 = set_opt(fd1,BAUD_RATE, DATA_BITS, NEVENT, NSTOP);/*设置串口属性*/
        if (nset1 == -1)
        {
            printf("%s\n", "设置串口属性失败！");
            exit(1); 
        }
        else
            printf("%s%s\n", "成功打开串口： ", dev.data());
        
    }
}

