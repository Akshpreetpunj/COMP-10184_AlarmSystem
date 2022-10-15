// COMP-10184 – Mohawk College 
// Application Name - COMP-10184_AlarmSystem 
// 
// Project Stage - Stage 5 Completed
//
// Project Description - This program uses a motion sensor (PIR Sensor) to detect motion when someone pass by its view. 
// The PIR sensor will start the countdown of 10 seconds before activating the alarm. The user need to press the button 
// within 10 seconds to stop the alarm otherwise the alarm will activate. The user can re-enable the alarm by pressing 
// the button again. This program demonstrate a home alarm system.
// 
// @author  Akshpreet Singh Punj 
// @id   000820040 
// 
// I created this work and I have not shared it with anyone else. 
// 
 
#include <Arduino.h> 
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6 

// defining the value of LED ON State
#define LED_ON false

// defining the value of LED OFF State
#define LED_OFF true

// defining the value of Button pressed state
#define BUTTON_PRESSED 0

// defining the value of Button not pressed state
#define BUTTON_NOT_PRESSED 1

// defining the delay value for the Alarm activation (10000 ms = 10 seconds)
#define DELAY 10000

// defining the delay value for the button
#define BUTTON_DELAY 200

// defining the delay value for the LED Light blink during the Alarm countdown 
#define BLINK_DELAY 125

// define all possible alarm states. 
#define ALARM_DISABLED  0 
#define ALARM_ENABLE    1 
#define ALARM_COUNTDOWN 2 
#define ALARM_ACTIVE    3 

// global variable for storing the alarm state 
int  iAlarmState; 
// global variable for storing the button state
int iButton;
// global variable for storing the PIR sensor state
bool bPIR;
// global variable for the loop
int count = 0; 

// ************************************************************* 
void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  // Introduction messages
  Serial.println("\n\nCOMP-10184 - Alarm System");
  Serial.println("Name: Akshpreet Singh Punj");
  Serial.println("Student ID: 000820040\n");

  // PIR Sensor signal is false be default (False = No Motion Detected)
  bPIR = false;

  // Alarm is enabled by default
  iAlarmState = ALARM_ENABLE;

  // Button is in not pressed by default
  iButton = BUTTON_NOT_PRESSED;

  // LED is off by default
  digitalWrite(LED_BUILTIN, LED_OFF);
}

// ***********************************************************
// This function is used for collecting the input/reading from the PIR sensor and button. The input will be 
// stored in the global variables.
void collectInputs() {
  // Reading the value of PIR sensor
  bPIR = digitalRead(PIN_PIR); 
  
  // Reading the value of button
  iButton = digitalRead(PIN_BUTTON); 
}

// ***********************************************************
// This function is used for checking the alarm state. The function will perform different actions
// depending on the alarm state.
void checkAlarmState() {
  // Checking if the motion is detected (true = Motion Detected) and the alarm is enabled 
  if (bPIR == true && iAlarmState == ALARM_ENABLE) {
    // changing the alarm state to alarm countdown
    iAlarmState = ALARM_COUNTDOWN;
    Serial.println("Motion Detected! - Alarm will sound in 10s!");
  } else if (iAlarmState == ALARM_COUNTDOWN) { // Checking if the alarm state is alarm countdown
    // Starting the 10 seconds alarm countdown when motion is detected
    while (count < DELAY && iButton == BUTTON_NOT_PRESSED) {
      // Blinking the LED for 10 seconds (4 blinks per second)
      digitalWrite(LED_BUILTIN, LED_ON);
      delay(BLINK_DELAY);
      digitalWrite(LED_BUILTIN, LED_OFF);
      delay(BLINK_DELAY);
      // Reading the button value
      iButton = digitalRead(PIN_BUTTON);
      count += 250; // Total time taken per blink = 250 ms (125 ms LED_ON time and 125 ms LED_OFF time)
    }
    // Adding the button delay to avoid immediately flipping between enabled and disabled states
    delay(BUTTON_DELAY);

    // Checking if 10 seconds countdown is complete
    if (count == DELAY) {
      // changing the alarm state to alarm active
      iAlarmState = ALARM_ACTIVE;
      Serial.println("Alarm activated!");
    } else {
      // changing the alarm state to alarm disabled
      iAlarmState = ALARM_DISABLED;
      Serial.println("Button pressed: Alarm Disabled");
    }
  } else if (iAlarmState == ALARM_ACTIVE) { // Checking if the alarm state is alarm active
    // Activating the alarm if the button is not pressed within 10 seconds of countdown
    // turning on the LED to solid state (Alarm active)
    digitalWrite(LED_BUILTIN, LED_ON);
  } else if (iAlarmState == ALARM_DISABLED) { // Checking if the alarm state is alarm disabled
    // Disabling the Alarm if the button is pressed within 10 seconds of countdown
    // turning the LED OFF (Alarm Disable)
    digitalWrite(LED_BUILTIN, LED_OFF);

    // Reading the value of button
    iButton = digitalRead(PIN_BUTTON);
    // Adding the button delay to avoid immediately flipping between enabled and disabled states
    delay(BUTTON_DELAY);

    // if the button is pressed by the user, enabling the alarm again 
    if (iButton == BUTTON_PRESSED) {
      // changing the alarm state to alarm enable
      iAlarmState = ALARM_ENABLE;
      Serial.println("Button pressed: Alarm Enabled");
    }
  }
}
 
// ************************************************************* 
void loop() {  
  // get button and motion detector inputs 
  collectInputs(); 
 
  // should we arm/activate/enable/disable the alarm? 
  checkAlarmState(); 
}