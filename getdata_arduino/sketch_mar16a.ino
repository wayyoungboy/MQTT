#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 接口1306显示屏
#include <SoftwareSerial.h>//调用软串口库
SoftwareSerial WIFISerial(3, 4);//软串口进行无线通信
int SpeakerPin = 7; //喇叭接口

void DrawSetup()
{
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(5, 10);
    u8g.print("Environment");
    u8g.setPrintPos(5, 22);
    u8g.print("acquisition");
    u8g.setPrintPos(5, 34);
    u8g.print("Connecting");
    u8g.setPrintPos(5, 58);
    u8g.print("By Ma Zhenzhen");
  } while ( u8g.nextPage() );
}


void setup() {
  // put your setup code here, to run once:
  WIFISerial.begin(115200);          //wifi端传输波特率
  pinMode(SpeakerPin, OUTPUT);        // 喇叭警报
  digitalWrite(SpeakerPin, 1);
  Serial.begin(115200);             // 串口传输时上位机的波特率（测试用）
  DrawSetup();//开机界面显示
 delay(10000);//静止10s
}


String zdata;
int data[5] = {0};
int n = 0;
int waring[5] = {0};
int zwaring = 0;
void loop() {

  if (WIFISerial.available() > 0) {
    delay(20);
    zdata = "";
    while (WIFISerial.available())   //时刻读取硬件串口数据
    {
      zdata += char(WIFISerial.read());
    }
    Serial.println(zdata);
    if (zdata[0] == 97) { //温度
      n = 0;
    }
    else if (zdata[0] == 98) { //湿度
      n = 1;
    }
    else if (zdata[0] == 99) { //PM2.5
      n = 2;
    }
    else if (zdata[0] == 100) { //乙烷
      n = 3;
    }
    else if (zdata[0] == 101) { //甲醛
      n = 4;
    }
    else {
      zdata = "";
      n = 6;
    }
    if (n < 5) {
      data[n] = "";
      data[n] = (int(zdata[1]) - 48) * 100 + (int(zdata[2])- 48)*10 +(int(zdata[3])-48);
      Serial.print("zdata="); Serial.print(int(zdata[0]));
      Serial.print("n="); Serial.print(n);
      Serial.print("data="); Serial.println(data[n]);
    }
  }
  DrawLoop();//环境温度显示
  zwaring = waring[0] + waring[1] + waring[2] + waring[3] + waring[4];
  if (zwaring != 0)
  digitalWrite(SpeakerPin, 0);
  else digitalWrite(SpeakerPin, 1);
}

void DrawLoop() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(5, 10);
    u8g.print("T="); u8g.print(data[0]);
    if (data[0] > 30) {
      u8g.print("Warning!");
      waring[0] = 1;
    } else  waring[0] = 0;
    u8g.setPrintPos(5, 22);
    u8g.print("H="); u8g.print(data[1]);
    if (data[1] > 30) {
      u8g.print("Warning!");
      waring[1] = 1;
    } else  waring[1] = 0;
    u8g.setPrintPos(5, 34);
    u8g.print("PM2.5="); u8g.print(data[2]);
    if (data[2] > 100) {
      u8g.print("Warning!");
      waring[2] = 1;
    } else  waring[2] = 0;
    u8g.setPrintPos(5, 46);
    u8g.print("CAS="); u8g.print(data[3]);
    if (data[3] > 30) {
      u8g.print("Warning!");
      waring[3] = 1;
    } else  waring[3] = 0;
    u8g.setPrintPos(5, 58);
    u8g.print("HCHO="); u8g.print(data[4]);
    if (data[4] > 60) {
      u8g.print("Warning!");
      waring[4] = 1;
    } else  waring[4] = 0;
  } while ( u8g.nextPage() );

}
