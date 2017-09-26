#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Definitions
#define PIN 6 // Which pin
#define STRIPS  7 // Amount of strips
#define PPS  7 // Pixels per strip
#define NUMPIXELS   STRIPS * PPS // The number of pixels
#define BRIGHTNESS  250 // Brightness of the LEDs

String command;
int dv = 1000;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() 
{
  // Initialize the serial connection
  Serial.begin(9600);
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
}

void loop ()
{
  if(Serial.available() > 0)
    {
        command = Serial.readStringUntil('\n');
    }

  if (command == "1")
  {
    for ( int v = 1; v > -2; v-- )
    {
      for (int i=0; i<STRIPS; i++)
      {
        for (int j=0; j<PPS; j++)
        {
          if ( i == 1 && j == 2 ) { pixels.setPixelColor ( i*PPS + j, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 1 && j == 3 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 1 && j == 4 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 2 && j == 1 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 2 && j == 5 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 3 && j == 1 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 3 && j == 2 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 3 && j == 3 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 3 && j == 4 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 3 && j == 5 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 4 && j == 1 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 4 && j == 5 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 5 && j == 1 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); }
          else if ( i == 5 && j == 5 ) { pixels.setPixelColor ( i*PPS + j + v, pixels.Color ( 255, 0, 0 ) ); } 
          else { pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 0, 0 ) ); } 
        }
      }
    
      pixels.show();
      delay(dv);
    }
  }

  if (command=="2")
  {
    for(int i=0; i<NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();
    }
  }    
}

