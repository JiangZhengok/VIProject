/***Author: Jiang Zheng ******
/***  Time: 2019-01-15  ******
/***   show_gps源文件    ******/
#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "string.h"
#include <sstream>  
#include <stdlib.h> 
#include <stdio.h> 
#include <cfloat>
#include <ctype.h>
#include <float.h>
#include <Eigen/Core> 
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace std;

double StartPoint_x = 0.0;
double StartPoint_y = 0.0;

struct csvdata
{
    double x;
    double y;
};

void charToDouble(const char* ch, double& res )
{
    sscanf(ch,"%lf",&res);
}

string LoopRate=""; string topic=""; string frame=""; string path="";

int main (int argc, char **argv)
{
	std::string strTemp;
    csvdata dat;
    double tmp;
	int rate=0;
    int i = 0, j=0;

	ros::init (argc, argv, "by_path");
	ros::NodeHandle ph;

	ros::param::get("~topic_name",topic);
	ros::param::get("~frame_id",frame);
	ros::param::get("~file_path",path);
	//ros::param::get("~loop_rate",LoopRate);
	//istringstream iss(LoopRate);   
    //iss >> rate;  
	//ros::Publisher path_pub = ph.advertise<nav_msgs::Path>("gps_path",1, true);
	ros::Publisher path_pub = ph.advertise<nav_msgs::Path>(topic, 1, true);

	ros::Rate loop_rate(1000);

	//std::string  strFileName = "/home/bit/my/readcsv/data/path.csv";   
	std::string  strFileName =  path; //"/home/bit/data/11.28/gps/save.csv";   
    std::fstream file;                                     //【1】声明一个文件输入输出流对象
    file.open(strFileName.c_str(),std::ios::in);           //【2】以读文件的方式打开文件
    if(!file.is_open())
    {
        std::cout<<"The file is loaded unsuccessfully!"<<std::endl;
        std::system("pause");
    }
    else
        std::cout<<"The file is loaded successfully and the file is being read........"<<std::endl;

	nav_msgs::Path path;
	//getline(file,strTemp); //跳过第一行
	//Eigen::AngleAxisd rotation_vector( -M_PI/180*112, Eigen::Vector3d (0, 0, 1)); //vins_bit_3km
	Eigen::AngleAxisd rotation_vector( -M_PI/180*(90+1.5), Eigen::Vector3d (0, 0, 1));   //vins_bit_5km
	Eigen::Vector3d v_start(0, 0, 0);
	while (ros::ok())
	{
		//if(!file.eof())
		if(getline(file,strTemp))
    	{
			path.header.stamp=ros::Time::now();
			path.header.frame_id = frame;
			cout << strTemp << endl;
			char *str = (char*)strTemp.c_str();

			char *temp = strtok(str,",");            
			charToDouble(temp, tmp );                
			dat.y = tmp;  // y 坐标 朝北
			if(i == 0)
			{
				StartPoint_y = tmp;
			}				
			temp = strtok(NULL,",");
			charToDouble(temp, tmp );                
			dat.x = tmp; //x 坐标 朝东
			if(i == 0)
			{
				StartPoint_x = tmp;
				i++;
			}	

			Eigen::Vector3d v1(dat.x, dat.y, 0), v2(0, 0, 0);
            //cout << vins_raw[i].x << " " << vins_raw[i].y << " " << vins_raw[i].z << endl;
		    v2 = rotation_vector * v1;
			if( j == 0)
			{
				v_start = v2;
				j++;

			}
    		   
			geometry_msgs::PoseStamped this_pose_stamped;
			//this_pose_stamped.pose.position.x = -(dat.x - StartPoint_x);
			//this_pose_stamped.pose.position.y = -(dat.y - StartPoint_y);
			
			//this_pose_stamped.pose.position.x = (dat.y - StartPoint_y);
			//this_pose_stamped.pose.position.y = -(dat.x - StartPoint_x);
			
			//this_pose_stamped.pose.position.x = -(dat.x - StartPoint_x);
			//this_pose_stamped.pose.position.y = -(dat.y - StartPoint_y);

			this_pose_stamped.pose.position.y = -(v2[0] - v_start[0]);
			this_pose_stamped.pose.position.x = -(v2[1] - v_start[1]);

			cout << this_pose_stamped.pose.position.x << ", " << this_pose_stamped.pose.position.y << endl;

			this_pose_stamped.pose.orientation.x = 0;
			this_pose_stamped.pose.orientation.y = 0;
			this_pose_stamped.pose.orientation.z = 0;
			this_pose_stamped.pose.orientation.w = 1;

			this_pose_stamped.header.stamp=ros::Time::now();
			//this_pose_stamped.header.frame_id="world";
			this_pose_stamped.header.frame_id = frame;

			path.poses.push_back(this_pose_stamped);
					
			path_pub.publish(path);
		}

		loop_rate.sleep();
	}

	return 0;
}
