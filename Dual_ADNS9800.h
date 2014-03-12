#include "ADNS9800.h"

const int16_t x_scale = 1;
const int16_t y_scale = 1;

template <const int NCS>
class adns_ctrl : 
public adns::controller<NCS> {
public:
    adns_ctrl() {
    }
    ~adns_ctrl() {
    }

    void setup();
    void loop();

    void get_xy(int16_t x, int16_t y);
    void get_xy_dist(int16_t x_sum, int16_t y_sum);
    void get_squal(uint16_t s);
    void get_fault();
    void clear();

private:
    void print_xy_serial();
    void print_xy_dist_serial();
    void print_squal_serial();

private:
    int16_t _x;
    int16_t _y;
    int16_t _x_dist;
    int16_t _y_dist;
    uint16_t _squal;
};

template <const int NCS>
void adns_ctrl<NCS>::get_xy(int16_t x, int16_t y)
{
    _x = x;
    _y = y;
}

template <const int NCS>
void adns_ctrl<NCS>::get_xy_dist(int16_t x_dist, int16_t y_dist)
{
    _x_dist = x_dist;
    _y_dist = y_dist;
    print_xy_dist_serial();
}

template <const int NCS>
void adns_ctrl<NCS>::get_squal(uint16_t s)
{
    _squal = s;
    print_squal_serial();
}

template <const int NCS>
void adns_ctrl<NCS>::get_fault()
{
    Serial.println("XY_LASER is shorted to GND");
}

template <const int NCS>
void adns_ctrl<NCS>::clear()
{
}

template <const int NCS>
void adns_ctrl<NCS>::print_xy_serial()
{
    Serial.print("x = ");
    Serial.print(_x / x_scale);
    Serial.print(" | ");
    Serial.print("y = ");
    Serial.println(_y / y_scale);
}

template <const int NCS>
void adns_ctrl<NCS>::print_xy_dist_serial()
{
    Serial.print("x = ");
    Serial.print(_x_dist / x_scale);
    Serial.print(" | ");
    Serial.print("y = ");
    Serial.println(_y_dist / y_scale);
}

template <const int NCS>
void adns_ctrl<NCS>::print_squal_serial()
{
    Serial.print("SQUAL:");
    Serial.println(_squal);
}

template <const int NCS>
void adns_ctrl<NCS>::setup()
{
    adns::controller<NCS>::setup();
}

template <const int NCS>
void adns_ctrl<NCS>::loop()
{
    adns::controller<NCS>::loop();
}


