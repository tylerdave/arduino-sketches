// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>

#define BLUE_BUTTON_PIN   2    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.
#define RED_BUTTON_PIN    4
#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 2

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_RGB + NEO_KHZ800);

uint32_t c_white = strip.Color(127, 127, 127);
uint32_t c_orig = strip.Color(255, 255, 16);
uint32_t c_new = strip.Color(255, 0, 0);

int incoming[2];

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);

//  Serial.print("white");
//  Serial.println();
//  Serial.print(String(c_white));
//  Serial.println();
//  Serial.print("orig");
//  Serial.println();
// Serial.print(String(c_orig));
//  Serial.println();
setAll(c_orig); 
delay(500);
setAll(c_new); 
delay(500);
setAll(c_orig); 

  
}

void loop() {
  //if(Serial.available() >= 3){
    //for (int i = 0; i < 3; i++){
     // incoming[i] = Serial.read();
   // }
    //c_new = strip.Color(incoming[0], incoming[1], incoming[2]);
    //Serial.print("new");
    //Serial.println();
    //Serial.print(String(c_orig));
    //Serial.println();
    setAll(c_new);
    //setAlternating(c_new, c_white);    
  //}
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




