/*
2019.09.15
T100スラスタ2基 linetrace
オンオフ制御

参考：BlueRobotics AnalogJoysticControl.ino
参考：https://qiita.com/Reizouko/items/075b0219ba76ff6e0a09
*/

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include <WProgram.h>
#endif

#include <ros.h>
#include <Servo.h>
#include <std_msgs/Int16.h>
#include <thruster/ThrusterCommand.h>

//ESC/Thruster Settings
static const int CENTER_THROTTLE = 1500; 

//Arduino Pins
static const byte THRUSTER_LEFT = 9;
static const byte THRUSTER_RIGHT = 10;

//Servos
Servo thrusterLeft;
Servo thrusterRight;

void ThrusterControl(const thruster::ThrusterCommand& command);

ros::NodeHandle nh; //nodeハンドラをインスタンス化。プログラムでPublisherとSubscriberを作成
ros::Subscriber<thruster::ThrusterCommand> thrusterSubscriber("thruster_command", &ThrusterControl); //Subscriberインスタンス化


void setup() {

	//pinMode(13, OUTPUT);  LED

    nh.initNode();
    nh.subscribe(thrusterSubscriber);
	
	//Set up Arduino pins to send servo signal to ESCs
	thrusterLeft.attach(THRUSTER_LEFT);
	thrusterRight.attach(THRUSTER_RIGHT);

	// send "stop" signal to ESC.
	thrusterLeft.writeMicroseconds(CENTER_THROTTLE); 
	thrusterRight.writeMicroseconds(CENTER_THROTTLE);

	delay(7000); // delay to allow the ESC to recognize the stopped signal
}


void ThrusterControl(const thruster::ThrusterCommand& command){

	thrusterLeft.writeMicroseconds(CENTER_THROTTLE+command.thrusterLeftCommand);
	thrusterLeft.writeMicroseconds(CENTER_THROTTLE+command.thrusterRightCommand);

	// Delay 1/10th of a second. No need to update at super fast rates.
  delay(100);

}


void loop(){

    nh.spinOnce();
    delay(1);

}


