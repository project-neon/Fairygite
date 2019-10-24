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

Servo weaponMotor;

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
//Conectar com Wifi
//
  Serial.println("");
  Serial.println("");
  Serial.print("conenctando a ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }

  udp.begin(80);

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
    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);
  }else{
    digitalWrite(ML1, 0);
    digitalWrite(ML2, 1);
  }

  if(powerMR > 0){
    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }else{
    digitalWrite(MR1, 0);
    digitalWrite(MR2, 1);
  }
  powerML = powerML * (1023 / 100.0);
  powerMR = powerMR * (1023 / 100.0);

  analogWrite(MLPWM, powerML);
  analogWrite(MRPWM, powerMR);
  
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
  
    request = "";
    delay(1);
    if (udp.parsePacket() > 0) {
      request = "";
      while (udp.available() > 0) { 
        char z = udp.read();
        request+= z;
      }
      /*
      * -100+1001
      * ---------
      * 000000000
      * 012345678
      */
      pow_linear = request.substring(0, 4).toInt(); // [-100:100]
      pow_angular = request.substring(4, 8).toInt(); // [-50:50]
      weapon = request.substring(8).toInt(); // [0:1]


      Serial.println("------------");
      Serial.println(pow_linear);
      Serial.println(pow_angular);
      Serial.println(weapon);
      Serial.println("------------");
      
      weaponMotor.writeMicroseconds(weapon * 1700);
    
    weaponMotor.writeMicroseconds(weapon * 1700);

//    sendToMotor(MLPWM, pow_linear);
//    sendToMotor(MRPWM, pow_linear);
  }
}
