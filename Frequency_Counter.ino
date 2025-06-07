// Frequency Counter Lib example

/*
  Martin Nawrath KHM LAB3
  Kunsthochschule f¸r Medien Kˆln
  Academy of Media Arts
  http://www.khm.de
  http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-frequency-counter-library/	
 */

/*
  AshenMoon:
  SSD1306 OLED Display
  Automatic Gate Switching
  https://github.com/AshenMoon/Frequency_Counter
*/

#include <FreqCounter.h>  // https://github.com/BlackBrix/Arduino-Frequency-Counter-Library

//The Frequency input is fixed to digital pin 5.

// SSD1306 Display required libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// The pins for I2C are defined by the Wire-library.
// On an arduino UNO and NANO: A4(SDA), A5(SCL)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define OLED_RESET 4              //OLED reset on pin 4
#define DISPLAY_I2C_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

unsigned long frequency;

int pinLed = 13;

void setup() {
  pinMode(pinLed, OUTPUT);

  Serial.begin(115200);  // connect to the serial port

  Serial.println("Frequency Counter");

  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  //delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

int gate = 100;  // Default 100 ms Gate Time

void displayFrequency() {
  // Update SSD1306 Display
  display.clearDisplay();
  display.setTextSize(2);       //set text sizez
  display.setTextColor(WHITE);  //set text color
  display.setCursor(0, 0);      //set cursor

  display.println(" Frequency");

  display.setTextSize(3);  //set text sizez
  if (frequency < 1000) {
    if (frequency < 100)
      display.print(' ');
    if (frequency < 10)
      display.print(' ');
    display.print("    ");
    display.println(frequency);
    display.print("     Hz");
  } else {
    if (frequency < 1000000)
      display.print(' ');
    if (frequency < 100000)
      display.print(' ');
    if (frequency < 10000)
      display.print(' ');
    display.print(frequency / 1000);
    display.print('.');
    display.print(((frequency / 100) % 10));
    display.println(((frequency / 10) % 10));
    display.print("    kHz");
  }

  display.display();
}

void updateGate() {
  // Set Gate according to the signal frequency
  gate = frequency > 999 ? 100 : 1000;
}

void loop() {

  // wait if any serial is going on
  FreqCounter::f_comp = 10;  // Cal Value / Calibrate with professional Freq Counter

  FreqCounter::start(gate);

  while (FreqCounter::f_ready == 0)
    ;

  unsigned int factor = 1000 / gate;

  frequency = FreqCounter::f_freq * factor;

  Serial.print("Freq: ");
  Serial.println(frequency);

  displayFrequency();
  updateGate();

  delay(20);
  digitalWrite(pinLed, !digitalRead(pinLed));  // blink Led
}
