#pragma once

#include <Arduino.h>
#include <Bounce2.h>
#include <EEPROM.h>
#include <GyverSegment.h>

#define START_PIN 2
#define MODE_PIN 3
#define SELECT_PIN 4
#define TOP_SENS_PIN 5
#define LOW_SENS_PIN 6
#define REW_RUN_PIN 7
#define FF_RUN_PIN 8
#define INIT_POS_BUTTON_PIN 9
#define WRITE_CORRECT_BUTTON 10
#define TOP_SPINDEL LOW
#define LOW_SPINDEL HIGH
#define TOP_SPINDEL_ADDRES 0
#define LOW_SPINDEL_ADDRES 4
#define TOP_SPINDEL_SERVO_SPEED_ADDRES 8
#define LOW_SPINDEL_SERVO_SPEED_ADDRES 12
#define TOP_SPINDEL_WORK_ADDRES 16
#define LOW_SPINDEL_WORK_ADDRES 20
//----------------дисплей
#define DIO_PIN 11
#define CLK_PIN 12
//----------------выходы на драйвера ШД
#define TOP_SPINDEL_PULSE_OUT 14
#define TOP_SPINDEL_DIR_OUT 15
#define LOW_SPINDEL_PULSE_OUT 17
#define LOW_SPINDEL_DIR_OUT 18
#define delayMks delayMicroseconds

Disp1637Colon disp(DIO_PIN, CLK_PIN);

int topSpinCounter = 0;
int lowSpinCounter = 0;
int correction = 80;
int countDown = 0;

Bounce DBR_start = Bounce();
Bounce DBR_select = Bounce();
Bounce DBR_top_zero_sens = Bounce();
Bounce DBR_low_zero_sens = Bounce();
Bounce DBR_mode_button = Bounce();
Bounce DBR_EE_write = Bounce();
Bounce DBR_ff_button = Bounce();
Bounce DDBR_rew_button = Bounce();
Bounce DBR_on_start_pos = Bounce();

void setup();

void loop();
