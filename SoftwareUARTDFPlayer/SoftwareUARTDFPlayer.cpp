#include "SoftwareUARTDFPlayer.h"

SoftwareUARTDFPlayer::SoftwareUARTDFPlayer(uint8_t transmitPin, uint8_t receivePin) {
  _transmitPin = transmitPin;
  _receivePin = receivePin;

  _bitPeriod = 1000000 / 9600; // 9600 bps
  _halfBitPeriod = _bitPeriod / 2;
}

bool SoftwareUARTDFPlayer::Reset(void){
  byte buf[10] = {0x7E, 0xFF, 0x06, 0x0C, 0x01, 0x00, 0x00, 0xFE, 0xEE, 0xEF};

  for (int i = 0; i < 10; i++) 
    write(buf[i]);
  byte rxbuf[10] = {0x7E, 0xFF, 0x06, 0x41, 0x00, 0x00, 0x00, 0xFE, 0xBA, 0xEF};
  for (int i = 0; i < 10; i++){
    if (rxbuf[i] != read())
      return false; //retorna erro se houver diferenca do dado da resposta com o esperado
  }
  return true; //retorna que o reset foi ok
}

void SoftwareUARTDFPlayer::Volume(byte volume){
  byte buf[10] = {0x7E, 0xFF, 0x06, 0x06, 0x01, 0x00, volume, 0xFE, 0x00, 0xEF};
  byte checksum = 0;

  for(int i = 1; i < 7; i++)
    checksum += buf[i];
  buf[8] = 0xFF - checksum + 1;
  for (int i = 0; i < 10; i++)
    write(buf[i]);
}

void SoftwareUARTDFPlayer::Play(byte folder, byte file){
 byte buf[10] = {0x7E, 0xFF, 0x06, 0x0F, 0x01, folder, file, 0xFE, 0x00, 0xEF};
  byte checksum = 0;

  for(int i = 1; i < 7; i++)
    checksum += buf[i];
  buf[8] = 0xFF - checksum + 1;
  for (int i = 0; i < 10; i++)
    write(buf[i]);
}

void SoftwareUARTDFPlayer::begin() {
  pinMode(_transmitPin, OUTPUT);
  pinMode(_receivePin, INPUT);
  digitalWrite(_transmitPin, HIGH);
  _isTransmitting = false;
  delay(100);
}

void SoftwareUARTDFPlayer::end() {
  pinMode(_transmitPin, INPUT);
  pinMode(_receivePin, INPUT);
}

void SoftwareUARTDFPlayer::write(uint8_t data) {
  if (!_isTransmitting) {
    sendStartBit();
    sendByte(data);
    sendStopBit();
  }
}

uint8_t SoftwareUARTDFPlayer::read() {
  uint8_t data = 0;

  while (digitalRead(_receivePin)); // Wait for start bit

  if (!digitalRead(_receivePin)) {
    //delayMicroseconds(_halfBitPeriod); // Align to middle of start bit

    for (byte i = 0; i < 8; i++) {
      delayMicroseconds(_bitPeriod);
      data |= digitalRead(_receivePin) << i; // Read each bit
    }
  }

  delayMicroseconds(_bitPeriod);
  return data;
}

bool SoftwareUARTDFPlayer::available() {
  return !digitalRead(_receivePin);
}

void SoftwareUARTDFPlayer::sendStartBit() {
  digitalWrite(_transmitPin, LOW);
  delayMicroseconds(_bitPeriod);
}

void SoftwareUARTDFPlayer::sendStopBit() {
  digitalWrite(_transmitPin, HIGH);
  delayMicroseconds(_bitPeriod);
}

void SoftwareUARTDFPlayer::sendByte(uint8_t data) {
  for (byte i = 0; i < 8; i++) {
    digitalWrite(_transmitPin, data & 1);
    delayMicroseconds(_bitPeriod);
    data >>= 1;
  }
}
