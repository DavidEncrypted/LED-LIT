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
int dv = 20; // Delayvalue in miliseconds.
int basedark[3] = {140, 70, 10}; //The darker color of the pixels
int baselight[3] = {255, 140, 20}; //The light color of the pixels
//The array holding the pixel values for the rain
int pixelfield[STRIPS][PPS];

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
}

void lightsOn ()
{
  //This function makes a rain like effect. The values of the pixels are randomly chosen and after each loop
  //shifted down one in the matrix that saves the values. 
  int randvalue;

  //In these loops we move the entire matrix contents one down and fill the line at the top, which has 
  //opened up, with new pixel values
  for ( int i = STRIPS - 1; i >= 0; i-- ){ //Moving up through the matrix
    for ( int j = 0; j < PPS; j++ ){
      pixelfield[i][j] = pixelfield[i-1][j];

      if(i == 0){
        randvalue = random(0,10);
        if(randvalue == 8){
          pixelfield[i][j] = 1; //Half bright pixel
        }else if(randvalue == 9){
          pixelfield[i][j] = 2; //Full brightness pixel
        }else{
          pixelfield[i][j] = 0; //Dark pixel
        }
      }
    }
  }

  /*
  
  //checking the values of the pixelfield
  Serial.print("[\n");
  for(int i = 0; i < STRIPS; i++){
    for(int j = 0; j < STRIPS; j++){
      Serial.print(pixelfield[i][j]);
      Serial.print(" ");
    }
    Serial.print("|\n");
  }
  Serial.print("]\n\n");
  */

  //Here we set the values of the pixels
  for ( int i = 0; i < STRIPS; i++ ){
    for ( int j = 0; j < PPS; j++ ){
      if(pixelfield[i][j] == 0){
        if(j % 2 == 1){
          pixels.setPixelColor (j*PPS + i, pixels.Color (0, 0, 0) ); 
        }else{
          pixels.setPixelColor (j*PPS + 6 - i, pixels.Color (0, 0, 0) ); 
        }
      }else if(pixelfield[i][j] == 1){
        if(j % 2 == 1){
          pixels.setPixelColor ( j*PPS + i, pixels.Color (baselight[2], baselight[0], baselight[1]) );
        }else{
          pixels.setPixelColor ( j*PPS + 6 - i, pixels.Color (baselight[2], baselight[0], baselight[1]) );
        }
      }else if(pixelfield[i][j] == 2){
        if(j % 2 == 1){
          pixels.setPixelColor ( j*PPS + i, pixels.Color (basedark[2], basedark[0], basedark[1]) );
        }else{
          pixels.setPixelColor ( j*PPS + 6 - i, pixels.Color (basedark[2], basedark[0], basedark[1]) );
        }
      }
      
    }
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

  if ( command == "off" )
  {
    lightsOff ();
    pixels.show ();
  }  

  if ( command == "1" )
  {
    lightsOn ();
    pixels.show ();
  }  
}

