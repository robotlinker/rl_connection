#include <rl_connection/SocketMatTransmissionServer.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "ros/ros.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "rl_server_node");
    ros::NodeHandle n;
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(10);

    SocketMatTransmissionServer socketMat;
    if (socketMat.socketConnect(9090) < 0)
    {
        return 0;
    }

    Mat image;
    while (1)
    {
        if(socketMat.receive(image) > 0)
        {
            cv::imshow("",image);
            cv::waitKey(30);
        }
    }

    socketMat.socketDisconnect();
    return 0;
}
