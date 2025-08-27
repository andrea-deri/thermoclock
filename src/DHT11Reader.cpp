#include "HardwareSerial.h"
#include "DHT11Reader.h"

byte DHT11Reader::read(int pin) {
	
  byte result = DHT11READER_OK;

  uint8_t extractedData[5] = {0, 0, 0, 0, 0};

	/*
  Clearing the buffer
  */
	//extractedData[0] = extractedData[1] = extractedData[2] = extractedData[3] = extractedData[4] = 0;

	/*
  Setting the pin mode to output in order to send the start signals from MCU to DHT11
  */
	pinMode(pin, OUTPUT);
	
  /*
  MCU send a low signal for starting the communication with DHT11.
  It must send a 18ms low-pulse in order to let the DHT11 sensor to detect 
  the activation signal. The duration value will be set to 20ms for approximation.
  */
  digitalWrite(pin, LOW);
	delay(STARTSIGNAL_DHTDETECTION_DELAY);

  /*
  MCU send an high signal in order to tell to DHT11 that it is awaiting
  the sensor response. It must send a 20us to 40us high-pulse in order to permits
  the effective starts of the communication from the DHT11 sensor. The duration value
  will be set to 50us for approximation.
  */
	digitalWrite(pin, HIGH);
	delayMicroseconds(STARTSIGNAL_WAITINGDHT_DELAY);

  /*
  Set the pin mode to input in order to receive the data signal from DHT11 to MCU.
  */
	pinMode(pin, INPUT);

	/*
  The DHT11 sensor send a low signal in order to tell to MCU that is ready to communicate.
  It must send a 80us low-pulse in order to starts the communication. Several manufacturer
  can define the pulse duration with different values, so the value is defined as an approximation
  of an hypotetical high-speed IO's Arduino-based MCU (1us for complete the digitalRead operation)
  for a signal duration of at least of 100us. 
  */
	unsigned int leftCycles = ACKNOWLEDGEDHT_CYCLE_DURATION;
	while(result == DHT11READER_OK && digitalRead(pin) == LOW) {
		if (leftCycles-- == 0) {
      result = DHT11READER_TIMEOUT;
    }
  }

  /*
  The DHT11 sensor send an high signal in order to tell to MCU that is ready to communicate.
  It must send a 80us high-pulse in order to starts the communication. Several manufacturer
  can define the pulse duration with different values, so the value is defined as an approximation
  of an hypotetical high-speed IO's Arduino-based MCU (1us for complete the digitalRead operation)
  for a signal duration of at least of 100us. 
  */
	leftCycles = ACKNOWLEDGEDHT_CYCLE_DURATION;
	while(result == DHT11READER_OK && digitalRead(pin) == HIGH) {
		if (leftCycles-- == 0) {
      result = DHT11READER_TIMEOUT;
    }
  }

  uint8_t measurementIdx = 0;
  uint8_t bitIdx = 7;
  short i = 0;

  while(result != DHT11READER_OK || i < TRANSMITTED_BITS) {

    /*
    The DHT11 sensor send a low signal in order to tell to MCU that is sending a new bit of data.
    It must send a 50us low-pulse in order to syncronize the sending.
    */
		leftCycles = STARTTRANSMISSION_DHT_DURATION;
		while(result == DHT11READER_OK && digitalRead(pin) == LOW) {
			if (leftCycles-- == 0) {
        result = DHT11READER_TIMEOUT;
      }
    }

    /*
    Tracing the start of the transmission for the data bit.
    */
		unsigned long bitTransmissionStart = micros();

    /*
    The DHT11 sensor send a high signal in order to send a new bit of data to MCU.
    The value of the bit is defined with the duration of the pulse: is the pulse lasts about 30us,
    it is a 0 value, if the pulse lasts 70us it is a 1 value. The paragon for the cycles will be
    calculated on the most long pulse, i.e. the one for the 1 data.
    */
		leftCycles = TRANSMISSION_DHTBIT1_DURATION;
		while(digitalRead(pin) == HIGH) {
			if (result == DHT11READER_OK && leftCycles-- == 0) {
        result = DHT11READER_TIMEOUT;
      }
    }

    if (result == DHT11READER_OK) {
      /*
      In order to set a 0 or 1 value to the bit, it is defined to find if the pulse signal has a duration
      for more than 70us-30us=40us: if so, the bit cannot certainly be with value 0, for exclusion it will be 1.
      For calculating the final value, the extracted 1-bit will be shifted for the cardinal position on which it
      was sent by DHT11 sensor: a bit that was sent before another will have a more significant bit value, so will be
      shifted by more position than the next one. 
      */
      if ((micros() - bitTransmissionStart) > TRANSMISSION_DHTSENTBITDIFFERENCE_DURATION) {
        extractedData[measurementIdx] = extractedData[measurementIdx] | (1 << bitIdx);
      }

      /*
      Move the bit index towards right if greater than zero. If equals to zero, it means that all the previous bit were
      inserted in the final float value, so the bit index will be reset to MSB bit and the measurement index (i.e. the single
      value extracted from sensor) will be incremented.
      */
      if (bitIdx != 0) {
        bitIdx--;
      } else {
        bitIdx = 7;
        measurementIdx++;
      }
      i++;
    }
    
	}

  if (result == DHT11READER_OK) {
    /*
    Calculating the value of the humidity for the integer part and the decimal one.
    */
    humidity = extractedData[0] + (extractedData[1] * 0.1);

    /*
    Calculating the value of the temperature for the integer part and the decimal one. If the decimal part is equals to 0x80=10000000,
    it means that the value is negative, due to the fact that the last bit define the sign value. Also, the decimal part will be pruned
    by eventual excedeed bits (and the max value will be 0x0f) and will be moltiplied by 0.1 and sum to the final value.
    */
    temperature = extractedData[2]; 
    if (extractedData[3] & 0x80) { 
      temperature = -1 - temperature;
    }
    temperature += (extractedData[3] & 0x0f) * 0.1;
    /*
    If the checksum value is different from the extracted elements, an error during the transmission is occurred.
    */
    if (extractedData[4] != (extractedData[0] + extractedData[1] + extractedData[2] + extractedData[3])) {
      result = DHT11READER_CHECKSUM_FAIL;
    }
  }

	return result;
}

