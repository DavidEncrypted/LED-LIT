// Libraries.
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Definitions.
#define PIN 6 // Which pin.
#define STRIPS  7 // Amount of strips.
#define PPS  7 // Pixels per strip.
#define NUMPIXELS   STRIPS * PPS // The number of pixels.
#define BRIGHTNESS  250 // Brightness of the LEDs.

String command; // Input.
int dv = 200; // Delayvalue.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
}

void checkerboard ()
{
  for ( int i = 0; i < STRIPS; i++ )
  {
    for ( int j = 0; j < PPS; j++ )
    {
      if ( ( i*PPS+j) % 2 == 0 )
      {
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( 255, 0, 0 ) );
      }
      
      else
      {
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 255, 0 ) );
      }
    }
  }
}

void walkingCheckerboard ()
{
  for ( int k = 0; k < 12; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        if ( ( i*PPS+j) % 2 == 0 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( 255 - k, k, 0 ) );
          pixels.show ();
          delay (dv);
        }
        
        else
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 255 - k, k ) );
          pixels.show ();
          delay (dv);
        }
      }
    }
  }
}

void changingCheckerboard ()
{
  for ( int m = 0; m < 12; m++)
  {
    for ( int  k = 0; k < 22; k++ )
    {
      for ( int i = 0; i < STRIPS; i++ )
      {
        for ( int j = 0; j < PPS; j++ )
        {
          if ( ( i*PPS+j) % 2 == 0 )
          {
            pixels.setPixelColor ( i*PPS + j, pixels.Color ( 255 - 21*m - k, 21* m + k, 0 ) );
          }
        }
      }
      
      pixels.show ();
      delay (10);
    }
  }
  
  for ( int m = 0; m < 12; m++)
  {
    for ( int  k = 0; k < 22; k++ )
    {
      for ( int i = 0; i < STRIPS; i++ )
      {
        for ( int j = 0; j < PPS; j++ )
        {
          if ( ( i*PPS+j) % 2 == 1 )
          {
            pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 21* m + k, 255 - 21*m - k ) );
          }
        }
      }
      
      pixels.show ();
      delay (10);
    }
  }
}

void lightsOff ()
{
  for ( int i = 0; i < STRIPS; i++ )
  {
    for ( int j = 0; j < PPS; j++ )
    {
      pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 0, 0 ) );
    }
  }
}

void loop ()
{
  if ( Serial.available() > 0 )
  {
    command = Serial.readStringUntil('\n');
  }

  if ( command == "1" )
  {
    checkerboard ();
    pixels.show ();
  }

  if ( command == "2" )
  {
    walkingCheckerboard ();
  }
  
  if ( command == "3" )
  {
    changingCheckerboard ();
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
}

