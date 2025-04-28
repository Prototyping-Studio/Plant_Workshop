// These constants won't change. They're used to give names to the pins used:                    //00
                                                                                                 //01
const int ledPin = 2;                                                                            //02 Digital output pin that the LED is attached to
const int pumpPin = 12;                                                                          //03 Digital output pin that the water pump is attached to
const int waterLevelPin = A3;                                                                     //04 Digital input pin water level sensor is connected to
const int moistureSensorPin = A0;                                                                //05 Analog input pin used to check the moisture level of the soil
                                                                                                 //06
                                                                                                 //07
// These are the values to edit for real-world home use                                          //08
                                                                                                 //09 
unsigned long checkInterval = 3600000;                                                           //10 time to wait before checking soil moisture - 1 hour (3600000ms)
int waterPumpTime = 5000;                                                                        //11 how long to run the pump in ms - 5 seconds to provide adequate water
int moistureThreshold = 320; //Analog Threshhold for values between 0-1023                       //12 threshold below which the plant needs watering (0-1023 scale, lower = drier)
int waterDetectionDelay = 1000;                                                                  //13 delay between water detection readings in ms
                                                                                                 //14 
                                                                                                 //15
// Global Value For Storage From Sensor                                                          //16
int moistureSensorValue = 0;                                                                     //17 somewhere to store the value read from the soil moisture sensor
                                                                                                 //18
void setup() {                                                                                   //19
  // put your setup code here, to run once:                                                      //20
  Serial.begin(9600); //THIS IS FOR DEBUGGING                                                    //21 Initialize serial monitor for debugging
  pinMode(ledPin, OUTPUT);                                                                       //22 set as output to send value to LED
  pinMode(pumpPin, OUTPUT);                                                                      //23 set as output to send value to PUMP
  pinMode(waterLevelPin, INPUT);                                                                 //24 Set water level pin as input to recieve value
  pinMode(moistureSensorPin, INPUT);                                                             //25 Set soil moisture pin as input to recieve value
                                                                                                 //26
  //flash the LED five times to confirm power on and operation of code:                          //27
  for (int i=0; i <= 4; i++){                                                                    //28 loop through to blink LED 5 times
    digitalWrite(ledPin, HIGH);                                                                  //29  turn the LED on 
    delay(300);                                                                                  //30
    digitalWrite(ledPin, LOW);                                                                   //31  turn the LED off 
    delay(300);                                                                                  //32
  }                                                                                              //33
  delay(2000);                                                                                   //34
  digitalWrite(ledPin, HIGH);                                                                    //35  turn the LED on 
  Serial.println("Self-watering plant system is ready!"); //THIS IS FOR DEBUGGING                //36
}                                                                                                //37  print to serial monitor
                                                                                                 //38
// Function to reliably detect water with debouncing                                             //39
boolean isWaterDetected() {                                                                      //40 checks for water in the reservoir multiple times to account for errors
  int waterDetectedCount = 0;                                                                    //41 declare a storage variable that keeps track of how many times water is detected out of 3 attempts
                                                                                                 //42 
  // Take multiple readings with delays between them                                             //43 Loop three times for readings
  for (int i = 0; i < 3; i++) {                                                                  //44 
    if (digitalRead(waterLevelPin) > 100) {                                                    //45 HIGH typically means water is detected
      waterDetectedCount++;                                                                      //46
    }                                                                                            //47 wait between readings (preset to 10 seconds)
    delay(waterDetectionDelay);                                                                  //48
  }                                                                                              //49
                                                                                                 //50
  // Return true if majority of readings detected water                                          //51
  return (waterDetectedCount > 1);                                                         //52 return true or false based on how many times water was detected
}                                                                                                //53
                                                                                                 //54
void loop() {                                                                                    //55
  // put your main code here, to run repeatedly:                                                 //56
                                                                                                 //57
  // Check water level with debouncing                                                           //58
  boolean waterDetected = isWaterDetected();                                                     //59 This variable is true or false, used in our conditional statement
  Serial.print("Water detected: ");    //THIS IS FOR DEBUGGING                                   //60 used for debugging to serial monitor, anytime you see Serial.print()
  Serial.println(waterDetected);     //THIS IS FOR DEBUGGING                                     //61
                                                                                                 //62
  if (waterDetected == false) {                                                                  //63 If water IS NOT detected, do the following, if water IS detected, go to the else statement
    Serial.println("Water reservoir is empty! Please refill.");                                  //64
    for (int i = 0; i < 10; i++) {                                                               //65  loop 10 times, and blink the LED each time
      digitalWrite(ledPin, LOW);                                                                 //66
      delay(500);                                                                                //67 
      digitalWrite(ledPin, HIGH);                                                                //68
      delay(500);                                                                                //69
    }                                                                                            //70
    delay(30000);                                                                                //71 Check again in 30 seconds if reservoir is empty. if is still empty, blink again
  }                                                                                              //72
  else {                                                                                         //73 ELSE (water IS in the tank)
    digitalWrite(ledPin, HIGH);                                                                  //74 LED on
                                                                                                 //75
    // check soil moisture level                                                                 //76
    moistureSensorValue = analogRead(moistureSensorPin);                                         //77 read the moisture sensor and save the value, somewhere between 0 and 1023
    Serial.print("Moisture level reading: ");    //THIS IS FOR DEBUGGING                         //78
    Serial.println(moistureSensorValue);          //THIS IS FOR DEBUGGING                        //79
                                                                                                 //80
    if (moistureSensorValue < moistureThreshold) {                                               //81 LOWER VALUE MEANS DRIER SOIL
      Serial.println("Soil is too dry, watering now!");    //THIS IS FOR DEBUGGING               //82
      //pulse the pump                                                                           //83
      digitalWrite(pumpPin, HIGH);                                                               //84 Turn the pump on for specified delay (preset to 5 seconds)
      Serial.println("Pump on");  //THIS IS FOR DEBUGGING                                        //85
      delay(waterPumpTime);                                                                      //86
      digitalWrite(pumpPin, LOW);                                                                //87 Turn the Pump off
      Serial.println("Pump off");    //THIS IS FOR DEBUGGING                                     //88
      delay(30000);                                                                              //89 30 second delay to allow the moisture to spread through the soil
    }                                                                                            //90
    else {                                                                                       //91
      delay(checkInterval);                                                                      //92 wait before checking the soil moisture level again (1 hour)
    }                                                                                            //93
  }                                                                                              //94
}                                                                                                //95
