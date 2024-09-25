#pragma once
#include "main.h"

void sink_tuning(){  
      disp.clear();
      disp.print(lowSpinCounter);
      disp.update();
      DBR_EE_write.update();
      DBR_select.update();
      DBR_on_start_pos.update();
      DBR_ff_button.update();
      DDBR_rew_button.update();
      //-------------------------------запись значения выбега
      if (DBR_EE_write.fell()) {
        EEPROM.put(SINK_ADDRES,lowSpinCounter + correction);
        disp.clear();
        disp.print("gone");
        disp.update();
        delay(1000);
      } 
      //-------------------------------возврат на исходную     кнопка поиск нуля
      if (DBR_on_start_pos.fell() && DBR_low_zero_sens.read() == HIGH ){  
            disp.clear();
            disp.print("on O");
            disp.update();
            lowSpinCounter = 0;
            int i = 0;
        while (DBR_low_zero_sens.read() == HIGH) {
          if(i>3000){
            disp.clear();
            disp.print("errO");
            disp.update();
            delay(100000);
            break;
          }
          DBR_low_zero_sens.update();
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(SINK_PULSE_OUT, LOW);
          delay(1);
          i++;
        }
        for(int i = 0; i < correction; i++){
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(SINK_PULSE_OUT, LOW);
          delay(1);
        }
      }
      //-------------------------------кнопка вперед
      else if (DBR_ff_button.read() == LOW){ 
        while (DBR_ff_button.read() == LOW){
          disp.clear();
          disp.print(lowSpinCounter);
          disp.update();
          DBR_ff_button.update();
          DBR_low_zero_sens.update();
          
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          if (DBR_low_zero_sens.read() == HIGH && lowSpinCounter > 0)lowSpinCounter--;
          digitalWrite(SINK_PULSE_OUT, LOW);
        }
      }
      //---------------------------------кнопка назад
      else if (DDBR_rew_button.read() == LOW){ 
        while (DDBR_rew_button.read() == LOW){
          disp.clear();
          disp.print(lowSpinCounter);
          disp.update();
          DDBR_rew_button.update();
          DBR_low_zero_sens.update();
          digitalWrite(SINK_DIR_OUT, HIGH);
          delayMks(10);
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          if (DBR_low_zero_sens.read() == HIGH) lowSpinCounter++;
          digitalWrite(SINK_PULSE_OUT, LOW);
          digitalWrite(SINK_DIR_OUT, LOW);
        }
      }
}