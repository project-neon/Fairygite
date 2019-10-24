//Wifi and communication includes
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Motor control includes
#include <Servo.h>


// WHEELS MOTOR PINS
#define ML1     5
#define ML2     16
#define MLPWM   4

#define MR1     0
#define MR2     2
#define MRPWM   14

// WEAPONS MOTOR PINS
#define WEAPONPWM 15

Servo myservo;

const char* ssid = "AndroidAP5554";
const char* password = "wzte4104";

String request = "";

int pow_linear = 0;
int pow_angular = 0;
int weapon = 0;

int pow_left = 0;
int pow_right = 0;
int pow_left_dir = 0;
int pow_right_dir = 0;


int weapon_status = 0;

WiFiUDP udp;

void setup() {
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MLPWM, OUTPUT);
  pinMode(MRPWM, OUTPUT);
//  
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  delay(10);


//
//Configuração do ESC
//

  myservo.attach(15, 1000, 2000);  // attaches the servo on GIO2 to the servo object  

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Iniciando ESC
//  delay(5000);
  Serial.println("Iniciando Configuração");
  Serial.println("Max");
 
  for (int i = 0; i < 40; i++)  {
    myservo.writeMicroseconds(2000);
    delay(50);
  } 
 
  digitalWrite(LED_BUILTIN, LOW); 
//  while(Serial.available() <=0);
  
  Serial.println("Min");
  for (int i = 0; i < 40; i++) {
    myservo.writeMicroseconds(1000);
    delay(50);
  }
  Serial.println("Fim de configuração");
  
  
}

void sendToMotor(int powerML, int powerMR){
  if(powerML > 0){
    Serial.print("ml+");
    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);
  }else{
    Serial.print("ml-");
    digitalWrite(ML1, 0);
    digitalWrite(ML2, 1);
  }

  if(powerMR > 0){
    Serial.print("mr+");
    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }else{
    Serial.print("mr-");
    digitalWrite(MR1, 0);
    digitalWrite(MR2, 1);
  }
  powerML = powerML * (1023 / 100.0);
  powerMR = powerMR * (1023 / 100.0);

  analogWrite(MLPWM, abs(powerML));
  analogWrite(MRPWM, abs(powerMR));
  
}

void stopA(){
  digitalWrite(ML1, 0);
  digitalWrite(ML2, 0);
}
void stopB(){
  digitalWrite(MR1, 0);
  digitalWrite(MR2, 0);
}

void loop() {
  /*
   * Request sera continuamente atualizado
   * Request virá num protocolo na forma "POW_LEFT:POW_RIGHT:WEAPON_SWITCH" sendo:
   * POW_LEFT: potencia, de 0 a 100, do motor esquerdo
   * POW_RIGHT: potencia, de 0 a 100, do motor direito
   * WEAPON_SWITCH: switch para ligar/desligar arma (caso tenha). 0 desligado, 1 ligado
   */

    sendToMotor(100,100);
    delay(1000); 
    sendToMotor(-100,-100);
    for (int vel = 0; vel <= 100; vel += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println(vel);
    sendToMotor(vel,vel);             // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
    for (int vel = 0; vel <= 100; vel += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println(-vel);
    sendToMotor(-vel,-vel);             // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }

  int pos;

  for (pos = 1000; pos <= 2000; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println(pos);
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 2000; pos >= 1000; pos -= 5) { // goes from 180 degrees to 0 degrees
    Serial.println(pos);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
 }
