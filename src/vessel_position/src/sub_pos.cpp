//Standard C++ headers
#include <chrono>
#include <memory>
//Dependencies
#include "rclcpp/rclcpp.hpp" //Common for ROS 2
#include <std_msgs/msg/float32_multi_array.hpp> //Built-in messages types for publishing data
#include <sensor_msgs/msg/joint_state.hpp> //Built-in messages types for publishing data

using namespace std::chrono_literals;
using std::placeholders::_1;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class MinimalPubSub : public rclcpp::Node //Create node class by inheriting
{ //this refers to the MinimalPublisher node
public:
  MinimalPubSub() //The public constructor intits node
  : Node("pubsub")
  { //           node  create publisher of type String      Topic name and value
    IMU_sub_ = this->create_subscription<sensor_msgs::msg::JointState>( //Constructor uses the node's create_subscription class for callbacks
      "/position/IMU", 50, std::bind(&MinimalPubSub::IMU_topic_callback, this, _1)); //No timer, instant response
    
    GPS_sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>( //Constructor uses the node's create_subscription class for callbacks
      "/position/GPS", 50, std::bind(&MinimalPubSub::GPS_topic_callback, this, _1)); //No timer, instant response

    publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/position/fusion", 50); //Init msg type, topic name and msg size
  }

private:
  void IMU_topic_callback(const sensor_msgs::msg::JointState::SharedPtr msg) const //Receives the string message data published over the topic
  {
    auto message = std_msgs::msg::Float32MultiArray();
    auto rpy = msg->position;
    auto accel = msg->velocity;
    auto gyro = msg->effort;
    auto position_ = curr_GPS_;
  
    RCLCPP_INFO(this->get_logger(), "I heard: '%f'", rpy[0]); //Write to console using macro

    if (curr_GPS_.capacity() != 0) { //Atleast one GPS reading
      //Use curr_GPS_ in KF
      //Publish KF estimate
    }

    rclcpp::sleep_for(std::chrono::nanoseconds(1)); //To have enough time to publish
    publisher_->publish(message);
  }
  void GPS_topic_callback(std_msgs::msg::Float32MultiArray::SharedPtr msg) //Receives the string message data published over the topic
  {
    curr_GPS_ = msg->data; //Get current data from GPS, GPS slower than IMU
  }

  //Declaration of fields
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr IMU_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr GPS_sub_;
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_;
  std::vector<float> curr_GPS_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv); //Init ROS 2
  rclcpp::spin(std::make_shared<MinimalPubSub>()); //Process data from the node, incl. timer callbacks
  rclcpp::shutdown();
  return 0;
}