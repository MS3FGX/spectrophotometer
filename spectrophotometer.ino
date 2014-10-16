/*
  Firmware for 3D printed spectrometer.
  Developed by Tom Nardi (MS3FGX@gmail.com)
  Released under the BSD 3 Clause license
*/

// Version info
#define VERSION 1.0

// Libraries for hardware
#include <Wire.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

// I/O Lines
#define SENSOR A0
#define LED 3
#define BUTTON 2

// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Calibration values
#define FULLBATT 5.15
#define NOSAMPLE 485
#define WATER 470

// Sensor data
#define SAMPLES 100
int SensorArray [SAMPLES];

// Button object
Bounce SCAN_SMPL = Bounce ();

// Misc variables
int sensor_reading;
int debug = 0;

void setup()
{
  // Init serial coms
  Serial.begin(57600);
  Serial.println("Power on");
  
  // Setup LCD
  lcd.begin(20,4);
  
  // Print version and battery
  lcd.setCursor(0,0);
  lcd.print("V");
  lcd.print(VERSION);
  
  lcd.setCursor(8,0);
  lcd.print("Batt:");
  lcd.print(BatteryPercent());
  lcd.print("%");
  
  // Setup reference LED
  pinMode(LED, OUTPUT);
  
  // Setup button
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  
  // If held, go to debug
  if (digitalRead(BUTTON) == LOW)
    debug = 1;
    
  // Setup bounce library
  SCAN_SMPL .attach(BUTTON);
  SCAN_SMPL .interval(10);

  // Exiting setup
  Serial.println("Ready");
}

void loop()
{  
  // Check for debug
  while (debug == 1)
  {
    // First line
    lcd.setCursor(0,0);
    lcd.print("DEBUG");
    lcd.setCursor(7,0);
    lcd.print("Volt:");
    lcd.print(readVcc()/1000.0);
    
    // LED on
    digitalWrite(LED, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Sensor: ");
    lcd.print(ReadSensor());
    
    // Wait before loop
    delay(100);
  }
  
  
  // Let user know we are ready
  lcd.setCursor(0,1);
  lcd.print("Ready");
  
  // Scan for button
  SCAN_SMPL.update();
  
  if (SCAN_SMPL.fell())
  {    
    // Notification
    ClearLine(1);
    lcd.print("Analyzing...");
    Serial.println("Scanning...");
    
    // Reference LED on
    digitalWrite(LED, HIGH);
    delay(50);
    
    // Read sensor and attempt to match
    sensor_reading = ReadSensor();
    if (sensor_reading >= NOSAMPLE)
    {
      ClearLine(1);
      lcd.print("No Sample!");
      Serial.println("No sample!");
    }
    else if (sensor_reading >= WATER)
    {
      ClearLine(1);
      lcd.print("Pure Water");
      Serial.println ("Water");
    }
    else
    {
      ClearLine(1);
      //lcd.print("Result: ");
      lcd.print(WaterPercent(sensor_reading));
      lcd.print("% water");
    }
    
    // Always print to serial for debug
    Serial.println(sensor_reading);
    
    // Reference LED off
    digitalWrite(LED, LOW);
  
    // Show results on screen for a bit
    delay(3000);
    
    // LCD clear
    ClearLine(1);
  }
}

int ReadSensor()
{
  // Load data into array
  for (int x = 0; x < (SAMPLES - 1); x++)
  {
    SensorArray[x] = analogRead(A0);
    delay(20);
  }

  // Process data and return result
  int i, x, y, total;
  
  // Sort the array 
  for(i = 0; i < (SAMPLES - 1); i++)
  {
    for(x = 0; x < (SAMPLES - (i + 1)); x++)
    {
      if(SensorArray[x] > SensorArray[x+1])
      {
         int temp = SensorArray[x];
         SensorArray[x] = SensorArray[x + 1];
         SensorArray[x + 1] = temp;
       }
     }
  }
    
   // Remove top and bottom 20
   int TrimmedArray [SAMPLES - 20];
   y = 0;
   for(x = 19; x < (SAMPLES - 20); x++)
   {
     TrimmedArray[y] = SensorArray[x];
     y++;
   }
    
   // Average remaining values
   total = 0;
   for(x = 0; x < (y - 1); x++)
     total += TrimmedArray[x];
      
   return(total / (y - 1));  
}

long readVcc()
{
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

int BatteryPercent()
{
  return(((readVcc()/1000.0) / FULLBATT) * 100);
}

int WaterPercent(double sensor)
{
  // Handle as double to keep precision
  return((sensor / WATER) * 100);
}

void ClearLine(int line)
{
  // What a hack...
  lcd.setCursor(0,line);
  lcd.println("                ");
  lcd.setCursor(0,line);
}

void blinkLED()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
} 
