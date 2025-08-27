#include <U8x8lib.h>
#include <Wire.h>

#include "DHT11Reader.h"
#include "fonts.h"


/* 
==================================================
==              CONSTANTS - PINS                ==
==================================================
*/

/*
The pin number on which the DHT temperature-and-humidity sensor
is connected as input to the microcontroller. 
*/
#define PIN_DHT_SENSOR 9

/*
The pin number on which the 'navigation' button is connected as input 
to the microcontroller.
*/
#define PIN_BUTTON_NAVIGATION 2

/*
The pin number on which the 'action' button is connected as input 
to the microcontroller.
*/
#define PIN_BUTTON_ACTION 3


/* 
==================================================
==           CONSTANTS - CLOCK COUNTER          ==
==================================================
*/

/*
The size in microseconds of a single tick, on which the
time is calculated.
Value: 10.000us or 10ms.
*/
#define TICK_IN_MICROSECONDS 10000UL

/*
The size in microseconds of a single second, needed for 
avoid writing always this large number.
Value: 1.000.000us (one million microseconds).
*/
#define MICROSECONDS_PER_SECOND 1000000UL

/*
The maximum amount of drift that can be defined to being 
automatically added on each second counted by the clock.
*/
#define MAX_DRIFT_PER_SECONDS 10


/* 
==================================================
==       CONSTANTS - FINITE STATE MACHINE       ==
==================================================
*/

/*
The state on Finite State Machine that define the
possibility to read the time in HH:mm format. 
*/
#define STATE_READ_TIME 0x00


/*
The state on Finite State Machine that define the
possibility to read the temperature and humidity.
*/
#define STATE_READ_TEMPERATURE 0x01

/*
The state on Finite State Machine that define the
possibility to read the time in HH:mm:ss format. 
*/
#define STATE_READ_TIME_WITH_SECONDS 0x02

/*
The state on Finite State Machine that define the
possibility to read the value of the drift compensation
added on each hour. 
*/
#define STATE_READ_DRIFT_COMPENSATION 0x03

/*
The state on Finite State Machine that define the
possibility to set the clock hours.
*/
#define STATE_CHANGE_HOURS 0x04

/*
The state on Finite State Machine that define the
possibility to set the clock minutes.
*/
#define STATE_CHANGE_MINUTES 0x05

/*
The state on Finite State Machine that define the
possibility to reset the seconds in the clock.
*/
#define STATE_RESET_SECONDS 0x06

/*
The state on Finite State Machine that define the
possibility to set the drift compensation added
on each hour.
*/
#define STATE_SET_DRIFT_COMPENSATION 0x07


/* 
==================================================
==          CONSTANTS - MISCELLANEOUS           ==
==================================================
*/

/* 
The flag that define if the debug mode is enabled or not.
If set to 0, the debug mode is disabled, otherwise it is enabled.
*/
#define DEBUG_MODE 1

/*
The value that define the value of the delay in milliseconds to wait
in order to avoid bouncing of pushbuttons.
*/
#define DEBOUNCING_DELAY 250


/* 
==================================================
==               VARIABLES - CLOCK              ==
==================================================
*/

/* 
Time extracted in current loop cycle in microseconds.
*/
unsigned long currentTime;

/*
Time extracted in previous loop cycle in microseconds.
*/
unsigned long previousTime;

/* 
Time difference in microseconds between previous 
misuration and current misuration.
*/
unsigned long elapsedTime;

/*
The tenth part of the second in current time.
*/
byte tenthsOfSecond;

/*
The hundredth part of the second in current time.
*/
byte hundredthsOfSecond;

/*
The left-most digit that define the hour unit value.
*/
byte hourLeftDigit = 0;

/*
The right-most digit that define the hour unit value.
*/
byte hourRightDigit = 0;

/*
The left-most digit that define the minute unit value.
*/
byte minuteLeftDigit = 0;

/*
The right-most digit that define the minute unit value.
*/
byte minuteRightDigit = 0;

/*
The left-most digit that define the second unit value.
*/
byte secondLeftDigit = 0;

/*
The right-most digit that define the second unit value.
*/
byte secondRightDigit = 0;

/*
The first digit of the minute unit value, previously 
calculated.
*/
byte previousMinuteRightDigit = -1;

/*
The first digit of the second unit value, previously 
calculated.
*/
byte previousSecondRightDigit = -1;


