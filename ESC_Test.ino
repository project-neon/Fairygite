/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 28 May 2015
  by Michael C. Miller
  modified 8 Nov 2013
  by Scott Fitzgerald

  http://arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define LED_BUILTIN 2

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  Serial.begin(115200);
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
  Serial.println("Iniciando teste:");
 
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  int pos;

  for (pos = 1000; pos <= 2000; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println(pos);
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 2000; pos >= 1000; pos -= 1) { // goes from 180 degrees to 0 degrees
    Serial.println(pos);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
