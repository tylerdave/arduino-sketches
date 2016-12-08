#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);

// For 36mm LED pixels: these pixels internally represent color in a
// different format.  Either of the above constructors can accept an
// optional extra parameter: WS2801_RGB is 'conventional' RGB order
// WS2801_GRB is the GRB order required by the 36mm pixels.  Other
// than this parameter, your code does not need to do anything different;
// the library will handle the format change.  Examples:
//Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin, WS2801_GRB);
//Adafruit_WS2801 strip = Adafruit_WS2801(25, WS2801_GRB);


/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  r = r / 2;
  g = g / 2;
  b = b / 2;
  
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


uint32_t c_white = Color(127, 127, 127);
uint32_t c_orig = Color(0, 128, 128);
uint32_t c_new = Color(128, 0, 0);
int32_t nc = -1;
int incoming[2];


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
    Serial.begin(9600);
setAll(c_orig); 
delay(200);
setAll(c_new); 
delay(200);
setAll(c_orig); 

}


void loop() {
  // setFive();
  uint32_t red = Color(127, 0, 0);
  uint32_t green = Color(0, 127, 0);
  uint32_t blue = Color(0, 0, 127);
  uint32_t white = Color(127, 127, 127);
  uint32_t orange = Color(127, 64, 0);
  uint32_t off = Color(0, 0, 0);

  
  setMulti(5, red, green, off, white, orange);
  
  // Some example procedures showing how to display to the pixels
  
  //colorWipe(Color(255, 0, 0), 50);
 // colorWipe(Color(0, 255, 0), 50);
 // colorWipe(Color(0, 0, 255), 50);
 // rainbow(20);
//  rainbowCycle(20);
  //  if(Serial.available() >= 3){
    //for (int i = 0; i < 3; i++){
     // incoming[i] = Serial.read();
   // }
   // c_new = Color(incoming[0], incoming[1], incoming[2]);
    //Serial.print("new");
    //Serial.println();
    //Serial.print(String(c_orig));
    //Serial.println();
    //setAll(c_new);    
   // setAlternating(c_new, c_orig);

  //}

}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void setAll(uint32_t c) {
  for (int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}


void setAlternating(uint32_t c1, uint32_t c2) {
  for (int i=0; i < strip.numPixels(); i++) {
    if (i % 2 == 1){
      strip.setPixelColor(i, c1);
    } else {
    strip.setPixelColor(i, c2);  
    }  
  }
  strip.show();
}

void setFive(){
  uint32_t red = Color(127, 0, 0);
  uint32_t green = Color(0, 127, 0);
  uint32_t blue = Color(0, 0, 127);
  uint32_t white = Color(127, 127, 127);
  uint32_t orange = Color(127, 83, 0);
  for (int i=0; i < strip.numPixels(); i++) {
    switch (i % 5) {
      case 0:
        strip.setPixelColor(i, green);
        break;
      case 1:
        strip.setPixelColor(i, red);
        break;
      case 2:
        strip.setPixelColor(i, blue);
        break;
      case 3:
        strip.setPixelColor(i, white);
        break;
      case 4:
        strip.setPixelColor(i, orange);
        break;
       
    }
    delay(100);
    strip.show();
  }
}

void setMulti(int numColors, uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, uint32_t c5){
  for (int i=0; i<strip.numPixels(); i++){
    switch (i % numColors) {
      case 0:
        strip.setPixelColor(i, c1);
        break;
      case 1:
        strip.setPixelColor(i, c2);
        break;
      case 2:
        strip.setPixelColor(i, c3);
        break;
      case 3:
        strip.setPixelColor(i, c4);
        break;
      case 4:
        strip.setPixelColor(i, c5);
        break;
    }
    delay(100);
    strip.show();
  }
}


