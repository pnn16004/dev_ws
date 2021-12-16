//Standard C++ headers
#include <chrono>
#include <memory>
//Dependencies
#include "rclcpp/rclcpp.hpp" //Common for ROS 2
#include <sensor_msgs/msg/joy.hpp> //Built-in messages types for publishing data

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include "message_filters/sync_policies/approximate_time.h"
#include "message_filters/synchronizer.h"

using namespace std::chrono_literals;
using std::placeholders::_1;

rclcpp::Node::SharedPtr ptrNode = nullptr;
rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr publisher_;

void approximate_sync_callback(const sensor_msgs::msg::Joy::SharedPtr IMU_msg, const sensor_msgs::msg::Joy::SharedPtr GPS_msg)
{
  auto messageIMU = IMU_msg->header.stamp.sec;
  auto messageGPS = GPS_msg->header.stamp.sec;

  RCLCPP_INFO(ptrNode->get_logger(), "IMU time: '%ld', and GPS time: '%ld'", messageIMU, messageGPS);

  auto position = messageIMU + messageGPS;
  RCLCPP_INFO(ptrNode->get_logger(), "Publishing: '%ld'", position);
  publisher_->publish(messageIMU);
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv); //Init ROS 2
  ptrNode = rclcpp::Node::make_shared("sub_pos");

  message_filters::Subscriber<sensor_msgs::msg::Joy> IMU_sub_(ptrNode, "/position/IMU");
  message_filters::Subscriber<sensor_msgs::msg::Joy> GPS_sub_(ptrNode, "/position/GPS");

  publisher_ = ptrNode->create_publisher<sensor_msgs::msg::Joy>("/position/pospub", 50); //Init msg type, topic name and msg size

  //Approximate time filter, timestamps doesn't need to match exactly
  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::msg::Joy, sensor_msgs::msg::Joy> approximate_policy;
  //message_filters::Synchronizer<approximate_policy>syncApproximate(approximate_policy(10), IMU_sub_, GPS_sub_);
  message_filters::Synchronizer<approximate_policy>syncApproximate(approximate_policy(10), IMU_sub_, IMU_sub_);
  syncApproximate.registerCallback(approximate_sync_callback);

  rclcpp::spin(ptrNode); //Process data from the node, incl. timer callbacks
  rclcpp::shutdown();
  return 0;
}