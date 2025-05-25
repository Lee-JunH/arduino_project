#include <SoftwareSerial.h>     //serial통신을 위한 라이브러리
#include <Adafruit_NeoPixel.h>  //neoPixel제어를 위한 라이브러리
#include <avr/io.h>             
#include <avr/interrupt.h>      

SoftwareSerial BTSerial(12, 13);  //BT의 TX:12, RX:13 (클래스 선언)

String ms = "";  //BT로부터 받을 메시지 변수
int num;         //메시지를 숫자로 저장할 변수
int a = 0;
int b = 3;
int c = 0;

Adafruit_NeoPixel pixels1(59, 5, NEO_GRB + NEO_KHZ800);  //삼각형 픽셀
Adafruit_NeoPixel pixels2(22, 4, NEO_GRB + NEO_KHZ800);  //화살표 픽셀

void setup() {
  Serial.begin(9600);    //PC-아두이노간 통신
  BTSerial.begin(9600);  //스마트폰-아두이노간 통신
  pixels1.begin();       //네오픽셀 시작
  pixels2.begin();
  pixels1.show();  //네오픽셀 초기화
  pixels2.show();

  pinMode(2, OUTPUT);  //초음파센서의 Trigger핀
  pinMode(3, INPUT);   //초음파센서의 ECHO핀

  pinMode(6, OUTPUT);  //모터 1
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);  //모터 2
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  cli();
  TCCR1A = 0;  // 타이머 설정 초기화
  TCCR1B = 0;

  // 1초에 한 번 인터럽트 발생
  OCR1A = 5156;                        // 비교 매치 레지스터 값 설정 (16 MHz / (1024 * 1 Hz) - 1)
  TCCR1B |= (1 << WGM12);               // CTC 모드 설정
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 분주율 설정

  TIMSK1 |= (1 << OCIE1A);  // 타이머 비교 인터럽트 활성화

  sei();  // 인터럽트 활성화
}

ISR(TIMER1_COMPA_vect) {
  
  if (a == 1) { //LED on
    static unsigned int count = 0;
    count++;
    if (count % 2 == 1)
    {
      if (b == 1) //화살표1
      {
        for (int i = 0; i < 22; i++) 
        {
          if((i >= 0 && i <= 5) || (i >= 12 && i <= 21))
          {
              pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
          }
           if(i>=6 && i<=11)
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
        }
      }
      if (b == 2) //화살표2
      {
        for (int i = 0; i < 22; i++) 
        {
          if((i >= 6 && i <= 12) || (i >= 12 && i <= 21))
          {
              pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
          }
          if(i>=0 && i<=5)
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
        }
      }
      if (b == 3) //끔
      {
        for (int i = 0; i < 22; i++) 
        {
          if (i % 2 == 0) 
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
          else
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));  // RGB 값 (빨간색)
          }
        }
      }
      for (int i = 0; i < 59; i++) { //LED
        if (i % 2 == 0) {
          pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));

        } else {
          pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
        }
      }
      pixels1.show();
      pixels2.show();
    } 
    else 
    {
      if (b == 1) //화살표1
      {
        for (int i = 0; i < 22; i++) 
        {
          if((i >= 0 && i <= 5) || (i >= 12 && i <= 21))
          {
              pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));  // RGB 값 (빨간색)
          }
          if(i>=6 && i<=11)
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
          
        }
      }
      if (b == 2) //화살표2
      {
        for (int i = 0; i < 22; i++) 
        {
          if((i >= 6 && i <= 12) || (i >= 12 && i <= 21))
          {
              pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));  // RGB 값 (빨간색)
          }
          if(i>=0 && i<=5)
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
        }
      }
      if (b == 3) //끔
      {
        for (int i = 0; i < 22; i++) 
        {
          if (i % 2 == 0) 
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          }
          else
          {
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));  // RGB 값 (빨간색)
          }
        }
      }
      for (int i = 0; i < 59; i++) { //LED
        if (i % 2 == 1) {
          pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
        } else {
          pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
        }
      }
      pixels1.show();
      pixels2.show();
    }
  }

  if (a == 0) { //LED off
    for (int i = 0; i < 59; i++) {
      pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
    }
    pixels1.show();
  }
  if(c == 1) // 직진 30
  {
    static unsigned int motor = 0;
    analogWrite(11, 230);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, 250);
     motor++;
     if(c==5){
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
     }
     if(motor == 120)
     {
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
      c = 0;
      motor = 0;
     }
  }
  if(c == 2) //후진 30
  {
    static unsigned int motor = 0;
    analogWrite(11, 220);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    analogWrite(6, 250);
    motor++;
    if(c==5){
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
    }

    if(motor == 120)
    {
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
      c = 0;
      motor = 0;
    }
  }
  if(c == 3) // 직진 50
  {
    static unsigned int motor = 0;
    analogWrite(11, 250);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, 250);
    motor++;
    if(c==5){
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
    }     
    if(motor == 200)
    {
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
      c = 0;
      motor = 0;
    }
  }
  if(c == 4) //후진 50
  {
    static unsigned int motor = 0;
    analogWrite(11, 200);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    analogWrite(6, 240);
    motor++;
    if(c==5){
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
    }    
    if(motor == 200)
    {
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
      c = 0;
      motor = 0;
    }
  }
}

void loop() {
  //모터 제어 부분
  if (BTSerial.available())  //블루투스 신호가 오면
  {
    ms = BTSerial.readStringUntil('c');  //c라는 문자를 받기 전을 ms에 저장
    num = ms.toInt();                    //num변수에 정수 저장

    if (num == 1)  //직진
    {
      analogWrite(11, 234);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 250);
    } else if (num == 2)  //후진
    {
      analogWrite(11, 195);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, 255);
    }
    else if (num == 3)  //좌회전
    {
      analogWrite(11,255);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 220);
    } else if (num == 4)  //우회전
    {
      analogWrite(11, 150);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 255);
    }
    else if (num == 5)
    {
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
      c=5;
    }
    if (num == 6) {
      a = 1;
    }
    if (num == 7) {
      a = 0;
    }

    if (num == 8) {
      b = 1;
    }

    if (num == 9) {
      b = 2;
    }

    if (num == 10){
      b = 3;
    }

    if(num == 11){
      c = 1;
    }
    
    if(num == 12){
      c = 2;
    }

    if(num == 13){
      c = 3;
    }
    
    if(num == 14){
      c = 4;
    }
  }
    //초음파 센서 제어
    // digitalWrite(2, HIGH);
    // delayMicroseconds(10);
    // digitalWrite(2, LOW);

    // unsigned long dur = pulseIn(3, HIGH);
    // float dis = ((float)(340 * dur) / 10000) / 2;  //거리계산식

    // if (dis < 3000.0) {
    //   BTSerial.print(String(dis));  //휴대폰으로 거리 송신
    // }
    // delay(1000);
}