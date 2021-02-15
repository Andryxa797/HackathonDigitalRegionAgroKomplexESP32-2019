#ifndef _MY_GSM_H
#define _MY_GSM_H
#include "main.h"

#define my_server "185.231.245.214"
#define my_resource  "/controller/hackathon/upload"




void Gsm_Gprs_Init();
void UploadServer(int getBattery1, float Pressure_BMP2801, float Humidity_Out1, float *temp_out1, float  *temp1, float *Humidity_Under1);
int getSolarFromADC(int PIN_ADC);
int getBatteryFromADC();



#endif