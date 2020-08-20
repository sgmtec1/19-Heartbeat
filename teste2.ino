//Open Source

#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //INCLUSÃO DE BIBLIOTECA

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

#define HBDEBUG(i)

int sensorPin = A0;
double alpha = 0.75;
int period = 100;
double change = 0.0;
double minval = 0.0;
const int delayMsec = 60; // 100msec per sample

void setup()

{
  lcd.begin (16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  lcd.clear();
}

void loop()
{
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead (sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  lcd.setCursor(0,0);
  lcd.print("rV=" + (String)rawValue + " ");
  oldValue = value;
  static int beatMsec = 0;
  int heartRateBPM = 0;
  if (heartbeatDetected(sensorPin, delayMsec)) {
    heartRateBPM = 60000 / beatMsec;
    //digitalWrite(ledPin,1);
    // Print msec/beat and instantaneous heart rate in BPM
    //Serial.print(beatMsec);
    lcd.setCursor(9,0);
    lcd.print("bM=" + (String)beatMsec + "    ");
    lcd.setCursor(0,1); 
    lcd.print("BPM=" + (String)heartRateBPM + "    ");
      beatMsec = 0;
  } else {
    //digitalWrite(ledPin,0);
  }
  delay(delayMsec);
  beatMsec += delayMsec;
  //delay (period);
}
bool heartbeatDetected(int IRSensorPin, int delay)
{
  static int maxValue = 0;
  static bool isPeak = false;
  int rawValue;
  bool result = false;
  rawValue = analogRead(IRSensorPin);
  // Separated because analogRead() may not return an int
  rawValue *= (1000/delay);
  HBDEBUG(Serial.print(isPeak); Serial.print("p, "));
  HBDEBUG(Serial.print(rawValue); Serial.print("r, "));
  HBDEBUG(Serial.print(maxValue); Serial.print("m, "));
  // If sensor shifts, then max is out of whack.
  // Just reset max to a new baseline.
  if (rawValue * 4L < maxValue) {
    maxValue = rawValue * 0.8;
    HBDEBUG(Serial.print("RESET, "));
  }
  // Detect new peak
  if (rawValue > maxValue - (1000/delay)) {
    if (rawValue > maxValue) {
      maxValue = rawValue;
    }
    // Only return true once per peak.
    if (isPeak == false) {
      result = true;
      Serial.print(result); Serial.print(",  *");
    }
    isPeak = true;
  } else if (rawValue < maxValue - (3000/delay)) {
    isPeak = false;
    maxValue-=(1000/delay);
 }
  HBDEBUG(Serial.print("\n"));
  return result;
}
