#ifndef _SENSORS_H
#define _SENSORS_H
#include "main.h"




void Sensors_Init();
void DS18B20_init(void);
float Get_Humidity_Out(float temperature,int ADC_PIN);
void DS18B20_init(void);
void get_temp(float *temp, int num, DallasTemperature sensors);
void Get_Humidity_Under(float *Humidityint, int ADC_PIN60, int ADC_PIN30, int ADC_PIN10);
void SensorsCompletionData();


#endif