#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "rl2023_ex4/transformation.h"
#include "tf/transform_broadcaster.h"

using namespace std;

int main(int argc, char** argv) {
    ros::init(argc, argv, "my_tf_broadcaster");
    ros::NodeHandle n;

    string f_a;
    string f_b;

    if(argc != 3) {
        ROS_ERROR("Too few or too many arguments, need the names of two frames to evaluate the transformation between them");
        return -1;
    }

    f_a = argv[1];
    f_b = argv[2];

    rl2023_ex4::transformation srv;
    srv.request.frame_a.data = f_a;
    srv.request.frame_b.data = f_b;
    
    ros::ServiceClient client = n.serviceClient<rl2023_ex4::transformation>("transformation_service");
    
    ROS_INFO("Waiting for the service server...");
    client.waitForExistence();
    ROS_INFO("Server is up!");

    tf::TransformBroadcaster br;
    tf::Transform transform;
    bool request = true;
    char buffer;

    ros::Rate r(100);

    while(ros::ok()) {
    
        if(request) {
            request = false;

            if(!client.call(srv)) {
                ROS_ERROR("Error calling the service");
                return 1;
            }

            transform.setOrigin(tf::Vector3(srv.response.pose.position.x,srv.response.pose.position.y,srv.response.pose.position.z));
            tf::Quaternion q(srv.response.pose.orientation.x,srv.response.pose.orientation.y,srv.response.pose.orientation.z,srv.response.pose.orientation.w);
            transform.setRotation(q);
            br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), f_a, f_b));
            ROS_INFO("If you want to publish a new transformation type 'c' ('q' to quit)");
        }

        cin >> buffer;
        if(buffer == 'c') {
            request = true;
            ROS_INFO("Please type the name of the first frame: ");
            cin >> f_a;
            ROS_INFO("Please type the name of the second frame: ");
            cin >> f_b;
        }
        if(buffer == 'q') return 0;
        else buffer = ' ';

        r.sleep();
    }

    return 0;
}
