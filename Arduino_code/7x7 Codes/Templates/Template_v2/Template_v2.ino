// Libraries.
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Universal definitions.
#define PIN 6 // Which pin.
#define STRIPS  7 // Amount of strips.
#define PPS  7 // Pixels per strip.
#define NUMPIXELS   STRIPS * PPS // The number of pixels.
#define BRIGHTNESS  255 // Brightness of the LEDs.
#define COLORS  3 // Amount of colors (red, blue and green).

int LEDMatrix[STRIPS][PPS][COLORS]; // The matrix that represents the real LED array.
int LEDArray[NUMPIXELS][COLORS]; // The LEDArray.

String command; // Input.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
}

void matrixToArray () { // Converts LEDMatrix to LEDArray.
  for ( int i = 0; i < STRIPS; i++ ) {
    for ( int j = 0; j < PPS; j++ ) {
      for ( int k = 0; k < COLORS; k++ ) {
        if ( i % 2 == 0 ) { LEDArray[i * PPS + j][k] = LEDMatrix[( PPS - 1 ) - j][( PPS - 1 ) - i][k]; }
        else { LEDArray[i * PPS + j][k] = LEDMatrix[j][( PPS - 1 ) - i][k]; }
      }
    }
  }
}

void arrayToColor () { // Converts the elements of LEDArray to actual color.
  for ( int i = 0; i < NUMPIXELS; i++ ) {
    pixels.setPixelColor ( i, pixels.Color ( LEDArray[i][2], LEDArray[i][0], LEDArray[i][1] ) );
  }
}

void colorToLight () { // Prints Array.
  pixels.show ();
}

void matrixToLight () { // Does all three previous functions in one.
  matrixToArray ();
  arrayToColor ();
  colorToLight ();
}

void lightsOn () {
  for ( int i = 0; i < STRIPS; i++ ) {
    for ( int j = 0; j < PPS; j++ ) {
      for ( int k = 0; k < COLORS; k++ ) {
        switch ( k ) {
          case 0: LEDMatrix[i][j][k] = 255; break;
          case 1: LEDMatrix[i][j][k] = 255; break;
          case 2: LEDMatrix[i][j][k] = 255; break;
        }
      }
    }
  }
  matrixToLight ();
}

void lightsOff () {
  for ( int i = 0; i < STRIPS; i++ ) {
    for ( int j = 0; j < PPS; j++ ) {
      for ( int k = 0; k < COLORS; k++ ) {
        switch ( k ) {
          case 0: LEDMatrix[i][j][k] = 0; break;
          case 1: LEDMatrix[i][j][k] = 0; break;
          case 2: LEDMatrix[i][j][k] = 0; break;
        }
      }
    }
  }
  matrixToLight ();
}

void loop () {
  if ( Serial.available() > 0 ) { command = Serial.readStringUntil('\n'); }
  if ( command == "0" ) { lightsOff (); }
  if ( command == "1" ) { lightsOn (); }
}

