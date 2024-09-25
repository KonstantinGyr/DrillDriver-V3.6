#pragma once
#include "main.h"

void drill_tuning(){
      disp.clear();
      disp.print(topSpinCounter);
      disp.update();
      DBR_EE_write.update();
      DBR_select.update();
      DBR_on_start_pos.update();
      DBR_ff_button.update();
      DDBR_rew_button.update();
      //------------------------запись значения выбега
      if (DBR_EE_write.fell()) {
        EEPROM.put(DRILL_ADDRES,topSpinCounter + correction);
        disp.clear();
        disp.print("gone");
        disp.update();
        delay(2000);
      }
      //----------------------- возврат на исходную  
      if (DBR_on_start_pos.fell() && DBR_top_zero_sens.read() == HIGH ){ 
            disp.clear();
            disp.print("on O");
            disp.update();
            topSpinCounter = 0;
            int i = 0;
        while (DBR_top_zero_sens.read() == HIGH) {
          if(i>3000){
            disp.clear();
            disp.print("errO");
            disp.update();
            delay(100000);
            break;
          }
          DBR_top_zero_sens.update();
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          delay(1);
        }
        for(int i = 0; i < correction; i++){
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(DRILL_PULSE_OUT, LOW);
          delay(1);
        }             
      }
      //------------------------------- кнопка вперед
      else if (DBR_ff_button.read() == LOW) {
        while (DBR_ff_button.read() == LOW){
          disp.clear();
          disp.print(topSpinCounter);
          disp.update();
          DBR_ff_button.update();
          DBR_top_zero_sens.update();
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          if (DBR_top_zero_sens.read() == HIGH && topSpinCounter > 0)
            topSpinCounter--;
          digitalWrite(DRILL_PULSE_OUT, LOW);
          //delay(1);
        }
      }
      //-----------------------------кнопка назад
      else if (DDBR_rew_button.read() == LOW){
        while (DDBR_rew_button.read() == LOW){
          disp.clear();
          disp.print(topSpinCounter);
          disp.update();
          DDBR_rew_button.update();
          DBR_top_zero_sens.update();
          digitalWrite(DRILL_DIR_OUT, HIGH);
          delayMks(10);
          digitalWrite(DRILL_PULSE_OUT, HIGH);
          delayMks(50);
          if (DBR_top_zero_sens.read() == HIGH) topSpinCounter++;
          digitalWrite(DRILL_PULSE_OUT, LOW);
          //delay(1);
          digitalWrite(DRILL_DIR_OUT, LOW);
        }
      }
}