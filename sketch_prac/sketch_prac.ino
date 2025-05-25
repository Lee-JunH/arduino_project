#include <SoftwareSerial.h>  //serial통신을 위한 라이브러리
#include <Adafruit_NeoPixel.h> //neoPixel제어를 위한 라이브러리

SoftwareSerial BTSerial(12,13); //BT의 TX:12, RX:13 (클래스 선언)

int num; //메시지를 숫자로 저장할 변수

Adafruit_NeoPixel pixels1(59, 5, NEO_GRB + NEO_KHZ800); //삼각형 픽셀
//Adafruit_NeoPixel pixels2(22, 4, NEO_GRB + NEO_KHZ800); //화살표 픽셀

void setup() {
  Serial.begin(9600); //PC-아두이노간 통신
  BTSerial.begin(9600); //스마트폰-아두이노간 통신
  pixels1.begin(); //네오픽셀 시작
  pixels1.show(); //네오픽셀 초기화
  //pixels2.begin();
  //pixels2.show();

  pinMode(2,OUTPUT); //초음파센서의 Trigger핀
  pinMode(3,INPUT); //초음파센서의 ECHO핀

  pinMode(6,OUTPUT); //모터 1
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT); //모터 2
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() {

  //1. 모터 제어 부분
  if(BTSerial.available()) //블루투스 신호가 오면
  {
    char num=BTSerial.read();

    if(num=='1') //직진
    {
      analogWrite(11,250);
      digitalWrite(10,HIGH);
      digitalWrite(9,LOW);
      digitalWrite(8,HIGH);
      digitalWrite(7,LOW);
      analogWrite(6,250);
    }
    else if(num=='2') //후진
    {
      analogWrite(11,10);
      digitalWrite(10,LOW);
      digitalWrite(9,HIGH);
      digitalWrite(8,LOW);
      digitalWrite(7,HIGH);
      analogWrite(6,10);
    }
    else if(num=='3') //좌회전
    {
      analogWrite(11,10);
      digitalWrite(10,HIGH);
      digitalWrite(9,LOW);
      digitalWrite(8,HIGH);
      digitalWrite(7,HIGH);
      analogWrite(6,0);
    }
    else if(num=='4') //우회전
    {
      analogWrite(11,0);
      digitalWrite(10,HIGH);
      digitalWrite(9,HIGH);
      digitalWrite(8,HIGH);
      digitalWrite(7,LOW);
      analogWrite(6,10);
    }
    else if(num=='5')
    {
      analogWrite(11,0);
      digitalWrite(10,HIGH);
      digitalWrite(9,HIGH);
      digitalWrite(8,HIGH);
      digitalWrite(7,HIGH);
      analogWrite(6,0);
    }
    else if(num=='6') //LED 켜짐 꺼짐
    {
      while(1)
      {
        char num=BTSerial.read();
        if(num=='1') //직진
        {
          analogWrite(11,250);
          digitalWrite(10,HIGH);
          digitalWrite(9,LOW);
          digitalWrite(8,HIGH);
          digitalWrite(7,LOW);
          analogWrite(6,250);
        }
        else if(num=='2') //후진
        {
          analogWrite(11,10);
          digitalWrite(10,LOW);
          digitalWrite(9,HIGH);
          digitalWrite(8,LOW);
          digitalWrite(7,HIGH);
          analogWrite(6,10);
        }
        else if(num=='3') //좌회전
        {
          analogWrite(11,10);
          digitalWrite(10,HIGH);
          digitalWrite(9,LOW);
          digitalWrite(8,HIGH);
          digitalWrite(7,HIGH);
          analogWrite(6,0);
        }
        else if(num=='4') //우회전
        {
          analogWrite(11,0);
          digitalWrite(10,HIGH);
          digitalWrite(9,HIGH);
          digitalWrite(8,HIGH);
          digitalWrite(7,LOW);
          analogWrite(6,10);
        }
        else if(num=='5')
        {
          analogWrite(11,0);
          digitalWrite(10,HIGH);
          digitalWrite(9,HIGH);
          digitalWrite(8,HIGH);
          digitalWrite(7,HIGH);
          analogWrite(6,0);
        }
        else if(num=='7') //LED 꺼짐
        {
          for(int i=0; i<10; i++)
          {
            pixels1.setPixelColor(i,pixels1.Color(0,0,0));
            pixels1.show();
          }
          break;
        }
        //LED 켜짐
        for(int i=0; i<10; i++)
        {
          if(i % 2 == 0)
          {
            pixels1.setPixelColor(i,pixels1.Color(255,0,0));
          }
          else
          {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
          }
          pixels1.show();
          delay(50);
        }
        for (int i = 0; i < 10; i++)
        {
          if(i % 2 == 1)
          {
            pixels1.setPixelColor(i,pixels1.Color(255,0,0));
          }
          else
          {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
          }
          pixels1.show();
          delay(50);
        }

        //2. 초음파 센서 제어 부분
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        delayMicroseconds(2); 
        digitalWrite(2,HIGH);
        delayMicroseconds(10);
        digitalWrite(2,LOW);

        unsigned long dur=pulseIn(3,HIGH);
        float dis = ((float)(340*dur)/10000)/2; //거리계산식

        if(dis<3000.0)
        {
          BTSerial.print(String(dis)); //휴대폰으로 거리 송신
        }
        delay(500);
      }
    }
  }

  //2. 초음파 센서 제어 부분
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  delayMicroseconds(2); 
  digitalWrite(2,HIGH);
  delayMicroseconds(10);
  digitalWrite(2,LOW);

  unsigned long dur=pulseIn(3,HIGH);
  float dis = ((float)(340*dur)/10000)/2; //거리계산식

  if(dis<3000.0)
  {
    BTSerial.print(String(dis)); //휴대폰으로 거리 송신
  }
  delay(500);
}




        // for(int i=0; i<10; i++)
        // {
        //   if(i % 2 == 0)
        //   {
        //     pixels1.setPixelColor(i,pixels1.Color(255,0,0));
        //   }
        //   else
        //   {
        //     pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
        //   }
        //   pixels1.show();
        //   delay(10);
        // }
        // for (int i=0; i<10; i++)
        // {
        //   if(i % 2 == 1)
        //   {
        //     pixels1.setPixelColor(i,pixels1.Color(255,0,0));
        //   }
        //   else
        //   {
        //     pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
        //   }
        //   pixels1.show();
        //   delay(10);
        // }