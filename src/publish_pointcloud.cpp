/**
*
* 函数功能:读取pcl点云文件并发布到topic上去
*
*
* maker: crp 
* data: 2016-6-8
*/


#include<iostream>
#include<string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>

#include<ros/ros.h>  
#include<pcl/point_cloud.h>  
#include<pcl_conversions/pcl_conversions.h>  
#include<sensor_msgs/PointCloud2.h>  
#include<pcl/io/pcd_io.h>

#include <ros/console.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

using namespace std;


int main (int argc, char **argv)  
{  

    
	std::string topic,path2cloud,frame_id;
        int hz=5;

	ros::init (argc, argv, "publish_pointcloud");  
	ros::NodeHandle nh;

	//nh.param<std::string>("path2cloud", path2cloud, "/media/trinity/WJN/slambook-master/ch5/joinMap/cmake-build-debug/gaoxiang_map.pcd");
    nh.param<std::string>("path2cloud", path2cloud, "/home/trinity/catkin_ws1/src/publish_pointcloud/data/test.copy.pcd");
	nh.param<std::string>("frame_id", frame_id, "camera");
	nh.param<std::string>("topic", topic, "/pointcloud/output");
	nh.param<int>("hz", hz, 5);
   
	ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2> (topic, 10);  

	pcl::PointCloud<pcl::PointXYZ> cloud;  
	sensor_msgs::PointCloud2 output;  
	pcl::io::loadPCDFile (path2cloud, cloud);  
	pcl::toROSMsg(cloud,output);// 转换成ROS下的数据类型 最终通过topic发布

	output.header.stamp=ros::Time::now();
	output.header.frame_id  =frame_id;




	ros::Rate loop_rate(hz);  
	while (ros::ok())  
	{  
		pcl_pub.publish(output);  
		ros::spinOnce();  
		loop_rate.sleep();  
	}  
	return 0;  
}  