/* 
==================================================
==            VARIABLES - TEMPERATURE           ==
==================================================
*/

/*
The value read from DHT11 evaluation that refers to
the room athmospheric temperature in celsius.
*/ 
int temperature = 0;

/*
The value read from DHT11 evaluation that refers to
the room athmospheric humidity percentage.
*/
byte humidity = 0;


/* 
==================================================
==                VARIABLES - FSM               ==
==================================================
*/

/*
The current state of the Finite State Machine.
*/
byte currentState = STATE_READ_TIME;

/*
The flag that define if the current state is a 
navigation state and the time calculation can 
proceed as usual.
*/
bool inNavigationState = true;

/*
The flag that define if the seconds could be
resetted in the contestual menu.
*/
bool resetSeconds = false;

/*
The flag that define if something about the state
in FSM is changes, either the state itself or an
internal computation.
*/
bool isStatusChanged = false;


/* 
==================================================
==           VARIABLES - DRIFT SETTING          ==
==================================================
*/

/*
The value on microseconds that define the offset to 
be removed from the calculation after the reset of 
the internal counters.
*/
unsigned long microsOffset = 0;

/*
The value that define the seconds to be added on each
hour in order to reduce the drift with real-time hour.
This is required because the crystal on microcontroller
is not reliable at all and is slower for some seconds
on each hour.
*/
float driftSecondsPerHours = 5.5;

/*
The value that define the microseconds to be added
on each tick (about 10ms) in order to reduce the drift 
with real-time hour. This value is calculated from the
value set in 'driftSecondsPerHours' variable.
*/
int driftMicrosPerTick;


/* 
==================================================
==            VARIABLES - PERIPHERALS           ==
==================================================
*/

/*
The instance of the driver that permits to handle the
SS1106 I2C OLED display using 8x8 font (via u8x8 lib).
*/
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

/*
The instance of the driver that permits to read values
from DHT11 sensor and extract temperature and humidity
calculated misurations.
*/
DHT11Reader dhtSensor;



/* 
==================================================
==               FUNCTIONS - MAIN               ==
==================================================
*/

/*
The method permits to handle the setup of the 
microcontroller.
*/
void setup() {

  // Setting the pin mode for the input elements
  pinMode(PIN_DHT_SENSOR, INPUT);
  pinMode(PIN_BUTTON_NAVIGATION, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ACTION, INPUT_PULLUP);

  // Initializing the drift in microseconds from
  // the defined value in seconds
  calculateDriftInMicroseconds();
  
  // Initializing U8x8 driver with used custom font
  u8x8.setFont(sevensegments); 
  u8x8.begin();

  // Activate serial communication and log starting 
  // configuration, ONLY if DEBUG_MODE is active
  #if DEBUG_MODE
    Serial.begin(115200);
    printConfigInSerialCom();
  #endif

  // Initializing the time of the previous misuration
  // with the end time of setup process 
  previousTime = now();
}


/*
The method permits to handle the main loop of the 
microcontroller.
*/
void loop() {

  // Choosing the state logic evaluation regarding the 
  // current state
  switch (currentState) {

    // State 'READ_TIME': reading time in HH:mm format.
    case STATE_READ_TIME:
      executeReadTime();
      break;
    
    // State 'READ_TEMPERATURE': reading DHT11 misuration.
    case STATE_READ_TEMPERATURE:
      executeReadTemperature();
      break;
    
    // State 'READ_TIME_WITH_SECONDS': reading time in 
    // HH:mm:ss format.
    case STATE_READ_TIME_WITH_SECONDS:
      executeReadTimeWithSeconds();
      break;

    // State 'READ_DRIFT_COMPENSATION': reading drift
    // compensation fixed second amount
    case STATE_READ_DRIFT_COMPENSATION:
      executeReadDriftCompensation();
      break;
    
    // State 'CHANGE_HOURS': setting new hours for clock
    case STATE_CHANGE_HOURS:
      executeChangeHours();
      break;
    
    // State 'CHANGE_MINUTES': setting new minutes for clock
    case STATE_CHANGE_MINUTES:
      executeChangeMinutes();
      break;
    
    // State 'RESET_SECONDS': resetting seconds for clock
    case STATE_RESET_SECONDS:
      executeResetSeconds();
      break;
    
    // State 'SET_DRIFT_COMPENSATION': setting drift
    // compensation fixed second amount
    case STATE_SET_DRIFT_COMPENSATION:
      executeSetDriftCompensation();
      break;
  }

  // If the current status is "in navigation" state, the time 
  // tick must be calculated, either if showed by display or not
  if (inNavigationState) {

    // Extracting current time and calculate the elapsed time from 
    // previous misuration. In this calculation, it is requried to 
    // add the drift compensation in microseconds 
    currentTime = now();
    elapsedTime = (currentTime - previousTime) + driftMicrosPerTick;

    // If the elapsed time is greater than the defined tick size in
    // microseconds, calculate the time and update the elapsed time
    if (elapsedTime >= TICK_IN_MICROSECONDS) {
      calculateTime();
      elapsedTime -= TICK_IN_MICROSECONDS;
    }
  }

}



