#include "mhz19b.h"

MHZ19B::MHZ19B()
{

}

void MHZ19B::setup(HardwareSerial* hs, int8_t tx, int8_t rx)
{
  this->serial = hs;
  this->serial->begin(9600, SERIAL_8N1, tx, rx);

  delay(500);
  uint32_t data_len = this->serial->available();
  if(data_len != 0){
    for(uint32_t i = 0; i < data_len; i++){
        uint8_t dummy = (uint8_t)this->serial->read();
    }
  }
}

bool MHZ19B::checksum(uint8_t data[], size_t size) {
  uint8_t sum = 0;

  for (int i = 0; i < size - 1; i++){
    sum = (sum + data[i]) & 0xff;
  }
  uint8_t c_sum = 0xff - sum;
  if (c_sum == data[size - 1]) return true;
  else return false;
}

uint16_t MHZ19B::read()
{
  const size_t SIZE = 9;
  uint8_t read_cmd[SIZE] = {0xFF, 0x01, 0x86, 0, 0, 0, 0, 0, 0x79};
  for (int i = 0; i < SIZE; i++){
    this->serial->write(read_cmd[i]);
  }

  uint8_t read_data[SIZE] = {0};
  delay(100);
  uint32_t data_len = this->serial->available();
  if(data_len != 0){
    for(int i = 0; i < data_len; i++){
        read_data[i] = (uint8_t)this->serial->read();
    }
  }
  uint16_t concentration = 0;

  //check read data
  if (read_data[0] == 0xff && read_data[1] == 0x86 && checksum(read_data, SIZE)){
    concentration = (read_data[2] << 8) + read_data[3];
  }
  return concentration;
}
