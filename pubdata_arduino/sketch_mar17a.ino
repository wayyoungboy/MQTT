#define DHT12Pin  10//D10
#define CASPin 0//A0
#define u8 unsigned short  //标号数据长度定义
#define u16 unsigned int
#define u32 unsigned long
#define CASPin 0//A0
#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <SoftwareSerial.h>//调用软串口库
SoftwareSerial WIFISerial(6, 7);//软串口进行无线通信
int dustPin = 1;
float dustVal =0;
int ledPower = 2;
int delayTime = 280;
int delayTime2 = 40;
float offTime = 9680;
int pm = 0;


Adafruit_SGP30 sgp;
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}


int DHT12_flag = 0;
int DHT12_error = 0;

double Humi = 0, Temprature = 0;
void DHT12_Init()
{
  pinMode(DHT12Pin, OUTPUT);
  digitalWrite(DHT12Pin, LOW);
  delay(50);
}
u8 DHT12_Rdata(void)
{
  u8 i;
  u16 j;
  u8 data = 0;
  u8 dbit = 0;
  pinMode(DHT12Pin, INPUT);
  for (i = 0; i < 8; i++)
  {
    while (digitalRead(DHT12Pin) == LOW) //check the last low state is over
    {
      if (++j >= 50000)
      {

      }
    }
    delayMicroseconds(50);//delay 26-70us,to overed data0 high level

    dbit = 0;
    if (digitalRead(DHT12Pin) == HIGH)
    {
      dbit = 1;
    }
    while (digitalRead(DHT12Pin) == HIGH) //等待高电平结束
    {
      if (++j >= 50000) //防止进入死循环
      {
        break;
      }
    }
    data <<= 1;
    data |= dbit;
  }
  return data;
}
u8 DHT12_ReadTemp()
{
  u32 j;

  u8 Humi_H, Humi_L, Temp_H, Temp_L, Temp_CAL, temp;
  pinMode(DHT12Pin, OUTPUT);
  digitalWrite(DHT12Pin, LOW);
  delay(20);
  digitalWrite(DHT12Pin, HIGH);
  pinMode(DHT12Pin, INPUT);
  delayMicroseconds(30);
  DHT12_flag = 0;
  if (digitalRead(DHT12Pin) == LOW)
  {
    DHT12_flag = 1;
    j = 0;
    while (digitalRead(DHT12Pin) == LOW) //判断从机发出 80us 的低电平响应信号是否结束
    {
      if (++j >= 500) //防止进入死循环
      {
        DHT12_error = 1;
        break;
      }
    }
    j = 0;
    while (digitalRead(DHT12Pin) == HIGH) //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
    {
      if (++j >= 800) //防止进入死循环
      {
        DHT12_error = 1;
        break;
      }
    }
    Humi_H = DHT12_Rdata();
    Humi_L = DHT12_Rdata();
    Temp_H = DHT12_Rdata();
    Temp_L = DHT12_Rdata();
    Temp_CAL = DHT12_Rdata();
    temp = (u8)(Humi_H + Humi_L + Temp_H + Temp_L);

    //Verify
    //if success ,continue
    if (Temp_CAL == temp) //如果校验成功，往下运行
    {
      Humi = Humi_H * 10 + Humi_L; //湿度

      if (Temp_L & 0X80) //为负温度
      {
        Temprature = 0 - (Temp_H * 10 + ((Temp_L & 0x7F)));
      }
      else   //为正温度
      {
        Temprature = Temp_H * 10 + Temp_L; //为正温度
      }
      //判断数据是否超过量程（温度：-20℃~60℃，湿度20％RH~95％RH）
      if (Humi > 950)
      {
        Humi = 950;
      }
      if (Humi < 200)
      {
        Humi = 200;
      }
      if (Temprature > 600)
      {
        Temprature = 600;
      }
      if (Temprature < -200)
      {
        Temprature = -200;
      }
      Temprature = Temprature / 10; //计算为温度值
      Humi = Humi / 10;

      // Serial.print(Temprature);
      // Serial.print(" ℃\r\n");

      // Serial.print(Humi);
      //Serial.print(" %\r\n");
    }
    else
    {
      Serial.print("CAL Error!!\r\n");

    }
  }
  else
  {
    DHT12_error = 0;
    //Serial.print("Sensor Error!!\r\n");
  }
}



void pmGet() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(delayTime);
  dustVal = analogRead(dustPin);
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(offTime);

  if (dustVal > 36.455);
  //Serial.println((float(dustVal/1024)-0.0356)*120000*0.035);
  pm = int((float(dustVal / 1024) - 0.0356) * 120000 * 0.035) / 10;

}
void setup()
{
  Serial.begin(115200);
  DHT12_Init();
  pinMode(ledPower,OUTPUT);
pinMode(dustPin,OUTPUT);
  WIFISerial.begin(115200);
  if (! sgp.begin()) {
    // Serial.println("Sensor not found :(");
    // while (1);
  }
}
int CASVal = 0;
void loop()
{
  // put your main code here, to run repeatedly:
  delay(1);
  DHT12_ReadTemp();
  Serial.print("a"); Serial.print(int(Temprature) % 1000 / 100); Serial.print(int(Temprature) % 100 / 10); Serial.println(int(Temprature) % 10);

  WIFISerial.print("a"); WIFISerial.print(int(Temprature) % 1000 / 100); WIFISerial.print(int(Temprature) % 100 / 10); WIFISerial.println(int(Temprature) % 10);
  delay(10);
  Serial.print("b"); Serial.print(int(Humi) % 1000 / 100); Serial.print(int(Humi) % 100 / 10); Serial.println(int(Humi) % 10);
  WIFISerial.print("b"); WIFISerial.print(int(Humi) % 1000 / 100); WIFISerial.print(int(Humi) % 100 / 10); WIFISerial.println(int(Humi) % 10);
  delay(10);
  if (! sgp.IAQmeasure()) {
    // Serial.println("Measurement failed");
    // return;
  }
  Serial.print("e"); Serial.print(sgp.TVOC % 1000 / 100);  Serial.print(sgp.TVOC % 100 / 10); Serial.println(sgp.TVOC % 10);
  WIFISerial.print("e");  WIFISerial.print(sgp.TVOC % 1000 / 100);   WIFISerial.print(sgp.TVOC % 100 / 10);  WIFISerial.println(sgp.TVOC % 10);
  delay(10);

  CASVal = analogRead(CASPin) / 10;
  Serial.print("d"); Serial.print(int(CASVal) % 1000 / 100); Serial.print(int(CASVal) % 100 / 10); Serial.println(int(CASVal) % 10);
  WIFISerial.print("d");  WIFISerial.print(int(CASVal) % 1000 / 100);  WIFISerial.print(int(CASVal) % 100 / 10);  WIFISerial.println(int(CASVal) % 10);
  delay(10);
  pmGet();
  if (pm>1000){
    pm=999;
    }
  Serial.print("c"); Serial.print(int(pm) % 1000 / 100); Serial.print(int(pm) % 100 / 10); Serial.println(int(pm) % 10);
  WIFISerial.print("c"); WIFISerial.print(int(pm) % 1000 / 100); WIFISerial.print(int(pm) % 100 / 10); WIFISerial.println(int(pm) % 10);
  delay(500);
}
