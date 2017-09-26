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

const uint8_t HSVpower[121] = 
{ 0, 2, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 34, 36, 38, 40,
42, 45, 47, 49, 51, 53, 55, 57, 59, 62, 64, 66, 68, 70, 72, 74, 76, 79, 81, 
83, 85, 87, 89, 91, 93, 96, 98, 100, 102, 104, 106, 108, 110, 113, 115, 117, 
119, 121, 123, 125, 127, 130, 132, 134, 136, 138, 140, 142, 144, 147, 149, 
151, 153, 155, 157, 159, 161, 164, 166, 168, 170, 172, 174, 176, 178, 181, 
183, 185, 187, 189, 191, 193, 195, 198, 200, 202, 204, 206, 208, 210, 212, 
215, 217, 219, 221, 223, 225, 227, 229, 232, 234, 236, 238, 240, 242, 244, 
246, 249, 251, 253, 255 }; // Array that has 121 bytes, which is on third of a 
// full circle, so you can go through it in 3 steps, because there are 3 colors (RGB).

void colorFlower ()
{
  for ( int k = 0; k < 360; k++)
  {
    for ( int i = 0; i < STRIPS; i++ ) // To go through the matrix vertically.
    {
      for ( int j = 0; j < PPS; j++ ) // To go through the matrix horizontally.
      {
        for ( int m = 0; m < (PPS/2+1); m++ )
        {
          if ( ( i == m && ( j >= m && j < PPS-m ) ) || 
               ( i == (PPS-(m+1) ) && ( j >= m && j < PPS-m ) ) || 
               ( j == m && (i >= m && i < STRIPS-m ) ) || 
               ( j == ( STRIPS - (m+1) ) && (i >= m && i < STRIPS-m ) ) )
          {
            pixels.setPixelColor ( i*PPS + j,pixels.Color ( powerHSV ( k + 30*m, 2), powerHSV ( k + 30*m, 0), powerHSV ( k + 30*m, 1 ) ) );
            pixels.show();
          }
        }
      }
    }
  }
}

int powerHSV (int angle, int color)
{
  int red, green, blue;
  int angleC = angle % 360;
  
  if ( angleC < 120 ) {red = HSVpower[120 - angleC]; green = HSVpower[angleC]; blue = 0; }
  else if ( angleC < 240 ) {red = 0; green = HSVpower[240-angleC]; blue = HSVpower[angleC-120]; }
  else {red = HSVpower[angleC-240]; green = 0; blue = HSVpower[360-angleC]; }
  
  if ( color == 0 ) { return red; }
  if ( color == 1 ) { return green; }
  if ( color == 2 ) { return blue; }
}

void loop ()
{
  if ( Serial.available() > 0 )
  {
    command = Serial.readStringUntil('\n');
  }

  if ( command == "1" ) // Makes what I call a "bloem" shape.
  {
    colorFlower ();
    delay(dv);
  }

  if ( command == "2" )
  {
    for ( int i = 0; i < NUMPIXELS; i++ )
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0) );
    }
  }    
}

