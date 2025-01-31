#include "main.h"
#include "run.h"
#include "drill_tuning.h"
#include "sink_tuning.h"
#include "modbus.h"
#include "correction.h"

#include <GyverSegment.h>
#define DISP1637_CLK_DELAY 100
int counter;

void setup(){
  Serial.begin(9600);
  Serial.println("---Start");
  // -------------------------------входы с подтяжкой
  pinMode(START_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  pinMode(SELECT_PIN,INPUT);
  pinMode(DRILL_SENS_PIN, INPUT);
  pinMode(SINK_SENS_PIN,INPUT);
  pinMode(REW_RUN_PIN, INPUT);
  pinMode(FF_RUN_PIN, INPUT);
  pinMode(INIT_POS_BUTTON_PIN, INPUT);
  pinMode(WRITE_CORRECT_BUTTON, INPUT);
  //---------------------------------выходы
  pinMode(DIO_PIN,OUTPUT);
  pinMode(CLK_PIN,OUTPUT);
  pinMode(DRILL_PULSE_OUT, OUTPUT);
  pinMode(DRILL_DIR_OUT, OUTPUT);
  pinMode(SINK_PULSE_OUT, OUTPUT);
  pinMode(SINK_DIR_OUT, OUTPUT);

  //-----------антидребезг
  //-----------------------------старт
  DBR_start.attach(START_PIN);
  DBR_start.interval(3);
  DBR_start.setPressedState(LOW);
  //-----------------------------выбор шпинделя
  DBR_select.attach(SELECT_PIN);
  DBR_select.interval(3);
  DBR_select.setPressedState(LOW);
  //-----------------------------сверление датчик
  DBR_drill_zero_sens.attach(DRILL_SENS_PIN);
  DBR_drill_zero_sens.interval(3);
  DBR_drill_zero_sens.setPressedState(LOW);
  //-----------------------------зенкование датчик
  DBR_sink_zero_sens.attach(SINK_SENS_PIN);
  DBR_sink_zero_sens.interval(3);
  DBR_sink_zero_sens.setPressedState(LOW);
  //-----------------------------режим
  DBR_mode_button.attach(MODE_PIN);
  DBR_mode_button.interval(3);
  DBR_mode_button.setPressedState(LOW);
  //-----------------------------коррекция/запись
  DBR_EE_write.attach(WRITE_CORRECT_BUTTON);
  DBR_EE_write.interval(3);
  DBR_EE_write.setPressedState(LOW);
  //-----------------------------вперед
  DBR_ff_button.attach(FF_RUN_PIN);
  DBR_ff_button.interval(3);
  DBR_ff_button.setPressedState(LOW);
  //-----------------------------назад
  DBR_rew_button.attach(REW_RUN_PIN);
  DBR_rew_button.interval(3);
  DBR_rew_button.setPressedState(LOW);
  //-----------------------------на старт
  DBR_on_start_pos.attach(INIT_POS_BUTTON_PIN);
  DBR_on_start_pos.interval(3);
  DBR_on_start_pos.setPressedState(LOW);

  //-------дисплей. печатать справа
  disp.printRight(true);  
  disp.setCursorEnd(); 
  Serial.println("display init");
  counter = 0;
  modbus.configureHoldingRegisters(holdingRegisters, 6); 
  pinMode(13, OUTPUT); //---dePin
  modbus.begin(1,9600);

  EEPROM.get(DRILL_SERVO_SPEED_ADDRES,holdingRegisters[0] );
  EEPROM.get(SINK_SERVO_SPEED_ADDRES,holdingRegisters[1]);
  EEPROM.get(DRILL_WORK_ADDRES,holdingRegisters[2]);
  EEPROM.get(SINK_WORK_ADDRES, holdingRegisters[3]);
  EEPROM.get(DRILL_EDIT_ADDRES,holdingRegisters[4]);
  EEPROM.get(SINK_EDIT_ADDRES,holdingRegisters[5]);
  delay(5000);
  readModbusRegisters();
  
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_256); // Режим сторжевого сброса , таймаут ~2с
  DBR_drill_zero_sens.update();
  DBR_sink_zero_sens.update();  
}


void loop(){
  DBR_mode_button.update();
  //-----------------------------------режим "Работа"
  if (DBR_mode_button.isPressed()){ 
    DBR_drill_zero_sens.update();
    DBR_sink_zero_sens.update();
    if(!DBR_drill_zero_sens.isPressed() || !DBR_sink_zero_sens.isPressed()){
      Serial.println("not start position");
      disp.clear();
      disp.print("no 0");
      disp.update();
    }
    else{
      Serial.println(" machine ready");
      disp.clear();
      disp.print("-go-");
      disp.update();
    }
    while(DBR_mode_button.isPressed()){
      DBR_start.update();
      DBR_EE_write.update();
      DBR_select.update();
      if(DBR_start.pressed()){
        counter++ ;
        Serial.println("START");
        Serial.println("counter : " + String(counter));
        DBR_drill_zero_sens.update();
        DBR_sink_zero_sens.update();
        if(DBR_drill_zero_sens.isPressed() && DBR_sink_zero_sens.isPressed()){
          RunDrill();//-----------------------------------Один рабочий цикл
          Watchdog.reset();
          DBR_drill_zero_sens.update();
          if(DBR_drill_zero_sens.isPressed() ) RunSink();
          Watchdog.reset();
        }
      }
      else if(DBR_EE_write.fell() ){//--------------коррекция выбега
        
        Serial.println("spindel : " + String(DBR_select.read()));
        Correction(DBR_select.read());
      }
      DBR_mode_button.update();
      Watchdog.reset();
    }
  }
  //-----------------------------------режим "Наладка"
  else if(DBR_mode_button.read() == HIGH) {
  //-----------------------------------верхний шпиндель 
    while(DBR_select.read() == DRILL){  
      DBR_mode_button.update();
      //-----------------------------------опрос MODBUS
      readModbusRegisters();
      if (DBR_mode_button.read() == LOW)break;
      drill_tuning();
      Watchdog.reset();
    }
    //---------------------------------нижний шпиндель
    while(DBR_select.read() == SINK){ 
      //-----------------------------------опрос MODBUS
      readModbusRegisters(); //
      DBR_mode_button.update();
      if (DBR_mode_button.read() == LOW)break;
      sink_tuning();
      Watchdog.reset();
    }
  }
  Watchdog.reset();
}