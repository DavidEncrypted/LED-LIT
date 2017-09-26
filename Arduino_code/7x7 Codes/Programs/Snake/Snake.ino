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
int dv = 300; // Delayvalue.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
}

void snakeOne ()
{
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    pixels.setPixelColor ( i, pixels.Color ( 0, 255, 0 ) );
    
    if ( i == 0 )
    {
      pixels.setPixelColor ( NUMPIXELS - 1, pixels.Color ( 0, 0, 0 ) );
    }
    
    else
    {
      pixels.setPixelColor ( i - 1 , pixels.Color ( 0, 0, 0 ) );
    }
    
    pixels.show ();
    delay (dv);
  }
}

void snakeTwo ()
{
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    pixels.setPixelColor ( i, pixels.Color ( 0, 255, 0 ) );
    
    if ( i == 0 )
    {
      pixels.setPixelColor ( NUMPIXELS - 2, pixels.Color ( 0, 0, 0 ) );
    }

    else if ( i == 1 )
    {
      pixels.setPixelColor ( NUMPIXELS - 1, pixels.Color ( 0, 0, 0 ) );
    }
    
    else
    {
      pixels.setPixelColor ( i - 2 , pixels.Color ( 0, 0, 0 ) );
    }
    
    pixels.show ();
    delay (dv);
  }
}

void snakeMoreThanTwo ()
{
  int lengthSnake = 3;
  
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    pixels.setPixelColor ( i, pixels.Color ( 255, 0, 0 ) );

    for ( int k = 0; k < lengthSnake; k++ )
    {
      if ( i == k )
      {
        pixels.setPixelColor ( NUMPIXELS - ( lengthSnake - k ), pixels.Color ( 0, 0, 0 ) );
      }
    }

    if ( i >= lengthSnake )
    {
      pixels.setPixelColor ( i - lengthSnake, pixels.Color ( 0, 0, 0 ) );
    }
    
    pixels.show ();
    delay (dv);
  }
}

void rainbowSnake ()
{
  int lengthSnake = 7;
  
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    pixels.setPixelColor ( i, pixels.Color ( 255 - i*5, 0, i*5 ) );

    for ( int k = 0; k < lengthSnake; k++ )
    {
      if ( i == k )
      {
        pixels.setPixelColor ( NUMPIXELS - ( lengthSnake - k ), pixels.Color ( 0, 0, 0 ) );
      }
    }

    if ( i >= lengthSnake )
    {
      pixels.setPixelColor ( i - lengthSnake, pixels.Color ( 0, 0, 0 ) );
    }
    
    pixels.show ();
    delay (dv);
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
    snakeOne ();
  }

  if ( command == "2" )
  {
    snakeTwo ();
  }

  if ( command == "3")
  {
    snakeMoreThanTwo ();
  }

  if ( command == "4")
  {
    rainbowSnake ();
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
}

