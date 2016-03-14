#include <Time.h>
#include <TimeAlarms.h>

#define CURRENT_HOUR 8
#define CURRENT_MINUTE 30
#define CURRENT_DAY 1
#define CURRENT_MONTH 1
#define CURRENT_YEAR 2016

#define WAKEUP_HOUR 8
#define WAKEUP_MINUTE 30
#define WAKEUP_DURATION 30L*60*1000 // 30 minutes
// #define WAKEUP_DURATION 3000

#define UPDATE_PERIOD 1000 // 1 second

// color at sunrise
#define START_R 182
#define START_G 126
#define START_B 91

// color at noon
#define END_R 255
#define END_G 254
#define END_B 230

#define R_SLOPE ((double)END_R - START_R) / WAKEUP_DURATION
#define G_SLOPE ((double)END_G - START_G) / WAKEUP_DURATION
#define B_SLOPE ((double)END_B - START_B) / WAKEUP_DURATION

#define R_INTERCEPT -((double)START_R / R_SLOPE)
#define G_INTERCEPT -((double)START_G / G_SLOPE)
#define B_INTERCEPT -((double)START_B / B_SLOPE)

#define REDPIN 3
#define GREENPIN 10
#define BLUEPIN 6

long start_time = 0;
long first_intercept = 0;
bool alarmOn = false;

void setup() {
  Serial.begin(9600); 

  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  write(0, 0, 0);

  // compute the crack of dawn (using setTime as a lousy hack to construct a time_t)
  setTime(WAKEUP_HOUR, WAKEUP_MINUTE, 0, CURRENT_DAY, CURRENT_MONTH, CURRENT_YEAR);
  first_intercept = (int)min(R_INTERCEPT, min(G_INTERCEPT, B_INTERCEPT));
  time_t dawn = now() + 1000 * first_intercept;
  // set the clock to where it should be
  setTime(CURRENT_HOUR, CURRENT_MINUTE, 0, CURRENT_DAY, CURRENT_MONTH, CURRENT_YEAR);

  // create the alarms 
  // Alarm.alarmRepeat(hour(dawn), minute(dawn), 00, startAlarm);
  // Alarm.alarmRepeat(hour(dawn), minute(dawn), 00, startAlarm);
  Alarm.alarmRepeat(hour(8), minute(30), 10, startAlarm);
  Serial.print("setup complete");
}

void startAlarm(){
  start_time = millis();
  alarmOn = true;
  Serial.print("\n--ALARM STARTED--\n");
}

void stopAlarm(){
  Serial.print("\n--STOP ALARM--\n");
  alarmOn = false;
  write(0, 0, 0);
}

void write(char r, char g, char b) {
  analogWrite(REDPIN,   r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN,  b);
}

void runAlarm() {
  Serial.print("\nrunalarm\n");
  long elapsed_time = millis() - start_time;
  if (elapsed_time < WAKEUP_DURATION - first_intercept) {
    write( (char)max((R_SLOPE * elapsed_time + start_time - R_SLOPE), 0),
           (char)max((G_SLOPE * elapsed_time + start_time - G_SLOPE), 0),
           (char)max((B_SLOPE * elapsed_time + start_time - B_SLOPE), 0));
  } else {
    stopAlarm();
  }
}

void loop() {
  if(alarmOn){
    runAlarm();  
  }
  Alarm.delay(UPDATE_PERIOD);
}
