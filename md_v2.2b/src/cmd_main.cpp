#include <ros/ros.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16MultiArray.h>

#include "md/vel_msg.h"
#include "md/monitor_msg.h"
//
typedef unsigned char  BYTE;

#define LEFT           	  0
#define RIGHT             1

#define BIT0              0x01
#define BIT1              0x02
#define BIT2              0x04
#define BIT3              0x08
#define BIT4              0x10
#define BIT5              0x20
#define BIT6              0x40
#define BIT7              0x80

typedef struct {
    long lPosiX;
    long lPosiY, lExPosiY;
    short sTheta;
    float fTheta;
    short sVoltIn;
    short sCurrent[2];
    BYTE byUS1;
    BYTE byUS2;
    BYTE byUS3;
    BYTE byUS4;
    BYTE byPlatStatus;
    BYTE byDocStatus;
    BYTE byMotStatus[2];

    BYTE fgAlarm[2], fgCtrlFail[2], fgOverVolt[2], fgOverTemp[2];
    BYTE fgOverLoad[2], fgHallFail[2], fgInvVel[2], fgStall[2];

    BYTE fgEmerON, fgBatChargeON, fgRccState;

    short sCmdLinearVel, sCmdAngularVel;
    short sRealLinearVel, sRealAngularVel;

    float fRealLinearVel, fRealAngularVel;

    int nAngleResolution;

    BYTE fgIntSpeed[2], fgDirection[2], fgRunBrake[2], fgStartStop[2];
    BYTE fgEncoderA[2], fgEncoderB[2];
    BYTE byIOMonitor[2];

}MotorDriver;
MotorDriver Md;

//tf_prefix add
std::string tf_prefix_;
bool has_prefix;

//ekf_localization
bool m_bEKF_option = false;

class MOROBOT
{
    private:

    public:
    bool first;
    double prev_coordinates[3];
    double coordinates[3]; //x,y,theta

    ros::NodeHandle pnh;
    ros::Publisher odom_publisher;
    tf::TransformBroadcaster odom_broadcaster;
    ros::Time current_time, last_time;

    MOROBOT()
	{
        first=true;
        odom_publisher = pnh.advertise<nav_msgs::Odometry>("odom", 50);
        printf("Create MOROBOT Class\n");

        current_time=ros::Time::now();
        last_time=ros::Time::now();
        for(int i=0;i<3;i++)
		{
            prev_coordinates[i] = 0;
            coordinates[i] = 0;
        }

    }
    ~MOROBOT()
	{
		printf("Distroy MOROBOT Class\n");
	}

		void read(){

				if(first) 
				{
					current_time=ros::Time::now();
					last_time=current_time;
					first=false;
				}
				else 
				{
					current_time=ros::Time::now();
					pub();
				}

		}


    void pub()
    {
        double dt=(current_time-last_time).toSec();
        double velocity[3];

        for(int i=0;i<3;i++)
	{
            velocity[i]=(coordinates[i]-prev_coordinates[i])/dt;
            prev_coordinates[i]=coordinates[i];
        }

		
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(coordinates[2]);
		
        geometry_msgs::TransformStamped odom_trans;
	if(!m_bEKF_option) //add...ekf_localization option _ wbjin
	{
		odom_trans.header.stamp = current_time;
		
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_footprint";
		
		odom_trans.transform.translation.x = coordinates[0];
		odom_trans.transform.translation.y = coordinates[1];
		odom_trans.transform.translation.z = 0.0;
		odom_trans.transform.rotation = odom_quat;
		odom_broadcaster.sendTransform(odom_trans);
	}
		
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;

	odom.header.frame_id = "odom";
	odom.child_frame_id = "base_footprint";
	
        //pose
        odom.pose.pose.position.x= coordinates[0];
        odom.pose.pose.position.y= coordinates[1];
        odom.pose.pose.position.z= 0.0;
        odom.pose.pose.orientation= odom_quat;
        odom.twist.twist.linear.x = velocity[0];
        odom.twist.twist.linear.y = velocity[1];
        odom.twist.twist.linear.z = 0.0;
	odom.twist.twist.angular.z = velocity[2];
        odom_publisher.publish(odom);

        last_time=current_time;
    }
};

