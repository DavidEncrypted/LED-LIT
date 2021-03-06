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

const uint8_t sinusLights[360]={
  0,   0,   0,   0,   0,   1,   1,   2, 
  2,   3,   4,   5,   6,   7,   8,   9, 
 11,  12,  13,  15,  17,  18,  20,  22, 
 24,  26,  28,  30,  32,  35,  37,  39, 
 42,  44,  47,  49,  52,  55,  58,  60, 
 63,  66,  69,  72,  75,  78,  81,  85, 
 88,  91,  94,  97, 101, 104, 107, 111, 
114, 117, 121, 124, 127, 131, 134, 137, 
141, 144, 147, 150, 154, 157, 160, 163, 
167, 170, 173, 176, 179, 182, 185, 188, 
191, 194, 197, 200, 202, 205, 208, 210, 
213, 215, 217, 220, 222, 224, 226, 229, 
231, 232, 234, 236, 238, 239, 241, 242, 
244, 245, 246, 248, 249, 250, 251, 251, 
252, 253, 253, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 253, 253, 
252, 251, 251, 250, 249, 248, 246, 245, 
244, 242, 241, 239, 238, 236, 234, 232, 
231, 229, 226, 224, 222, 220, 217, 215, 
213, 210, 208, 205, 202, 200, 197, 194, 
191, 188, 185, 182, 179, 176, 173, 170, 
167, 163, 160, 157, 154, 150, 147, 144, 
141, 137, 134, 131, 127, 124, 121, 117, 
114, 111, 107, 104, 101,  97,  94,  91, 
 88,  85,  81,  78,  75,  72,  69,  66, 
 63,  60,  58,  55,  52,  49,  47,  44, 
 42,  39,  37,  35,  32,  30,  28,  26, 
 24,  22,  20,  18,  17,  15,  13,  12, 
 11,   9,   8,   7,   6,   5,   4,   3, 
  2,   2,   1,   1,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0};

int twoColorsAlternating ( int k, int c )
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

int powerHSV ( int k, int c )
{
  int r, g, b;
  k = k % 256;

  if ( k <= 85 )
  {
    r = 255 - k*3;
    g = k*3;
    b = 0;
  }

  else if ( k <= 170 )
  {
    r = 0;
    g = 255 - ( k - 85 ) * 3;
    b = ( k - 85 ) * 3;
  }

  else
  {
    r = ( k - 170 ) * 3;
    g = 0;
    b = 255 - ( k - 170 ) * 3;
  }


  if (c == 0) { return r; }
  if (c == 1) { return g; }
  if (c == 2) { return b; }
}

int regularHSV ( int k, int c )
{
  int r, g, b;
  k = k % 256;

  if ( k <= 42 )
  {
    r = 255;
    g = k*6;
    b = 0;
  }

  else if ( k <= 85 )
  {
    r = 255 - ( k - ( 85 - 42 ) ) * 6;
    g = 255;
    b = 0; 
  }

  else if ( k <= 127 )
  {
    r = 0;
    g = 255;
    b = ( k - 85 ) * 6;
  }

  else if ( k <= 170 )
  {
    r = 0;
    g = 255 - ( k - ( 170 - 42 ) ) * 6;
    b = 255;
  }

  else if ( k <= 212 )
  {
    r = ( k - 170 ) * 6;
    g = 0;
    b = 255;
  }

  else
  {
    r = 255;
    g = 0;
    b = 255 - ( k - ( 255 - 42 ) ) * 6;
  }


  if (c == 0) { return r; }
  if (c == 1) { return g; }
  if (c == 2) { return b; }
}

int sinus ( int k, int c )
{
  int r, g, b;

  r = sinusLights [ k % 360 ];
  g = sinusLights [ ( k + 120 ) % 360 ];
  b = sinusLights [ ( k + 240 ) % 360 ];

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
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( twoColorsAlternating ( k, 2 ), twoColorsAlternating ( k, 0 ), twoColorsAlternating ( k, 1 ) ) );
      }
    }

    pixels.show ();
    delay(dv);
  }
}

void powerHSVRainbow ()
{
  for ( int k = 0; k < 256; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( powerHSV ( k, 2 ), powerHSV ( k, 0 ), powerHSV ( k, 1 ) ) );
      }
    }

    pixels.show ();
    delay(dv);
  }
}

void regularHSVRainbow ()
{
  for ( int k = 0; k < 256; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( regularHSV ( k, 2 ), regularHSV ( k, 0 ), regularHSV ( k, 1 ) ) );
      }
    }

    pixels.show ();
    delay(dv);
  }
}

void sinusRainbow ()
{
  for ( int k = 0; k < 360; k++ )
  {
    for ( int i = 0; i < STRIPS; i++ )
    {
      for ( int j = 0; j < PPS; j++ )
      {
        pixels.setPixelColor ( i*PPS + j, pixels.Color ( sinus ( k, 2 ), sinus ( k, 0 ), sinus ( k, 1 ) ) );
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
    twoColorRainbow ();
  }

  if ( command == "2" )
  {
    powerHSVRainbow ();
  }

  if ( command == "3" )
  {
    regularHSVRainbow ();
  }

  if ( command == "4" )
  {
    sinusRainbow ();
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
}

