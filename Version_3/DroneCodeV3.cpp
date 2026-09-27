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

// Calibrating offsets
float Pich_offset = 0.0;
float Roll_offset = 0.0;

// Pin and Pluse Ranges for the ESC
const int TopRight = 18;      // SignalWires GPIO Pins
const int TopLeft = 4;
const int BotRight = 19;
const int BotLeft = 23;

// Max and Min pulses for the ESCs
const int Min_pulse = 1000;  // 0% throttle (1000us)
const int Max_pulse = 2000;  // 100% throttle (2000us)

// Data Structure for Rreceiving and Transceiving data
typedef struct struct_message {
   int throttle;  // 0 to 100 (0 = 0% throttle, 1000 = 100% throttle)
   int pitch;     // -1000 to +1000 (-1000 = full back , +1000 = full foward)
   int roll;      // -1000 to +1000 (-1000 = full left , +1000 = full right)
} struct_message;

typedef struct telemetry_message {
   int motorTR;
   int motorTL;
   int motorBR;
   int motorBL;
} telemetry_message;

// Global variables for sharing data between interuptions and callback loop
unsigned long LastTelemetryTime = 0;
const int TelemetryInterval = 50; // Send telemetry every 50ms

volatile bool NewDataAvailable = false;   
struct_message IncomingData;       // Recieving Data
telemetry_message OutgoingData;    // Telmetry Data

// Fail Safe Timers
unsigned long LastPacketTime = 0;
const unsigned long STAGE1_TIMEOUT = 300;  // 300ms  Controlled level descent
const unsigned long STAGE2_TIMEOUT = 2000; // 2s   Hard disarm

//------------------------------------------------------------------------------------------------------------------------------------------------

// PID Gains
float Kp_pitch = 1.3, Ki_pitch = 0.02, Kd_pitch = 18.0;
float Kp_roll  = 1.3, Ki_roll  = 0.02, Kd_roll  = 18.0;

// PID State Variables
float error_pitch, prev_error_pitch, i_pitch, pid_pitch;
float error_roll,  prev_error_roll,  i_roll,  pid_roll;

unsigned long PrevTime = 0;

//------------------------------------------------------------------------------------------------------------------------------------------------

//  Callback: When Data is Sent
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // Reserved for outgoing telemetry status
}

// Callback: When Data Recvieved
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  memcpy(&IncomingData, data, sizeof(IncomingData));
  NewDataAvailable = true;
  LastPacketTime = millis(); // Refresh connection timer
}

void StopAllMotors() {
  ESC_TopRight.writeMicroseconds(Min_pulse);
  ESC_TopLeft.writeMicroseconds(Min_pulse);
  ESC_BotRight.writeMicroseconds(Min_pulse);
  ESC_BotLeft.writeMicroseconds(Min_pulse);

  // Reset I-terms when disarmed to prevent wind-up
  i_pitch = 0;
  i_roll = 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!mpu.begin()) {
    Serial.println("Error: MPU6050 not detected on drone!");
    
  } else {
      Serial.println("Drone IMU (MPU6050) Ready!");

      mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
      mpu.setGyroRange(MPU6050_RANGE_500_DEG);
      mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

      Serial.println("Calibrating MPU6050... DO NOT MOVE DRONE!");
      delay(2000); // 2-second delay so you can take your hands off the drone

      int Num_samples = 200;
      for (int i = 0; i < Num_samples; i++) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
  
        Pich_offset += atan2(-a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 57.3;
        Roll_offset += atan2(-a.acceleration.x, a.acceleration.z) * 57.3;
        delay(10);
    }

    Pich_offset /= Num_samples;
    Roll_offset /= Num_samples;

    Serial.print("Offsets -> Pitch: "); Serial.print(Pich_offset);
    Serial.print(" | Roll: "); Serial.println(Roll_offset);
    Serial.println("Calibration complete!");
  }
  
  // Timer for ESC Signals
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  // Attaches the pins and Max and Min pulses for ESC
  ESC_TopRight.attach(TopRight, Min_pulse, Max_pulse);
  ESC_TopLeft.attach(TopLeft, Min_pulse, Max_pulse);
  ESC_BotRight.attach(BotRight, Min_pulse, Max_pulse);
  ESC_BotLeft.attach(BotLeft, Min_pulse, Max_pulse);

  // Arming ESCs, Motors
  Serial.println("Arming ESCs...");
  StopAllMotors();
  delay(3000);
  Serial.println("ESCs Armed.");

  //Setting Up ESP Now
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send and receive callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register Controller as peer
  memcpy(peerInfo.peer_addr, controllerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Drone Receiver Ready! Waiting for joystick signals...\n");
  
  LastPacketTime = millis(); // Initialize failsafe timer
  PrevTime = micros();
}

//------------------------------------------------------------------------------------------------------------------------------------------------

