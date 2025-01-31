#pragma once
#include "main.h"

int editDrill;

void drill_tuning(){
      disp.clear();
      disp.print(drillSpinCounter);
      disp.update();
      DBR_EE_write.update();
      DBR_select.update();
      DBR_on_start_pos.update();
      DBR_ff_button.update();
      DBR_rew_button.update();
      EEPROM.get(DRILL_EDIT_ADDRES,editDrill);
      //------------------------запись значения выбега
      if (DBR_EE_write.fell()) {
        EEPROM.put(DRILL_ADDRES,drillSpinCounter + run_out + editDrill );
        disp.clear();
        disp.print("gone");
        disp.update();
        delay(1000);
      }
      //----------------------- возврат на исходную  
      if (DBR_on_start_pos.fell() && DBR_drill_zero_sens.read() == HIGH ){ 
            disp.clear();
            disp.print("on O");
            disp.update();
            Serial.println("run to O");
        for(int i = 0; i < (drillSpinCounter + run_out + 80 ); i++){
          DBR_drill_zero_sens.update();
          if(DBR_drill_zero_sens.read() == LOW){
            drillSpinCounter = 0;
            Serial.println("on O");
            break;
          }
          if(i/100 == 0)Watchdog.reset();

          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          delay(1);
        }
        for(int i = 0; i < run_out; i++){
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          delay(1);
        }             
      }
      //------------------------------- кнопка назад
      else if (DBR_ff_button.read() == LOW) {
        Serial.println("drill revers");
        while (DBR_ff_button.read() == LOW){
          disp.clear();
          disp.print(drillSpinCounter);
          disp.update();
          DBR_ff_button.update();
          DBR_drill_zero_sens.update();
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(10);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          //delay(1);
          if (DBR_drill_zero_sens.read() == HIGH && drillSpinCounter > 0)drillSpinCounter--;
          Watchdog.reset();
        }
      }
      //-----------------------------кнопка вперед
      else if (DBR_rew_button.read() == LOW){
        Serial.println("drill forward");
        while (DBR_rew_button.read() == LOW){
          disp.clear();
          disp.print(drillSpinCounter);
          disp.update();
          DBR_rew_button.update();
          DBR_drill_zero_sens.update();
          digitalWrite(DRILL_DIR_OUT, HIGH);
          delayMks(10);
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(10);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          digitalWrite(DRILL_DIR_OUT, LOW);
          //delay(1);
          if (DBR_drill_zero_sens.read() == HIGH) drillSpinCounter++;
          Watchdog.reset();
        }
      }
}