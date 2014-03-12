#include <SPI.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "Dual_ADNS9800.h"

adns_ctrl<9> ac;

void setup() {
    Serial.begin(9600);
    ac.setup();
}

void loop() {
    ac.loop();
}


