//voltage between 3.3 V to 5 V

#include <SimpleDHT.h>

// 	for DHT11
//	VCC : 5V or 3V
//	GND : GND
//	DATA : 2

int pinDHT11 = 2;
SimpleDHT11 dht11;


//void setup
void setup(){
	Serial.begin(9600);
} 

//void loop
void loop(){
	Serial.println("==================");
	Serial.println("Sample DHT11...");

	//read with raw sample data
	/*
	//FUNCTION WITH BYTE
	byte temperature = 0;
	byte humidity = 0;
	byte data[40] = {0};
	if( dht11.read(pinDHT11, &temperature, &humidity, data)){
		Serial.print("Read DHT11 failed");
		return;
	}
	*/

	float temperature = 0.0;
	float humidity = 0.0;
	byte data[40] = {0};
	if( dht11.read2(pinDHT11, &temperature, &humidity, data)){
		Serial.print("Read DHT11 failed");
		return;
	}

	Serial.print("Sample RAW Bits: ");
	for(int i = 0; i < 40; i++){
		Serial.print((int) data[i]);	
		for(int i > 0 && ((i +1) % 4) == 0){	
			Serial.print(' ');
		}
	}
	
	Serial.println("");
	Serial.println("Sample OK: ");
	Serial.println((int) temperature);
	Serial.println("*C, ");
	Serial.println((int) humidity);
	Serial.println(" %");
	
//DHT11 sampling rate is 1HZ, so a delay small delay before it samples again
	delay(1000);

} 
