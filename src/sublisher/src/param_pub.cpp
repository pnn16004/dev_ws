// Standard C++ headers
#include <stdlib.h>

#include <chrono>
#include <memory>
// Dependencies
#include "rclcpp/rclcpp.hpp"         //Common for ROS 2
#include "std_msgs/msg/float32.hpp"  //Built-in messages types for publishing data

using namespace std::chrono_literals;

#define SERVO_MAX_DEGREE (90)  // Maximum angle in degree upto which servo can rotate
#define STEP (1)               // Size of motor step in degrees

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node  // Create node class by inheriting
{                                             // this refers to the MinimalPublisher node
   public:
    MinimalPublisher()                    // The public constructor intits node
        : Node("param_pub"), count_(0) {  //           node  create publisher of type String      Topic name and value
        this->declare_parameter<std::string>("my_topic", "topic");
        this->get_parameter("my_topic", param_topic_);
        this->declare_parameter<std::float_t>("my_start", 0);
        this->get_parameter("my_start", param_start_);
        this->declare_parameter<bool>("my_turn", false);
        this->get_parameter("my_turn", param_turn_);

        publisher_ =
            this->create_publisher<std_msgs::msg::Float32>(param_topic_, 50);  // Init msg type, topic name and msg size
        timer_ = this->create_wall_timer(  // Init timer and start timer_callback to execute 2 times/s
            10ms, std::bind(&MinimalPublisher::timer_callback, this));

        angle = param_start_;
        flip = param_turn_;

        // srand (time(NULL));
    }

   private:
    void timer_callback()  // Function for setting messages and publishing
    {
        auto message = std_msgs::msg::Float32();

        // angle = rand() % (2 * SERVO_MAX_DEGREE + 1) + (-SERVO_MAX_DEGREE);
        message.data = angle;

        int bit;

        if (flip)
            bit = 1;
        else
            bit = -1;

        angle += bit * STEP;

        if (abs(angle) >= abs(SERVO_MAX_DEGREE)) flip = !flip;

        RCLCPP_INFO(this->get_logger(), "Publishing: '%f'",
                    message.data);  // Macro for printing published messages to console
        publisher_->publish(message);
    }
    // Declaration of fields
    std::string param_topic_;
    std::float_t param_start_;
    bool param_turn_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    size_t count_;
    float angle;
    bool flip;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);                            // Init ROS 2
    rclcpp::spin(std::make_shared<MinimalPublisher>());  // Process data from the node, incl. timer callbacks
    rclcpp::shutdown();
    return 0;
}
