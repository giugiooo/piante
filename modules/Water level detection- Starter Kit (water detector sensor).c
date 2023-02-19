//function
String analogPinConverter(int value){
	String analogPinValue;
	
	switch(value){
		//pin D14 = A0
		case 14: analogPinValue = "A0"; break;
		//pin D15 = A1
		case 15: analogPinValue = "A1"; break;
		//pin D16 = A2
		case 16: analogPinValue = "A2"; break;
		//pin D17 = A3
		case 17: analogPinValue = "A3"; break;
		//pin D18 = A4
		case 18: analogPinValue = "A4"; break;
		//pin D19 = A5
		case 19: analogPinValue = "A5"; break;
		default :_ analogPinValue = "Pin Error"; break;
	} 
	return analogPinValue; 
}


//Global Constants
const int sensorPin = A0; 
//if TapWater there is no conductibility so values change 
const bool tapWater = true
 
//Global variables
int HistoryValue = 0;
char printBuffer[45];
String analogPin;


//Void Setup
void setup(){
	Serial.begin(9600);
	analogPin = analogPinConverter(sensorPin);
}

int levelConverter(int sensorValue, bool isCleanWater = true){
	int percentage;
	static int maxValue = 350;
	
	if(isCleanWater){
		percentage = map(sensorValue, 0, maxValue, 0, 100);
		if(percentage > 100 ){
			maxValue = sensorValue;
			percentage = 100;
		}
	}else{
		percentage = map(sensorValue, 0, 1023, 0 , 100);
	}
	return percentage;
}

//void loop

void loop(){
	int currentValue = analogRead(sensorPin);
	int diffValue = HistoryValue - currentValue;
	if((abs(diffValue) >= 10) || ((currentValue == 0) && (HistoryValue != 0 ))){
		int percentValue = levelConverter(currentValue, tapWater);
		sprintf(printBuffer, "PIN %s level is %04d)\n", analogPin.c_str(), percentValue);
		Serial.print(printBuffer);
		HistoryValue = currentValue;
	}
}

