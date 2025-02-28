#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

// Definir pines para el driver L298N
#define PWM_PIN 5        // PWM (Enable en L298N)
#define DIR_PIN1 18      // Dirección IN1 en L298N
#define DIR_PIN2 19      // Dirección IN2 en L298N
#define LED_PIN 32       // LED de depuración

// Configuración de PWM en ESP32
#define PWM_CHANNEL 0     // Canal PWM
#define PWM_FREQ 20000    // Frecuencia de 20 kHz
#define PWM_RESOLUTION 8  // Resolución de 8 bits (0-255)

// Variables de Micro-ROS
rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
rclc_executor_t executor;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 cmd_pwm_msg;

// Función para controlar el motor con PWM
void set_motor_speed(int pwm_value) {
    pwm_value = constrain(pwm_value, 0, 255);  // Asegurar rango válido

    if (pwm_value > 0) {
        digitalWrite(DIR_PIN1, HIGH);
        digitalWrite(DIR_PIN2, LOW);
    } else {
        digitalWrite(DIR_PIN1, LOW);
        digitalWrite(DIR_PIN2, LOW);
    }

    ledcWrite(PWM_CHANNEL, pwm_value);  // Aplicar PWM
}

// Callback de suscripción a /cmd_pwm
void pwm_callback(const void *msg) {
    int pwm_value = ((std_msgs__msg__Int32*)msg)->data;
    set_motor_speed(pwm_value);  // Aplicar PWM recibido
}

void setup() {
    set_microros_transports();

    pinMode(DIR_PIN1, OUTPUT);
    pinMode(DIR_PIN2, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    // Configurar PWM en ESP32
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    allocator = rcl_get_default_allocator();
    rclc_support_init(&support, 0, NULL, &allocator);
    
    // Crear nodo Micro-ROS
    rclc_node_init_default(&node, "motor_node", "", &support);
    
    // Crear suscripción al tópico /cmd_pwm
    rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "/cmd_pwm"
    );

    rclc_executor_init(&executor, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor, &subscriber, &cmd_pwm_msg, &pwm_callback, ON_NEW_DATA);
}

void loop() {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    delay(100);
}
