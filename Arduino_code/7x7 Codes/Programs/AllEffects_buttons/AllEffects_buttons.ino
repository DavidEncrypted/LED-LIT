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
  //For the Game of Life
#define w  5 //The extra width added to the game of life world outside the matrix borders
#define h  5 //The extra height added to the game of life world outside the matrix borders
#define fillperc  40 //percentage of cells that is alive at the beginning of the game of life

int LEDMatrix[STRIPS][PPS][COLORS]; // The matrix that represents the real LED array.
int LEDArray[NUMPIXELS][COLORS]; // The LEDArray.

int light1[3] = {255, 140, 20}; //White light
int light2[3] = {0, 0, 0}; //The second light stage, this is off
int nolight[3] = {0, 0, 0}; //No light

//The pins the two buttons are connected to
const int upButtonPin = 2;   
const int downButtonPin = 3;
//Hold the status of the buttons
int upButtonState_1 = 0;   //current state
int upButtonState_0 = 0;   //previous state
int downButtonState_1 = 0;  
int downButtonState_0 = 0; 
//Whether a button was pressed, determining if a function needs to be broken
boolean pressed = false;

  //For the horizontal rainbow
int shift = 20; // Shift between layers for the horizontal rainbow

  //For the color flower pattern
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

  //For the white rain effect
int basedark[3] = {140, 70, 10}; //The darker color of the pixels
int baselight[3] = {255, 140, 20}; //The light color of the pixels
//The array holding the pixel values for the rain
int pixelfield[STRIPS][PPS];

  //For the Game of Life
int basecolor[3] = {255, 115, 20}; //The default color of the pixels
//The field on which the game of life takes place
int world[STRIPS + w*2][PPS + h* 2];
//A copy of this world used for determining the neighbours
int newworld[STRIPS + w*2][PPS + h*2];
//The section of the world which will be shown on the matrix
int pixelfield_GoL[STRIPS][PPS];

//String command; // Input.
int command = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() // This function gets called once at the beginning.
{
  // Initialize the serial connection
  Serial.begin ( 9600 );
  pixels.setBrightness ( BRIGHTNESS );
  pixels.begin (); // This initializes the NeoPixel library.
  pixels.show (); // Initialize all pixels to 'off'
  
  // initialize the pushbutton pin as an input:
  pinMode(upButtonPin, INPUT);     
  pinMode(downButtonPin, INPUT);
  
    //for the game of life
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

void diamond () {
  
  int y_down_r, y_down_l, y_up_r, y_up_l;
  
  //the first loop runs over the different frames of the animated diamond shape  
  for(int fr = 0; fr <= STRIPS; fr++){
    readButtons();
    if(pressed){
      break;
    }
    
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

void twoColorRainbow ()
{
  for ( int k = 0; k < 256; k++ )
  {
    readButtons();
    if(pressed){
      break;
    }
    
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
    delay(30);
  }
}

void colorFlower ()
{
  for ( int k = 0; k < 360; k++)
  {
    readButtons();
    if(pressed){
      break;
    }
    
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

void snakeOne ()
{
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    readButtons();
    if(pressed){
      break;
    }
    
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
    delay (300);
  }
}

void snakeTwo ()
{
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    readButtons();
    if(pressed){
      break;
    }
    
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
    delay (300);
  }
}

void snakeMoreThanTwo ()
{
  int lengthSnake = 3;
  
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    readButtons();
    if(pressed){
      break;
    }
    
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
    delay (300);
  }
}

void rainbowSnake ()
{
  int lengthSnake = 7;
  
  for ( int i = 0; i < NUMPIXELS; i++ )
  {
    readButtons();
    if(pressed){
      break;
    }
    
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
    delay (300);
  }
}

void whiterain ()
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
    delay(20);
  }
}

void gameoflife (){ //This function runs John Conway's Game of Life on the 7x7 matrix
  int nbours; //the amount of neighbours
  
  //Update the pixelfield which is going to be displayed on the matrix
  for(int i = 0; i < STRIPS; i++){
    for(int j = 0; j < PPS; j++){
      pixelfield_GoL[i][j] = world[i+w][j+h];
    }
  }
  
  //Here we show the field on the matrix. This is done before iterating to the next generation,
  //as otherwise we would never see the begin configuration
  for ( int i = 0; i < STRIPS; i++ ){
    for ( int j = 0; j < PPS; j++ ){
      if(pixelfield_GoL[i][j] == 0){ //sets the dead cells
        if(j % 2 == 1){
          pixels.setPixelColor (j*PPS + i, pixels.Color (0, 0, 0) ); 
        }else{
          pixels.setPixelColor (j*PPS + 6 - i, pixels.Color (0, 0, 0) ); 
        }
      }else if(pixelfield_GoL[i][j] == 1){ //sets the alive cells
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

void resetGoL(){
    //For the game of life
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

void readButtons(){
  //Reads the input from the buttons. This function has to be run every loop in the different
  //colour programs/functions.
  
  // read the state of the pushbutton value:
  upButtonState_1 = digitalRead(upButtonPin);
  downButtonState_1 = digitalRead(downButtonPin);
  
  //When the button is pressed (holding the button will do nothing, only pressing it)
  //change the command value and clear all the pixels
  if(upButtonState_1 == HIGH && upButtonState_0 == LOW){
    command += 1;
    lightsOff (); 
    pressed = true;
  }
  if(downButtonState_1 == HIGH && downButtonState_0 == LOW){
    command -= 1;
    lightsOff (); 
    pressed = true;
  }
  
  upButtonState_0 = upButtonState_1;
  downButtonState_0 = downButtonState_1;
}

void loop () {
  
  //if ( Serial.available() > 0 ) { command = Serial.readStringUntil('\n'); }
  
  //Serial.print(command);
  
  //Check input if it has not already happened this loop
  if(pressed == false){
    readButtons();
  }
  
  //Reset pressed
  if(pressed){
    pressed = false;
  }
  
  //Checks if the command variable doesn't go too far. If it does, moves it to the beginning or end of the list
  if(command < 0){
    command = 8;
  } else if (command > 8){
    command = 1;
  }
  
  
  if ( command == 0 ) { 
    lightsOff (); 
    resetGoL();
  } else if ( command == 1 ) { //shows a diamond pattern (for some reason only once)
    diamond (); 
  } else if ( command == 2 ){ //shows a vertical rainbow
    twoColorRainbow ();
  } else if ( command == 3 ) {// Makes what I call a "bloem" shape.
    colorFlower ();
    delay(200);
  } else if ( command == 4 ){ //The four different snake programs
    snakeOne ();
  } else if ( command == 5 ){
    snakeTwo ();
  } else if ( command == 6){
    snakeMoreThanTwo ();
  } else if ( command == 7){
    rainbowSnake ();
  } else if ( command == 8 ){ //shows the white rain pattern
    whiterain ();
    pixels.show ();
  } /*else if ( command == "5" ){ //Shows the Game of Life
    gameoflife ();
    pixels.show ();
    delay(500);
  }*/
  
}

