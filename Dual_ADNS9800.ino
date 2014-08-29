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
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  ac_a.setup();
  ac_b.setup();

  delay(100);

  ac_a.finish();
  ac_b.finish();
}

template <const int SSa, const int MOTa, const int RSTa, const int SSb, const int MOTb, const int RSTb>
void print_avg(adns_ctrl<SSa, MOTa, RSTa>& a, adns_ctrl<SSb, MOTb, RSTb>& b) {  
  int16_t ax_dist = a.convert_twos_compliment(a.get_x_dist());
  int16_t bx_dist = b.convert_twos_compliment(b.get_x_dist());
  int16_t ay_dist = a.convert_twos_compliment(a.get_y_dist());
  int16_t by_dist = b.convert_twos_compliment(b.get_y_dist());

  Serial.print("@M,");
  Serial.print((ax_dist + bx_dist) / 2);
  Serial.print(",");
  Serial.print((ay_dist + by_dist) / 2);
  Serial.print(",");
  Serial.print(micros());
  Serial.println(",00000000");
}

void loop() {
  ac_a.loop();
  ac_b.loop();
  if (ac_a.get_moved() || ac_b.get_moved()) {
    print_avg(ac_a, ac_b);
  }
}







