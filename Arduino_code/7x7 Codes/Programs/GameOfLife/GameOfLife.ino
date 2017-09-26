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
#define w  5 //The extra width added to the game of life world outside the matrix borders
#define h  5 //The extra height added to the game of life world outside the matrix borders
#define fillperc  40 //percentage of cells that is alive at the beginning of the game of life

String command; // Input.
int dv = 500; // Delayvalue in miliseconds.
int basecolor[3] = {255, 115, 20}; //The default color of the pixels

//The field on which the game of life takes place
int world[STRIPS + w*2][PPS + h* 2];
//A copy of this world used for determining the neighbours
int newworld[STRIPS + w*2][PPS + h*2];
//The section of the world which will be shown on the matrix
int pixelfield[STRIPS][PPS];

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);


void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'

  //Fill the field with random pixel values: either 0 (dead) or 1 (alive)
  for ( int i = 0; i < STRIPS + 2*w; i++ ){ 
    for ( int j = 0; j < PPS + 2*h; j++ ){
      if(random(0,101) <= fillperc){
        world[i][j] = 1;
      }else{
        world[i][j] = 0;
      }
    }
  }
}

void gameoflife (){ //This function runs John Conway's Game of Life on the 7x7 matrix
  int nbours; //the amount of neighbours
  
  //Update the pixelfield which is going to be displayed on the matrix
  for(int i = 0; i < STRIPS; i++){
    for(int j = 0; j < PPS; j++){
      pixelfield[i][j] = world[i+w][j+h];
    }
  }
  
  //Here we show the field on the matrix. This is done before iterating to the next generation,
  //as otherwise we would never see the begin configuration
  for ( int i = 0; i < STRIPS; i++ ){
    for ( int j = 0; j < PPS; j++ ){
      if(pixelfield[i][j] == 0){ //sets the dead cells
        if(j % 2 == 1){
          pixels.setPixelColor (j*PPS + i, pixels.Color (0, 0, 0) ); 
        }else{
          pixels.setPixelColor (j*PPS + 6 - i, pixels.Color (0, 0, 0) ); 
        }
      }else if(pixelfield[i][j] == 1){ //sets the alive cells
        if(j % 2 == 1){
          pixels.setPixelColor ( j*PPS + i, pixels.Color (basecolor[2], basecolor[0], basecolor[1]) );
        }else{
          pixels.setPixelColor ( j*PPS + 6 - i, pixels.Color (basecolor[2], basecolor[0], basecolor[1]) );
        }
      }
    }
  }
  
  //here we create a copy of the world
  for(int i = 0; i < (STRIPS + 2*w); i++){
    for(int j = 0; j < (PPS + 2*h); j++){
      newworld[i][j] = world[i][j];
    }
  }

  //Here the neighbours are checked, which determines whether a cell dies or comes to life
  //This is done in the 
  for ( int i = 1; i < (STRIPS + 2*w - 2); i++ ){ 
    for ( int j = 1; j < (PPS + 2*h - 2); j++ ){
      nbours = 0; 

      nbours += world[i-1][j-1];
      nbours += world[i-1][j];
      nbours += world[i-1][j+1];
      
      nbours += world[i][j-1];
      nbours += world[i][j+1];
      
      nbours += world[i+1][j-1];
      nbours += world[i+1][j];
      nbours += world[i+1][j+1];

      if(nbours == 3 || (nbours == 2 && newworld[i][j] == 1)){
        newworld[i][j] = 1;
      }else{
        newworld[i][j] = 0;
      }

    }
  }
  
  //Put the changes in the original world
  for(int i = 0; i < (STRIPS + 2*w); i++){
    for(int j = 0; j < (PPS + 2*h); j++){
      world[i][j] = newworld[i][j];
    }
  }

  /*
  //checking the values of the pixelfield
  Serial.print("[\n");
  for(int i = 0; i < STRIPS; i++){
    for(int j = 0; j < PPS; j++){
      Serial.print(pixelfield[i][j]);
      Serial.print(" ");
    }
    Serial.print("|\n");
  }
  Serial.print("]\n\n");
  */

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
  
  //Fill the field with random pixel values again
  for ( int i = 0; i < STRIPS + 2*w; i++ ){ 
    for ( int j = 0; j < PPS + 2*h; j++ ){
      if(random(0,101) <= fillperc){
        world[i][j] = 1;
      }else{
        world[i][j] = 0;
      }
    }
  }
}

void loop ()
{
  if ( Serial.available() > 0 )
  {
    command = Serial.readStringUntil('\n');
  }
  //Serial.print("yes");
  
  if ( command == "1" )
  {
    gameoflife ();
    pixels.show ();
    delay(dv);
  }

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }    
  
  Serial.print(command);
}

