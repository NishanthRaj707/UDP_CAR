# ESP32 UDP Wi-Fi Controlled Robot Car

## Overview

This project implements a high-performance Wi-Fi controlled robot car using an ESP32 running ESP-IDF and a Python UDP client application.

Unlike traditional HTTP/WebSocket-controlled cars, this project uses the lightweight UDP protocol to achieve significantly lower latency and faster command transmission, making it suitable for real-time robotic control.

The ESP32 acts as a UDP server and receives motion commands from a Python client connected over the same Wi-Fi network.

---

## Features

* ESP32 ESP-IDF Based Firmware
* UDP Communication Protocol
* Low Latency Robot Control
* Forward Movement
* Backward Movement
* Left Turn
* Right Turn
* Emergency Stop
* Wi-Fi Station Mode
* Python UDP Client
* Modular Motor Control Functions
* L298N Motor Driver Support
* Bidirectional Communication (ACK Responses)

---

## Why UDP Instead of HTTP?

### Traditional HTTP Control

```text
Button Click
      │
      ▼
HTTP Request
      │
      ▼
TCP Handshake
      │
      ▼
HTTP Parsing
      │
      ▼
ESP32 Processing
```

Issues:

* Higher latency
* More bandwidth overhead
* Additional packet processing
* Slower response time

---

### UDP-Based Control

```text
Button Click
      │
      ▼
UDP Packet
      │
      ▼
ESP32 Receives
      │
      ▼
Motor Action
```

Advantages:

* Extremely fast
* Minimal overhead
* No connection establishment
* Real-time response
* Suitable for robotics and IoT control

---

## System Architecture

```text
┌────────────────────┐
│   Python Client    │
│    (Laptop PC)     │
└─────────┬──────────┘
          │ UDP
          ▼
┌────────────────────┐
│    Wi-Fi Network   │
│ Router / Hotspot   │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│       ESP32        │
│    UDP Server      │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│      L298N         │
│   Motor Driver     │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│     DC Motors      │
└────────────────────┘
```

---

## Communication Flow

```text
Python Client
      │
      │ Send Command
      ▼
UDP Packet
      │
      ▼
ESP32 UDP Socket
      │
      ▼
recvfrom()
      │
      ▼
Command Processing
      │
      ▼
Motor Function
      │
      ▼
GPIO Output
      │
      ▼
L298N Driver
      │
      ▼
Motor Movement
```

---

## Supported Commands

| Command | Action        |
| ------- | ------------- |
| F       | Move Forward  |
| B       | Move Backward |
| L       | Turn Left     |
| R       | Turn Right    |
| S       | Stop          |

---

## Hardware Requirements

### Microcontroller

* ESP32 Development Board

### Motor Driver

* L298N Dual H-Bridge Driver

### Motors

* 2x DC Geared Motors

### Power Supply

* 7V – 12V Battery Pack

### Additional Components

* Chassis
* Wheels
* Jumper Wires
* Switch

---

## GPIO Configuration

| ESP32 Pin | L298N Pin | Function              |
| --------- | --------- | --------------------- |
| GPIO14    | IN1       | Left Motor Direction  |
| GPIO27    | IN2       | Left Motor Direction  |
| GPIO26    | IN3       | Right Motor Direction |
| GPIO25    | IN4       | Right Motor Direction |

---

## Circuit Diagram

```text
                ESP32
          ┌────────────────┐
          │                │
GPIO14 ───┼───────── IN1    │
GPIO27 ───┼───────── IN2    │
GPIO26 ───┼───────── IN3    │
GPIO25 ───┼───────── IN4    │
GND    ───┼───────── GND    │
          └────────────────┘
                    │
                    ▼
            ┌──────────────┐
            │    L298N     │
            └─────┬───┬────┘
                  │   │
                  ▼   ▼
             Left   Right
             Motor  Motor
```

---

## Important Wiring Note

The ESP32 and L298N **must share a common ground**.

```text
Battery (-)
      │
      ├── ESP32 GND
      │
      └── L298N GND
```

Failure to establish a common ground may result in:

* Unstable motor operation
* Random motor activation
* Control signal failure
* Wi-Fi instability

---

## Software Stack

### ESP32 Firmware

* ESP-IDF
* FreeRTOS
* LWIP Socket API
* ESP Wi-Fi Driver

### PC Application

* Python 3
* socket module

---

## UDP Server Configuration

```text
Port : 1234
Protocol : UDP
Mode : Wi-Fi Station
```

Example:

```text
ESP32 IP : 192.168.1.100
Port     : 1234
```

---

## Project Structure

```text
ESP32-UDP-CAR/
│
├── firmware/
│   └── main.c
│
├── client/
│   └── udp_client.py
│
├── images/
│   └── circuit_diagram.png
│
└── README.md
```

---

## Build Instructions

### ESP-IDF Firmware

```bash
idf.py set-target esp32
idf.py build
idf.py flash
idf.py monitor
```

---

## Python Client

Run:

```bash
python udp_client.py
```

Ensure:

* Laptop and ESP32 are connected to the same Wi-Fi network.
* Correct ESP32 IP address is configured inside the client.

---

## Example Monitor Output

```text
WiFi Started
Connecting To WiFi...
Connected
ESP32 IP: 192.168.1.105
Socket Created
UDP Server Listening On Port 1234
Received from 192.168.1.20:54321 -> F
Moving forward
```

---

## Future Improvements

* PWM Speed Control
* Live Telemetry
* Battery Monitoring
* Autonomous Navigation
* Camera Streaming
* Obstacle Detection
* IMU-Based Stabilization
* ROS Integration
* Mobile Application

---

## Educational Objectives

This project demonstrates:

* ESP32 Networking
* UDP Socket Programming
* FreeRTOS Tasks
* Wi-Fi Communication
* Motor Driver Interfacing
* Embedded Robotics
* Real-Time Control Systems
* IoT Communication Protocols

---

## License

This project is released under the MIT License.

Feel free to modify, distribute, and use it for educational and commercial purposes.

---

## Author

Developed using ESP-IDF, FreeRTOS, LWIP Networking Stack, and Python.

A practical implementation of low-latency UDP-based robotic control using ESP32.
