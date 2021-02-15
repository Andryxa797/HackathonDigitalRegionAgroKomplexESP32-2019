#include "sensors.h"

 float Humidity_Under[3];
 float Humidity_Out;
 float  temp[9];
 float temp_out[1];

bool flag_batt = 1;

//Выбор ноги подключения внешнего DS18B20 и количество внешних датчиков

#define ONE_WIRE_BUS_OUT 2
#define countSensors_out 1
DeviceAddress *sensorsUnique_out;

//Выбор ноги подключения подземных DS18B20 и количество подземных датчиков

#define ONE_WIRE_BUS_UNDER 4
#define countSensors_under 9
DeviceAddress *sensorsUnique_under;
static float temp_under[countSensors_under];
//Создание объектов 1-wire
OneWire oneWire_out(ONE_WIRE_BUS_OUT);
OneWire oneWire_under(ONE_WIRE_BUS_UNDER);
//
DallasTemperature sensor_out(&oneWire_out);
DallasTemperature sensor_under(&oneWire_under);
//
#define EEPROM_SIZE 14


void Sensors_Init(){
  DS18B20_init();
  EEPROM.begin(EEPROM_SIZE);
}
void SensorsCompletionData(){
  
  //Получение температуры с вшених датчиков
  get_temp(temp_out, countSensors_out, sensor_out);
  //Получение температуры с подземных датчиков
  get_temp(temp_under, countSensors_under, sensor_under);
  temp[0] = temp_under[0];
  temp[1] = temp_under[5];
  temp[2] = temp_under[4];
  temp[3] = temp_under[3];
  temp[4] = temp_under[8];
  temp[5] = temp_under[6];
  temp[6] = temp_under[2];
  temp[7] = temp_under[7];
  temp[8] = temp_under[1];

  //Получение Bлажности с подземных датчиков
  Get_Humidity_Under(Humidity_Under,32,34,33);
  //Получение Bлажности Bоздуха
  Humidity_Out = Get_Humidity_Out(temp_out[0],15);

  
  if(0)
  {
    // EEPROM.put(0,val);
    // EEPROM.put(1, val1);

    EEPROM.commit();
    //записыBаем полученные значения Bо флеш
  }
  // EEPROM.get(0, val1);
  // EEPROM.get(1, val);
}

//==============Измерение внешней влажности===============//
float  Get_Humidity_Out(float temperature,int ADC_PIN)
{
  double True_RH;
  double Sensor_RH;
  int ADC = 0;
  ADC = analogRead(ADC_PIN);

  Sensor_RH = (((double)ADC/4096.0)-0.1515)/0.00636;
  return True_RH = (double)Sensor_RH/(1.0546-0.00216*temperature);
}

//==============Инициализация датчиков температуры===============//
void DS18B20_init(void)
{
   //Инициализация 2 линий датчиков.
  sensor_out.begin();
  sensor_under.begin();
  
 // выполняем поиск устройств на шине out
 //countSensors_out = sensor_out.getDeviceCount();
 // выполняем поиск устройств на шине under
 //countSensors_under = sensor_under.getDeviceCount();

 // выделяем память в динамическом массиве под количество обнаруженных сенсоров вне
 sensorsUnique_out = new DeviceAddress[countSensors_out];
 // выделяем память в динамическом массиве под количество обнаруженных сенсоров под землей
 sensorsUnique_under = new DeviceAddress[countSensors_under];

 // делаем запрос на получение адресов датчиков вне
  for (int i = 0; i < countSensors_out; i++) {
    sensor_out.getAddress(sensorsUnique_out[i], i);
  }
  
  // делаем запрос на получение адресов датчиков под землей
  for (int i = 0; i < countSensors_under; i++) {
    sensor_under.getAddress(sensorsUnique_under[i], i);
  }
//  for (int i = 0; i < countSensors_under; i++) {
//    for (int j = 0; j < 8; j++) {
//      Serial.print(sensorsUnique_under[i][j]);
//    }
//    Serial.println();
//  }
  
  // устанавливаем разрешение всех внешних датчиков в 12 бит
  for (int i = 0; i < countSensors_out; i++) {
    sensor_out.setResolution(sensorsUnique_out[i], 12);
  }
  // устанавливаем разрешение всех подземных датчиков в 12 бит
  for (int i = 0; i < countSensors_under; i++) {
    sensor_under.setResolution(sensorsUnique_under[i], 12);
  }

}

//==============получение температуры с датчиков и запись в массив===============//
void get_temp(float *temp, int num, DallasTemperature sensors)
{
  // отправляем запрос на измерение температуры всех внешних сенсоров
  sensors.requestTemperatures();
  // считываем данные из регистра каждого датчика по очереди
  for (int i = 0; i < num; i++) {
    temp[i] = sensors.getTempCByIndex(i);
  }
}

//==============Измерение влажности почвы===============//
void Get_Humidity_Under(float *Humidityint, int ADC_PIN60, int ADC_PIN30, int ADC_PIN10)
{
  int MAX60 = 2383;
  int MIN60 = 3375;
  int MAX30 = 2703;
  int MIN30 = 3263;
  int MAX10 = 1760;
  int MIN10 = 3299;

  
  int wilg60 = analogRead(ADC_PIN60);
  int wilg30 = analogRead(ADC_PIN30);
  int wilg10 = analogRead(ADC_PIN10);

  Humidityint[2] = map(wilg60,MIN60,MAX60, 0, 100);
  Humidityint[1] = map(wilg30,MIN30,MAX30, 0, 100);
  Humidityint[0] = map(wilg10,MIN10,MAX10, 0, 100);
  //double wil60 = map(wilg60,MIN60,MAX60, 0, 100);
  //Serial.println(wil60);
  //Serial.println(wilg60);
  //Serial.println();
  //double wil30 = map(wilg30,MIN30,MAX30, 0, 100);
  //Serial.println(wil30);
  //Serial.println(wilg30);
  //Serial.println();
  //double wil10 = map(wilg10,MIN10,MAX10, 0, 100);
  //Serial.println(wil10);
  //Serial.println(wilg10);
  //Serial.println();
}