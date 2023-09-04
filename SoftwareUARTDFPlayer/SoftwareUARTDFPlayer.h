//04 september 2023
//software serial with DFPlayer mini commands
//tonmaker: @tonmaker70
//sisaewjs@hotmail.com

#ifndef SoftwareUARTDFPlayer_h
#define SoftwareUARTDFPlayer_h

#include <Arduino.h>

class SoftwareUARTDFPlayer{
  public:
    SoftwareUARTDFPlayer(uint8_t transmitPin, uint8_t receivePin);

    void begin();
    void end();
    void write(uint8_t data);
    uint8_t read();
    bool available();
    void Play(byte folder, byte file);
    bool Reset();
    void Volume(byte volume);

  private:
    uint8_t _transmitPin;
    uint8_t _receivePin;

    uint8_t _bitPeriod;
    uint16_t _halfBitPeriod;

    bool _isTransmitting;

    void sendStartBit();
    void sendStopBit();
    void sendByte(uint8_t data);
};

#endif
