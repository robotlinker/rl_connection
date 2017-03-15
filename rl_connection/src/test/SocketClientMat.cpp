#include <rl_connection/SocketMatTransmissionClient.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "ros/ros.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{  
    ros::init(argc, argv, "rl_client_node");
    ros::NodeHandle n;
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(10);

    SocketMatTransmissionClient socketMat;  

    if (socketMat.socketConnect("192.168.1.105", 9090) < 0)
    {  
        return 0;  
    }  
      
   VideoCapture capture(0);
   Mat image;
  
    while (1)  
    {  
        if (!capture.isOpened())
            return 0;  
  
        capture >> image;  
  
        if (image.empty())  
            return 0;  
  
        socketMat.transmit(image);  
    }  
  
    socketMat.socketDisconnect();  
    return 0;  
}  
