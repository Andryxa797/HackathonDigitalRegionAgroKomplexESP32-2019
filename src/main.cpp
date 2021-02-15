#include "main.h"

#define uS_TO_S_FACTOR 1000000 
#define TIME_TO_SLEEP 60        

extern float Humidity_Under[3];
extern float Humidity_Out;
extern float  temp[9];
extern float temp_out[1];
extern float Pressure_BMP280;

extern int getSolar;
extern int getBattery;

void SendSerialData();

void setup() {
SerialMon.begin(115200);
SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
Wire.begin();
  Bmp280_Init();
  Gsm_Gprs_Init();
  Sensors_Init();
  Pressure_BMP280_void();
  SensorsCompletionData();
  getBattery = getBatteryFromADC();
  SendSerialData();
  UploadServer(getBattery, Pressure_BMP280, Humidity_Out, temp_out, temp, Humidity_Under);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  esp_restart();

}
void loop() {

}

void SendSerialData(){
   SerialMon.print("Humidity_Under 1 - ");
  SerialMon.println(Humidity_Under[0]);
   SerialMon.print("Humidity_Under 2 - ");
  SerialMon.println(Humidity_Under[1]);
   SerialMon.print("Humidity_Under 3 - ");
  SerialMon.println(Humidity_Under[2]);
  SerialMon.print("Humidity_Out - ");
  SerialMon.println(Humidity_Out);
  SerialMon.print("Pressure_bmp280 - ");
  SerialMon.println(Pressure_BMP280);
   SerialMon.print("Temp_out - ");
  SerialMon.println(temp_out[0]);
  for(int i = 0; i<9; i++){
  SerialMon.print("Temp_out " + String(i+1) + " - " );
    SerialMon.println(temp[i]);
  }
  SerialMon.print("Solar  - ");
  SerialMon.print(getSolar);
  SerialMon.println("V");
  SerialMon.print("Temp_out - ");
  SerialMon.print(getBattery);
  SerialMon.println("V");

}