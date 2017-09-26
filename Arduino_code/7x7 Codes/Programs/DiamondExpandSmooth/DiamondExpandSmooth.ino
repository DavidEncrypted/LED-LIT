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

int light1[3] = {255, 140, 20}; //White light
int light2[3] = {0, 0, 0}; //The second light stage, this is off
int nolight[3] = {0, 0, 0}; //No light

boolean runprogram = false;

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

void arrayToColor () { // Converts the elements of LEDArray to actual color. Does also do RGB to BRG conversion
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

void printmatrix(){
  Serial.print("[\n");
  for(int i = 0; i < STRIPS; i++){
    for(int j = 0; j < PPS; j++){
      Serial.print(LEDMatrix[i][j][0] > 0);
      Serial.print(" ");
    }
    Serial.print("|\n");
  }
  Serial.print("]\n\n");
}

void clearmatrix(){
  for ( int i = 0; i < PPS; i++ ) {
    for ( int j = 0; j < STRIPS; j++ ) {
      for ( int k = 0; k < COLORS; k++ ) {
        LEDMatrix[i][j][k] = nolight[k];
      }
    }
  }
}

//The equations which determine the shapes on the matrix
int diagLineDown(int x, int b){
  return -1 * x + b;
}

int diagLineUp(int x, int b){
  return x + b;
}

void lightsOn () {
  
  int y_down_r, y_down_l, y_up_r, y_up_l;
  
  //the first loop runs over the different frames of the animated diamond shape  
  for(int fr = 0; fr <= STRIPS; fr++){
    //This loop runs over the x coordinates of the matrix
    for ( int j = 0; j < STRIPS; j++ ) {
      //The inverted y coordinates of the lines
      y_down_r = diagLineDown(j, PPS - 1 + fr);
      y_down_l = diagLineDown(j, PPS - 1 - fr);
      y_up_r = diagLineUp(j, fr);
      y_up_l = diagLineUp(j, -fr);
      
      //Here we loop over the 3 RGB values
      for ( int k = 0; k < COLORS; k++ ) {
        
        if(fr > 0){
          //Turn the previous lines off using the color settings from light2
          LEDMatrix[PPS - 1 - diagLineDown(j, PPS - 1 + fr - 1)][j][k] = light2[k]; 
          LEDMatrix[PPS - 1 - diagLineDown(j, PPS - 1 - (fr - 1))][j][k] = light2[k]; 
          LEDMatrix[PPS - 1 - diagLineUp(j, fr - 1)][j][k] = light2[k]; 
          LEDMatrix[PPS - 1 - diagLineUp(j, -(fr - 1))][j][k] = light2[k];  
        }
        
        if(j <= STRIPS/2 && j >= STRIPS/2 - fr){
           LEDMatrix[PPS - 1 - y_down_l][j][k] = light1[k];
           LEDMatrix[PPS - 1 - y_up_r][j][k] = light1[k];
        }
        if(j >= STRIPS/2 && j <= STRIPS/2 + fr){
          LEDMatrix[PPS - 1 - y_down_r][j][k] = light1[k];
          LEDMatrix[PPS - 1 - y_up_l][j][k] = light1[k]; 
        }
        
      }
    }
    
    delay(150);
    matrixToLight ();
  }
}

void lightsOff () {
  for ( int i = 0; i < STRIPS; i++ ) {
    for ( int j = 0; j < PPS; j++ ) {
      for ( int k = 0; k < COLORS; k++ ) {
        LEDMatrix[i][j][k] = nolight[k];
      }
    }
  }
  matrixToLight ();
}

void loop () {
  if ( Serial.available() > 0 ) { command = Serial.readStringUntil('\n'); }
  
  Serial.print(command);
  
  if ( command == "off" ) { 
    lightsOff (); 
  }
  if ( command == "1" ) { 
    lightsOn (); 
  }
  
}

