#include "main.h"
#include "run.h"
#include "top_spindel_tuning.h"
#include "low_spindel_tuning.h"
#include "modbus.h"
#include "correction.h"

#include <GyverSegment.h>
#define DISP1637_CLK_DELAY 100

void setup(){
  Serial.begin(9600);
  Serial.println("start");
  // -------------------------------входы с подтяжкой
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(SELECT_PIN,INPUT_PULLUP);
  pinMode(TOP_SENS_PIN, INPUT_PULLUP);
  pinMode(LOW_SENS_PIN,INPUT_PULLUP);
  pinMode(REW_RUN_PIN, INPUT_PULLUP);
  pinMode(FF_RUN_PIN, INPUT_PULLUP);
  pinMode(INIT_POS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(WRITE_CORRECT_BUTTON, INPUT_PULLUP);
  Serial.println("input init");
  //---------------------------------выходы
  pinMode(DIO_PIN,OUTPUT);
  pinMode(CLK_PIN,OUTPUT);
  pinMode(TOP_SPINDEL_PULSE_OUT, OUTPUT);
  pinMode(TOP_SPINDEL_DIR_OUT, OUTPUT);
  pinMode(LOW_SPINDEL_PULSE_OUT, OUTPUT);
  pinMode(LOW_SPINDEL_DIR_OUT, OUTPUT);
  Serial.println("output init");
  //-----------------------------антидребезг
  DBR_start.attach(START_PIN);
  DBR_start.interval(3);
  DBR_select.attach(SELECT_PIN);
  DBR_select.interval(3);
  DBR_top_zero_sens.attach(TOP_SENS_PIN);
  DBR_top_zero_sens.interval(1);
  DBR_low_zero_sens.attach(LOW_SENS_PIN);
  DBR_low_zero_sens.interval(1);
  DBR_mode_button.attach(MODE_PIN);
  DBR_mode_button.interval(3);
  DBR_EE_write.attach(WRITE_CORRECT_BUTTON);
  DBR_EE_write.interval(3);
  DBR_ff_button.attach(FF_RUN_PIN);
  DBR_ff_button.interval(1);
  DDBR_rew_button.attach(REW_RUN_PIN);
  DDBR_rew_button.interval(1);
  DBR_on_start_pos.attach(INIT_POS_BUTTON_PIN);
  DBR_on_start_pos.interval(3);
  Serial.println("debounser init");
  //---------------------дисплей. печатать справа
  disp.printRight(true);  
  disp.setCursorEnd(); 
  Serial.println("display init");
  modbus.configureHoldingRegisters(holdingRegisters, 4); 
  pinMode(13, OUTPUT); //---dePin
  modbus.begin(1,9600);

  Serial.println("HR" + String(holdingRegisters[0]) + ", " + String(holdingRegisters[1]) + ", "
                  + String(holdingRegisters[2]) + ", " +String(holdingRegisters[3]) );
  EEPROM.get(TOP_SPINDEL_SERVO_SPEED_ADDRES,holdingRegisters[0] );
  EEPROM.get(LOW_SPINDEL_SERVO_SPEED_ADDRES,holdingRegisters[1]);
  EEPROM.get(TOP_SPINDEL_WORK_ADDRES,holdingRegisters[2]);
  EEPROM.get(LOW_SPINDEL_WORK_ADDRES, holdingRegisters[3]);
  readModbusRegisters();
  Serial.println("HR" + String(holdingRegisters[0]) + ", " + String(holdingRegisters[1]) + ", "
                  + String(holdingRegisters[2]) + ", " +String(holdingRegisters[3]) );
  
}

void loop(){
  DBR_top_zero_sens.update();
  DBR_low_zero_sens.update();
  DBR_start.update();
  DBR_mode_button.update();
  DBR_EE_write.update();
  DBR_select.update();
  //-----------------------------------режим "Работа"
  if (DBR_mode_button.read() == LOW){ 
    disp.clear();
    disp.print("-go-");
    disp.update();
    readModbusRegisters();
  //-----------------------------------Один рабочий цикл
    if (DBR_top_zero_sens.read() == LOW && DBR_low_zero_sens.read() == LOW && DBR_start.fell()){
      RunTop();
      DBR_top_zero_sens.update();
      if(DBR_top_zero_sens.read() == LOW ) RunLow();
    }
    //---------------------------------коррекция выбега 
    else if(DBR_EE_write.fell() ){
      Correction(DBR_select.read());
    }
  }
  //-----------------------------------режим "Наладка"
  else if ( DBR_mode_button.read() == HIGH ){
  //-----------------------------------верхний шпиндель 
    while(DBR_select.read() == TOP_SPINDEL){  
      DBR_mode_button.update();
      //-----------------------------------опрос MODBUS
      //readModbusRegisters();
      if (DBR_mode_button.read() == LOW)break;
      top_spindel_tuning();
    }
    //---------------------------------нижний шпиндель
    while(DBR_select.read() == LOW_SPINDEL){ 
      //-----------------------------------опрос MODBUS
     // readModbusRegisters(); //
      DBR_mode_button.update();
      if (DBR_mode_button.read() == LOW)break;
      low_spindel_tuning();
    }
  }
}