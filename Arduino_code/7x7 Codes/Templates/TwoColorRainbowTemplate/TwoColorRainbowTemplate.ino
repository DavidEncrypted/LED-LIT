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
int dv = 10; // Delayvalue.

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

void twoColorRainbows ()
{
  for ( int k = 0; k < 256; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        if ( i == 0 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( redGreen ( k, 2 ), redGreen ( k, 0 ), redGreen ( k, 1 ) ) );        
        }

        if ( i == 1 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( redBlue ( k, 2 ), redBlue ( k, 0 ), redBlue ( k, 1 ) ) ); 
        }

        if ( i == 2 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( greenBlue ( k, 2 ), greenBlue ( k, 0 ), greenBlue ( k, 1 ) ) ); 
        }

        if ( i == 3 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( 255, redGreen ( k, 0 ), redGreen ( k, 1 ) ) );        
        }

        if ( i == 4 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( redBlue ( k, 2 ), redBlue ( k, 0 ), 255 ) ); 
        }

        if ( i == 5 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( greenBlue ( k, 2 ), 255, greenBlue ( k, 1 ) ) ); 
        }

        if ( i == 6 )
        {
          pixels.setPixelColor ( i*PPS + j, pixels.Color ( greenBlue ( k, 2 ), 128, greenBlue ( k, 1 ) ) ); 
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

  if ( command == "1" )
  {
    twoColorRainbows ();
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
}

