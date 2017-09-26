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
#define BRIGHTNESS  255 // Brightness of the LEDs.

String command; // Input.
int dv = 30; // Delayvalue.
int shift = 20; // Shift between layers.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
}

int redGreen ( int k, int c )
{
  int r, g, b;
  k = k % 256;

  if ( k <= 127 )
  {
    r = 255 - k*2;
    g = k*2;
    b = 0;
  }

  else
  {
    r = ( k - 128 ) * 2;
    g = 255 - ( k - 128 ) * 2;
    b = 0;
  }

  if (c == 0) { return r; }
  if (c == 1) { return g; }
  if (c == 2) { return b; }
}

int redBlue ( int k, int c )
{
  int r, g, b;
  k = k % 256;

  if ( k <= 127 )
  {
    r = 255 - k*2;
    g = 0;
    b = k*2;
  }

  else
  {
    r = ( k - 128 ) * 2;
    g = 0;
    b = 255 - ( k - 128 ) * 2;
  }

  if (c == 0) { return r; }
  if (c == 1) { return g; }
  if (c == 2) { return b; }
}

int greenBlue ( int k, int c )
{
  int r, g, b;
  k = k % 256;

  if ( k <= 127 )
  {
    r = 0;
    g = 255 - k*2;
    b = k*2;
  }

  else
  {
    r = 0;
    g = ( k - 128 ) * 2;
    b = 255 - ( k - 128 ) * 2;
  }

  if (c == 0) { return r; }
  if (c == 1) { return g; }
  if (c == 2) { return b; }
}

void lightsOn ()
{
  for ( int i = 0; i < STRIPS; i++ )
  {
    for ( int j = 0; j < PPS; j++ )
    {
      pixels.setPixelColor ( i*PPS + j, pixels.Color ( 0, 255, 0 ) );
    }
  }
}

void twoColorRainbow ()
{
  for ( int k = 0; k < 256; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        for ( int m = 0; m < PPS/2+1; m++)
        {
          if ( i <= 3 )
          {
            if ( m == i )
            {
              pixels.setPixelColor ( i*PPS + j, pixels.Color ( redBlue ( k + m*shift, 2 ), redBlue ( k + m*shift, 0 ), redBlue ( k + m*shift, 1 ) ) );
            }
          }
            
          else
          {
            if ( m == ( (STRIPS-1) - i) )
            {
              pixels.setPixelColor ( i*PPS + j, pixels.Color ( redBlue ( k + m*shift, 2 ), redBlue ( k + m*shift, 0 ), redBlue ( k + m*shift, 1 ) ) );
            }
          }
        }
      }
    }

    pixels.show ();
    delay(dv);
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

  if ( command == "on" )
  {
    lightsOn ();
    pixels.show ();
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
}

