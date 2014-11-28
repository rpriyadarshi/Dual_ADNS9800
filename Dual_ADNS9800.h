#include "ADNS9800.h"

const uint16_t x_scale = 1;
const uint16_t y_scale = 1;

template <const int SS, const int MOT, const int RST>
class adns_ctrl : 
public adns::controller<SS, MOT, RST> {
public:
  adns_ctrl() {
  }
  ~adns_ctrl() {
  }

  void setup();
  void loop();

  uint16_t get_x() const;
  uint16_t get_y() const;
  uint16_t get_x_dist() const;
  uint16_t get_y_dist() const;
  uint16_t get_squal() const;

  void set_xy(uint16_t x, uint16_t y);
  void set_xy_dist(uint16_t x_sum, uint16_t y_sum);
  void set_squal(byte s);

  void clear();
  void print() const;

private:
  void print_header() const;
  void print_xy() const;
  void print_xy_dist() const;
  void print_squal() const;
  void print_nl() const;

private:
  uint16_t _x;
  uint16_t _y;
  uint16_t _x_dist;
  uint16_t _y_dist;
  byte _squal;
};

template <const int SS, const int MOT, const int RST>
uint16_t adns_ctrl<SS, MOT, RST>::get_x() const
{
  return _x;
}

template <const int SS, const int MOT, const int RST>
uint16_t adns_ctrl<SS, MOT, RST>::get_y() const
{
  return _y;
}

template <const int SS, const int MOT, const int RST>
uint16_t adns_ctrl<SS, MOT, RST>::get_x_dist() const
{
  return _x_dist;
}

template <const int SS, const int MOT, const int RST>
uint16_t adns_ctrl<SS, MOT, RST>::get_y_dist() const
{
  return _y_dist;
}

template <const int SS, const int MOT, const int RST>
uint16_t adns_ctrl<SS, MOT, RST>::get_squal() const
{
  return _squal;
}


template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::set_xy(uint16_t x, uint16_t y)
{
  _x = x;
  _y = y;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::set_xy_dist(uint16_t x_dist, uint16_t y_dist)
{
  _x_dist = x_dist;
  _y_dist = y_dist;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::set_squal(byte s)
{
  _squal = s;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::clear()
{
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_header() const
{
  Serial.print("@D,");
  Serial.print(SS);
  Serial.print(",");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_xy() const
{
  int16_t x = adns::controller<SS, MOT, RST>::convert_twos_compliment(_x);
  int16_t y = adns::controller<SS, MOT, RST>::convert_twos_compliment(_y);
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_xy_dist() const
{
  int16_t x_dist = adns::controller<SS, MOT, RST>::convert_twos_compliment(_x_dist);
  int16_t y_dist = adns::controller<SS, MOT, RST>::convert_twos_compliment(_y_dist);
  Serial.print(x_dist);
  Serial.print(",");
  Serial.print(y_dist);
  Serial.print("");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_squal() const
{
  Serial.print(_squal);
  Serial.print(",");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_nl() const
{
  Serial.println(",00000000");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print() const
{
  print_header();
  print_squal();
  //print_xy();
  print_xy_dist();
  adns::controller<SS, MOT, RST>::print_time();
  print_nl();
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::setup()
{
  adns::controller<SS, MOT, RST>::setup();
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::loop()
{
  adns::controller<SS, MOT, RST>::loop();
}





