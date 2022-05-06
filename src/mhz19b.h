#pragma once
#include <M5StickC.h>
#include <stdint.h>

class MHZ19B {
private:
  HardwareSerial* serial;
  bool checksum(uint8_t data[], size_t size);

public:
  MHZ19B();
  ~MHZ19B() = default;
  void setup(HardwareSerial* hs, int8_t tx, int8_t rx);
  uint16_t read();
};