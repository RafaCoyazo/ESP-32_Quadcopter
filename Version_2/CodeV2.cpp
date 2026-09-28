#include <ELECHOUSE_CC1101_SRC_DRV.h>

const char message[] = "Hello World";

void setup() {
  Serial.begin(9600);
  delay(1000);

  // pinMode(10, OUTPUT);
  ELECHOUSE_cc1101.setSpiPin(13, 12, 11, 8);

  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("Connection OK");
  } else {
    Serial.println("Connection Error - Check Wiring!");
  }

  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setCCMode(1);        
  ELECHOUSE_cc1101.setModulation(0);    
  ELECHOUSE_cc1101.setMHZ(433.92);     
  ELECHOUSE_cc1101.setSyncMode(2);     
  ELECHOUSE_cc1101.setCrc(1);           

  Serial.println("Transmitting 'Hello World' every 1 second...");
}

void loop() {
  // Transmit the string
  ELECHOUSE_cc1101.SendData((byte*)message, strlen(message), 100);

  Serial.print("Sent: ");
  Serial.println(message);

  // Wait 1000 milliseconds (1 second) before sending again
  delay(1000); 
}
