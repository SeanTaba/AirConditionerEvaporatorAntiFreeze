#include <OneWire.h>
#include <Arduino.h>

OneWire ds(2);
OneWire ds2(3);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    pinMode(13,OUTPUT);
    digitalWrite(13,HIGH);
}

void loop() {
    byte i;
    byte data[12],data2[12];
    byte addr[8],addr2[8];

    while(!ds.search(addr)){
        ds.reset_search();
//    Serial1.println("retrying to find sensor1");
        delay(250);}
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial1.print("CRC is not valid for sensor1!\n");
        return;
    }
    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end

    delay(1000);

    ds.reset();
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
    }

    while(!ds2.search(addr2))
    {
        ds2.reset_search();
        delay(250);
    }
    if ( OneWire::crc8( addr2, 7) != addr2[7]) {
        Serial1.print("CRC is not valid for sensor2!\n");
        return;
    }
    ds2.reset();
    ds2.select(addr2);
    ds2.write(0x44,1);

    delay(1000);

    ds2.reset();
    ds2.select(addr2);
    ds2.write(0xBE);

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data2[i] = ds2.read();
    }

    int16_t intTemperature1 = (data[1] << 8u) + data[0];
    double temperature1 = intTemperature1 / 16.0;
    Serial1.print(millis());
    Serial1.print(",");
    Serial1.print(temperature1);
    Serial1.print(",");

    int16_t intTemperature2 = (data2[1] << 8u) + data2[0];
    double temperature2 = intTemperature2 / 16.0;
    Serial1.println(temperature2);

    if (temperature1 <= 0 | temperature2 <= 0)
    {
        digitalWrite(13,LOW);
    } else if (temperature1 >= 5 & temperature2 >= 5)
        digitalWrite(13,HIGH);
}