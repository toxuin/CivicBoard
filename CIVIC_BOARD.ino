#include <Servo.h>
#include <SoftwareSerial.h>

#define BATTERY_PIN A6
#define LED_PIN 13
#define ESC_PIN 9
#define CONTROL_BTN_PIN 10
#define BUZZER_PIN 11
#define BLUETOOTH_TX 2
#define BLUETOOTH_RX 3

Servo esc;
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX

int UP = 180;
int DOWN = 20;
int CENTER = 120;

unsigned int pos = DOWN;

// BLUETOOTH STUFF
String Data = "";

void setup() {
  esc.attach(ESC_PIN);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(500);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(CONTROL_BTN_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT_PULLUP);
  
  tone(BUZZER_PIN, 300, 100);
  Serial.println("READY!");
    
  //esc.write(0);
  //delay(2000);
} 


void loop() { 
  
  /*
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
  */
  //outputVar("position:", pos);
  
  // ERROR CONTROL
  if (pos > 180) pos = 180;
  else if (pos < 10) pos = 10;
  
  getBluetoothCommands();

  // COMMIT 
  //esc.write(pos);
  //delay(200);
  
  
  
}

void getBluetoothCommands() {
  while (bluetooth.available()) {
    char character = bluetooth.read();
    Data = Data + character;
    if (character == 59) { // ;
      Serial.print("Received: ");
      Serial.println(Data);
      
      
      if (Data == "BATT;") {
        bluetooth.print(batteryVoltage(), DEC);
        Serial.print("LED ON");
      } else if (Data == "OFF;") {
        digitalWrite(LED_PIN, LOW);
        Serial.print("LED OFF");
      } else {
        Serial.print("NOT A COMMAND! LENGTH: ");
        Serial.println(Data.length());
      }
      
      
      Data = "";
    }
  }
}

float batteryVoltage() { 
  float voltageSum = 0;
  for (int i = 0; i < 10; i++) {
    int batteryVoltage = analogRead(BATTERY_PIN);    
    voltageSum += batteryVoltage * (3.3 / 1023.0);
  }
  return voltageSum/10;
}

