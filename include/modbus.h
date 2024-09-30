#pragma once
#include "main.h"
#include "ModbusRTUSlave.h"

const uint8_t dePin = 13;
uint16_t holdingRegisters[4];
ModbusRTUSlave modbus(Serial,dePin);

void readModbusRegisters(){
    modbus.poll();
    delay(10);
  //   Serial.println("modbus_poll");
    EEPROM.put(DRILL_SERVO_SPEED_ADDRES,holdingRegisters[0]);
    EEPROM.put(SINK_SERVO_SPEED_ADDRES,holdingRegisters[1]);
    EEPROM.put(DRILL_WORK_ADDRES,holdingRegisters[2]);
    EEPROM.put(SINK_WORK_ADDRES,holdingRegisters[3]);
    
}