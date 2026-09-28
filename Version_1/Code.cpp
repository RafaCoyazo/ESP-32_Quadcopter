#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // Required for printDetails()

// Trying to make connection to the antena on board
RF24 radio(8, 9); 
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("\n--- HARDWARE DIAGNOSTIC REPORT ---");
  radio.printDetails(); 
}

void loop() {
  
  const char text[32] = "Hello Diddyblud";
  radio.write(&text, sizeof(text));
  Serial.println("Data Sent: Diddy");
  delay(1000);
}
