#include <ros/ros.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <geometry_msgs/Twist.h>
#include <unistd.h>
#include <ctime>

ros::Publisher pub_agv;

ros::Time t_tmp;

geometry_msgs::Twist teleop_cmd;
geometry_msgs::Twist zero;

void teleopRemap(const geometry_msgs::Twist::ConstPtr& msg)
{
   teleop_cmd.linear.x=msg->linear.x;
   teleop_cmd.angular.z=msg->angular.z;

   t_tmp=ros::Time::now();

   if(teleop_cmd.linear.x<0.02 && teleop_cmd.linear.x>0.01)
      teleop_cmd.linear.x=0.02;

   if(teleop_cmd.linear.x>-0.02 && teleop_cmd.linear.x<-0.01)
      teleop_cmd.linear.x=-0.02;

   if(teleop_cmd.angular.z<0.08 && teleop_cmd.angular.z>0.01)
      teleop_cmd.angular.z=0.08;  

   if(teleop_cmd.angular.z>-0.08 && teleop_cmd.angular.z<-0.01)
      teleop_cmd.angular.z=-0.08;

   pub_agv.publish(teleop_cmd);

}

void naviRemap(const geometry_msgs::Twist::ConstPtr& msg)
{

   teleop_cmd.linear.x=msg->linear.x;
   teleop_cmd.angular.z=msg->angular.z;

   t_tmp=ros::Time::now();

   if(teleop_cmd.linear.x<0.02 && teleop_cmd.linear.x>0.01)
      teleop_cmd.linear.x=0.02;

   if(teleop_cmd.linear.x>-0.02 && teleop_cmd.linear.x<-0.01)
      teleop_cmd.linear.x=-0.02;

   if(teleop_cmd.angular.z<0.08 && teleop_cmd.angular.z>0.01)
      teleop_cmd.angular.z=0.08;  

   if(teleop_cmd.angular.z>-0.08 && teleop_cmd.angular.z<-0.01)
      teleop_cmd.angular.z=-0.08;

   pub_agv.publish(teleop_cmd);

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "topic_remap_x1_plus", ros::init_options::AnonymousName | ros::init_options::NoSigintHandler);
  ros::NodeHandle n;

  ros::Subscriber sub_teleop = n.subscribe("/cmd_vel_mux/input/teleop", 1, teleopRemap);
  ros::Subscriber sub_navi = n.subscribe("navigation_velocity_smoother/raw_cmd_vel", 1, naviRemap);


  pub_agv = n.advertise<geometry_msgs::Twist>("/cmd_vel/agv", 1);

  ros::Rate loop_rate(10);
  
  while (ros::ok())
  {
     if(t_tmp.sec!=ros::Time::now().sec)
        pub_agv.publish(zero);

     ros::spinOnce();
     loop_rate.sleep();
  }

  return 0;
}
