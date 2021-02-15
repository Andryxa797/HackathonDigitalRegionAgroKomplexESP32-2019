#ifndef _MAIN_H
#define _MAIN_H

#define SerialMon Serial
#define SerialAT Serial1

#include <Arduino.h>
#include "Arduino.h"
#include "string.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoHttpClient.h>

#include <DallasTemperature.h>
#include <EEPROM.h>
#include <OneWire.h>

#include "my_gsm.h"
#include "sensors.h"
#include "bmp280.h"

#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22
#define SerialMon Serial
#define SerialAT Serial1



#endif