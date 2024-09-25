#pragma once
#include "main.h"

void Correction(bool select){
  int value = 0 ;
  DBR_EE_write.update();
  readModbusRegisters();
  //-------------считывание значения из памяти
  EEPROM.get(select == SINK? SINK_ADDRES: DRILL_ADDRES, value); 
  for(int i = 0; i < 1000; i++){
    disp.clear();
    disp.print(value);
    disp.update();
    DBR_ff_button.update();
    DBR_rew_button.update();
    DBR_EE_write.update();
    //коррекция "+""-"" : одно нажатие - 8 шагов(0,1мм)
    if(DBR_ff_button.fell()){
      value -= 8;
      i = 0;
    }
    else if(DBR_rew_button.fell()){
      value += 8;
      i = 0;
    }
    else if(DBR_EE_write.fell()){
      EEPROM.put(select == SINK? SINK_ADDRES: DRILL_ADDRES, value);
      disp.clear();
      disp.print("done");
      disp.update();
      delay(1000);
      return;
    }
    Watchdog.reset();
  }
  disp.clear();
  disp.print("end");
  disp.update();
  delay(1000);
  return;
}