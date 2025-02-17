#ifndef TRAPEZOIDAL_NODE_H
#define TRAPEZOIDAL_NODE_H

// ROS includes
#include<ros/ros.h>
#include<ros/time.h>
#include<vector>
#include <geometry_msgs/Pose2D.h>
#include <sensor_msgs/JointState.h>
#include <trapezoidal_planning/WayPoint.h>
#include <math.h>


struct Vector2f
{
    float x;
    float y;
};

class TrapezoidalNode
{
public:
    TrapezoidalNode();
    TrapezoidalNode(double);
    ~TrapezoidalNode();
    void subscriberCallback(const geometry_msgs::Pose2D& msg);
    void computingCallback(const ros::TimerEvent&);
    void PlanTrajectory(const Vector2f& qi, const Vector2f& qf, float freq);
    void PlanTrajectoryFromWaypointsBuffer(float freq);
    void emittingCallback(const ros::TimerEvent&);
    bool serviceCallback(trapezoidal_planning::WayPoint::Request& req, trapezoidal_planning::WayPoint::Response& res);
    bool isComputing();
    bool isEmitting();
    bool isFree();

private:
    std::vector<Vector2f> buffer_ ;
    std::vector<sensor_msgs::JointState> currentTrajectory_ ;
    Vector2f kv, ka;
    double freq_ ;

    ros::NodeHandle nh_ ;
    ros::Publisher pub_ ;
    ros::Timer emittingTimer_ ;
    ros::Timer computingTimer_ ;

    // Service variables :
    ros::ServiceServer srv_ ;
    trapezoidal_planning::WayPoint srvData_ ;



    //provisional : services will be used later
    float l1_, l2_ ; //arm lengths
    void mgd(const float &theta1, const float &theta2, float &x, float &y)
    {
        x = l1_*std::cos(theta1) + l2_*std::cos(theta1 + theta2);
        y = l1_*std::sin(theta1) + l2_*std::sin(theta1 + theta2);
    };
    void mgi(const float &x, const float &y, float &theta1, float &theta2, const bool &elbowLow = true)
    {
        theta2 = std::acos((l1_*l1_ + l2_*l2_)/(2*l1_*l2_));
        if(!elbowLow)
        {
            theta2 = - theta2 ;
        }
        theta1 = std::atan2(y, x) - theta2 ;
    };

};

#endif // TRAPEZOIDAL_NODE_H
