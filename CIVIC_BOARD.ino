#include <Servo.h>
#include <SoftwareSerial.h>

Servo esc;
SoftwareSerial bluetooth(3, 2); // RX, TX

int ledPin = 13;
int escPin = 9;
int upBtnPin = 5;
int centerBtnPin = 6;
int downBtnPin = 7;

const int UP = 180;
const int DOWN = 20;
const int CENTER = 120;

unsigned int pos = DOWN;

// BLUETOOTH STUFF
String Data = "";

void setup() {
  esc.attach(escPin);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(500);
  
  pinMode(ledPin, OUTPUT);
  pinMode(upBtnPin, INPUT);
  pinMode(centerBtnPin, INPUT);
  pinMode(downBtnPin, INPUT);
  
  Serial.println("READY!");
  
  //esc.write(0);
  //delay(2000);
} 


void loop() { 
  
  // BUTTONS FOR ESC
  if (digitalRead(downBtnPin) == HIGH) {
    // pos = DOWN;
    pos--;
  } else if (digitalRead(centerBtnPin) == HIGH) {
    // pos = CENTER;
    pos = DOWN;
  } else if (digitalRead(upBtnPin) == HIGH) {
    // pos == UP;
    pos++;
  }
  //outputVar("position:", pos);
  
  // ERROR CONTROL
  if (pos > 180) pos = 180;
  else if (pos < 10) pos = 10;

  startListenForBT();

  // COMMIT 
  //esc.write(pos);
  //delay(200);
  
  
  
}

void startListenForBT() {
  while (bluetooth.available()) {
    Serial.println("BLUETOOTH OK");
    char character = bluetooth.read();
    Data = Data + character;
    if (character == 59) { // ;
      Serial.print("Received: ");
      Serial.println(Data);
      
      
      if (Data == "ON;") {
        digitalWrite(ledPin, HIGH);
        Serial.print("LED ON");
      } else if (Data == "OFF;") {
        digitalWrite(ledPin, LOW);
        Serial.print("LED OFF");
      } else {
        Serial.print("NOT A COMMAND! LENGTH: ");
        Serial.println(Data.length());
      }
      
      
      Data = "";
    }
  }
}

