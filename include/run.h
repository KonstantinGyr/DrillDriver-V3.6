#pragma once
#include "main.h"

int high_freq_pause = 80; //макс скорость
int work_pause ;          //сверло скорость подачи
int low_freq_pause = 2560;//мин скорость
int FF_spindel;           //выбег 
int run_spindel;
int work_zone ;           //работа (шаги) 
int return_spindel;       //возврат на исходную
int accel ;               //ускорение
int slow_down = 160 ;           //замедление
int pulse = 20;           //импульс длительность (мкс)

 
void twoStagesServo(int run_spindel,int work_pause, int work_zone ,int pulsePin,int dirPin){
  Serial.print("Two stage. "  );
  Serial.println((dirPin == 15 )? " Drill ":" Sink");
  accel = 200;
  float num = low_freq_pause;
  digitalWrite(dirPin, HIGH);
  delayMks(1);
  //--------------------------------быстрое перемещение
  for (int i = 0; i < run_spindel; i++){
    if (i < accel)num -= num / (accel / 10);
    else if (i > run_spindel - (work_pause - high_freq_pause)/10)num += 10;// замедление
    digitalWrite(pulsePin, HIGH);
    delayMks(pulse);
    digitalWrite(pulsePin, LOW);
    delayMks((int)num + high_freq_pause);
  }
  num = 0.1;
  //------------------------------------------сверление
  for (int i = 0; i < work_zone; i++){
    if (i > work_zone - slow_down)num += num / (slow_down / 10);
    digitalWrite(pulsePin, HIGH);
    delayMks(pulse);
    digitalWrite(pulsePin, LOW);
    delayMks((int)num + work_pause);
  }
  digitalWrite(dirPin, LOW);
}

void oneStageServo(int work_pause, int work_zone,int pulsePin,int dirPin){
  float num = low_freq_pause;
  (960 - work_pause*2)/4 > 0 ? accel = (960 - work_pause*2)/4 : accel = 40;
  Serial.print("One stage. "  );
  Serial.println((dirPin == 15 )? " Drill ":" Sink");
  digitalWrite(dirPin, HIGH);
  delayMks(1);
  //--------------------------------быстрое перемещение
  for (int i = 0; i < work_zone; i++){
    if (i < accel)num -= num / (accel / 10);                          // разгон
    else if (i > work_zone - slow_down) num += num / (slow_down / 10);// замедление
    digitalWrite(pulsePin, HIGH);
    delayMks(pulse);
    digitalWrite(pulsePin, LOW);
    delayMks((int)num + work_pause);
  }
  digitalWrite(dirPin, LOW);
}

void returnSpindel(int return_spindel,int pulsePin, bool spindel){
  float num = low_freq_pause;
  accel = 200; 
  Serial.println("Return");
  for (int i = 0; i < return_spindel; i++){
    if (i < accel) num -= num / (accel / 10);
    //-------------------------------------------------------------завершение по датчику
     DBR_sink_zero_sens.update() ;
     DBR_drill_zero_sens.update();
    if (spindel?  DBR_sink_zero_sens.fell(): DBR_drill_zero_sens.fell())break; 
    digitalWrite(pulsePin, HIGH);
    delayMks(pulse);
    digitalWrite(pulsePin, LOW);
    delayMks((int)num + high_freq_pause);
  }
  num = 0.1;
  for(int i = 0 ; i < run_out; i++){
    num += num/ (run_out/10);
    digitalWrite(pulsePin, HIGH);
    delayMks(pulse);
    digitalWrite(pulsePin, LOW);
    delayMks((int)num + high_freq_pause); 
  }
}

void RunDrill(){
  //------считывание из памяти значений подачи и выбега
  EEPROM.get(DRILL_ADDRES, FF_spindel);
  EEPROM.get(DRILL_SERVO_SPEED_ADDRES,work_pause);
  EEPROM.get(DRILL_WORK_ADDRES,work_zone);
  if (FF_spindel > 2000)  FF_spindel = 2000;
  run_spindel = FF_spindel - work_zone ;
  if((FF_spindel - work_zone) < 240 +((work_pause - high_freq_pause)/10)){
    
     oneStageServo(work_pause,FF_spindel,DRILL_PULSE_OUT,DRILL_DIR_OUT );
  }
  else {
     twoStagesServo(run_spindel,work_pause,work_zone,DRILL_PULSE_OUT,DRILL_DIR_OUT );   
  }
  return_spindel = FF_spindel + 400 ; // один оборот в плюс
  //DBR_drill_zero_sens.update();
  if(digitalRead(DRILL_SENS_PIN) ){
  returnSpindel(return_spindel,DRILL_PULSE_OUT,DRILL);
  }
}
void RunSink(){
  //------------считывание из памяти значений подачи и выбега
  EEPROM.get(SINK_ADDRES, FF_spindel);
  EEPROM.get(SINK_SERVO_SPEED_ADDRES,work_pause);
  EEPROM.get(SINK_WORK_ADDRES,work_zone);
  if (FF_spindel > 1200)  FF_spindel = 1200; 
  run_spindel = FF_spindel - work_zone;
  if((FF_spindel - work_zone) < 240 +((work_pause - high_freq_pause)/10)){
    oneStageServo(work_pause,FF_spindel,SINK_PULSE_OUT,SINK_DIR_OUT);
  }
  else  {
    twoStagesServo(run_spindel,work_pause,work_zone,SINK_PULSE_OUT,SINK_DIR_OUT);
  }
  return_spindel = FF_spindel + 400 ;      // один оборот в плюс
  DBR_drill_zero_sens.update();
  if(digitalRead(SINK_SENS_PIN) ){
    returnSpindel(return_spindel,SINK_PULSE_OUT,SINK);
  }
}