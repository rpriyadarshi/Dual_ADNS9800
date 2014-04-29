#include "ADNS9800_SROM_A4.h"

extern const unsigned short firmware_length;
extern prog_uchar firmware_data[];

namespace adns {
  // Registers
#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel                        0x0a
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20 // Should be written as 0x80 (Bit 7 complementary to Bit 7 in LASER_CTRL1, which is 0) 
#define REG_LASER_CTRL1                          0x21 // Not in Datasheet (Bit 7 must be Complement to Bit 7 of LASER_CTRL0, Bit 6 must be 1, Bit 2 must apparently be 1, too) (After inititialisation the register is 0x42) 
#define REG_LASER_CFG0                           0x22 // Not in Datasheet (Laser Power Level) 
#define REG_LASER_CFG1                           0x23 // Not in Datasheet (Must be Complement of LASER_CFG0) 
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64

  // | ADNS-9800 | Arduino Uno Pins
  // | SS        | 4, 5 -> Green wires
  // | MO        | 11
  // | SC        | 13
  // | MI        | 12
  // | MOT       | 2 (attachInterrupt() at int.0 = pin 2; int.1 = pin 3)
  // | VI        |+5V (First You must Activate 5V Mode)
  // | AG        | Gnd
  // | DG        | Gnd

  template <const int SS, const int MOT, const int RST>
    class controller {
public:
    enum MotionBurst {
      Motion = 0, Observation, 
      Delta_X_L, Delta_X_H, Delta_Y_L, Delta_Y_H, SQUAL, 
      Pixel_Sum, Maximum_Pixel, Minimum_Pixel, 
      Shutter_Upper, Shutter_Lower, 
      Frame_Period_Upper, Frame_Period_Lower, EndData
    };

    controller() {
    }
    ~controller() {
    }

    void setup();
    void finish();
    void loop();

    virtual void get_xy(uint16_t x, uint16_t y) = 0;
    virtual void get_xy_dist(uint16_t x_sum, uint16_t y_sum) = 0;
    virtual void get_squal(byte s) = 0;

    virtual void clear() = 0;
    virtual void print() = 0;

    void reset_xy_dist();

protected:
    int16_t convert_twos_compliment(uint16_t u);

private:
    void upload_firmware();
    void perform_startup();
    void display_registers();

    static void com_begin();
    static void com_end();
    static byte read_reg(byte reg_addr);
    static void write_reg(byte reg_addr, byte data);
    static void read_motion_burst_data();
    static void copy_data();
    static void update_motion_burst_data();
    static uint16_t join_byte(byte l, byte h);

    void print_observation();

    void print_fault();
    void print_lp_valid();
    void print_op_mode();
    void print_frame_pix_first();

private:
    static byte _boot_complete;
    static byte _data[EndData];
    static uint16_t _ux;
    static uint16_t _uy;
    static uint16_t _ux_dist;
    static uint16_t _uy_dist;

    static byte _motion;
    static byte _fault;
    static byte _lp_valid;
    static byte _mode;
    static byte _frame_pix_first;
    static byte _squal;
    static byte _observation;
    static byte _moved;

    static int _reset;
  };

  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_boot_complete = 0;

  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_data[EndData];
  template <const int SS, const int MOT, const int RST>
    uint16_t controller<SS, MOT, RST>::_ux = 0;
  template <const int SS, const int MOT, const int RST>
    uint16_t controller<SS, MOT, RST>::_uy = 0;
  template <const int SS, const int MOT, const int RST>
    uint16_t controller<SS, MOT, RST>::_ux_dist = 0;
  template <const int SS, const int MOT, const int RST>
    uint16_t controller<SS, MOT, RST>::_uy_dist = 0;

  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_motion = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_fault = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_lp_valid = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_mode = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_frame_pix_first = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_squal = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_observation = 0;
  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::_moved = 0;

