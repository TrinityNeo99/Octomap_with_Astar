/**
*
* 函数功能:读取pcl点云文件并发布到topic上去
*
*
* maker: crp 
* data: 2016-6-8
*/

#include "publish_pcd_nvg.h"
#include <Astar.h>
list<Point *> useAstar (void);
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


    ros::NodeHandle ph;
    ros::Publisher path_pub = ph.advertise<nav_msgs::Path>("path",1, true);

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    nav_msgs::Path path;
    //nav_msgs::Path path;
    path.header.stamp=current_time;
    path.header.frame_id="camera";



    int x = -4;
    int y = -4;
    list<Point *> nvg_path;
    nvg_path = useAstar();
    geometry_msgs::PoseStamped mypose;
    for (auto &p : nvg_path)
    {
        mypose.pose.position.x = (p->y - 4.5) * 0.8;
        mypose.pose.position.y = -(p->x - 5.5) * 0.8 ;
        mypose.header.stamp = current_time;
        mypose.header.frame_id = "camera";
        path.poses.push_back(mypose);
    }


    int a[5] = {1,2,3,4,5};

    for(int i = 0; i < 5 ;i ++)
    {
       //
    }



	ros::Rate loop_rate(hz);
	while (ros::ok())
	{  
		pcl_pub.publish(output);
        path_pub.publish(path);
		ros::spinOnce();  
		loop_rate.sleep();  
	}  
	return 0;  
}

list<Point *> useAstar (void)
{
    //初始化地图，用二维矩阵代表地图，1表示障碍物，0表示可通
    vector<vector<int>> maze = {
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };
    Astar astar;
    astar.InitAstar(maze);

    //设置起始和结束点
    Point start(5, 1);
    Point end(4, 9);
    //A*算法找寻路径
    list<Point *> path = astar.GetPath(start, end, false);
    //打印
    for (auto &p : path)
    {
        cout << '(' << p->x << ',' << p->y << ')' << endl;
        maze[p->x][p->y] = 5;
    }



    for(vector<vector<int>> :: iterator i = maze.begin(); i < maze.end(); i++)
    {
        for(vector<int> :: iterator j = i->begin(); j < i->end(); j++)
        {
            if(*j == 1)cout << "#  ";
            else if(*j == 0) cout << "   ";
            else if(*j == 5) cout << "*  ";
        }
        cout << endl;
    }
    return path;
}