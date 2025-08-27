#ifndef DHT_11
#define DHT_11

#include <Arduino.h>

/*
The number of transmitted bits from DHT11 sensor. This value is calculated as follows:
  8 bits for humidity integer part + 
  8 bits for humidity decimal part + 
  8 bits for temperature integer part +
  8 bits for temperature decimal part +
  8 bits for checksum, equals to the 8 bits of the sum of the previous four measurations 
*/
#define TRANSMITTED_BITS 40

/*
The state related to the correct execution of the measurement.
*/
#define DHT11READER_OK 0x00

/*
The state related to the termination of the measurement due to timeout during acknowledgement or communication.
*/
#define DHT11READER_TIMEOUT 0x01

/*
The state related to the error of the measurement after the calculation of the checksum.
*/
#define DHT11READER_CHECKSUM_FAIL 0x02

/*
The value of the duration for the start low-pulse signal that the
MCU made to DHT11 to start the communication. This is a delay value that
is imposed to MCU in order to execute the next step.
*/
#define STARTSIGNAL_DHTDETECTION_DELAY 20

/*
The value of the duration for the start high-pulse signal that the
MCU made to DHT11 to start the communication. This is a delay value that
is imposed to MCU in order to execute the next step.
*/
#define STARTSIGNAL_WAITINGDHT_DELAY 50

/*
---------------------------------------------------------------------------------------------------------------------------
The following values are the number of read cycles to be executed to wait the responses from DHT11 sensor.
They are calculated as an approximation of an hypotetical high-speed IO's Arduino-based MCU (1us for complete 
the digitalRead operation). So, if a digitalRead operation is made, a reading cycle means that is passed 1us. If the
DHT11 send a pulse signal that has a certain duration (for example 50us), the MCU must execute 50 reading cycles.
Obviously. if the IO operation needs more time to be executed, the cycles will be the sames but it doesn't matter,
as this library is not made for critical missions.
*/
/*
The value of the duration for either the low-pulse and the high-pulse signals that the DHT11 send to MCU in order to
execute a sort of "acknowledgement" that the MCU will interpretate as "the DHT11 is read to send data".
*/
#define ACKNOWLEDGEDHT_CYCLE_DURATION 100


/*
The value of the duration for for the low-pulse signal that the DHT11 send to MCU before the single bit transmission.  
*/
#define STARTTRANSMISSION_DHT_DURATION 50

/*
The value of the duration for for the high-pulse signal that the DHT11 send to MCU for transmit a 0-value bit.  
*/
#define TRANSMISSION_DHTBIT0_DURATION 30

/*
The value of the duration for for the high-pulse signal that the DHT11 send to MCU for transmit a 1-value bit.  
*/
#define TRANSMISSION_DHTBIT1_DURATION 70

/*
The value of the difference between the duration of the transmission of 0-bit and the transmission of 1-bit.
This value will be used for calculating that the DHT11 sensor has sent a 0 bit instead of 1 bit. 
*/
#define TRANSMISSION_DHTSENTBITDIFFERENCE_DURATION TRANSMISSION_DHTBIT1_DURATION - TRANSMISSION_DHTBIT0_DURATION

/*
This class permits to read the values of termperature and humidity from the DHT11 sensor. 
*/
class DHT11Reader {

  public:

    /*
    This method permits to read the value of the temperature and humidity from the sensor connected to MCU 
    in the passed pin. The calculated values of temperature and humidity can be retrieved simply calling them in the instance.
    The returned value is the state of the communication and measurement. 
    */
    byte read(int pin);

    /*
    The value of the temperature (in °C) calculated from the read() operation.
    If you read this value before the calculation can lead to wrong values. 
    */
    float temperature;

    /*
    The value of the humidity (in %RH) calculated from the read() operation.
    If you read this value before the calculation can lead to wrong values. 
    */
    float humidity;
};

#endif
