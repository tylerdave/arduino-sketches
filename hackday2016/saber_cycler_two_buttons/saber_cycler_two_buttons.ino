// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>

#define GREEN_BUTTON_PIN   2    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.
#define RED_BUTTON_PIN    4
#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 21

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
bool oldColorChange = HIGH;
int showType = 0;
int c_white = strip.Color(127, 127, 127);
int c_orig = strip.Color(0, 127, 64);

int current_state = 0;
int target_pixels = 0;
int current_pixels = 0;
int current_color = c_orig;

void setup() {
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 // Serial.begin(9600);
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(GREEN_BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(100);
    // Check if button is still low after debounce.
    newState = digitalRead(GREEN_BUTTON_PIN);
    if (newState == LOW) {
        current_state = current_state ^ 1;
    }
  }

  bool newColorChange = digitalRead(RED_BUTTON_PIN);

  if (newColorChange == LOW && oldColorChange == HIGH){
    // Short delay to debounce button.
    delay(100);
    // Check if button is still low after debounce.
    newColorChange = digitalRead(RED_BUTTON_PIN);
    if (newColorChange == LOW) {
       // current_color = c_green;
        flashFlash();
    }
  }

  if (current_state == 1){
    target_pixels = strip.numPixels();
  } else {
    target_pixels = 0;
  }
 // Serial.println("state: ");
//  Serial.println(current_state);
//  Serial.println(current_pixels);
 // delay(1500);
  current_pixels = moveToTarget(current_pixels, target_pixels);
 // Serial.println("state: ");
  //Serial.println(current_state);
  //Serial.println(current_pixels);
  //delay(200);

 // if (current_state == 1){
    extendOn(current_color, 1, current_pixels);
//  }
  
  // Set the last button state to the old state.
  oldState = newState;

}


int moveToTarget(int8_t cur, uint8_t target){
  int increment = strip.numPixels() / 21;
  //Serial.println("increment");
  //Serial.println(increment);
  if (cur < target){
  //  Serial.println("increase");
    cur = cur + increment;
    if (cur > target){
      return target;
    } else {
      return cur;
    }
  } else {
    //Serial.println("decrease");
    cur = cur - increment;
    if (cur < target){
      return target;
    } else {
      return cur;
    }
  }
 // return cur;
}

void flashFlash(){
  int pixels = current_pixels;
  
  for (int i=0; i <3; i++){
  for (int i=0; i < pixels; i++){
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(30);
  for (int i=0; i < pixels; i++){
    strip.setPixelColor(i, strip.Color(0,0, 0));
  }
  strip.show();
  delay(20);
  for (int i=0; i < pixels; i++){
    strip.setPixelColor(i, strip.Color(0,0, 255));
  }
  strip.show();
  delay(30);
  }
}

void extendOn(uint32_t c, uint8_t wait, uint8_t num) {
  for (int i=0; i < strip.numPixels(); i++) {
     if (i < num){
       strip.setPixelColor(i, c);   
       strip.show();
       delay(wait);
     } else {
      strip.setPixelColor(i, 0); 
     }
  }
  strip.show();
}

void extendOff(uint8_t wait) {
  for (int i=strip.numPixels(); i>=0; i--) {
      strip.setPixelColor(i, 0);  
      strip.show();
     delay(wait);
      }
}

void theaterChase2(uint32_t c, uint8_t wait, uint8_t cycles) {
  for (int j=0; j<cycles; j++) {  // cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


