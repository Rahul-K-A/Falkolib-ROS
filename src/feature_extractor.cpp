#include "falkolib_feature_extractor/feature_extractor.h"

falkolib::FALKOExtractor extractor;

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    falkolib::LaserScan scan1(scan->angle_min, scan->angle_increment * scan->ranges.size(), scan->ranges.size()) ;
    vector<double> d_scans;
    for(const float& range : scan->ranges)
    {
        double d_range = (double)(range);
        d_scans.push_back(d_range);
    }
    scan1.fromRanges(d_scans);
    vector<falkolib::FALKO> keypoints1, keypoints2;
    extractor.extract(scan1, keypoints1);
    tf
}

int main()
{
    ROS_INFO("Hello world");
}