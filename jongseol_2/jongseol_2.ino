#include <SoftwareSerial.h>     //serial통신을 위한 라이브러리
#include <Adafruit_NeoPixel.h>  //neoPixel제어를 위한 라이브러리
#include <avr/io.h>
#include <avr/interrupt.h>

Adafruit_NeoPixel pixels2(22, 2, NEO_GRB + NEO_KHZ800);  //화살표 픽셀 2번 핀
Adafruit_NeoPixel pixels1(59, 3, NEO_GRB + NEO_KHZ800);  //삼각형 픽셀 3번 핀
SoftwareSerial BTSerial(12, 13);  //BT의 TX:12번 핀, RX:13번 핀 (클래스 선언)

int num;  //메시지를 숫자로 저장할 변수

int a = 0;
int b = 3;
int c = 0;

void setup() {
  Serial.begin(9600);    //PC-아두이노간 통신
  BTSerial.begin(9600);  //스마트폰-아두이노간 통신
  pixels1.begin();       //네오픽셀 시작
  pixels2.begin();
  pixels1.show();  //네오픽셀 초기화
  pixels2.show();

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
  OCR1A = 5156;                         // 비교 매치 레지스터 값 설정 (16 MHz / (1024 * 1 Hz) - 1)
  TCCR1B |= (1 << WGM12);               // CTC 모드 설정
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 분주율 설정

  TIMSK1 |= (1 << OCIE1A);  // 타이머 비교 인터럽트 활성화

  sei();  // 인터럽트 활성화
}

ISR(TIMER1_COMPA_vect) {
  //LED off
  if (a == 0) {
    for (int i = 0; i < 59; i++)
      pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
    pixels1.show();
  }
  //LED on
  if (a == 1) {
    static unsigned int count = 0;
    count++;
    if (count % 2 == 1) {
      if (b == 1)  //화살표1
      {
        for (int i = 0; i < 22; i++) {
          if ((i >= 0 && i <= 5) || (i >= 12 && i <= 21))
            pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
          if (i >= 6 && i <= 11)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        }
      }
      if (b == 2)  //화살표2
      {
        for (int i = 0; i < 22; i++) {
          if ((i >= 6 && i <= 12) || (i >= 12 && i <= 21))
            pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
          if (i >= 0 && i <= 5)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        }
      }
      if (b == 3)  //끔
      {
        for (int i = 0; i < 22; i++) {
          if (i % 2 == 0)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          else
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));  // RGB 값 (빨간색)
        }
      }
      //LED
      for (int i = 0; i < 59; i++) {
        if (i % 2 == 0)
          pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
        else
          pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
      }
      pixels1.show();
      pixels2.show();
    } else {
      if (b == 1)  //화살표1
      {
        for (int i = 0; i < 22; i++) {
          if ((i >= 0 && i <= 5) || (i >= 12 && i <= 21))
            pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));  // RGB 값 (빨간색)
          if (i >= 6 && i <= 11)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        }
      }
      if (b == 2)  //화살표2
      {
        for (int i = 0; i < 22; i++) {
          if ((i >= 6 && i <= 12) || (i >= 12 && i <= 21))
            pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));  // RGB 값 (빨간색)
          if (i >= 0 && i <= 5)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        }
      }
      if (b == 3)  //끔
      {
        for (int i = 0; i < 22; i++) {
          if (i % 2 == 0)
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          else
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));  // RGB 값 (빨간색)
        }
      }
      //LED
      for (int i = 0; i < 59; i++) {
        if (i % 2 == 1)
          pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
        else
          pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
      }
      pixels1.show();
      pixels2.show();
    }
  }
}

void loop() {

  //motor control
  if (BTSerial.available())
  {
    char data = BTSerial.read();
    if (data == '1')  //직진
    {
      analogWrite(11, 250);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 250);
    }
    else if (data == '2')  //후진
    {
      analogWrite(11, 200);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, 200);
    }
    else if (data == '3')  //좌회전
    {
      analogWrite(11, 253);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 253);
    }
    else if (data == '4')  //우회전
    {
      analogWrite(11, 253);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, 253);
    }
    else if (data == '5')  //정지
    { 
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
    }
    if (data == '6'){  //LED on
      a = 1;
    }
    if (data == '7') {  //LED off
      a = 0;
    }
    if (data == '8') {  //화살표1 LED
      b = 1;
    }
    if (data == '9') {  //화살표2 LED
      b = 2;
    }
    if (data == 'a') {  //화살표 LED off
      b = 3;
    }
  }

  //auto control
  if (Serial.available())
  {
    char received_char = Serial.read();
    if (received_char == 'G')  //직진
    {
      analogWrite(11, 185);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 185);
    }
    else if (received_char == 'L')  //좌회전
    {
      analogWrite(11, 240);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 240);
      delay(100);
      analogWrite(11, 200);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 200);
      delay(10);
    }
    else if (received_char == 'R')  //우회전
    {      
      analogWrite(11, 240);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, 240);
      delay(100);
      analogWrite(11, 200);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 200);
      delay(10);
    }
    else if (received_char == 'S')  //정지
    { 
      analogWrite(11, 0);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, 0);
    }
  }
}