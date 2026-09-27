#include <esp_now.h>
#include <WiFi.h>

// Joystick Pins
const int PIN_THROTTLE = 34; 
const int PIN_PITCH    = 33;
const int PIN_ROLL     = 32;

// Mac Address
uint8_t droneAddress[] = {0x20, 0x9B, 0xA9, 0x98, 0x0B, 0x38};
esp_now_peer_info_t peerInfo;

// Data Structure for recieving and telemetry
typedef struct struct_message {
   int throttle;  // 0 to 1000
   int pitch;     // -1000 to +1000
   int roll;      // -1000 to +1000
} struct_message;

typedef struct telemetry_message {
   int motorTR;
   int motorTL;
   int motorBR;
   int motorBL;
} telemetry_message;

struct_message outgoingData;
telemetry_message incomingTelemetry;

// Global variable to hold the throttle value
float CurrentThrottle = 0; 

// Call backs
// Callback when data is sent
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  //Extra code in case needed to do something when data is sent
}

// Callback when telemetry is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  
  // Check if we received the correct telemetry size
  if (len == sizeof(telemetry_message)) {
    memcpy(&incomingTelemetry, data, sizeof(incomingTelemetry));
    
    // Print the motor pulses to the Serial Monitor
    Serial.print("Telemetry | TR: "); Serial.print(incomingTelemetry.motorTR);
    Serial.print(" | TL: "); Serial.print(incomingTelemetry.motorTL);
    Serial.print(" | BR: "); Serial.print(incomingTelemetry.motorBR);
    Serial.print(" | BL: "); Serial.println(incomingTelemetry.motorBL);
  }
}

// Set up ----------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register Drone as peer
  memcpy(peerInfo.peer_addr, droneAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Controller Ready! Reading joysticks...");
}

// Main Loop -------------------------------------------------------------------------------------------------------------------------------------------

void loop() {
  // Read Joysticks
  int RawThrottle = analogRead(PIN_THROTTLE);
  int RawPitch    = analogRead(PIN_PITCH);
  int RawRoll     = analogRead(PIN_ROLL);

  // Throttle Logic
  // The centered joystick reads around 2048 so aprox change would be blow 1800 or above 2200
  if (RawThrottle > 2200) {
    CurrentThrottle += 10; // Pushing up increases throttle
  } else if (RawThrottle < 1800) {
    CurrentThrottle -= 10; // Pulling down decreases throttle
  }

  // Keep the counter between 0 and 1000
  CurrentThrottle = constrain(CurrentThrottle, 0, 1000);
  outgoingData.throttle = (int)CurrentThrottle;

  // Pitch and Roll remain mapped directly to the stick position
  outgoingData.pitch    = map(RawPitch, 0, 4095, -1000, 1000);
  outgoingData.roll     = map(RawRoll, 0, 4095, -1000, 1000);

  // Add a small deadband so the drone doesn't drift when sticks are centered
  if (abs(outgoingData.pitch) < 500) outgoingData.pitch = 0;
  if (abs(outgoingData.roll) < 500)  outgoingData.roll = 0;

  //Sending to the drone
  esp_now_send(droneAddress, (uint8_t *) &outgoingData, sizeof(outgoingData));

  // Run at ~20Hz (every 50ms) so we don't flood the Wi-Fi channel
  delay(50);
}
