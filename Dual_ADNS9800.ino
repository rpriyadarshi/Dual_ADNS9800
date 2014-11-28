#include <SPI.h>
#include <avr/pgmspace.h>
#include "Dual_ADNS9800.h"

class dual_adns {
public:
  dual_adns() { 
    reset(); 
  };
  ~dual_adns() {
  }

  void setup();
  void loop();
  void reset();
  void calculate();
  void print() const;

private:
  adns_ctrl<4, 0, 6> _ac_b;
  adns_ctrl<5, 1, 6> _ac_a;

  struct _adns_data {
    int16_t _x_dist;
    int16_t _y_dist;
    long _time;
  };

  struct _adns_cache {
    struct _adns_data _curr;
    struct _adns_data _prev;
  };

  struct _calc_cache {
    int16_t _x_dist;
    int16_t _y_dist;
    long _time;
    long _diff_time;
    double _angle;
  };

  struct _adns_cache _a;
  struct _adns_cache _b;
  struct _calc_cache _calc;
};

void dual_adns::reset() {
  memset(&_a, 0, sizeof(_a));
  memset(&_b, 0, sizeof(_b));
  memset(&_calc, 0, sizeof(_calc));
}

void dual_adns::calculate() {
  if (_ac_a.get_reset() || _ac_b.get_reset()) {
    reset();
  }
  _a._curr._x_dist = _ac_a.convert_twos_compliment(_ac_a.get_x_dist());
  _b._curr._x_dist = _ac_b.convert_twos_compliment(_ac_b.get_x_dist());
  _a._curr._y_dist = _ac_a.convert_twos_compliment(_ac_a.get_y_dist());
  _b._curr._y_dist = _ac_b.convert_twos_compliment(_ac_b.get_y_dist());
  _a._curr._time = _ac_a.get_time();
  _b._curr._time = _ac_b.get_time();

  _calc._x_dist = (_a._curr._x_dist + _b._curr._x_dist) / 2;
  _calc._y_dist = (_a._curr._y_dist + _b._curr._y_dist) / 2;
  _calc._time = (_a._curr._time + _b._curr._time) / 2;
  _calc._diff_time = _b._curr._time - _a._curr._time;
  _calc._angle = atan2(_b._curr._y_dist - _a._curr._y_dist, 
    _b._curr._x_dist - _a._curr._x_dist);

  _a._prev._x_dist = _a._curr._x_dist;
  _b._prev._x_dist = _b._curr._x_dist;
  _a._prev._y_dist = _a._curr._y_dist;
  _b._prev._y_dist = _b._curr._y_dist;
  _a._prev._time = _a._curr._time;
  _b._prev._time = _b._curr._time;
}

void dual_adns::print() const {  
  Serial.print("@M,");
  Serial.print(_calc._x_dist);
  Serial.print(",");
  Serial.print(_calc._y_dist);
  Serial.print(",");
  Serial.print(_calc._time);
  Serial.print(",");
  Serial.print(_calc._diff_time);
  Serial.print(",");
  Serial.print(_calc._angle);
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
    calculate();
    print();
  }
}

dual_adns da;
void setup() {
  da.setup();
}

void loop() {
  da.loop();
}







