#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float32.h>

// Define motor driver pins (L298N)
#define PWM_PIN 5        // PWM signal (Enable pin on L298N)
#define DIR_PIN1 18      // Direction pin 1 (IN1 on L298N)
#define DIR_PIN2 19      // Direction pin 2 (IN2 on L298N)
#define LED_PIN 2        // Debug LED

rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
rclc_executor_t executor;
rcl_subscription_t subscriber;
std_msgs__msg__Float32 cmd_pwm_msg;

// Function to set motor speed & direction
void set_motor_speed(float pwm_value) {
    int pwm_output = abs(pwm_value) * 255;  // Convert [-1,1] to [0,255]
    
    if (pwm_value > 0) {
        digitalWrite(DIR_PIN1, HIGH);
        digitalWrite(DIR_PIN2, LOW);
    } else if (pwm_value < 0) {
        digitalWrite(DIR_PIN1, LOW);
        digitalWrite(DIR_PIN2, HIGH);
    } else {
        digitalWrite(DIR_PIN1, LOW);
        digitalWrite(DIR_PIN2, LOW);
    }
    
    analogWrite(PWM_PIN, pwm_output);  // Set PWM
}

// Callback function for /cmd_pwm
void pwm_callback(const void *msg) {
    float pwm_value = ((std_msgs__msg__Float32*)msg)->data;
    pwm_value = constrain(pwm_value, -1.0, 1.0);  // Ensure range [-1,1]
    
    set_motor_speed(pwm_value);  // Apply motor control
}

void setup() {
    set_microros_transports();

    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN1, OUTPUT);
    pinMode(DIR_PIN2, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    allocator = rcl_get_default_allocator();
    rclc_support_init(&support, 0, NULL, &allocator);
    
    rclc_node_init_default(&node, "motor_node", "", &support);
    rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "/cmd_pwm"
    );

    rclc_executor_init(&executor, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor, &subscriber, &cmd_pwm_msg, &pwm_callback, ON_NEW_DATA);
}

void loop() {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    delay(100);
}
