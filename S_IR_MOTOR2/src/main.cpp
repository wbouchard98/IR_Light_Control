#include <Arduino.h>
#include <IRremote.hpp>
#include <AccelStepper.h>
#include <ArduinoJson.h>
#define IR_RECEIVE_PIN 12

#define IN1_PIN 3
#define IN2_PIN 1
#define IN3_PIN 38
#define IN4_PIN 33

#define TYPE AccelStepper::HALF4WIRE

#define MAXSPEED 1000
#define MINSPEED 100

#define LED_BLUE 6
#define LED_GREEN 5
#define LED_RED 14

int currspeed = 200;
int prevspeed = 200;
int inc = 100;
int dir = +1;

unsigned long last = millis();

// Note the pin order 1,3,2,4!
auto stepper = AccelStepper(TYPE, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);

JsonDocument myDoc;
const char* IR_Name;
const char* IR_Color;
long IR_Value;

void setup() 
{
  Serial.begin(9600);
  const char* incomingString = "{\"Name\":\"DEFAULT_NAME\",\"Color\":\"DEFAULT_COLOR\",\"Value\":8}";
  DeserializationError error = deserializeJson(myDoc, incomingString);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  stepper.setMaxSpeed(MAXSPEED);
}

void loop() 
{

  if(millis()-last>350)
  {
    if (IrReceiver.decode()) 
    {

      //essayer de juste mettre un flag ici et faire le traitement des données après et réinitialiser le flag
      uint16_t command = IrReceiver.decodedIRData.command;
      if (command == 0x4A) 
      {         // VOL+
        //Serial.println("++");
        prevspeed = currspeed;
        currspeed = min(currspeed + inc, MAXSPEED);
        myDoc["Value"] = command; 
        myDoc["Color"] = "White";
        myDoc["Name"] = "++";  
      } 
      else if (command == 0x52) 
      {  // VOL-
        //Serial.println("--");
        myDoc["Value"] = command; 
        myDoc["Color"] = "White";
        myDoc["Name"] = "--";
        prevspeed = currspeed;
        currspeed = max(currspeed - inc, MINSPEED);
      } 
      else if (command == 0x47) 
      { // Music/DMX
        //dir = -dir;
        //delay(200);
        //Serial.println("DMX");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Blue";
        myDoc["Name"] = "DMX";
      } 
      else if (command == 0x45) 
      {  // On/Off
        //Serial.println("On/OFF");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Red";
        myDoc["Name"] = "On/off";
        currspeed = currspeed > 0 ? 0 : prevspeed;
        delay(200);
      }
      //Augmente volume
      else if (command == 0x40) 
      {
          // do something else
        //Serial.println("Stop/Start");  
        myDoc["Value"] = command; 
        myDoc["Color"] = "Green";
        myDoc["Name"] = "Stop/Start";
      }
      //Music Skyrim
      else if (command == 0x7) 
      {
          // do something else
        //Serial.println("MUSIC1"); 
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "Music1"; 
      }    
      //Music Naruto
      else if (command == 0x15) 
      {
          // do something else
        //Serial.println("MUSIC2"); 
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "Music2";
      }
      //Usic relax
      else if (command == 0x9) 
      {
          // do something else
        //Serial.println("MUSIC3");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "Music3";   
      }

      //Réduit volume
      else if (command == 0x19) 
      {
          // do something else
        //Serial.println("I AM SPEED");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "Flash";   
      }

      //LED Rouge
      else if (command == 0xC) 
      {
          // do something else
        //Serial.println("A1 Sauce");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A1";  
      }

      //LED Vert
      else if (command == 0x18) 
      {
          // do something else
        //Serial.println("A2");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A2";  
      }

      //LED Bleu
      else if (command == 0x5E) 
      {
          // do something else
        //Serial.println("A3");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A3";  
      }

      //LED Mauve
      else if (command == 0x8) 
      {
          // do something else
        //Serial.println("A4");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A4";  
      }

      //LED Rose
      else if (command == 0x1C) 
      {
          // do something else
        //Serial.println("A5");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A5";  
      }

      //LED Jaune
      else if (command == 0x5A) 
      {
          // do something else
        //Serial.println("A6");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A6"; 
      }

      //LED Orange
      else if (command == 0x42) 
      {
          // do something else
        //Serial.println("Why do you exist");
        myDoc["Value"] = command; 
        myDoc["Color"] = "Black";
        myDoc["Name"] = "A7";
      }
      IrReceiver.resume();
      IR_Name = myDoc["Name"];
      IR_Color = myDoc["Color"];
      IR_Value = myDoc["Value"];

      Serial.printf("Name: %s\n",IR_Name);
      Serial.printf("Color: %s\n", IR_Color);
      Serial.printf("Value: 0x%02X\n", IR_Value);
    }
    
    last=millis();
  }
  stepper.setSpeed(dir * currspeed);
  stepper.runSpeed();
}