//It is a message callback function.
//It is a function that oprates when a topic message named 'monitor_topic' is received.
//The input message is to receive the 'monitor_msg' message from 'md' package in msg directory
void monitorCallBack(const md::monitor_msg::ConstPtr& monitor)
{
    int nGap;
    static int nExsTheta;

    Md.lPosiX             = monitor->lPosiX;
    Md.lPosiY             = monitor->lPosiY;
//    Md.sTheta             = monitor->sTheta;
    Md.fTheta             = (float)(monitor->sTheta)/10;
    Md.sRealLinearVel     = monitor->sRealLinearVel;
    Md.fRealAngularVel    = (float)(monitor->sRealAngularVel)/10;
    Md.sVoltIn            = monitor->sVoltIn;
    Md.sCurrent[LEFT]     = monitor->sLeftMotCur;
    Md.sCurrent[RIGHT]    = monitor->sRightMotCur;
    Md.byUS1              = monitor->byUS1;
    Md.byUS2              = monitor->byUS2;
    Md.byUS3              = monitor->byUS3;
    Md.byUS4              = monitor->byUS4;
    Md.byPlatStatus       = monitor->byPlatStatus;
    Md.byDocStatus        = monitor->byDocStatus;
    Md.byMotStatus[LEFT]  = monitor->byLeftMotStatus;
    Md.byMotStatus[RIGHT] = monitor->byRightMotStatus;
    Md.byIOMonitor[LEFT]  = monitor->byLeftIOMonitor;
    Md.byIOMonitor[RIGHT] = monitor->byRightIOMonitor;

    Md.fgEmerON      = Md.byPlatStatus & BIT0;
    Md.fgBatChargeON = Md.byDocStatus & BIT0;
    Md.fgRccState    = (Md.byDocStatus & BIT7) >> 7;

    Md.fgAlarm[LEFT]    = Md.byMotStatus[LEFT] & BIT0;
    Md.fgCtrlFail[LEFT] = Md.byMotStatus[LEFT] & BIT1;
    Md.fgOverVolt[LEFT] = Md.byMotStatus[LEFT] & BIT2;
    Md.fgOverTemp[LEFT] = Md.byMotStatus[LEFT] & BIT3;
    Md.fgOverLoad[LEFT] = Md.byMotStatus[LEFT] & BIT4;
    Md.fgHallFail[LEFT] = Md.byMotStatus[LEFT] & BIT5;
    Md.fgInvVel[LEFT]   = Md.byMotStatus[LEFT] & BIT6;
    Md.fgStall[LEFT]    = Md.byMotStatus[LEFT] & BIT7;

    Md.fgAlarm[RIGHT]    = Md.byMotStatus[RIGHT] & BIT0;
    Md.fgCtrlFail[RIGHT] = Md.byMotStatus[RIGHT] & BIT1;
    Md.fgOverVolt[RIGHT] = Md.byMotStatus[RIGHT] & BIT2;
    Md.fgOverTemp[RIGHT] = Md.byMotStatus[RIGHT] & BIT3;
    Md.fgOverLoad[RIGHT] = Md.byMotStatus[RIGHT] & BIT4;
    Md.fgHallFail[RIGHT] = Md.byMotStatus[RIGHT] & BIT5;
    Md.fgInvVel[RIGHT]   = Md.byMotStatus[RIGHT] & BIT6;
    Md.fgStall[RIGHT]    = Md.byMotStatus[RIGHT] & BIT7;

    Md.fgIntSpeed[LEFT]  = Md.byIOMonitor[LEFT] & BIT0;
    Md.fgDirection[LEFT] = (Md.byIOMonitor[LEFT] & BIT2)>>2;
    Md.fgRunBrake[LEFT]  = (Md.byIOMonitor[LEFT] & BIT3)>>3;
    Md.fgStartStop[LEFT] = (Md.byIOMonitor[LEFT] & BIT4)>>4;
    Md.fgEncoderA[LEFT]  = (Md.byIOMonitor[LEFT] & BIT5)>>5;
    Md.fgEncoderB[LEFT]  = (Md.byIOMonitor[LEFT] & BIT6)>>6;

    Md.fgIntSpeed[RIGHT]  = Md.byIOMonitor[RIGHT] & BIT0;
    Md.fgDirection[RIGHT] = (Md.byIOMonitor[RIGHT] & BIT2)>>2;
    Md.fgRunBrake[RIGHT]  = (Md.byIOMonitor[RIGHT] & BIT3)>>3;
    Md.fgStartStop[RIGHT] = (Md.byIOMonitor[RIGHT] & BIT4)>>4;
    Md.fgEncoderA[RIGHT]  = (Md.byIOMonitor[RIGHT] & BIT5)>>5;
    Md.fgEncoderB[RIGHT]  = (Md.byIOMonitor[RIGHT] & BIT6)>>6;


    //nGap = Md.sTheta - nExsTheta;
    //ROS_INFO("%4d %4d", nGap, Md.sTheta);
    //nExsTheta = Md.sTheta;
    //ROS_INFO("%d, %d, %d, %d", Md.sCmdLinearVel, Md.sRealLinearVel, Md.sCmdAngularVel, Md.sRealAngularVel);
    printf("sub-> x: %ld  y: %ld  theta: %3.1f  linearVel: %d  angularVel: %4.1f  L_Cur: %d  R_Cur: %d  US1:%d  US2:%d  US3:%d  "
           "volt:%d  Emergecy:%d  Charge:%d  DocState:%d  LMotStatu:%d  RMotStatu:%d  LDirPin:%d  LSSPin:%d  "
           "RDirPin:%d  RSSPin:%d \n",
           Md.lPosiX, Md.lPosiY, Md.fTheta, Md.sRealLinearVel, Md.fRealAngularVel, Md.sCurrent[LEFT], Md.sCurrent[RIGHT], Md.byUS1, Md.byUS2, Md.byUS3, Md.sVoltIn,
           Md.fgEmerON, Md.fgBatChargeON, Md.fgRccState, Md.byMotStatus[LEFT], Md.byMotStatus[RIGHT], Md.fgDirection[LEFT], Md.fgStartStop[LEFT]
           , Md.fgDirection[RIGHT], Md.fgStartStop[RIGHT]);
}
/////////////////////////////////////for example to get the 'Md.sCmdAngularVel'////////////////
void keyboardCallBack(const geometry_msgs::Twist& keyVel)   //from turtlebot3_teleop_key node
{
    Md.sCmdLinearVel  = keyVel.linear.x * 1000;// mm/s
    if(Md.nAngleResolution)
        Md.sCmdAngularVel = keyVel.angular.z * 180/31.41592;// 0.1 deg/s
    else
        Md.sCmdAngularVel = keyVel.angular.z * 180/3.141592;// 1 deg/s
}
///////////////////////////////////////////////////////////////////////////////////////////////

