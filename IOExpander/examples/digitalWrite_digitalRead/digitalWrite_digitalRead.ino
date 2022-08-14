#include <IOExpander.h>

IOEXPANDER iox;

#define sda 21
#define scl 22
#define freq 400000

#define sw1 1
#define sw2 15
#define led1 6
#define led2 7
#define led3 11
// debug
byte value = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  // Initialization
  if (iox.init(sda, scl, freq))
  {
    Serial.println("IOExpander initialization success");
  }
  // configure pinMode
  iox.pinMode(sw1, INPUT_PULLUP);
  iox.pinMode(sw2, INPUT_PULLUP);
  iox.pinMode(led1, OUTPUT);
  iox.pinMode(led2, OUTPUT);
  iox.pinMode(led3, OUTPUT);

  iox.showAllConfigure();
}

void loop() {
  Serial.print("sw1 : "); Serial.println(iox.digitalRead(sw1));
  Serial.print("sw2 : "); Serial.println(iox.digitalRead(sw2));
  iox.digitalWrite(led1, HIGH);
  iox.digitalWrite(led2, LOW);
  iox.digitalWrite(led3, LOW);
  iox.showAllConfigure();
  delay(1000);
  Serial.print("sw1 : "); Serial.println(iox.digitalRead(sw1));
  Serial.print("sw2 : "); Serial.println(iox.digitalRead(sw2));
  iox.digitalWrite(led1, LOW);
  iox.digitalWrite(led2, HIGH);
  iox.digitalWrite(led3, LOW);
  iox.showAllConfigure();
  delay(1000);
  Serial.print("sw1 : "); Serial.println(iox.digitalRead(sw1));
  Serial.print("sw2 : "); Serial.println(iox.digitalRead(sw2));
  iox.digitalWrite(led1, LOW);
  iox.digitalWrite(led2, LOW);
  iox.digitalWrite(led3, HIGH);
  iox.showAllConfigure();
  delay(1000);
  
}
