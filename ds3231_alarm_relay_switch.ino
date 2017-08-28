#include <Himadri_DS3231.h>
#include <LiquidCrystal.h>

// Declaring object of the DS3231 class
Himadri_DS3231 ds3231;

// Declaring and initializing object of LCD
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

// timeParameters are struct datatype to store date and time
timeParameters timeVals;

// alarmParameters are struct datatype to store alarm date and time
alarmParameters alarmVals;

// Days array 
String weekDay[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

// state is a volatile variable to change state when interrupt from DS3231 occur when alarm matches the time
volatile boolean state = false;

// Interrupt PIN number where the INT/SQW is attached on Arduino
const int intSqwPin = 2;

// This variable is required the track the milliseconds when interrupt started
long initialMills = 0;

// The PIN where the BASE PIN of P2222A transistor is connected. This transistor is acting as a switch.
int relayPin = 8;

// This is the Interrupt Service Routine function that will be called when the interrupt occur.
void isr() {
  // Make the replayPin to HIGH state
  digitalWrite(relayPin, HIGH);
  // Print out that an interrupt occur
  Serial.print("External Interrupt Detected");
  // Store the milliseconds when the interrupt occur
  initialMills = millis();  
  // Change the state to true as the interrupt occur
  state = true;
}

void setup() {
  Serial.begin(9600);

  // Set digital PIN 2 as INPUT
  DDRD &=~ 0x04;    // 00000100

  // Make the PIN mode of PIN 8 to Output
  pinMode(relayPin, OUTPUT);

  if (!ds3231.begin()) {    // It will initialize DS3231 register values
    Serial.println("Could not find a valid DS3231 sensor, check wiring!");
    while (1) {}
  } else {
    Serial.println("Initialization Done!");

    // Initialize the timeParameters
//    ds3231.initializeDateTime("Aug 28 2017 MON", "05:15:00 AM", &timeVals); 
    
    /* Set Date and Time into DS3231 Timekeeping Register
    */
//    if (!ds3231.setDateTime(&timeVals)) {   // Set the time using the timeParameters
//      Serial.println("Error setting time and date!");
//    } else {
//      Serial.println("Date and Time setup correctly!");
//    } 

    // Set Alarm1 of DS3231. It will make an interrupt request when the Second is 10
    ds3231.setAlarm(Second, 10, false, false, Alarm1);

    // Enable the Alarm1
    ds3231.enableAlarm(Alarm1);

    // Attach ISR(Interrupt Service Routine) with the Interrupt
    attachInterrupt(0, isr, FALLING);
  }

  lcd.begin(16, 2);
  lcd.print("Starting Up!");
  
  delay(1000);
  
  lcd.clear();
}

void loop() {
  if(ds3231.readDateTime(&timeVals)) {  // Read the Date and Time from Timekeeping Register of DS3231
    lcd.setCursor(1, 0);

    lcd.print(timeVals.y);
    lcd.print("/");
    if (timeVals.m < 10) {
      lcd.print("0" + (String)timeVals.m);
    } else {
      lcd.print(timeVals.m);
    }
    lcd.print("/");
    if (timeVals.d < 10) {
      lcd.print("0" + (String)timeVals.d);
    } else {
      lcd.print(timeVals.d);
    }
  
    lcd.setCursor(12, 0);
    lcd.print(weekDay[timeVals.day - 1]);
  
    lcd.setCursor(0, 1);
    if (timeVals.hh < 10) {
      lcd.print("0" + (String)timeVals.hh);
    } else {
      lcd.print(timeVals.hh);
    }
    lcd.print(":");
    if (timeVals.mm < 10) {
      lcd.print("0" + (String)timeVals.mm);
    } else {
      lcd.print(timeVals.mm);
    }
    lcd.print(":");
    if (timeVals.ss < 10) {
      lcd.print("0" + (String)timeVals.ss);
    } else {
      lcd.print(timeVals.ss);
    }
      
    if (timeVals.md == 0) {
      lcd.print("AM");
    } else if (timeVals.md == 1) {
      lcd.print("PM");
    }
  
    lcd.setCursor(11, 1);
    lcd.print(ds3231.readTemp(Celsius));  
  } else {
    Serial.println("Error Reading Date and Time from DS3231.");
  }
  
  if(state == true) {
    // Get the current miliseconds 
    long currentMills = millis(); 

    // Check if the difference between the current miliseconds and the initial interrupt milliseconds is greater or eaqual to 10 seconds then switch off the RELAY
    if(currentMills - initialMills >= 10000) {      
        digitalWrite(relayPin, LOW);
        
        state = false;
        
        initialMills = 0;
        
        currentMills = 0;   
        
        // It will clear the Alarm Flag after the alarm interrupt.
        // Without clearing the flag it will do the interrupt for the second time.
        ds3231.clearINTStatus(BothAlarm);
    }
  }
}