void loop() {
  // Calculate delta time (dt) in seconds
  unsigned long CurrentTime = micros();
  float dt = (CurrentTime - PrevTime) / 1000000.0;

  // Rest clock
  PrevTime = CurrentTime;
  
  // Read MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Raw Pitch && Roll
  float RawPitch = atan2(-a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 57.3;
  float RawRoll  = atan2(-a.acceleration.x, a.acceleration.z) * 57.3;
 
  // Apply calibration offsets
  float DronePitch = RawPitch - Pich_offset;
  float DroneRoll  = RawRoll  - Roll_offset;

  // Flight variables declared before evaluation
  float TargetPitch = 0.0;  // it is 0 bc of prev Testing
  float TargetRoll  = 0.0;
  int BaseThrottle  = Min_pulse;

  unsigned long timeSinceLastPacket = millis() - LastPacketTime;

  // Using the Data \\ -------------------------------------------------------

  // Failsafe 
  if (TimeSinceLastPacket > STAGE2_TIMEOUT) {
    
    // --- STAGE 2: Hard Cut (Safety Disarm) ---
    
    StopAllMotors();
    
    return; // Exit loop early to prevent motor updates
    
  } 
  else if (TimeSinceLastPacket > STAGE1_TIMEOUT) {
    
 StopAllMotors(); // Is a hard cut off bc I hurt myslef :(
    
 return; // Exit loop early to prevent motor updates
    
  } 
  else {
    // Joystick inputs (-1000 to +1000) to target tilt angles (-20° to +20°)
    TargetPitch  = map(IncomingData.pitch, -1000, 1000, -20, 20);
    TargetRoll   = map(IncomingData.roll,  -1000, 1000, -20, 20);
    BaseThrottle = map(IncomingData.throttle, 0, 1000, Min_pulse, Max_pulse);
    
    if (IncomingData.throttle <= 10) {
    
      // Safety: If throttle is at absolute minimum (disarmed state), turn off motors completely
    StopAllMotors();

    return;
    }
  }

// Calculate Errors
error_pitch = TargetPitch - DronePitch;
error_roll  = TargetRoll  - DroneRoll;

//  P (Proportional) 
float p_pitch = 0;  //Kp_pitch * error_pitch; Zeroed for testing purposes
float p_roll  = 0;  //Kp_roll  * error_roll;  

// --- D (Derivative) ---
float d_pitch = -Kd_pitch * (g.gyro.x * 57.3);
float d_roll  = -Kd_roll  * (g.gyro.y * 57.3);

// --- I (Integral) & Total PID ---
// Only calculate I-term when throttle is active to prevent ground wind-up
if (BaseThrottle > 1050) {
    i_pitch += Ki_pitch * error_pitch * dt;
    i_roll  += Ki_roll  * error_roll  * dt;
    i_pitch  = constrain(i_pitch, -40, 40); 
    i_roll   = constrain(i_roll,  -40, 40);

    pid_pitch = p_pitch + i_pitch + d_pitch;
    pid_roll  = p_roll  + i_roll  + d_roll;
} else {
    // Reset I-terms and PID outputs to 0 when idle on the ground
    i_pitch = 0;
    i_roll = 0;
    pid_pitch = 0;
    pid_roll = 0;
  }
  
  // Constrain total PID output to a safe, flight-ready pulse limit (+/- 40us)
  pid_pitch = constrain(pid_pitch, -40, 40);
  pid_roll  = constrain(pid_roll, -40, 40);

  // Quadcopter X-Frame Motor Mixing Math
  // TopRight (Front-Right): -pitch, -roll
  // TopLeft  (Front-Left):  -pitch, +roll
  // BotRight (Rear-Right):  +pitch, -roll
  // BotLeft  (Rear-Left):   +pitch, +roll

  // Apply PID offsets to Motor Matrix
  int PulseTR = BaseThrottle - pid_pitch - pid_roll;
  int PulseTL = BaseThrottle - pid_pitch + pid_roll;
  int PulseBR = BaseThrottle + pid_pitch - pid_roll;
  int PulseBL = BaseThrottle + pid_pitch + pid_roll;

  //  DOing mirco pulses
  PulseTR = constrain(PulseTR, Min_pulse, Max_pulse);
  PulseTL = constrain(PulseTL, Min_pulse, Max_pulse);
  PulseBR = constrain(PulseBR, Min_pulse, Max_pulse);
  PulseBL = constrain(PulseBL, Min_pulse, Max_pulse);

  // Sending the pulses
  ESC_TopRight.writeMicroseconds(PulseTR);
  ESC_TopLeft.writeMicroseconds(PulseTL);
  ESC_BotRight.writeMicroseconds(PulseBR);
  ESC_BotLeft.writeMicroseconds(PulseBL);

  // Limiting the amount of into gets sent to not overworks computer
  if (millis() - LastTelemetryTime > TelemetryInterval) {
    // Pack the current pulses into the struct
    outgoingData.motorTR = PulseTR;
    outgoingData.motorTL = PulseTL;
    outgoingData.motorBR = PulseBR;
    outgoingData.motorBL = PulseBL;

    // Send it back to the controller's MAC address
    esp_now_send(controllerAddress, (uint8_t *) &outgoingData, sizeof(outgoingData));
      
    // Reset the timer
    LastTelemetryTime = millis();
    }
    
  // Serial Debug Print
  Serial.print("Throt: "); Serial.print(IncomingData.throttle);
  Serial.print(" | Pitch: "); Serial.print(IncomingData.pitch);
  Serial.print(" | Roll: "); Serial.println(IncomingData.roll);

  // Delay to prevent overload
  delay(10);
  
}
