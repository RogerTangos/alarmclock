#include <FastLED.h>
#include <Time.h>
#include <TimeAlarms.h>

// #define WAKEUP_TIME 3*60*1000 // 30 minutes

#define WAKEUP_TIME 1800000 // 30 minutes
#define WAKEUP_TIME 18000 // 30 minutes

#define UPDATE_PERIOD 100 // .01 second , use even numbers

// color at sunrise
#define START_R 182;
#define START_G 126;
#define START_B 91;
#define START_W 0;
#define START_BRIGHT 0.0;

// color at noon
#define END_R 255;
#define END_G 254;
#define END_B 230;
#define END_W 0;
#define END_BRIGHT 1.0;


#define START_SATURATION 255
#define START_HUE 0
#define START_VALUE 0

#define END_SATURATION 20
#define END_HUE 64
#define END_VALUE 255


#define REDPIN 3
#define GREENPIN 10
#define BLUEPIN 6

CHSV color;
int start_time = millis();
bool alarmOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  write();

  color.saturation = START_SATURATION;
  color.hue = START_HUE;
  color.value = START_VALUE;


  setTime(8,29,55,1,1,11); // set time to Saturday 8:29:55am Jan 1 2011
  // create the alarms 
  Alarm.alarmRepeat(8,30,00, startAlarm);  // 8:30am every day
  // turn off alarm after 2 hr
  Alarm.alarmRepeat(8,30,30, stopAlarm);  // 8:30am every day
  Serial.print("setup complete");
}

void startAlarm(){
  alarmOn = true;
  Serial.print("\n--ALARM STARTED--\n");
}

void stopAlarm(){
  Serial.print("\n--STOP ALARM--\n");
  alarmOn = false;



  color.saturation = START_SATURATION;
  color.hue = START_HUE;
  color.value = START_VALUE;
  write();
  }

void write() {


  analogWrite(REDPIN, value);






  CRGB rgb;
  hsv2rgb_rainbow(color, rgb);

  analogWrite(REDPIN,   rgb.r );
  analogWrite(GREENPIN, rgb.g );
  analogWrite(BLUEPIN,  rgb.b );
}

void runAlarm() {
  Serial.print("\nrunalarm\n");
  long elapsed_time = millis() - start_time;
  float percent_passed = (float)elapsed_time / (float)WAKEUP_TIME;
  
  // if percent passed is negative or greater than one,
  // just keep the brightness up
  if (abs(percent_passed) > 1.0 ){
    percent_passed = 1.0;
  }

  color.hue = (END_HUE - START_HUE) * percent_passed;
  color.saturation = (START_SATURATION - END_SATURATION) * (1-percent_passed);
  color.value = (END_VALUE - START_VALUE) * percent_passed;

  // Saturation mins out at specified value
  if (color.saturation < END_SATURATION) {
    color.saturation = END_SATURATION;
  }
}

void loop() {

  if(alarmOn){
    runAlarm();  
    // Serial.print("\n--ALARM RUNNING---\n");
  } else {
    // Serial.print("\n--ALARM NOT RUNNING---\n");
  }
  
  // put your main code here, to run repeatedly:
  // Serial.print("NO FORMAT\n");

  
  // Serial.print("hue: ");
  // Serial.print(color.hue);
  // Serial.print("|");
  // Serial.print(" saturation: ");
  // Serial.print(color.saturation);
  // Serial.print("|");
  // Serial.print(" value: ");
  // Serial.print(color.value);
  // Serial.print("|");
  // Serial.print(" pct passed: ");
  // Serial.print("=");
  // // Serial.print(elapsed_time);
  // Serial.print("/");
  // Serial.print(WAKEUP_TIME);
  // Serial.print("\n---\n");

  write();
  Alarm.delay(UPDATE_PERIOD);

}
