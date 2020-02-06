/*
 * Arduino interface for the use of WS2812 strip LEDs
 * Uses Adalight protocol and is compatible with Boblight, Prismatik etc...
 * "Magic Word" for synchronisation is 'Ada' followed by LED High, Low and Checksum
 * @author: Wifsimster <wifsimster@gmail.com> 
 * @library: FastLED v3.001
 * @date: 11/22/2015
 */

// Uncomment to use resitor to control brightness
// #define MANUAL_BRIGHTNESS

#define FASTLED_ALLOW_INTERRUPTS 0 //Flickering hack
#include "FastLED.h"
#define NUM_LEDS 72
#define DATA_PIN 2


// #define BRIGHTNESS 100 
#define FRAMES_PER_SEC 60

// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 115200

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

#ifdef MANUAL_BRIGHTNESS
  int brightness = 100;
#else
  #define BRIGHTNESS 200
#endif

// Initialise LED-array
CRGB leds[NUM_LEDS];

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, 0, 7);
}

void setup() {
  // Use NEOPIXEL to keep true colors
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

#ifdef MANUAL_BRIGHTNESS
  FastLED.setBrightness( brightness );
#else
  FastLED.setBrightness( BRIGHTNESS );
#endif
  
  
  // Welcome.
  rainbow();
  LEDS.delay(2000);
  for( int i = 0; i < 120; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 2); 
    LEDS.delay(1000/120); 
  }
  
  // Confirm black.
  LEDS.showColor(CRGB(0, 0, 0));
  
  Serial.begin(serialRate);
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
}

void updateBightness() {
#ifdef MANUAL_BRIGHTNESS
  int bright_tmp = analogRead(A6);
  float ratio = 1024 / 200.0;
  bright_tmp = bright_tmp / ratio + 50;

  if ( abs(brightness - bright_tmp) > 7 ) {
    brightness = bright_tmp;
    FastLED.setBrightness( brightness );
  }
#endif
}

void loop() { 
  
  // Wait for first byte of Magic Word
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  
  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  updateBightness();
  // Shows new values
  FastLED.delay(1000 / FRAMES_PER_SEC);
  // flush the serial buffer
  while(Serial.available()) { Serial.read(); }
}