  template <const int SS, const int MOT, const int RST>
    int controller<SS, MOT, RST>::_reset = HIGH;

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::reset_xy_dist() {
    _ux = _uy = _ux_dist = _uy_dist = 0;
    _reset = HIGH;
    Serial.print("# [");
    Serial.print(SS);
    Serial.println("] RESET");
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::com_begin() {
    digitalWrite(SS, LOW);
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::com_end() {
    digitalWrite(SS, HIGH);
  }

  template <const int SS, const int MOT, const int RST>
    byte controller<SS, MOT, RST>::read_reg(byte reg_addr) {
    com_begin();

    // send adress of the register, with MSBit = 0 to indicate it's a read
    SPI.transfer(reg_addr & 0x7f);
    delayMicroseconds(100); // tSRAD
    // read data
    byte data = SPI.transfer(0);

    delayMicroseconds(1); // tSCLK-_ncs for read operation is 120ns
    com_end();
    delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-_ncs

    return data;
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::write_reg(byte reg_addr, byte data) {
    com_begin();

    // send adress of the register, with MSBit = 1 to indicate it's a write
    SPI.transfer(reg_addr | 0x80);
    // sent data
    SPI.transfer(data);

    delayMicroseconds(20); // tSCLK-_ncs for write operation
    com_end();
    delayMicroseconds(100); // tSWW/tSWR (=120us) minus tSCLK-_ncs. Could be shortened, but is looks like a safe lower bound 
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::read_motion_burst_data() {
    com_begin();

    // send adress of the register, with MSBit = 1 to indicate it's a write
    SPI.transfer(REG_Motion_Burst & 0x7f);
    delayMicroseconds(100); // tSRAD
    // read data
    for (int i = 0; i < Pixel_Sum; ++i) {
      _data[i] = SPI.transfer(0);
    }

    com_end();
    delayMicroseconds(1); //  tBEXIT
  }

  template <const int SS, const int MOT, const int RST>
    uint16_t controller<SS, MOT, RST>::join_byte(byte l, byte h){
    uint16_t b = h;
    b <<= 8;
    b |= l;
    return b;
  }

  template <const int SS, const int MOT, const int RST>
    int16_t controller<SS, MOT, RST>::convert_twos_compliment(uint16_t b){
    //Convert from 2's complement
    if (b & 0x8000) {
      //return -1 * ((b ^ 0xffff) + 1);
      return -(0x8000 - (0x7fff & b));
    } 
    else {
      return b;
    }
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::copy_data()
  {
    _ux = join_byte(_data[Delta_X_L], _data[Delta_X_H]);
    _uy = join_byte(_data[Delta_Y_L], _data[Delta_Y_H]);
    _ux_dist += _ux;
    _uy_dist += _uy;
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::update_motion_burst_data() {
    if(_boot_complete != SS) return;
    read_motion_burst_data();

    _observation = _data[Observation];
    _motion = _data[Motion] & 0x80;
    _fault = _data[Motion] & 0x40;
    _lp_valid = _data[Motion] & 0x20;

    if (!_fault && _motion && _lp_valid && (_observation == 0x7f)) {
      _mode = _data[Motion] & 0x02;
      _mode |= (_data[Motion] & 0x04 << 1);
      _frame_pix_first = _data[Motion] & 0x01;
      _squal = _data[SQUAL];
      copy_data();
      _moved = 1;
    }
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::upload_firmware() {
    // send the firmware to the chip, cf p.18 of the datasheet
    Serial.print("# [");
    Serial.print(SS);
    Serial.println("] Uploading firmware...");

    // set the configuration_IV register in 3k firmware mode
    write_reg(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 

    // write 0x1d in SROM_enable reg for initializing
    write_reg(REG_SROM_Enable, 0x1d); 

    // wait for more than one frame period
    delay(10); // assume that the frame rate is as low as 100fps... even if it should never be that low

    // write 0x18 to SROM_enable to start SROM download
    write_reg(REG_SROM_Enable, 0x18); 

    // write the SROM file (=firmware data) 
    com_begin();
    SPI.transfer(REG_SROM_Load_Burst | 0x80); // write burst destination adress
    delayMicroseconds(15);

    // send all bytes of the firmware
    unsigned char c;
    for(unsigned int i = 0; i < firmware_length; i++){
      c = (unsigned char)pgm_read_byte(firmware_data + i);
      SPI.transfer(c);
      delayMicroseconds(15);
    }

    com_end();
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::perform_startup() {
    com_end(); // ensure that the serial port is reset
    com_begin(); // ensure that the serial port is reset
    com_end(); // ensure that the serial port is reset
    write_reg(REG_Power_Up_Reset, 0x5a); // force reset
    delay(50); // wait for it to reboot
    // read registers 0x02 to 0x06 (and discard the data)
    read_reg(REG_Motion);
    read_reg(REG_Delta_X_L);
    read_reg(REG_Delta_X_H);
    read_reg(REG_Delta_Y_L);
    read_reg(REG_Delta_Y_H);
    // upload the firmware
    upload_firmware();
    delay(10);
    //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
    // reading the actual value of the register is important because the real
    // default value is different from what is said in the datasheet, and if you
    // change the reserved bytes (like by writing 0x00...) it would not work.
    byte laser_ctrl0 = read_reg(REG_LASER_CTRL0);
    write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );

    delay(1);

    // set the configuration_I register
    // 0x01 = 50, minimum
    // 0x44 = 3400, default
    // 0x8e = 7100
    // 0xA4 = 8200, maximum
    write_reg(REG_Configuration_I, 0x01);

    Serial.print("# [");
    Serial.print(SS);
    Serial.println("] Optical Chip Initialized");
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::display_registers() {
    int oreg[] = { 
      REG_Product_ID, REG_Inverse_Product_ID, REG_SROM_ID, REG_Motion, REG_LASER_CTRL0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     };
    const char* oregname[] = {
      "Product_ID","Inverse_Product_ID","SROM_Version","Motion", "LASER_CTRL0"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    };
    byte regres;

    com_begin();

    int rctr = 0;
    for(rctr = 0; rctr < 5; rctr++){
      SPI.transfer(oreg[rctr]);
      delay(1);
      Serial.print("# [");
      Serial.print(SS);
      Serial.print("] ");
      Serial.print(oregname[rctr]);
      Serial.print(" (0x");
      Serial.print(oreg[rctr],HEX);
      Serial.print(")" );
      regres = SPI.transfer(0);
      Serial.print(regres,BIN);  
      Serial.print(" (0x");
      Serial.print(regres,HEX);  
      Serial.println(")");
      delay(1);
    }
    com_end();
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::print_fault()
  {
    Serial.print("# [");
    Serial.print(SS);
    Serial.print("] ");
    Serial.println("ERROR: Fault detected, XY_LASER is shorted to GND");
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::print_lp_valid()
  {
    Serial.print("# [");
    Serial.print(SS);
    Serial.print("] ");
    Serial.println("ERROR: Laser power register values do not have complementary values");
    // Avoid too many messages
    delay(100);
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::print_op_mode()
  {
    Serial.print("# [");
    Serial.print(SS);
    Serial.print("] ");
    Serial.print("REST Mode ");
    Serial.println(_mode);
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::print_frame_pix_first()
  {
    Serial.print("# [");
    Serial.print(SS);
    Serial.print("] ");
    Serial.print("Frame Pix First ");
    Serial.println(_frame_pix_first);
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::print_observation()
  {
    Serial.print("# [");
    Serial.print(SS);
    Serial.print("] ");
    Serial.print("Observation Error [");
    Serial.print(_observation, BIN);
    Serial.println("]");
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::setup() {
    pinMode (SS, OUTPUT);
    pinMode (RST, INPUT_PULLUP);

    attachInterrupt(MOT, update_motion_burst_data, FALLING);

    perform_startup();  
    display_registers();
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::finish() {
    _boot_complete = SS;
    reset_xy_dist();
  }

  template <const int SS, const int MOT, const int RST>
    void controller<SS, MOT, RST>::loop() {
    if (_fault) {
      print_fault();
      return;
    }
    if (! _lp_valid) {
      print_lp_valid();
      return;
    }
    if (_mode) {
      print_op_mode();
      return;
    }
    if (_observation != 0x7f) {
      print_observation();
      _observation = 0x7f;
      return;
    }
    _reset = digitalRead(RST);
    if (_reset == LOW) {
      reset_xy_dist();
    }
    if (! _moved) return;
    if(_motion) {
      clear();
      get_squal(_squal);
      get_xy(_ux, _uy);
      get_xy_dist(_ux_dist, _uy_dist);
      print();
    }
    _moved = 0;
  }
};

































