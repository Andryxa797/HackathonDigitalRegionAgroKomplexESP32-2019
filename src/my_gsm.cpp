#include "my_gsm.h"

int getSolar;
int getBattery;



const char apn[]  = "internet.megafon.ru"; 
const char gprsUser[] = "gdata"; 
const char gprsPass[] = "gdata"; 
const char simPIN[]   = ""; 
       
const int  port = 7000;                      

#define TINY_GSM_MODEM_SIM800      
#define TINY_GSM_RX_BUFFER   1024 
#include <TinyGsmClient.h> 

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif



TwoWire I2CPower = TwoWire(0);
TinyGsmClient client(modem);


#define uS_TO_S_FACTOR 1000000     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3600        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

int getSolarFromADC(int PIN_ADC)
{
  int sol_mv = 0;
  uint32_t oversample = 0;
  for (size_t i = 0; i < 100; i++)
  {
    oversample += (uint32_t)analogRead(PIN_ADC);
  }
  sol_mv = (int)oversample / 100;
  return sol_mv = (3300 / 4096) * sol_mv;//тут нужно посмотреть опытным путем будет 3600 или 3300, я склоняюсь к 3300 (предпоследний множитель)
}
int getBatteryFromADC()
{
  int bat_mv = 0;
  uint32_t oversample = 0;
  for (size_t i = 0; i < 100; i++)
  {
    oversample += (uint32_t)analogRead(35);
  }
  bat_mv = (int)oversample / 100;
  return bat_mv = ((float)bat_mv / 4096) * 3600 * 2;
}

void Gsm_Gprs_Init() {
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  delay(3000);
  SerialMon.println("Initializing modem...");
  modem.restart();
  SerialMon.println("Test AT:");
  if(modem.testAT() == 1){
  SerialMon.println("Test - Good");
  }
  else{
  SerialMon.println("Test - Bad");
  }
  delay(1000);
}


void UploadServer(int getBattery1, float Pressure_BMP2801, float Humidity_Out1, float *temp_out1, float  *temp1, float *Humidity_Under1) {
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  }
  else {
    SerialMon.println(" - OK");
    SerialMon.print("Connecting to ");
    SerialMon.print(my_server);
    if (!client.connect(my_server, port)) {
      SerialMon.println(" fail");
    }
    else {
    ////////////=======================================================================================///////////////////
      SerialMon.println(" - OK");
      SerialMon.println("Performing HTTP POST request...");
      String my_sendData = "{\"Upow\":" + String(getBattery1) + ",\"pr\":" + String((int)Pressure_BMP2801) + ",\"hm\":" + String(Humidity_Out1) + ",\"t\": " + String(temp_out1[0]) + ",\"t0\":" + String(temp1[0]) + ",\"t1\":" +  String(temp1[1]) + ",\"t2\":" +  String(temp1[2]) + ",\"t3\":" +  String(temp1[3]) + ",\"t4\":" +  String(temp1[4]) + ",\"t5\":" +  String(temp1[5]) + ",\"t6\":" +  String(temp1[6]) + ",\"t7\":" +  String(temp1[7]) + ",\"t8\":" +  String(temp1[8]) + ",\"soil1\":" +  String(Humidity_Under1[0]) + ",\"soil2\":" + String(Humidity_Under1[1]) + ",\"soil3\":" + String(Humidity_Under1[2]) + ",\"stationID\":\"0011\",\"device_type\":\"Z\",\"firmware_version\":1}";
      HttpClient http(client, my_server, port);
      http.beginRequest();
      int responce = http.post(my_resource);
      SerialMon.println("Otvet server - " + String(responce) );
      http.sendHeader("Content-Type", "application/x-www-form-urlencoded");
      http.sendHeader("Content-Length", my_sendData.length());
      http.beginBody();
      http.beginBody();
      http.print(my_sendData);
      http.endRequest();
      String ResponceBody = http.responseBody();
      SerialMon.println(ResponceBody);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        while (client.available()) {
          char c = client.read();
          SerialMon.print(c);
          timeout = millis();
        }
      }
      SerialMon.println();
      client.stop();
      SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    ////////////=======================================================================================///////////////////
    }
  }
}