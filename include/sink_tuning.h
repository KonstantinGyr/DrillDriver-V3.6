#pragma once
#include "main.h"

void sink_tuning(){  
      disp.clear();
      disp.print(sinkSpinCounter);
      disp.update();
      DBR_EE_write.update();
      DBR_select.update();
      DBR_on_start_pos.update();
      DBR_ff_button.update();
      DBR_rew_button.update();
      //-------------------------------запись значения выбега
      if (DBR_EE_write.fell()) {
        EEPROM.put(SINK_ADDRES,sinkSpinCounter + run_out + 80);
        disp.clear();
        disp.print("gone");
        disp.update();
        delay(1000);
      } 
      //-------------------------------возврат на исходную     кнопка поиск нуля
      if (DBR_on_start_pos.fell() && DBR_sink_zero_sens.read() == HIGH ){  
            disp.clear();
            disp.print("on O");
            disp.update();
            Serial.println("sink go to O");
        for(int i = 0; i < (sinkSpinCounter + 200); i++){
          DBR_sink_zero_sens.update();
          if(DBR_sink_zero_sens.read() == LOW){
            sinkSpinCounter = 0;
            Serial.println("on O");
            break;
          }
          if(i/100 == 0)Watchdog.reset();
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(SINK_PULSE_OUT, LOW);
          delay(1);
        }
        for(int i = 0; i < run_out; i++){
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(50);
          digitalWrite(SINK_PULSE_OUT, LOW);
          delay(1);
        }
      }
      //-------------------------------кнопка вперед
      else if (DBR_ff_button.read() == LOW){ 
        Serial.println("sink forward");
        while (DBR_ff_button.read() == LOW){
          disp.clear();
          disp.print(sinkSpinCounter);
          disp.update();
          DBR_ff_button.update();
          DBR_sink_zero_sens.update();
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(10);
          digitalWrite(SINK_PULSE_OUT, LOW);
          if (DBR_sink_zero_sens.read() == HIGH && sinkSpinCounter > 0)sinkSpinCounter--;
          Watchdog.reset();
        }
      }
      //---------------------------------кнопка назад
      else if (DBR_rew_button.read() == LOW){ 
        Serial.println("sink rewers");
        while (DBR_rew_button.read() == LOW){
          disp.clear();
          disp.print(sinkSpinCounter);
          disp.update();
          DBR_rew_button.update();
          DBR_sink_zero_sens.update();
          digitalWrite(SINK_DIR_OUT, HIGH);
          delayMks(10);
          digitalWrite(SINK_PULSE_OUT, HIGH);
          delayMks(10);
          digitalWrite(SINK_PULSE_OUT, LOW);
          digitalWrite(SINK_DIR_OUT, LOW);
          if (DBR_sink_zero_sens.read() == HIGH) sinkSpinCounter++;
          Watchdog.reset();
        }
      }
}