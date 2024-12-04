#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <iostream>
using namespace std;

class Teleop
{
public:
    Teleop(ros::NodeHandle &input_n, double linear_vel, double angular_vel);

private:
    void callback(const sensor_msgs::Joy::ConstPtr& joy_msg);
    ros::NodeHandle n;
    ros::Subscriber sub;
    ros::Publisher pub;
    double vlinear, vangular; // 控制速度
    int axis_ang, axis_lin;   // 游戏手柄轴映射
};

Teleop::Teleop(ros::NodeHandle &input_n, double linear_vel, double angular_vel)
    : n(input_n), vlinear(linear_vel), vangular(angular_vel)
{
    n.param<int>("axis_linear", axis_lin, 1);
    n.param<int>("axis_angular", axis_ang, 2);
    pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    sub = n.subscribe<sensor_msgs::Joy>("/joy", 10, &Teleop::callback, this);
}

void Teleop::callback(const sensor_msgs::Joy::ConstPtr& joy_msg)
{
    geometry_msgs::Twist v;
    v.linear.x = joy_msg->axes[axis_lin] * vlinear;
    v.angular.z = joy_msg->axes[axis_ang] * vangular;
    ROS_INFO("linear:%.3lf angular:%.3lf", v.linear.x, v.angular.z);
    pub.publish(v);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "teleop");
    ros::NodeHandle n("~");
    double vel_linear = 0.5; // 默认值
    double vel_angular = 1.0; // 默认值
    n.param("vel_linear", vel_linear, 0.5);
    n.param("vel_angular", vel_angular, 1.0);
    Teleop teleop(n, vel_linear, vel_angular);
    ros::spin();
    return 0;
}