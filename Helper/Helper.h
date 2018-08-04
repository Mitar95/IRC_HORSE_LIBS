#include <Arduino.h>
#include "LogFormat.h"

typedef struct TimeFormat{
    int year;
    int mounth;
    int day;
    int hour;
    int minute;
    int second;
    int end_hour;
    int end_minute;
}TimeFormat;

typedef struct CapacityTable{
    float SENSOR_DS3;
    float SENSOR_MS1;
    float SENSOR_AS1;
    float SENSOR_HS11;

    uint16_t FAN_DFN1;
    uint16_t FAN_MFN1;
    uint16_t FAN_AFN1;
    uint16_t FAN_HFN3;

    float MLIN_MM1;
}CapacityTable;

void SetEndTime(TimeFormat* tf, int duration){
    tf->end_minute += duration;
    if(tf->end_minute >= 60){
        tf->end_minute -= 60;
        tf->end_hour++;
        if(tf->end_hour >= 24){
            tf->end_hour = 0;
        }
    }
}

HardwareSerial* InitSerial(HardwareSerial *ser, unsigned int baud){
    // Open serial communications and wait for port to open:
    ser->begin(baud);
    while (!ser) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    return ser;
}

void PrintSensor(HardwareSerial* console, String id, float value, bool isSent){
    if(isSent){
        console->print("Da " + id + "\t");
        console->println(value, 1);
    }else{
        console->print("Ne " + id + "\t");
        console->println(value, 1);

    }
}

void EndProgram(){
    while(1){
        delay(1000);
    }
}
