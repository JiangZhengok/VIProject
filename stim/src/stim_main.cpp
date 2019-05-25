#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Imu.h"

#include "stimpublish/STIM300Driver.h"
using namespace std;

string Port_1=""; string topic=""; string frame="";
int main(int argc, char **argv)
{  
	
	bool isfirst = false;	
	ros::Time t_start, t_now_stim;	
	ros::Duration countDur;

	stimdata stimdat;
	sensor_msgs::Imu imu_msg;
	
	//Initialize the node and set the name.
	ros::init(argc, argv, "stim_pub"); 
	
	//the handler of this process
	ros::NodeHandle n;

	ros::param::get("~device_id",Port_1);
	ros::param::get("~topic_name",topic);
	ros::param::get("~frame_id",frame);
	
	ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>(topic, 1000);
	
	//set the data sending frequency, which in this case is 10Hz
	//ros::Rate loop_rate(1000);
	
	
	if(STIMInit(Port_1))
	{
		
		//The ros::ok() line stops the node if Ctrl+C is pressed or if ROS stops all the nodes
		while(ros::ok())
		{
			if(stim_log_display(stimdat))
			{
				if(!isfirst)
				{
					imu_msg.header.stamp = ros::Time::now();
					t_start = imu_msg.header.stamp;
					t_now_stim = t_start;
					isfirst = true;
				}
				else
				{
					countDur = ros::Duration(0, logger.stimCountInterval * 500000);
					//t_interval = ros::Time(0, logger.stimCountInterval * 500000);
					t_now_stim += countDur;
				}

				//imu_msg.header.stamp = ros::Time::now();      
				imu_msg.header.stamp = t_now_stim;      
				imu_msg.header.frame_id = frame;  //"imulink";

				imu_msg.orientation.x = 0.1;
				imu_msg.orientation.y = 0.1;
				imu_msg.orientation.z = 0.1;
				imu_msg.orientation.w = 0.1;

				imu_msg.linear_acceleration.x = stimdat.acc.x; 
				imu_msg.linear_acceleration.y = stimdat.acc.y;
				imu_msg.linear_acceleration.z = stimdat.acc.z;

				imu_msg.angular_velocity.x = stimdat.gyro.x; 
				imu_msg.angular_velocity.y = stimdat.gyro.y; 
				imu_msg.angular_velocity.z = stimdat.gyro.z;

				imu_pub.publish(imu_msg);
			
				//Sleep for the required time to get a 10Hz frequency
				//loop_rate.sleep();
			}
			ros::spinOnce();
		}		
	}
	return 0;
}