/* 
==================================================
==               FUNCTIONS - FSM                ==
==================================================
*/

/*
The method permits to show the clock time in HH:mm format.
The value is updated each minute in the display.
*/
void executeReadTime() {

  // Show the new counted hour IF AND ONLY IF the minute is changed
  // changed or if FSM has changed its state in 'READ_TIME'
  if (isStatusChanged || previousMinuteRightDigit != minuteRightDigit) {

    // Updating counter of previous analyzed minute  
    previousMinuteRightDigit = minuteRightDigit;

    // Setting string buffer, interpolating time digits
    char timeContentBuffer[6];
    snprintf(timeContentBuffer, sizeof(timeContentBuffer), "%d%d:%d%d", hourLeftDigit, hourRightDigit, minuteLeftDigit, minuteRightDigit);

    // Finally, print the content using an enlarged font 
    printInLargeFont(&u8x8, 3, 2, timeContentBuffer);

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'READ_TEMPERATURE'
    changeStatus(STATE_READ_TEMPERATURE, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_READ_TIME]; New state: [STATE_READ_TEMPERATURE]; Navigation state? [Y].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  } 

  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Switching to state 'CHANGE_HOURS'
    changeStatus(STATE_CHANGE_HOURS, false);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [ACTION]; Old state: [STATE_READ_TIME]; New state: [STATE_CHANGE_HOURS]; Navigation state? [N].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  }
}


/*
The method permits to show the misurations from DHT11 sensor
about temperature (in Celsius) and humidity (in percentage).
The value is updated each minute in the display.
*/
void executeReadTemperature() {

  // Show the new temperature and humidity IF AND ONLY IF the minute is 
  // changed or if FSM has changed its state in 'READ_TEMPERATURE'
  if (isStatusChanged || previousMinuteRightDigit != minuteRightDigit) {

    // Updating counter of previous analyzed minute  
    previousMinuteRightDigit = minuteRightDigit;

    // Set the flag to true in order to permit the print if print on display at each tick
    isStatusChanged = true;

    // Calculating values from DHT11 sensor
    dhtSensor.read(PIN_DHT_SENSOR);
    temperature = dhtSensor.temperature;
    humidity = dhtSensor.humidity;

    // Setting string buffer, interpolating time digits
    char temperatureContentBuffer[12];
    snprintf(temperatureContentBuffer, sizeof(temperatureContentBuffer), "%dC  %d%%", temperature, humidity);

    // Finally, print the content using an enlarged font 
    printInLargeFont(&u8x8, 0, 2, temperatureContentBuffer);

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'STATE_READ_TIME_WITH_SECONDS'
    changeStatus(STATE_READ_TIME_WITH_SECONDS, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_READ_TEMPERATURE]; New state: [STATE_READ_TIME_WITH_SECONDS]; Navigation state? [Y].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  } 
}


/*
The method permits to show the clock time in HH:mm:ss format.
The value is updated each minute in the display.
*/
void executeReadTimeWithSeconds() {

  // Show the new counted hour IF AND ONLY IF the minute is changed
  // changed or if FSM has changed its state in 'READ_TEMPERATURE'
  if (isStatusChanged || previousSecondRightDigit != secondRightDigit) {

    // Updating counter of previous analyzed minute  
    previousSecondRightDigit = secondRightDigit;

    // Setting string buffer, interpolating time digits
    char timeContentBuffer[9];
    snprintf(timeContentBuffer, sizeof(timeContentBuffer), "%d%d:%d%d:%d%d", hourLeftDigit, hourRightDigit, minuteLeftDigit, minuteRightDigit, secondLeftDigit, secondRightDigit);

    // Finally, print the content using an enlarged font 
    printInLargeFont(&u8x8, 0, 2, timeContentBuffer);

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'STATE_READ_DRIFT_COMPENSATION'
    changeStatus(STATE_READ_DRIFT_COMPENSATION, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_READ_TIME_WITH_SECONDS]; New state: [STATE_READ_DRIFT_COMPENSATION]; Navigation state? [Y].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  } 
}


