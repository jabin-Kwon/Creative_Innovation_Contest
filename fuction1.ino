#include <DFRobotDFPlayerMini.h> 
#include <SoftwareSerial.h>

//SoftwareSerial mp3_serial(10, 11);
//DFRobotDFPlayerMini mp3;

// 분무기 1
int AA1 = 5;
int AB1 = 4;

// 분무기 2
int BA = 7;
int BB = 6;


void setup() 
{
  // 초음파 1
  pinMode(2, OUTPUT);  
  pinMode(3, INPUT);

  // 분무기 1(벌레)
  pinMode(BA, OUTPUT);
  pinMode(BB, OUTPUT);

  // 분무기 2(비둘기)
  pinMode(AA1, OUTPUT);
  pinMode(AB1, OUTPUT);

  /*Serial.begin(9600);
  mp3_serial.begin(9600);

  mp3.begin(mp3_serial);

  mp3.volume(30);*/
}

void loop() 
{
  int light = analogRead(A0); // 조도센서 입력 값
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  long duration = pulseIn(3, HIGH);
  long distance = duration / 58.824;
  Serial.println(String("distance: ")+distance);
  Serial.println(String("light: ")+light);
  
  // 낮
    if ((light > 500) && (distance < 20))
    {
      // 정방향으로 모터 회전
      digitalWrite(AA1, HIGH); 
      digitalWrite(AB1, LOW);
      delay(5000);
      
      // 모터 정지
      digitalWrite(AA1, LOW); 
      digitalWrite(AB1, LOW);
      delay(1000);
    }
   // 밤
    else if (light < 500 && distance < 20)
    {
      // 정방향으로 모터 회전
      digitalWrite(BA, HIGH); 
      digitalWrite(BB, LOW);
      delay(5000);

      // 모터 정지
      digitalWrite(BA, LOW); 
      digitalWrite(BB, LOW);
      delay(1000); 
    }
    else
    {
      delay(6000);
    }

}