//Node main function
int main(int argc, char** argv)
{
    static int nResolution, nCnt1, nOperMode, nResetOdometry, nResetAngle, nResetAlarm;


    ros::init(argc, argv, "vel_cmd_node");
    ros::NodeHandle nh;

    //Read Conveyor Option Param Read//
    nh.getParam("ekf_option", m_bEKF_option);
    printf("##ekf_option: %d \n", m_bEKF_option);

    MOROBOT morobot;

    ros::Publisher cmd_vel_pub = nh.advertise<md::vel_msg>("vel_topic", 10);             //Publisher declaration.
    ros::Publisher switch_pub = nh.advertise<std_msgs::UInt16MultiArray>("switch", 10);
    ros::Subscriber monitor_sub = nh.subscribe("monitor_topic", 10, monitorCallBack);    //Subscriber declaration.
    ros::Subscriber keyboard_sub = nh.subscribe("cmd_vel", 10, keyboardCallBack);

    ros::Rate r(10);                                                                      //Set the loop period -> 50ms.

    nOperMode = 0;
    md::vel_msg vel;          //vel_msg declares message 'vel' as message file.

    nh.getParam("md_node/angleresolution", Md.nAngleResolution);

    while(ros::ok())
    {

        if(nCnt1++ == 10)     //Store the value of the parameter in the variable once per 500mS.
        {
            nCnt1 = 0;
            nh.getParam("vel_cmd_node/reset_odometry", nResetOdometry);
            nh.getParam("vel_cmd_node/reset_angle", nResetAngle);
            nh.getParam("vel_cmd_node/reset_alarm", nResetAlarm);
        }

        vel.nLinear         = Md.sCmdLinearVel;
        vel.nAngular        = Md.sCmdAngularVel;
        vel.byResetOdometry = nResetOdometry;
        vel.byResetAngle    = nResetAngle;
        vel.byResetAlarm    = nResetAlarm;
        cmd_vel_pub.publish(vel);                 //Publish the message 'vel'

        std_msgs::UInt16MultiArray switch_msg;

             if(Md.sCmdLinearVel == 0.0 && Md.sCmdAngularVel == 0.0)
             {
                 switch_msg.data = {0, 0, 1};
             }
             else if(Md.sCmdAngularVel > 0.0)
             {
                 switch_msg.data = {0, 1, 0};
             }
             else if(Md.sCmdAngularVel < 0.0)
             {
                 switch_msg.data = {0, 1, 1};
             }
             else
             {
                 switch_msg.data = {1, 0, 0};
             }

        switch_pub.publish(switch_msg);            //Publish the message 'switch(led)'

        morobot.coordinates[0] = Md.lPosiY/1000.0000;
        morobot.coordinates[1] = -1 * Md.lPosiX/1000.0000;
	morobot.coordinates[2] = Md.fTheta*3.141592/180;

        morobot.read();

        ros::spinOnce();
        r.sleep();                                //Go to sleep according to the loop period defined
    }
}
