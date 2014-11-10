#include <SPI.h>
#include <avr/pgmspace.h>
#include "Dual_ADNS9800.h"

class dual_adns {
public:
  dual_adns();
  ~dual_adns() {
  }

  void setup();
  void loop();
  void calculate_avg();
  void print_avg();

private:
  adns_ctrl<4, 0, 6> _ac_b;
  adns_ctrl<5, 1, 6> _ac_a;

  struct _adns {
    int16_t _x_dist;
    int16_t _y_dist;
    long _time;
  };
  
  struct _adns_cache {
    struct _adns _curr;
    struct _adns _prev;
  };
  
  struct _adns_cache _a;
  struct _adns_cache _b;
  struct _adns _avg;
  long _diff_time;
};

dual_adns::dual_adns() {
}

void dual_adns::calculate_avg() {
  _a._curr._x_dist = _ac_a.convert_twos_compliment(_ac_a.get_x_dist());
  _b._curr._x_dist = _ac_b.convert_twos_compliment(_ac_b.get_x_dist());
  _a._curr._y_dist = _ac_a.convert_twos_compliment(_ac_a.get_y_dist());
  _b._curr._y_dist = _ac_b.convert_twos_compliment(_ac_b.get_y_dist());
  _a._curr._time = _ac_a.get_time();
  _b._curr._time = _ac_b.get_time();

  _avg._x_dist = (_a._curr._x_dist + _b._curr._x_dist) / 2;
  _avg._y_dist = (_a._curr._y_dist + _b._curr._y_dist) / 2;
  _avg._time = (_a._curr._time + _b._curr._time) / 2;
  _diff_time = _b._curr._time - _a._curr._time;

  _a._prev._x_dist = _a._curr._x_dist;
  _b._prev._x_dist = _b._curr._x_dist;
  _a._prev._y_dist = _a._curr._y_dist;
  _b._prev._y_dist = _b._curr._y_dist;
  _a._prev._time = _a._curr._time;
  _b._prev._time = _b._curr._time;
}

void dual_adns::print_avg() {  
  Serial.print("@M,");
  Serial.print(_avg._x_dist);
  Serial.print(",");
  Serial.print(_avg._y_dist);
  Serial.print(",");
  Serial.print(_avg._time);
  Serial.print(",");
  Serial.print(_diff_time);
  Serial.println(",00000000");
}

void dual_adns::setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  _ac_a.setup();
  _ac_b.setup();

  delay(100);

  _ac_a.finish();
  _ac_b.finish();
}

void dual_adns::loop() {
  _ac_a.loop();
  _ac_b.loop();
  if (_ac_a.get_moved() || _ac_b.get_moved()) {
    calculate_avg();
    print_avg();
  }
}

dual_adns da;
void setup() {
  da.setup();
}

void loop() {
  da.loop();
}






