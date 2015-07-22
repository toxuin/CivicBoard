//#include <Servo.h>
#include <SoftwareServo.h> 
#include <SoftwareSerial.h>
#include <Tlc5940.h>
#include <tlc_fades.h>

#define BATTERY_PIN A3
#define ESC_PIN 6
#define SAFETY_PIN 12
#define BUZZER_TLC_CHANNEL 15
#define BLUETOOTH_TX 5
#define BLUETOOTH_RX 4

#define RGB_LED_COUNT 5

#define ESC_WRITE_INTERVAL 30

#define ESC_MINIMUM 9
#define ESC_MIDPOINT 90
#define ESC_MAXIMUM 170

SoftwareServo esc;

SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX

unsigned long escMillis = 0;
unsigned long accelMillis = 0;
unsigned long previousEscMillis = 0;
unsigned long previousAccelMillis = 0;

unsigned int pos = ESC_MIDPOINT;
unsigned int actualPos = ESC_MIDPOINT;
unsigned int accelerationDelay = 200;
unsigned int deccelerationDelay = 50;

unsigned int tlcRedPins[] = {0, 3, 6, 9, 12};
unsigned int tlcGreenPins[] = {1, 4, 7, 10, 13};
unsigned int tlcBluePins[] = {2, 5, 8, 11, 14};

// BLUETOOTH STUFF
String Data = "";

// ANIMATION THINGS
bool anim = false;
bool randomPulsing = false;
unsigned int bpm = 0;

void setup() {
  esc.attach(ESC_PIN);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(500);
  
  Tlc.init();
  delay(100);
  
  pinMode(SAFETY_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT);
  
  esc.write(ESC_MIDPOINT);
  pos = ESC_MIDPOINT;
  actualPos = ESC_MIDPOINT;
  
  beep(300, 300);
  Serial.println(F("READY!"));
} 

void loop() { 
  // ERROR CONTROL
  if (pos > ESC_MAXIMUM) pos = ESC_MAXIMUM;
  else if (pos < ESC_MINIMUM) pos = ESC_MINIMUM;
  
  // GRADUAL ACCELERATION
  if (pos < actualPos) {
    //actualPos = pos;
    
    accelMillis = millis();
    if (accelMillis - previousAccelMillis > deccelerationDelay) {
      if (actualPos > ESC_MIDPOINT) actualPos = ESC_MIDPOINT;
      else actualPos--;
      Serial.print(F("Deccelerated to "));
      Serial.println(actualPos);
      previousAccelMillis = accelMillis;
    }
    
    
  } else if (pos > actualPos) {
    accelMillis = millis();
    if (accelMillis - previousAccelMillis > accelerationDelay) {
      if (actualPos < ESC_MIDPOINT) actualPos = ESC_MIDPOINT;
      else actualPos++;
      Serial.print(F("Accelerating to "));
      Serial.print(pos);
      Serial.print(F(", now "));
      Serial.println(actualPos);
      previousAccelMillis = accelMillis;
    }
  }
  
  // ERROR CONTROL
  if (actualPos > ESC_MAXIMUM) actualPos = ESC_MAXIMUM;
  else if (actualPos < ESC_MINIMUM) actualPos = ESC_MINIMUM;
  
  getBluetoothCommands();

  // COMMIT 
  if (digitalRead(SAFETY_PIN) == LOW) {
      pos = ESC_MINIMUM;
      actualPos = ESC_MINIMUM;
      beep(750, 300);
      delay(300);
  }
  
  escMillis = millis();
  if (escMillis - previousEscMillis > ESC_WRITE_INTERVAL) {
    previousEscMillis = escMillis;
    esc.write(actualPos);
  }
  
  // ANIM PART
  if (anim) {
    if (bpm != 0) {
      for (int channel = 0; channel < 15; channel++) {
        if (!tlc_isFading(channel)) {
          Tlc.set(channel, random(4095));
          Tlc.update();
          tlc_addFade(channel, Tlc.get(channel), 0, millis(), millis()+(60000/bpm));
        }
      }
    } else {
      for (int channel = 0; channel < 15; channel++) {
        if (!tlc_isFading(channel)) {
            tlc_addFade(channel, Tlc.get(channel), random(4095), millis(), millis() + 1000);
        }
      }      
    }
    
    tlc_updateFades();
  }
 
  SoftwareServo::refresh();
}

