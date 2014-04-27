#include <SPI.h>
#include <avr/pgmspace.h>
#include "Dual_ADNS9800.h"

adns_ctrl<4, 0, 6> ac_a;
adns_ctrl<5, 1, 6> ac_b;

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(8);

  ac_b.setup();
  ac_a.setup();
  
  delay(100);
  
  ac_b.finish();
  ac_a.finish();
}

void loop() {
  ac_a.loop();
  ac_b.loop();
}




