#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"
#include <pigpiod_if2.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#define PWM1B 18
#define PWM1F 23
#define PWM2B 24
#define PWM2F 25
#define PWM3B 12
#define PWM3F 16

#define ENCODER10 17
#define ENCODER11 27
#define ENCODER20 22
#define ENCODER21 10
#define ENCODER30 9
#define ENCODER31 11

#define TIME_TO_WAIT 200000

volatile sig_atomic_t cycle_flag = false;
volatile int pi_  = pigpio_start(NULL, NULL);

void handle_alarm( int sig ) {
	if(!cycle_flag){
		set_PWM_dutycycle(pi_, PWM1F,0);
		set_PWM_dutycycle(pi_, PWM2F,0);
		set_PWM_dutycycle(pi_, PWM3F,0);
		set_PWM_dutycycle(pi_, PWM1B,0);
		set_PWM_dutycycle(pi_, PWM2B,0);
		set_PWM_dutycycle(pi_, PWM3B,0);
	}
	cycle_flag=false;
}

static void getCtrlMsg(std_msgs::msg::UInt8MultiArray::SharedPtr control) {
	cycle_flag=true;
	set_PWM_dutycycle(pi_, PWM1F,control->data[0]);
	set_PWM_dutycycle(pi_, PWM1B,control->data[1]);
	set_PWM_dutycycle(pi_, PWM2F,control->data[2]);
	set_PWM_dutycycle(pi_, PWM2B,control->data[3]);
	set_PWM_dutycycle(pi_, PWM3F,control->data[4]);
	set_PWM_dutycycle(pi_, PWM3B,control->data[5]);
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  signal( SIGALRM, handle_alarm );
  set_mode(pi_, ENCODER10, PI_INPUT);
  set_mode(pi_, ENCODER11, PI_INPUT);
  set_mode(pi_, ENCODER20, PI_INPUT);
  set_mode(pi_, ENCODER21, PI_INPUT);
  set_mode(pi_, ENCODER30, PI_INPUT);
  set_mode(pi_, ENCODER31, PI_INPUT);
  set_mode(pi_, PWM1F, PI_OUTPUT);
  set_mode(pi_, PWM2F, PI_OUTPUT);
  set_mode(pi_, PWM3F, PI_OUTPUT);
  set_mode(pi_, PWM1B, PI_OUTPUT);
  set_mode(pi_, PWM2B, PI_OUTPUT);
  set_mode(pi_, PWM3B, PI_OUTPUT);
  auto node = rclcpp::Node::make_shared("control_client");

  auto control_info_sub = node->create_subscription<std_msgs::msg::UInt8MultiArray>(
                        "control",rclcpp::SensorDataQoS(),getCtrlMsg);
						
  ualarm(TIME_TO_WAIT,TIME_TO_WAIT);
  rclcpp::spin(node);

  rclcpp::shutdown();


  return 0;
}

