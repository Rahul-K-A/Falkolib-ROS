#include "falkolib_feature_extractor/feature_extractor.h"

falkolib::FALKOExtractor extractor;
ros::Publisher pub;

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    /*Params: Min angle, FOV, number of scans*/
    falkolib::LaserScan scan1(scan->angle_min, scan->angle_increment * scan->ranges.size(), scan->ranges.size()) ;
    /*Falkolib requires ranges in double format*/
    vector<double> d_scans;
    for(const float& reading : scan->ranges)
    {
        double d_range = (double)(reading);
        d_scans.push_back(d_range);
    }
    scan1.fromRanges(d_scans);
    vector<falkolib::FALKO> keypoints1, keypoints2;
    extractor.extract(scan1, keypoints1);
    /*Publish keypoints as markers*/
    visualization_msgs::MarkerArray mArray;
    uint32_t id = 0;
    for(const falkolib::FALKO& keypoint : keypoints1)
    {
        visualization_msgs::Marker marker;
        marker.header.frame_id = "base_scan";
        marker.header.stamp = ros::Time();
        marker.ns = "feature_extract";
        marker.id = id++;
        marker.type = visualization_msgs::Marker::SPHERE;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = keypoint.point[0];
        marker.pose.position.y = keypoint.point[1];
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0;
        marker.pose.orientation.y = 0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;
        marker.color.a = 1.0; // Don't forget to set the alpha!
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        mArray.markers.push_back(marker);
    }
    pub.publish(mArray);
    ROS_INFO("Published %u keypoints", id);
}


int main(int argc, char** argv)
{
    /*Init node*/
    ros::init(argc, argv, "falkolib_feature_extractor");
    ros::NodeHandle nh;

    /*Subscribe to laser topic*/
    ros::Subscriber sub = nh.subscribe("scan", 1000, LaserCallback);
    pub = nh.advertise<visualization_msgs::MarkerArray>("keypoint_marker", 10);

    ros::spinOnce();
    ros::Rate rate(10.0);
    while(ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
    }
}