void getBluetoothCommands() {
  while (bluetooth.available()) {
    char character = bluetooth.read();
    Data = Data + character;
    if (character == 59) { // ;
      Serial.print(F("Received: "));
      Serial.println(Data);
      
      if (Data == "BATT;") {
        bluetooth.print(F("B"));
        bluetooth.print(batteryVoltage(), 2);
        bluetooth.println(F(";"));
        Serial.println(F("BATTERY REQUESTED"));
      }
      
      
      else if (Data == "FADE;") {
        anim = !anim;
        if (!anim) {
          Tlc.clear();
          Tlc.update();
        }
        Serial.println(F("ANIM STARTED"));
      }
      
      else if (Data == "PING;") {
        bluetooth.println(F("PONG;"));
        Serial.println(F("PING REQUESTED"));
      }
      
      else if (Data == "STOP;") {
        pos = ESC_MINIMUM;
        Serial.println(F("STOP REQUESTED"));
      }
        
      else if (Data == "BEEP;") {
        beep(200, 100);
        Serial.println(F("BEEP REQUESTED"));
      }  
      
      else if (Data.charAt(0) == 'A') {
        String value = Data.substring(1, Data.length() - 1); // STRIPPING THE TRAILING ';'
        accelerationDelay = value.toInt();
        Serial.print(F("Acceleraion delay set to "));
        Serial.println(accelerationDelay);
      }
      
      
      else if (Data.charAt(0) == 'D') {
        String value = Data.substring(1, Data.length() - 1);
        deccelerationDelay = value.toInt();
        Serial.print(F("Decceleraion delay set to "));
        Serial.println(deccelerationDelay);
      }
      
      else if (Data.charAt(0) == 'T') {
        int single_channel = -1;
        
        // SELECT CHANNEL (R, G, B)
        unsigned int *channel;
        if (Data.charAt(1) == 'R') {
          channel = tlcRedPins;
        } else if (Data.charAt(1) == 'G') {
          channel = tlcGreenPins;
        } else if (Data.charAt(1) == 'N') {
          channel = tlcBluePins;
        } else {
          // THIS IS SPECIAL CHANNEL DATA
          char buf = Data.charAt(1);
          single_channel = (int) strtol(&buf, NULL, 16);
        }
        
        // PARSE HEX (0-255)
        String val = Data.substring(2, Data.length() - 1);
        unsigned int number = (int) strtol(val.c_str(), NULL, 16);
        unsigned int value = map(number, 0, 255, 0, 4095);
        
        // SET CHANNEL
        if (single_channel < 0) {
          for (int cpin = 0; cpin < RGB_LED_COUNT; cpin++) {
            Tlc.set( *(channel + cpin), value);
            Tlc.update();
          
            Serial.print(F("Setting channel "));
            Serial.print( *(channel + cpin));
            Serial.print(F(" to "));
            Serial.println(value);
          }
        } else {
          Tlc.set(single_channel, value);
          Tlc.update();
          
          Serial.print(F("Setting channel "));
          Serial.print(single_channel);
          Serial.print(F(" to "));
          Serial.println(value);
        }
      }
      
      
      
      else if (Data.charAt(0) == 'P') {
        String value = Data.substring(1, Data.length() - 1);
        bpm = (int) strtol(value.c_str(), NULL, 16);
        Serial.print(F("GOT BPM: "));
        Serial.println(bpm);
      }
      
      
      else if (Data.charAt(0) == 'E') {
        String value = Data.substring(1, Data.length() - 1);
        pos = value.toInt();
        Serial.print(F("ACCELERATOR TO "));
        Serial.println(pos);
      }
      
      else if (Data == "COST;") {
        pos = ESC_MIDPOINT;
        Serial.println(F("COASTING..."));
      }
      
      else {
        Serial.print(F("NOT A COMMAND! LENGTH: "));
        Serial.println(Data.length());
      }
      
      Data = "";
    }
  }
  
  delay(5); // FILL THE BUFFERS
}

float batteryVoltage() { 
  float voltageSum = 0;
  for (int i = 0; i < 10; i++) {
    int batteryVoltage = analogRead(BATTERY_PIN);    
    voltageSum += batteryVoltage * (3.3 / 1023.0);
  }
  return voltageSum/10;
}

void beep(unsigned int pitch, unsigned int duration) {
  Tlc.set(BUZZER_TLC_CHANNEL, pitch);
  Tlc.update();
  delay(duration);
  Tlc.set(BUZZER_TLC_CHANNEL, 0);
  Tlc.update();
}

