// Frequency Counter Lib example

/*
  Martin Nawrath KHM LAB3
  Kunsthochschule f¸r Medien Kˆln
  Academy of Media Arts
  http://www.khm.de
  http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-frequency-counter-library/	
 */

//The Frequency input is fixed to digital pin 5.

#include <FreqCounter.h>


unsigned long frequency;
int cnt;
int pinLed = 13;

void setup() {
  pinMode(pinLed, OUTPUT);

  Serial.begin(115200);  // connect to the serial port

  Serial.println("Frequency Counter");
}

int gate = 100;  // 100 ms Gate Time

void loop() {

  // wait if any serial is going on
  FreqCounter::f_comp = 10;  // Cal Value / Calibrate with professional Freq Counter

  FreqCounter::start(gate);

  while (FreqCounter::f_ready == 0);

  unsigned int factor = 1000 / gate;

  frequency = FreqCounter::f_freq * factor;

  Serial.print(cnt++);
  Serial.print("  Freq: ");
  Serial.println(frequency);

  if(frequency > 999)
    gate = 100;
  else
    gate = 1000;
  
  delay(20);
  digitalWrite(pinLed, !digitalRead(pinLed));  // blink Led
}
