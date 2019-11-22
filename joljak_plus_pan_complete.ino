#include <HuemonelabKit.h> //블루투스와 RGB LED 코드는 휴몬랩 라이브러리 사용
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address

#define FAN1 9 //DC팬 핀 설정
#define FAN2 2 //DC팬 핀 설정
#define FAN3 7 //DC팬 핀 설정


int n=0;
Bluetooth BT(8,3); // 블루투스 TX, RX핀 설정


int dustPin1 = A0; //미세먼지센서의 아날로그 핀 설정
int ledPower1 = 5; //미세먼지센서의 적외선감지 핀 설정
int dustPin2 = A1; //미세먼지센서의 아날로그 핀 설정
int ledPower2 = 6;
int dustPin3 = A2; //미세먼지센서의 아날로그 핀 설정
int ledPower3 = A4;
int dustPin4 = A3; //미세먼지센서의 아날로그 핀 설정
int ledPower4 =A5;

int samplingTime = 280; //샘플링 시간 설정 (내부 LED를 켠 후 출력 신호 측정)
int deltaTime = 40; // LED를 다시 끄기 전 40(마이크로 초)동안 일시 정지
int sleepTime = 9680;

float voMeasured1 = 0; //미세먼지센서가 그대로 읽어오는 아날로그 값
float calcVoltage1 = 0; // 미세먼지센서가 측정하는 전기신호 (IR Diode로 들어오는 빛 주기)
float dustDensity1 = 0; //미센먼지 밀도 계산식으로 환산된 값

float voMeasured2 = 0; //미세먼지센서가 그대로 읽어오는 아날로그 값
float calcVoltage2 = 0; // 미세먼지센서가 측정하는 전기신호 (IR Diode로 들어오는 빛 주기)
float dustDensity2 = 0; //미센먼지 밀도 계산식으로 환산된 값

float voMeasured3 = 0; //미세먼지센서가 그대로 읽어오는 아날로그 값
float calcVoltage3 = 0; // 미세먼지센서가 측정하는 전기신호 (IR Diode로 들어오는 빛 주기)
float dustDensity3 = 0; //미센먼지 밀도 계산식으로 환산된 값

float voMeasured4 = 0; //미세먼지센서가 그대로 읽어오는 아날로그 값
float calcVoltage4 = 0; // 미세먼지센서가 측정하는 전기신호 (IR Diode로 들어오는 빛 주기)
float dustDensity4 = 0; //미센먼지 밀도 계산식으로 환산된 값

char server[] = "192.168.0.3";
IPAddress ip(192,168,0,3); 
EthernetClient client; 


void setup(){
  Serial.begin(9600);
  pinMode(ledPower1,OUTPUT); //미세먼지 센서 적외선 LED 핀모드 설정
  pinMode(ledPower2,OUTPUT); //미세먼지 센서 적외선 LED 핀모드 설정
  pinMode(ledPower3,OUTPUT); //미세먼지 센서 적외선 LED 핀모드 설정
  pinMode(ledPower4,OUTPUT); //미세먼지 센서 적외선 LED 핀모드 설정
  BT.begin(9600); //휴몬랩 라이브러리 블루투스 설정
  pinMode(FAN1,OUTPUT); //DC팬 핀모드 설정
  pinMode(FAN2,OUTPUT); //DC팬 핀모드 설정
  pinMode(FAN3,OUTPUT); //DC팬 핀모드 설정
  
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);
  }
  
  delay(2000);
}


void loop(){
  if(n==0)
  {
    digitalWrite(FAN3,LOW);
    digitalWrite(FAN2,LOW);
    digitalWrite(FAN1,LOW);
    n++;
  }
  digitalWrite(ledPower1,LOW); // 미세먼지 Led 켜기
  digitalWrite(ledPower2,LOW); // 미세먼지 Led 켜기
  digitalWrite(ledPower3,LOW); // 미세먼지 Led 켜기
  digitalWrite(ledPower4,LOW); // 미세먼지 Led 켜기
  delayMicroseconds(samplingTime);
  voMeasured1 = analogRead(dustPin1); //미세먼지 값 읽기
  voMeasured2 = analogRead(dustPin2); //미세먼지 값 읽기
  voMeasured3 = analogRead(dustPin3); //미세먼지 값 읽기
  voMeasured4 = analogRead(dustPin4); //미세먼지 값 읽기
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower1,HIGH); // 미세먼지 Led 끄기
  digitalWrite(ledPower2,HIGH); // 미세먼지 Led 끄기
  digitalWrite(ledPower3,HIGH); // 미세먼지 Led 끄기
  digitalWrite(ledPower4,HIGH); // 미세먼지 Led 끄기
  delayMicroseconds(sleepTime);

calcVoltage1 = voMeasured1*(5.0/1024.0);
dustDensity1 = (0.17*calcVoltage1-0.1)*1000;
if (dustDensity1<0)
  dustDensity1=0;
  
calcVoltage2 = voMeasured2*(5.0/1024.0);
dustDensity2 = (0.17*calcVoltage2-0.1)*1000;
if (dustDensity2<0)
  dustDensity2=0;

calcVoltage3 = voMeasured3*(5.0/1024.0);
dustDensity3 = (0.17*calcVoltage3-0.1)*1000;
if (dustDensity3<0)
  dustDensity3=0;

calcVoltage4 = voMeasured4*(5.0/1024.0);
dustDensity4 = (0.17*calcVoltage4-0.1)*1000;
if (dustDensity4<0)
  dustDensity4=0;
  //먼지 밀도를 출력 전압(vo)에 비례하여 ㎍/m³단위의 먼지 밀도로 매핑 (Chris Nafis의 계산법 참고)
  Serial.print(dustDensity1);
  Serial.print(", ");
  Serial.print(dustDensity2);
  Serial.print(", ");
  Serial.print(dustDensity3);
  Serial.print(", ");
  Serial.println(dustDensity4);

//시리얼모니터/블루투스에 미세먼지 농도값 표시
 char data[12] = {0,};
 Serial.println(data);
 sprintf(data,"%03d%03d%03d%03d", (int)dustDensity1, (int)dustDensity2, (int)dustDensity3, (int)dustDensity4);
 Serial.println(data);
 BT.print(data);
 Sending_To_phpmyadmindatabase();  
 delay(1000);

if (BT.available() ) 
  {
    char text = BT.read();
    Serial.print(text);
    if (text == 'a') 
    {
      digitalWrite(FAN1,HIGH);
    } 
    else if (text == 'b')
    {
      digitalWrite(FAN1,LOW);
    }
    else if (text == 'c')
    {
      digitalWrite(FAN2,HIGH);
    }
    else if (text == 'd')
    {
      digitalWrite(FAN2,LOW);
    }
    else if (text == 'e')
    {
      digitalWrite(FAN3,HIGH);
    
    }else if (text == 'f')
    { 
      digitalWrite(FAN3,LOW);
    }
   
  }
}




void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /testcode/dustdust.php?dust1=");
    client.print("GET /testcode/dustdust.php?dust1=");     //YOUR URL
    Serial.println(dustDensity1);
    client.print(dustDensity1);
    client.print("&dust2=");
    Serial.println("&dust2=");
    client.print(dustDensity2);
    Serial.println(dustDensity2);
    client.print("&dust3=");
    Serial.println("&dust3=");
    client.print(dustDensity3);
    Serial.println(dustDensity3);
    client.print("&dust4=");
    Serial.println("&dust4=");
    client.print(dustDensity4);
    Serial.println(dustDensity4);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.3");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
