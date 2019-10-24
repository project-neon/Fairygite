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

#define MESSAGE_TIMEOUT 50 //milli

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
int pow_esc = 0;

int last_message_time = 0;

int weapon_status = 0;
int weapon_power = 0;

WiFiUDP udp;

void setup() {
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MLPWM, OUTPUT);
  pinMode(MRPWM, OUTPUT);
  Serial.begin(115200);
  delay(10);
  //
  //Configuração do ESC
  //

  weaponMotor.attach(15, 1000, 2000);  // attaches the servo on GIO2 to the servo object  

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Iniciando ESC
  //  delay(5000);
  Serial.println("Iniciando Configuração da ESC");
  Serial.println("Mapeando Max");
 
  for (int i = 0; i < 40; i++)  {
    weaponMotor.writeMicroseconds(2000);
    delay(50);
  } 
 
  digitalWrite(LED_BUILTIN, LOW); 
  
  Serial.println("Mapeando Min");
  for (int i = 0; i < 40; i++) {
    weaponMotor.writeMicroseconds(1000);
    delay(50);
  }
  Serial.println("Fim de configuração da ESC");
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
}

void sendToWeapon(int power) {
  pow_esc = 1000 + power * 10;
  weaponMotor.writeMicroseconds(pow_esc);
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
  
    request = "";
    delay(1);
    if (udp.parsePacket() > 0) {
      request = "";
      while (udp.available() > 0) { 
        char z = udp.read();
        request+= z;
      }

      last_message_time = millis();
      /*
      * -100+1001666
      * ------------
      * 000000000011
      * 012345678901
      * 
      * [0, 3]: velocidade linear
      * [4, 7]: velocidade angular
      * [8]: switch da arma
      * [9, 11]: potencia da arma
      */
      pow_linear = request.substring(0, 4).toInt(); // [-100:100]
      pow_angular = request.substring(4, 8).toInt(); // [-50:50]
      weapon_status = request.substring(8, 9).toInt(); // [0:1]
      weapon_power = request.substring(9).toInt(); // [0:100]
  } else {
      // IDLE MODE (NO COMM)
      if ((millis() - last_message_time) > MESSAGE_TIMEOUT) {
        pow_linear = 0; // [-100:100]
        pow_angular = 0; // [-100:100]
        weapon_status = 0; // [0:1]
        weapon_power = 0; // [0:100]
      }
  }
  Serial.println("------------");
  Serial.println(pow_linear);
  Serial.println(pow_angular);
  Serial.println(weapon_status);
  Serial.println(weapon_power);
  Serial.println("------------");

  if(weapon_status) {
    sendToWeapon(weapon_power);
  } else {
    sendToWeapon(0);
  }
  
  if (pow_linear != 0) {
    pow_angular/2;
  }
  
  pow_left = pow_linear + pow_angular;
  pow_right = pow_linear - pow_angular;
  
  sendToMotor(pow_left, pow_right);
}
