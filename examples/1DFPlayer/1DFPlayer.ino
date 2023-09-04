#include "SoftwareUARTDFPlayer.h"

#define TRANSMIT_PIN 23
#define RECEIVE_PIN 4

SoftwareUARTDFPlayer mp3Effects(TRANSMIT_PIN, RECEIVE_PIN);

void setup() {
  Serial.begin(9600); // Para fins de depuração
  mp3Effects.begin();
  mp3Effects.write(0x00);
  if (!mp3Effects.Reset()){
    Serial.println("\nERROR");
    while(1);
  }
  Serial.println("\nINICIALIZADO");
  mp3Effects.Volume(30);
}

void loop(){
  delay(5000);
  Serial.println("Folder 1, file 1");
  mp3Effects.Play(1, 1);
  delay(5000);
  Serial.println("Folder 1, file 2");
  mp3Effects.Play(1, 2);
  delay(5000);
  Serial.println("Folder 1, file 3");
  mp3Effects.Play(1, 3);
}