/*
The method permits to show the value of the drift compensation in 
seconds per hour. The value is displayed only one time at state change
because of its static nature.
*/
void executeReadDriftCompensation() {

  // Refresh the drift compensation value only one time, at the state 
  if (isStatusChanged) {
    
    // Build the delay content in its buffer
    char driftContentBuffer[7];
    dtostrf(driftSecondsPerHours, 2, 1, driftContentBuffer);
    strcat(driftContentBuffer, "s");

    // Finally, print the content 
    printInMediumFont(&u8x8, 0, 0, "HH DELAY");
    printInLargeFont(&u8x8, 0, 3, driftContentBuffer);

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'STATE_READ_TIME'
    changeStatus(STATE_READ_TIME, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_READ_DRIFT_COMPENSATION]; New state: [STATE_READ_TIME]; Navigation state? [Y].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  } 

  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Switching to state 'SET_DRIFT_COMPENSATION'
    changeStatus(STATE_SET_DRIFT_COMPENSATION, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [ACTION]; Old state: [STATE_READ_DRIFT_COMPENSATION]; New state: [STATE_SET_DRIFT_COMPENSATION]; Navigation state? [N].\n"));
    #endif

    // Clean the display and wait some time in order to avoid button bouncing
    u8x8.clear();
    delay(DEBOUNCING_DELAY);
  }
}


/*
The method permits to show the clock time in HH:mm format.
The value is updated each minute in the display.
*/
void executeChangeHours() {

  // Refresh the content value only when the state require refresh
  if (isStatusChanged) {

    // Build the time content in its buffer
    char timeContentBuffer[8];
    snprintf(timeContentBuffer, sizeof(timeContentBuffer), "%d%d{: %d%d", hourLeftDigit, hourRightDigit, minuteLeftDigit, minuteRightDigit);
    
    // Finally, print the content using an enlarged font 
    printInLargeFont(&u8x8, 1, 2, timeContentBuffer);
    
    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state and (indirectly) store updated value if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'STATE_CHANGE_MINUTES'
    changeStatus(STATE_CHANGE_MINUTES, false);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_CHANGE_HOURS]; New state: [STATE_CHANGE_MINUTES]; Navigation state? [N].\n"));
    #endif

    // Cleaning the display
    u8x8.clear();
  } 

  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Incrementing the counter by a single unit
    int counter = ((hourLeftDigit * 10) + hourRightDigit) + 1;

    // If counter reach the maximum number of hours, reset it
    if (counter == 24) {
      counter = 0;
    }

    // Finally, refresh the left and right digit of the hour 
    hourLeftDigit = int(counter / 10);
    hourRightDigit = int(counter % 10);

    // Change the flag of the change-status in order to force display refresh
    isStatusChanged = true;
  }  

  // Waiting some time in order to either avoid button bouncing and 
  // reduce refresh on display
  delay(DEBOUNCING_DELAY);
}


/*
The method permits to change the minutes of the clock using
the input passed by user. The value is updated only the if 
value is changed by user input.
*/
void executeChangeMinutes() {

  // Refresh the content value only when the state require refresh
  if (isStatusChanged) {

    // Build the time content in its buffer
    char timeContentBuffer[8];
    snprintf(timeContentBuffer, sizeof(timeContentBuffer), "%d%d :}%d%d", hourLeftDigit, hourRightDigit, minuteLeftDigit, minuteRightDigit);

    // Finally, print the content using an enlarged font 
    printInLargeFont(&u8x8, 1, 2, timeContentBuffer);

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

  // Changing state and (indirectly) store updated value if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Switching to state 'STATE_RESET_SECONDS'
    changeStatus(STATE_RESET_SECONDS, false);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_CHANGE_MINUTES]; New state: [STATE_RESET_SECONDS]; Navigation state? [N].\n"));
    #endif

    // Cleaning the display
    u8x8.clear();

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  } 

  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Incrementing the counter by a single unit
    int counter = ((minuteLeftDigit * 10) + minuteRightDigit) + 1;

    // If counter reach the maximum number of minutes, reset it
    if (counter == 60) {
      counter = 0;
    }

    // Finally, refresh the left and right digit of the minute 
    minuteLeftDigit = int(counter / 10);
    minuteRightDigit = int(counter % 10);

    // Change the flag of the change-status in order to force display refresh
    isStatusChanged = true;

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  }  
}


