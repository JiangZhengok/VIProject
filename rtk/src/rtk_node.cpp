/***Author: Jiang Zheng ******
/***  Time: 2019-03-20  ******
/***   rtk ROS节点源文件  ******/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rtk/rtk_msgs.h"
#include "../include/rtk.h"
#include <unistd.h> 
#include <string.h>

using namespace std;

string Port_1=""; string topic=""; string frame="";
int main(int argc, char **argv)
{
     ros::init(argc, argv, "rtk_pub");
     ros::NodeHandle n;
     ros::param::get("~device_id",Port_1);
	ros::param::get("~topic_name",topic);
	ros::param::get("~frame_id",frame);
     rtk::rtk_msgs gps_msg;
     
     int nread=0;
     char buf[512],tar_value[512];
     GPS_INFO GPS; 
     port_init(Port_1);  //串口初始化 
     ros::Publisher rtk_pub = n.advertise<rtk::rtk_msgs>(topic, 1000);

     while(ros::ok())
     {           
          memset(buf,0,BUFLEN);
          memset(tar_value,0,sizeof(tar_value));
          nread = read(fd1, buf, 512);/*读串口*/
          if (nread > 0)
          {
               if(Get_GPSData("GPGGA",buf,tar_value) <70)
               {
                    sleep(1);
                    continue;
               }
               gps_parse(tar_value,&GPS);

               memset(tar_value,0,sizeof(tar_value));
               if(Get_GPSData("GPRMC",buf,tar_value) <70)
               {
                    sleep(1);
                    continue;
               }
               gps_parse(tar_value,&GPS);
               //show_gps(&GPS);

               gps_msg.header.stamp = ros::Time::now();         
               gps_msg.header.frame_id = frame;  

               gps_msg.latitude    =  GPS.latitude;
               gps_msg.longitude   =  GPS.longitude;
               gps_msg.high        =  GPS.high;    

               rtk_pub.publish(gps_msg);
          }  

          sleep(0.5);    /*睡眠，等待数据多一点 */  

          ros::spinOnce();        
     }           
     //close(fd); 
     // return 0; 
}
