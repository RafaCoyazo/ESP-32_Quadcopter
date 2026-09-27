#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ESP32Servo.h>

// Instance of MPU and Electronic Speed Converter (esc)
Adafruit_MPU6050 mpu;
Servo ESC_TopRight, ESC_TopLeft, ESC_BotRight, ESC_BotLeft;

//------------------------------------------------------------------------------------------------------------------------------------------------