/*
The method permits to change the seconds of the clock using
the input passed by user. The value is updated only if the 
value is changed by user input.
*/
void executeResetSeconds() {

  // Refresh the content value only when the state require refresh
  if (isStatusChanged) {

    // Finally, print the content 
    printInMediumFont(&u8x8, 0, 0, "SEC AS 0");
    if (resetSeconds) {
      printInLargeFont(&u8x8, 0, 3, "  N }Y");
    } else {
      printInLargeFont(&u8x8, 0, 3, " }N  Y");
    }

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;
  }

   // Changing state and (indirectly) store updated value if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Before changing state, reset the seconds' value if the flag is active 
    if (resetSeconds) {
      resetInternalCounter();
      resetSeconds = false;
    }

    // Switching to state 'STATE_READ_TIME'
    changeStatus(STATE_READ_TIME, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_RESET_SECONDS]; New state: [STATE_READ_TIME]; Navigation state? [Y].\n"));
    #endif

    // Cleaning the display
    u8x8.clear();

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  } 

  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Changing status of the reset seconds' flag 
    resetSeconds = !resetSeconds;

    // Change the flag of the change-status in order to force display refresh
    isStatusChanged = true;

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  }
}


/*
The method permits to change the drift compensation of the 
clock using the input passed by user. The value is updated 
only if the value is changed by user input.
*/
void executeSetDriftCompensation() {

  // Refresh the content value only when the state require refresh
  if (isStatusChanged) {

    // Update the flag in order to set that the status is not changed 
    isStatusChanged = false;

    // Build the delay content in its buffer
    char driftContentBuffer[7];
    dtostrf(driftSecondsPerHours, 2, 1, driftContentBuffer);
    strcat(driftContentBuffer, "s");

    // Finally, print the content 
    printInMediumFont(&u8x8, 0, 0, "SET:");
    printInLargeFont(&u8x8, 0, 3, driftContentBuffer);
  }

  // Changing state and (indirectly) store updated value if navigation button is pushed
  if (digitalRead(PIN_BUTTON_NAVIGATION) == 0) {

    // Before changing state, update the drift from counted value to microseconds
    calculateDriftInMicroseconds();
    #if DEBUG_MODE
      printConfigInSerialCom();
    #endif

    // Switching to state 'STATE_READ_TIME'
    changeStatus(STATE_READ_TIME, true);

    // Debug Mode: logging state switch 
    #if DEBUG_MODE
      Serial.print(F("Button: [NAVIGATION]; Old state: [STATE_READ_DRIFT_COMPENSATION]; New state: [STATE_READ_TIME]; Navigation state? [Y].\n"));
    #endif

    // Cleaning the display
    u8x8.clear();

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  } 
  
  // Changing state if action button is pushed
  else if (digitalRead(PIN_BUTTON_ACTION) == 0) {

    // Incrementing drift-per-second counter by 0.5s
    float counter = driftSecondsPerHours + 0.5;
    if (counter == MAX_DRIFT_PER_SECONDS) {
      counter = 0;
    }
    driftSecondsPerHours = counter;

    // Change the flag of the change-status in order to force display refresh
    isStatusChanged = true;

    // Waiting some time in order to avoid button bouncing
    delay(DEBOUNCING_DELAY);
  }
}



/* 
==================================================
==              FUNCTIONS - CLOCK               ==
==================================================
*/

