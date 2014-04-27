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

  void get_xy(uint16_t x, uint16_t y);
  void get_xy_dist(uint16_t x_sum, uint16_t y_sum);
  void get_squal(byte s);

  void clear();
  void print();

private:
  void print_header();
  void print_xy();
  void print_xy_dist();
  void print_squal();
  void print_nl();

private:
  uint16_t _x;
  uint16_t _y;
  uint16_t _x_dist;
  uint16_t _y_dist;
  byte _squal;
};

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::get_xy(uint16_t x, uint16_t y)
{
  _x = x;
  _y = y;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::get_xy_dist(uint16_t x_dist, uint16_t y_dist)
{
  _x_dist = x_dist;
  _y_dist = y_dist;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::get_squal(byte s)
{
  _squal = s;
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::clear()
{
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_header()
{
  Serial.print("@ ");
  Serial.print(SS);
  Serial.print(" ");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_xy()
{
  int16_t x = adns::controller<SS, MOT, RST>::convert_twos_compliment(_x);
  int16_t y = adns::controller<SS, MOT, RST>::convert_twos_compliment(_y);
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_xy_dist()
{
  int16_t x_dist = adns::controller<SS, MOT, RST>::convert_twos_compliment(_x_dist);
  int16_t y_dist = adns::controller<SS, MOT, RST>::convert_twos_compliment(_y_dist);
  Serial.print(x_dist);
  Serial.print(" ");
  Serial.print(y_dist);
  Serial.print(" ");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_squal()
{
  Serial.print(_squal);
  Serial.print(" ");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print_nl()
{
  Serial.println("");
}

template <const int SS, const int MOT, const int RST>
void adns_ctrl<SS, MOT, RST>::print()
{
  print_header();
  print_squal();
  print_xy();
  print_xy_dist();
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




