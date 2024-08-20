/*Zener Diode Tester, ver.8, tested
Arduino Nano, LM317
Software by Kamil Zachej, SK
*/

#include <Arduino.h>

int value;
int currentvalue;
int maxvalue;                                                            //value selected from 10 conseqent meassurement reading
int start = LOW;                                                         //digital reading of button START
const int pushButton = 3;
byte current = A1;
bool on = true;
bool error = false;
bool buttonState = false;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 2000;                                       //time limit to enter service mode after switched on
const int v12Pin=4;                           
const int v24Pin=5;                           
const int v36Pin=6;                           
const int v48Pin=7;                          
const int dvmPin=8;                          
const int errPin=9;

void readCurrent() {                                                      //function read current 10x, define max value
  maxvalue = 0;
  for (int i = 0; i<10; i++){
    currentvalue = analogRead(current);
    delay(100);    
      if (maxvalue <= currentvalue)
      {
        maxvalue = currentvalue;
        }
        else{         
        maxvalue = maxvalue;
        }
        value = maxvalue;      
  }
}

void service(){                                                            //service mode
      digitalWrite(v12Pin, HIGH);
      Serial.println("Service Mode 12V");
         do{
            flash(1);                                                      //blinking LED function 1 time for the first step of srvice
            start = digitalRead(pushButton);
          }while (start != HIGH);        
            digitalWrite(v12Pin, LOW);
            delay(500);
            digitalWrite(v24Pin, HIGH);
            Serial.println("Service Mode 24V");
         do{
            flash(2);
            start = digitalRead(pushButton);
          }while (start != HIGH);        
            digitalWrite(v24Pin, LOW);
            delay(500);
            digitalWrite(v36Pin, HIGH);
            //flash(3);
            Serial.println("Service Mode 36V");
        do{
            flash(3);
            start = digitalRead(pushButton);
          }while (start != HIGH);        
            digitalWrite(v36Pin, LOW);
            delay(500);
            digitalWrite(v48Pin, HIGH);
            //flash(4);
            Serial.println("Service Mode 48V");       
        do{
            flash(4); 
            start = digitalRead(pushButton);
          }while (start != HIGH);        
            digitalWrite(v48Pin, LOW);
            Serial.println("End of Service Mode");
            on = false;
}
                 
void flash(int a){                                                          //function blink for service mode
  for (int i=0; i<a; i++){
        digitalWrite(errPin, HIGH);
        delay(100);
        digitalWrite(errPin, LOW);
        delay(100);
      } 
  delay(800); 
}

void connectDvm(){                                                          //function for connecting voltmeter for 10s.
                digitalWrite(dvmPin, HIGH);                                
                delay(10000);
                digitalWrite(dvmPin, LOW);
}

void setup() {
pinMode(v12Pin, OUTPUT);
pinMode(v24Pin, OUTPUT);
pinMode(v36Pin, OUTPUT);
pinMode(v48Pin, OUTPUT);
pinMode(dvmPin, OUTPUT);
pinMode(pushButton,INPUT);
pinMode(errPin, OUTPUT);
startMillis = millis();                                                   //start time
digitalWrite(errPin, LOW);
Serial.println("Tester ready - push button START");  
}

void loop() {
  if (on == true){                                                        //if service mode activated - enter to service mode 
       do  { 
           currentMillis = millis();
           start = digitalRead(pushButton);
              if (start == HIGH) {
                  buttonState = true;
                  }
       }
       while ((unsigned long)(currentMillis - startMillis < period));
              if (buttonState == true) {
                  service();                  
                  }      
  }   

  do{
     start = digitalRead(pushButton);
     }while (start != HIGH);
        digitalWrite(errPin, LOW);          
        digitalWrite(v12Pin, HIGH);                                       //meassurement voltage 12V and current 20mA
        delay(200);
        Serial.println("Nastavene 12V");       
            readCurrent();
            if (value > 60) {                                             //test if current through Zener diode is present
                connectDvm();                                            
                Serial.println("DVM zapojeny na 12V");
                digitalWrite(v12Pin, LOW);
                }
            else {                                                        //if current is not present
                digitalWrite(v12Pin, LOW);                                //disconnect 12V
                delay(500);                                               //delay between two relays activity
                digitalWrite(v24Pin, HIGH);                               //connect 24V and current 10mA      
                Serial.println("Nastavene 24V");
                              
                readCurrent();                              
                if (value > 25) {
                    connectDvm();
                    Serial.println("DVM zapojeny na 24V");
                    digitalWrite(v24Pin, LOW);
                   }
                else {                                        
                    digitalWrite(v24Pin, LOW);
                    delay(500);                      
                    digitalWrite(v36Pin, HIGH);                    
                    Serial.println("Nastavene 36V"); 
                       
                    readCurrent();                
                    if (value > 15) {                   
                        connectDvm();
                        Serial.println("DVM zapojeny na 36V");
                        digitalWrite(v36Pin, LOW);
                      }
                   else {                              
                        digitalWrite(v36Pin, LOW);
                        delay(500);                                                         
                        digitalWrite(v48Pin, HIGH);                       
                        Serial.println("Nastavene 48V");                            
                        readCurrent();                                                   
                        if (value > 8)  {                             
                            connectDvm();
                            Serial.println("DVM zapojeny na 48V");
                            digitalWrite(v48Pin, LOW);
                        }
                        else {                                             //error, if current was not detected under all voltages
                            digitalWrite(v48Pin, LOW);                                 
                            digitalWrite(errPin, HIGH);                
                            error = true;
                            Serial.println("ERROR");
                        }
                     }   
                 }
             }         
                 
}  
}

