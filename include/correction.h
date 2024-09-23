#pragma once
#include "main.h"

void Correction(bool select){
  int value = 0 ;
  DBR_EE_write.update();
  readModbusRegisters();
  //-------------считывание значения из памяти
  EEPROM.get(select == LOW_SPINDEL? LOW_SPINDEL_ADDRES: TOP_SPINDEL_ADDRES, value); 
  for(int i = 0; i < 1000; i++){
    disp.clear();
    disp.print(value);
    disp.update();
    DBR_ff_button.update();
    DDBR_rew_button.update();
    DBR_EE_write.update();
    //коррекция "+""-"" : одно нажатие - 8 шагов(0,1мм)
    if(DBR_ff_button.fell()){
      value -= 8;
      i = 0;
    }
    else if(DDBR_rew_button.fell()){
      value += 8;
      i = 0;
    }
    else if(DBR_EE_write.fell()){
      EEPROM.put(select == LOW_SPINDEL? LOW_SPINDEL_ADDRES: TOP_SPINDEL_ADDRES, value);
      disp.clear();
      disp.print("done");
      disp.update();
      delay(1000);
      return;
    }
  }
  disp.clear();
  disp.print("end");
  disp.update();
  delay(1000);
  return;
}