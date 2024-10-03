#pragma once

#include <Arduino.h>
#include <Bounce2.h>
#include <EEPROM.h>
#include <GyverSegment.h>
#include <GyverWDT.h>

#define START_PIN 2
#define MODE_PIN 3
#define SELECT_PIN 4
#define DRILL_SENS_PIN 5
#define SINK_SENS_PIN 6
#define REW_RUN_PIN 7
#define FF_RUN_PIN 8
#define INIT_POS_BUTTON_PIN 9
#define WRITE_CORRECT_BUTTON 10
#define DRILL LOW
#define SINK HIGH
#define DRILL_ADDRES 0
#define SINK_ADDRES 4
#define DRILL_SERVO_SPEED_ADDRES 8
#define SINK_SERVO_SPEED_ADDRES 12
#define DRILL_WORK_ADDRES 16
#define SINK_WORK_ADDRES 20
//----------------дисплей
#define DIO_PIN 11
#define CLK_PIN 12
//----------------выходы на драйвера ШД
#define DRILL_PULSE_OUT 14
#define DRILL_DIR_OUT 15
#define SINK_PULSE_OUT 17
#define SINK_DIR_OUT 18
#define delayMks delayMicroseconds

Disp1637Colon disp(DIO_PIN, CLK_PIN);

int drillSpinCounter = 0;
int sinkSpinCounter = 0;
int run_out = 160;
int countDown = 0;

Bounce2::Button DBR_start = Bounce2::Button();
Bounce2::Button DBR_select = Bounce2::Button();
Bounce2::Button DBR_drill_zero_sens = Bounce2::Button();
Bounce2::Button DBR_sink_zero_sens = Bounce2::Button();
Bounce2::Button DBR_mode_button = Bounce2::Button();
Bounce2::Button DBR_EE_write = Bounce2::Button();
Bounce2::Button DBR_ff_button = Bounce2::Button();
Bounce2::Button DBR_rew_button = Bounce2::Button();
Bounce2::Button DBR_on_start_pos = Bounce2::Button();

void setup();

void loop();
