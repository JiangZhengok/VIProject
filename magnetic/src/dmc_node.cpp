/***Author: Jiang Zheng ******
/***  Time: 2019-03-10  ******
/***   DMC ROS节点源文件    ******/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "magnetic/dmc_msgs.h"
#include "../include/dmc.h"
#include <unistd.h> 
#include <string.h>

using namespace std;

string Port_1=""; string topic=""; string frame="";
int main(int argc, char **argv)
{
     ros::init(argc, argv, "dmc_pub");
     ros::NodeHandle n;
     ros::param::get("~device_id",Port_1);
	ros::param::get("~topic_name",topic);
	ros::param::get("~frame_id",frame);
     magnetic::dmc_msgs dmc_msg;
     
     int nread=0, nwrite=0, sn;
     DMCData MyDmc;
     char WriteBuff[4] = {0x24, 0x4E, 0x46, 0x2A}; // DMC读数指令
     char GpsPose[] = "$GPGGA,014154.00,3957.8229620,N,11618.3032507,E,1,7,2.1,53.259,M,-8.698,M,,,3.0*74";
     port_init(Port_1);  //串口初始化
     sn = sizeof(GpsPose) / sizeof(char);
     nwrite = write(fd, GpsPose, sn);

     ros::Publisher dmc_pub = n.advertise<magnetic::dmc_msgs>(topic, 1000);

     while(ros::ok())
     {          
          nwrite=write(fd,WriteBuff,4);
          if( nwrite == 4)
          {
               while( !ReceiveChar() ); //等待读完一帧
               if( DecodeDCMDatagram(MyDmc) ) //解码
               {
                    dmc_msg.header.stamp = ros::Time::now();         
                    dmc_msg.header.frame_id = frame;  //"dmclink";

                    dmc_msg.Tyaw   = MyDmc.TNorth.TYaw;
                    dmc_msg.Dyaw   = MyDmc.DNorth.DYaw;
                    dmc_msg.Tpitch = MyDmc.TNorth.TPitch;
                    dmc_msg.Dpitch = MyDmc.DNorth.DPitch;

                    dmc_msg.magnetic.x = MyDmc.dmc.x; 
                    dmc_msg.magnetic.y = MyDmc.dmc.y; 
                    dmc_msg.magnetic.z = MyDmc.dmc.z;

                    dmc_msg.gravity.x = MyDmc.gyro.x; 
                    dmc_msg.gravity.y = MyDmc.gyro.y;
                    dmc_msg.gravity.z = MyDmc.gyro.z;

                    dmc_pub.publish(dmc_msg);

               }
               else  cout << "解码失败！" << endl;
               usleep(47000);       //20Hz频率             
          }  
          ros::spinOnce();
     }      
     //close(fd); 
    // return 0; 
}
