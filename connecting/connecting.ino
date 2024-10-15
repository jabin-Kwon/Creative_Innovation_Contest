#include <WiFi.h> 
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// 핀 설정
#define TRIG_PIN 5  // Trig 핀
#define ECHO_PIN 18  // Echo 핀
#define THRESHOLD_DISTANCE 20  // 20cm 이내에 물체가 있으면 감지

TinyGPSPlus gps;  // GPS 라이브러리 객체
HardwareSerial ss(1);  // Serial1 포트를 사용 (TX, RX 핀 설정)

// Wi-Fi 설정
const char* ssid = "iPhone";  // Wi-Fi 이름
const char* password = "qwert12345";  // Wi-Fi 비밀번호B@
const char* serverName = "http://172.20.10.2:5000/post-data";  // Flask 서버의 IP 주소

// Wi-Fi 연결 설정
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

// 물체 감지 함수
bool detectObject() {
  long duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo 핀에서 반사 신호를 수신하여 거리 계산
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;  // 거리 계산 (cm)

  // 감지 거리 이내에 물체가 있으면 true 반환
  return (distance <= THRESHOLD_DISTANCE);
}

// 서버로 GPS 정보 전송
void sendData(String lat, String lng, String has) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // 서버 연결 및 헤더 설정
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // 타임아웃 설정 (예: 5000ms = 5초)
    http.setTimeout(5000);

    // 전송할 데이터 설정
    String httpRequestData = "latitude=" + lat + "&longitude=" + lng + "&hasTrash=" + has;
    Serial.print("Sending data: ");
    Serial.println(httpRequestData);

    // HTTP POST 요청 보내기
    int httpResponseCode = http.POST(httpRequestData);

    // HTTP 요청이 완료되었는지 확인
    if (httpResponseCode > 0) {
      String response = http.getString();  // 서버로부터 응답 읽기
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // HTTP 연결 종료
    http.end();
    
  } else {
    Serial.println("Error: WiFi not connected");
  }
}



void setup() {
  Serial.begin(115200);
  ss.begin(9600, SERIAL_8N1, 16, 17);  // GPS 모듈과 통신 (TX, RX 핀)

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Wi-Fi 연결
  connectToWiFi();
}

void loop() {
  String latitude = "37.2477454";  // 기본값으로 초기화
  String longitude = "127.0784705";  // 기본값으로 초기화
  String hasTrash = "false"; // 기본값으로 초기화

  if (detectObject()) {  // 물체 감지 시
    Serial.println("Object detected");

    while (ss.available() > 0) {  // GPS 데이터 읽기
      gps.encode(ss.read());
      if (gps.location.isUpdated()) {  // GPS 데이터가 업데이트되면
        latitude = String(gps.location.lat(), 6);  // GPS에서 수신한 위도 값
        longitude = String(gps.location.lng(), 6);  // GPS에서 수신한 경도 값
      }
    }

    hasTrash = "true";  // 물체가 감지되면 true로 설정
  } else {
    Serial.println("No object detected");
  }

  // GPS 데이터 및 물체 유무 상태 출력
  Serial.print("Latitude: ");
  Serial.println(latitude);
  Serial.print("Longitude: ");
  Serial.println(longitude);
  Serial.print("hasTrash: ");
  Serial.println(hasTrash);

  // 서버로 데이터 전송
  sendData(latitude, longitude, hasTrash);

  delay(1000);  // 1초마다 감지
}

