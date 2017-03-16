#include <rl_connection/SocketMatTransmissionClient.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";
SocketMatTransmissionClient socketMat;
using namespace cv;
using namespace std;

bool setupConnection()
{
  if (socketMat.socketConnect("192.168.0.102", 9090) < 0)
  {
      return false;
  }
}

void imageCallback(const sensor_msgs::Image::ConstPtr& msg)
{
 cv_bridge::CvImagePtr cv_ptr;
  try
  {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
//  // Draw an example circle on the video stream
//  if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//    cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

  // Update GUI Window
  //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
  //cv::waitKey(3);

 cv::Mat dImg =  cv_ptr->image;
 double min = 0;
 double max = 1000;
 cv::Mat img_scaled_8u;
 cv::Mat (cv_ptr->image-min).convertTo(img_scaled_8u, CV_8UC1, 255. / (max - min));
 //cv::cvtColor(img_scaled_8u, dImg, CV_GRAY2RGB);

 // Update GUI Window
 cv::imshow(OPENCV_WINDOW, dImg);
 cv::waitKey(3);

 //VideoCapture capture(0);
 //Mat image;
//         if (!capture.isOpened())
//             return 0;

         //capture >> image;

//         if (image.empty())
//             return 0;
           //socketMat.transmit(image);


 socketMat.transmit(dImg);
 ROS_INFO("send a new image");
}

int main(int argc, char **argv)
{  
    ros::init(argc, argv, "rl_client_node");
    ros::NodeHandle n;
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Subscriber image_sub_ = n.subscribe("/image_converter/output_video", 1000, imageCallback);
    ros::Rate loop_rate(10);

    setupConnection();

   VideoCapture capture(0);
   Mat image;
//    while (ros::ok()){
//      ros::spinOnce();
//      loop_rate.sleep();
//    }
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
