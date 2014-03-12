#include <SPI.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "Dual_ADNS9800.h"

adns_ctrl<9, 0> ac9;
adns_ctrl<10, 1> ac10;

void setup() {
    Serial.begin(9600);
    ac9.setup();
    ac10.setup();
}

void loop() {
    ac9.loop();
    ac10.loop();
}


