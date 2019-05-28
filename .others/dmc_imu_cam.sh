#!/bin/bash

echo 0000 | sudo -S sh -c 'echo 1000 > /sys/module/usbcore/parameters/usbfs_memory_mb'
sleep 1
echo "  "
echo "******************************"
echo "  "
echo "*****多传感器融合定位系统*****"
echo "  "
echo "******李磊磊 江正 王京哲******"
echo "  "
echo "******************************"

echo "usbfs_memory_mb:"

cat /sys/module/usbcore/parameters/usbfs_memory_mb
sleep 1

echo 0000 | sudo -S sh -c 'chmod 0666 /sys/bus/usb-serial/devices/ttyUSB0/latency_timer'
echo 0 > /sys/bus/usb-serial/devices/ttyUSB0/latency_timer
sleep 1
echo "/dev/ttyUSB0_latency_timer:"
cat /sys/bus/usb-serial/devices/ttyUSB0/latency_timer
sleep 1

echo 0000 | sudo -S sh -c 'chmod 0666 /sys/bus/usb-serial/devices/ttyUSB1/latency_timer'
echo 0 > /sys/bus/usb-serial/devices/ttyUSB1/latency_timer
sleep 1
echo "/dev/ttyUSB1_latency_timer:"
cat /sys/bus/usb-serial/devices/ttyUSB1/latency_timer
sleep 1

echo 0000 | sudo -S sh -c 'chmod 0666 /sys/bus/usb-serial/devices/ttyUSB2/latency_timer'
echo 0 > /sys/bus/usb-serial/devices/ttyUSB2/latency_timer
sleep 1
echo "/dev/ttyUSB2_latency_timer:"
cat /sys/bus/usb-serial/devices/ttyUSB2/latency_timer
sleep 1

ls -l /dev |grep ttyUSB
sleep 1
#gnome-terminal -x bash -c "cd ~/My/ROS/catkin_ws;rosrun pointgrey_camera_driver list_cameras;roslaunch pointgrey_camera_driver camera.launch;exec bash"

rosrun pointgrey_camera_driver list_cameras
sleep 1
gnome-terminal -x bash -c "roslaunch pointgrey_camera_driver camera.launch"

sleep 1
gnome-terminal -x bash -c "roslaunch stim stim.launch;exec bash"
sleep 1
gnome-terminal -x bash -c "rostopic echo /imu_data;exec bash"

sleep 1
gnome-terminal -x bash -c "roslaunch magnetic dmc.launch;exec bash"
sleep 1
gnome-terminal -x bash -c "rostopic echo /dmc_data;exec bash"

sleep 1
gnome-terminal -x bash -c "roslaunch rtk rtk.launch;exec bash"
sleep 1
gnome-terminal -x bash -c "rostopic echo /rtk_data;exec bash"

#sleep 1
#gnome-terminal -x bash -c "roslaunch xsens_driver xsens_driver.launch;exec bash"
#sleep 1
#gnome-terminal -x bash -c "rostopic echo /raw_fix;exec bash"
#sleep 1
#gnome-terminal -x bash -c "rostopic echo /imu/data;exec bash"
sleep 1
gnome-terminal -x bash -c "rqt_image_view;exec bash"