/*
The method permits to calculate the current time, 
updating the single digits starting from hundredth
of second to hour. The method provide also the
update of the previous time variable, including
the calculation of drift compensation.
*/
void calculateTime() {

  // Updating the previous time value, adding the 
  // standard tick size in microseconds but also 
  // removing the drift reducing by some microseconds. 
  // This permits to make the computation more accurate
  // in long term.
  previousTime = (previousTime + TICK_IN_MICROSECONDS) - driftMicrosPerTick;

  // HH:mm:ss.s[ss] <=============  hundredths of seconds - starting evaluation
  hundredthsOfSecond++;
  if (hundredthsOfSecond == 10) {

    // HH:mm:ss.[s]ss <===========  tenths of seconds - starting evaluation
    hundredthsOfSecond = 0;
    tenthsOfSecond++;
    if (tenthsOfSecond == 10) {

      // HH:mm:s[s].sss <=========  right-most digit of second value - starting evaluation
      tenthsOfSecond = 0;
      secondRightDigit++;
      if (secondRightDigit == 10) {

        // HH:mm:[s]s.sss <=======  left-most digit of second value - starting evaluation
        secondRightDigit = 0;
        secondLeftDigit++;
        if (secondLeftDigit == 6) {

          // HH:m[m]:ss.sss <=====  right-most digit of minute value - starting evaluation
          secondLeftDigit = 0;
          minuteRightDigit++;
          if (minuteRightDigit == 10) {

            // HH:[m]m:ss.sss <===  left-most digit of minute value - starting evaluation
            minuteRightDigit = 0;
            minuteLeftDigit++;
            if (minuteLeftDigit == 6) {

              // H[H]:mm:ss.sss <=  right-most digit of hour value - starting evaluation
              minuteLeftDigit = 0;
              hourRightDigit++;
              if (hourRightDigit == 10) {

                hourRightDigit = 0;
                hourLeftDigit++;
              } 
              // H[H]:mm:ss.sss <=  right-most digit of hour value - closing evaluation

              // [H]H:mm:ss.sss <=  left-most digit of hour value - starting evaluation
              else if (hourLeftDigit == 2 && hourRightDigit == 4) {

                hourLeftDigit = 0;
                hourRightDigit = 0;
              } 
              // [H]H:mm:ss.sss <=  left-most digit of hour value - closing evaluation

            } 
            // HH:[m]m:ss.sss <===  left-most digit of minute value - closing evaluation

          } 
          // HH:m[m]:ss.sss <=====  right-most digit of minute value - closing evaluation

        } 
        // HH:mm:[s]s.sss <=======  left-most digit of second value - closing evaluation

      } 
      // HH:mm:s[s].sss <=========  right-most digit of second value - closing evaluation

    } 
    // HH:mm:ss.[s]ss <===========  tenths of seconds - closing evaluation

  } 
  // HH:mm:ss.s[ss] <=============  hundredths of seconds - closing evaluation
}


/*
The method permits to reset all the internal variable
alongside the 'reset second' event. 
*/
void resetInternalCounter() {

  // reset offset with current microseconds
  microsOffset = micros();

  // reset previous calculation to zero
  previousTime = 0;

  // reset second components to zero
  hundredthsOfSecond = 0;
  tenthsOfSecond = 0;
  secondLeftDigit = 0;
  secondRightDigit = 0;

  // force refresh display by overflowing byte value on next value
  previousSecondRightDigit = 255;
}


/*
The method permits to return the 'now' timestamp in 
microseconds, using also the offset in microseconds
previously calculated.
*/
long now() {
  return micros() - microsOffset;
}


/*
The method permits to calculate the drift compensation from
in microseconds-per-tick from the value defined by user in 
seconds-per-hour.
*/
void calculateDriftInMicroseconds() {

  int driftPerHoursInMicros = float(driftSecondsPerHours / 3600) * MICROSECONDS_PER_SECOND;
  driftMicrosPerTick = driftPerHoursInMicros / (MICROSECONDS_PER_SECOND / TICK_IN_MICROSECONDS);
}



/* 
==================================================
==          FUNCTIONS - MISCELLANEOUS           ==
==================================================
*/

/*
The method permits to update the current status of the
microcontroller, changing all internal variables.
*/
void changeStatus(byte status, boolean inNavigation) {

  // Setting the current state with the one passed in method
  currentState = status;
  
  // Setting the flag that define if the state is in navigation mode
  // so the clock can continue the update process
  inNavigationState = inNavigation;

  // Statically set the flag on status change
  isStatusChanged = true;  
}


/*
The method permits to print the configuration of the
clock in serial communication.
*/
void printConfigInSerialCom() {
  Serial.println(F("=== CONFIGURATION: ==="));
  Serial.print(F("Drift added on each hour (s): "));
  Serial.println(driftSecondsPerHours);
  Serial.print(F("Drift added on each tick (us): "));
  Serial.println(driftMicrosPerTick);
  Serial.println(F("======================"));
}
