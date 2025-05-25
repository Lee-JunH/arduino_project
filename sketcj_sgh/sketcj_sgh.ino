//삼각형 + 모터제어 성공
#include <SoftwareSerial.h>     //serial통신을 위한 라이브러리
#include <Adafruit_NeoPixel.h>  //neoPixel제어를 위한 라이브러리
#include <avr/io.h>
#include <avr/interrupt.h>

    SoftwareSerial BTSerial(12, 13);  //BT의 TX:12, RX:13 (클래스 선언)

String ms = "";  //BT로부터 받을 메시지 변수
int num;         //메시지를 숫자로 저장할 변수
int speed;       //속도를 저장할 변수
int a = 10;
int b = 0;


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
  OCR1A = 15624;                        // 비교 매치 레지스터 값 설정 (16 MHz / (1024 * 1 Hz) - 1)
  TCCR1B |= (1 << WGM12);               // CTC 모드 설정
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 분주율 설정

  TIMSK1 |= (1 << OCIE1A);  // 타이머 비교 인터럽트 활성화

  sei();  // 인터럽트 활성화
}

ISR(TIMER1_COMPA_vect) {
  if (a == 11) {

    static unsigned int count = 0;
    count++;



    if (count % 2 == 1) 
    {
      if (b) {
        for (int i = 0; i < 59; i++) 
        {
          if (i % 2 == 0) {
            pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
            pixels2.setPixelColor((i % 22), pixels1.Color(255, 0, 0));
          } 
          
          else {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          }

        }

        pixels1.show();
      }
      
      
      
      else if (!b) {
        for (int i = 0; i < 59; i++) {
          if (i % 2 == 0) {
            pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          } else {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          }
        }
        pixels1.show();
      }
    } 
    
    
    
    
    else 
    {
      if (b) {
        for (int i = 0; i < 59; i++) {
          if (i % 2 == 1) {
            pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
            pixels2.setPixelColor((i % 22), pixels1.Color(255, 0, 0));
          } else {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          }
        }
        pixels1.show();
      }



      else if (!b) {
        for (int i = 0; i < 59; i++) {
          if (i % 2 == 1) {
            pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          } else {
            pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));  // RGB 값 (빨간색)
            pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
          }
        }
        pixels1.show();
      }
    }
  }




  if (a == 10) {
    for (int i = 0; i < 59; i++) {
      pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
      pixels2.setPixelColor((i % 22), pixels1.Color(0, 0, 0));
    }
    pixels1.show();
  }
}

void loop() {
  //모터 제어 부분
  if (BTSerial.available())  //블루투스 신호가 오면
  {
    ms = BTSerial.readStringUntil('c');  //c라는 문자를 받기 전을 ms에 저장
    num = ms.toInt();                    //num변수에 정수 저장

    switch (num) {
      case 1:  //전진
        analogWrite(11, 255);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, 205);

      case 2:  //후진
        analogWrite(11, 255);
        digitalWrite(10, LOW);
        digitalWrite(9, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(7, HIGH);
        analogWrite(6, 205);

      case 3:  //좌회전
        analogWrite(11, 100);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        analogWrite(6, 0);

      case 4:  //우회전
        analogWrite(10, 0);
        digitalWrite(10, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, 255);

      case 5:  //정지
        analogWrite(11, 0);
        digitalWrite(10, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        analogWrite(6, 0);

      case 7:  //LED OFF
        a = 10;

      case 6:  //LED ON
        a = 11;

      case 8:  // ->
        b = 1;

      case 9:  // <-
        b = 2;

      case 10:  //화살표 off
        b = 0;
      default :
        break ;
        
    }
    //초음파 센서 제어 부분
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    delayMicroseconds(2);
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);

    unsigned long dur = pulseIn(3, HIGH);
    float dis = ((float)(340 * dur) / 10000) / 2;  //거리계산식

    if (dis < 3000.0) {
      BTSerial.print(String(dis));  //휴대폰으로 거리 송신
    }
    delay(500);
  }
}