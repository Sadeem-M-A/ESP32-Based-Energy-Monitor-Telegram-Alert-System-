# Smart IoT Energy Monitoring & Telegram Alert System

## 📌 Overview
An end-to-end Smart IoT Energy Monitoring system developed using the ESP32 microcontroller. The system measures real-time power consumption, streams telemetry data via the MQTT protocol to a public broker, and triggers instant automated alerts to a Telegram bot when power usage exceeds a safety threshold.

## 🛠️ Hardware & Tools
* Microcontroller: ESP32
* Sensor Simulation: Potentiometer (Simulating Current / Power Readings)
* Simulation Environment: Wokwi Simulator

## 📡 Protocols & Tech Stack
* Language/IDE: C++ / Arduino IDE
* Messaging Protocol: MQTT Protocol (Broker: `broker.hivemq.com`)
* Alerting API: Telegram Bot API (HTTP GET Requests)

## ⚡ Features
* Real-Time Power Calculation: Calculates dynamic power using $P = V \times I$.
* Telemetry Streaming: Continuous live data streaming via MQTT.
* Instant Automated Alerts: Sends real-time Telegram notifications when power exceeds 2000 W.
