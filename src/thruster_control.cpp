#include <ros/ros.h>
#include<cmath>
#include<std_msgs/String.h>
#include<std_msgs/UInt16.h>
#include<lower_camera/Line.h>
#include<thruster/ThrusterCommand.h>


/*
2019.09.21
ライントレース
こちらが本物（2019.09.15 by 宮澤）

参考：豊橋技科大
*/

ros::Publisher command_pub;
lower_camera::Line line;

const double PI = 3.14;

//線が右側or左側どちらにあるか判定する領域
double rightBorder = 382;
double leftBorder = 254;



void line2dCallBack(const lower_camera::Line& line)
{
  thruster::ThrusterCommand command;
  ROS_INFO("I heard[%2.20f,%2.20f]" , line.px,line.py);
    // 線がロボットの右にあるとき
    if(line.px >= rightBorder) {
      command.thrusterLeftCommand = 300;
      command.thrusterRightCommand = 0;
    
    // 線がロボットの左にあるとき
    }else if(line.px <= leftBorder){
        command.thrusterLeftCommand = 0;
        command.thrusterRightCommand = 300;
    
    //線がロボットの中央にあるとき
      }else{
          double dif_angle = atan2(line.vy,line.vx);
          if(0<dif_angle<80*PI/180){
              command.thrusterLeftCommand = 200;
              command.thrusterRightCommand = -200;
          } 
          if(100*PI/180<dif_angle<180*PI/180){
              command.thrusterLeftCommand = -200;
              command.thrusterRightCommand = 200;
          } 
          //傾き修正できたら
          else {
              command.thrusterLeftCommand = 300;
              command.thrusterRightCommand = 300;

          }
      }

  command_pub.publish(command);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "thruster_control");
  ros::NodeHandle nh;

  //1秒間に1つのメッセージをPublishする
  //ros::Rate loop_rate(1);

  ros::Subscriber sub = nh.subscribe("line", 1, line2dCallBack);
  command_pub = nh.advertise<thruster::ThrusterCommand>("thruster_command", 100);

  ros::spin();
  //loop_rate.sleep();


  return 0;
}
