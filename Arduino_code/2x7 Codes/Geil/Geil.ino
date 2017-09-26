#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Definitions
#define PIN 6
#define STRIPS  2
#define PIXELSPERSTRIP  7
#define NUMPIXELS   STRIPS * PIXELSPERSTRIP
#define BRIGHTNESS  255

String command;
int dv = 500;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//#if defined (__AVR_ATtiny85__)
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//#endif
  // End of trinket special code

  //initialize the serial connection
  Serial.begin(9600);
  
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
}

void loop (){

  if(Serial.available() > 0)
    {
        command = Serial.readStringUntil('\n');
        //x = Serial.parseInt();
    }

  if(command=="1")
    {
      for(int i=0; i<PIXELSPERSTRIP/2+1; i++)
        {
          pixels.setPixelColor(i, pixels.Color(0,255-i*60,i*60));
          pixels.setPixelColor(i+PIXELSPERSTRIP, pixels.Color(0,255-i*60,i*60));
          pixels.setPixelColor(PIXELSPERSTRIP-1-i, pixels.Color(0,255-i*60,i*60));
          pixels.setPixelColor(2*PIXELSPERSTRIP-1-i, pixels.Color(0,255-i*60,i*60)); 
          pixels.show();
          delay(dv);
        }
    }
     // pixels.setPixelColor(i, pixels.Color(0,0,0));
     // pixels.setPixelColor(PIXELSPERSTRIP-1-i, pixels.Color(0,0,0));
     // pixels.setPixelColor(i+PIXELSPERSTRIP, pixels.Color(0,0,0));
     // pixels.setPixelColor(2*PIXELSPERSTRIP-1-i, pixels.Color(0,0,0));  
      for(int i=0; i<NUMPIXELS; i++)
        {
          pixels.setPixelColor(i, pixels.Color(0,0,0));
          pixels.show();
        } 
      delay(dv);
        
  if(command=="2"){
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();
    }
  }
}

