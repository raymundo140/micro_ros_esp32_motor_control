# Micro-ROS Motor Control con ESP32 y ROS 2

Este proyecto permite controlar la velocidad y dirección de un motor de corriente continua utilizando una **ESP32** con **Micro-ROS** y **ROS 2**. La ESP32 recibe comandos desde un nodo ROS 2 a través de un enlace serial, permitiendo modificar la velocidad (0-255) y la dirección de giro (1 = avance, -1 = reversa, 0 = detenerse). 

## 🚀 Características
- 🛠 **Micro-ROS en ESP32**: Comunicación con ROS 2 a través del Micro-ROS Agent.
- 🔄 **Control de dirección y velocidad**: Configuración de PWM y señales digitales en el driver **L298N**.
- 🖥 **Comandos desde la terminal**: Control total del motor enviando comandos a ROS 2.
- 📡 **Integración con ROS 2**: Uso de tópicos para la comunicación con la ESP32.

---

## 📦 Requisitos
Antes de comenzar, asegúrate de tener instalados los siguientes componentes:

### 🔧 **Hardware**
- ✅ **ESP32** (Cualquier modelo compatible con Micro-ROS)
- ✅ **Driver L298N** (Para controlar el motor)
- ✅ **Motor DC** (Corriente continua)
- ✅ **Fuente de alimentación** (Se recomienda 7.4V - 12V para el L298N)
- ✅ **Cable USB** para programar la ESP32

### 💻 **Software**
- 🟢 **Ubuntu con ROS 2 Humble o superior**
- 🟢 **Micro-ROS Agent** instalado en la computadora
- 🟢 **Arduino IDE o PlatformIO** (Para programar la ESP32)
- 🟢 **Repositorio Micro-ROS para ESP32** instalado

---

## ⚙️ Instalación

### 1️⃣ **Clonar este repositorio**
```bash
git clone https://github.com/TU_USUARIO/micro_ros_esp32_motor_control.git
cd micro_ros_esp32_motor_control
