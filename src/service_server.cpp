#include "ros/ros.h"
#include "rl2023_ex4/transformation.h"
#include "tf/transform_listener.h"
#include "geometry_msgs/Pose.h"

using namespace std;

class my_service_server {
    public:
        my_service_server();
        bool service_cb(rl2023_ex4::transformation::Request &req, rl2023_ex4::transformation::Response &res);
    private:
        ros::NodeHandle n;
        tf::TransformListener listener;
        tf::StampedTransform transform;
        ros::ServiceServer service;
};

my_service_server::my_service_server() {
    service = n.advertiseService("transformation_service", &my_service_server::service_cb,this);
}

bool my_service_server::service_cb(rl2023_ex4::transformation::Request &req, rl2023_ex4::transformation::Response &res) {

    try {
        listener.lookupTransform(req.frame_a.data, req.frame_b.data, ros::Time(0), transform);
    }
    catch(tf::TransformException ex) {
        ROS_ERROR("%s", ex.what());
        ros::Duration(1.0).sleep();
        return false;
    }
    res.pose.position.x = transform.getOrigin().x();
    res.pose.position.y = transform.getOrigin().y();
    res.pose.position.z = transform.getOrigin().z();
    res.pose.orientation.x = transform.getRotation().x();
    res.pose.orientation.y = transform.getRotation().y();
    res.pose.orientation.z = transform.getRotation().z();
    res.pose.orientation.w = transform.getRotation().w();

    /*ROS_INFO_STREAM(" Transform: " << 
        
            transform.getOrigin().x() << ", " << 
            transform.getOrigin().y() << ", " <<
            transform.getOrigin().z() << ", " << 
            transform.getRotation().x() << ", " << 
            transform.getRotation().y() << ", " << 
            transform.getRotation().z()
        );*/    //debug
         
    return true;
}

int main(int argc, char** argv) {
    ros::init(argc, argv,"my_tf_server");

    my_service_server my_server; 

    ros::spin();

    return 0;
}