// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      49

#define BRIGHTNESS 50


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 10; // delay in ms
int rainbowcount = 0;

String command;
bool rainbowup = true;


// uint8_t is the same as byte
// uint16_t is unsigned int
// I just noticed that I mixed these in this sketch, sorry

const uint8_t lights[360]={
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

const uint8_t HSVlights[61] = 
{0, 4, 8, 13, 17, 21, 25, 30, 34, 38, 42, 47, 51, 55, 59, 64, 68, 72, 76,
81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 127, 132, 136, 140, 144,
149, 153, 157, 161, 166, 170, 174, 178, 183, 187, 191, 195, 200, 204, 208,
212, 217, 221, 225, 229, 234, 238, 242, 246, 251, 255};

const uint8_t HSVpower[121] = 
{0, 2, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 34, 36, 38, 40,
42, 45, 47, 49, 51, 53, 55, 57, 59, 62, 64, 66, 68, 70, 72, 74, 76, 79, 81, 
83, 85, 87, 89, 91, 93, 96, 98, 100, 102, 104, 106, 108, 110, 113, 115, 117, 
119, 121, 123, 125, 127, 130, 132, 134, 136, 138, 140, 142, 144, 147, 149, 
151, 153, 155, 157, 159, 161, 164, 166, 168, 170, 172, 174, 176, 178, 181, 
183, 185, 187, 189, 191, 193, 195, 198, 200, 202, 204, 206, 208, 210, 212, 
215, 217, 219, 221, 223, 225, 227, 229, 232, 234, 236, 238, 240, 242, 244, 
246, 249, 251, 253, 255};

uint8_t outputPins[6] = {3, 5, 6, 9, 10, 11}; // PWM pins

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//#if defined (__AVR_ATtiny85__)
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//#endif
  // End of trinket special code

  //initialize the serial connection
  Serial.begin(9600);
  
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
}

// setRGBpoint (0, ...) for pins 3, 5, 6; setRGBpoint (1, ...) for pins 9, 10, 11.
// See array above
void setRGBpoint(byte LED, uint8_t red, uint8_t green, uint8_t blue){
  for(int i=0;i<NUMPIXELS;i++){

      pixels.setPixelColor(i, pixels.Color(red,green,blue));
  
      pixels.show(); // This sends the updated pixel color to the hardware.
      } 
}

// the real HSV rainbow
void trueHSV(byte LED, int angle)
{
  byte red, green, blue;

  if (angle<60) {red = 255; green = HSVlights[angle]; blue = 0;} else
  if (angle<120) {red = HSVlights[120-angle]; green = 255; blue = 0;} else 
  if (angle<180) {red = 0, green = 255; blue = HSVlights[angle-120];} else 
  if (angle<240) {red = 0, green = HSVlights[240-angle]; blue = 255;} else 
  if (angle<300) {red = HSVlights[angle-240], green = 0; blue = 255;} else 
                 {red = 255, green = 0; blue = HSVlights[360-angle];} 
  setRGBpoint(LED, red, green, blue);
}

// the 'power-conscious' HSV rainbow
void powerHSV(byte LED, int angle)
{
  byte red, green, blue;
  if (angle<120) {red = HSVpower[120-angle]; green = HSVpower[angle]; blue = 0;} else
  if (angle<240) {red = 0;  green = HSVpower[240-angle]; blue = HSVpower[angle-120];} else
                 {red = HSVpower[angle-240]; green = 0; blue = HSVpower[360-angle];}
    setRGBpoint(LED, red, green, blue);
}

// sine wave rainbow
void sineLED(byte LED, int angle)
{
  setRGBpoint(LED, lights[(angle+120)%360], lights[angle],  lights[(angle+240)%360]);
}
/*
void setup() {
}

void loop() {
  
for (int k=0; k<360; k++)
{

// uncomment the mode (or modes) you need below.
// with all six PWM outputs connected you may use 2 modes, change one 0 to 1.

trueHSV(0, k);

//powerHSV(0, k);

//sineLED(0, k);

delay(30);
}
}
*/
void loop() {
  
  if(Serial.available() > 0)
    {
        command = Serial.readStringUntil('\n');
        //x = Serial.parseInt();
    }
    
  if(command=="1"){ // turn light on
    for(int i=0;i<NUMPIXELS;i++){

      // pixels.Color takes BRG values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255,255,255));
      
      pixels.show(); // This sends the updated pixel color to the hardware.
  
      delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }
  else if(command=="trueHSVrainbow"){// run rainbow programme
    for (int k=0; k<360; k++){
      // uncomment the mode (or modes) you need below.
      // with all six PWM outputs connected you may use 2 modes, change one 0 to 1.
      
      trueHSV(0, k);

      delay(0);
    }
  }
  else if(command=="powerHSVrainbow"){// run rainbow programme
    for (int k=0; k<360; k++){
      // uncomment the mode (or modes) you need below.
      // with all six PWM outputs connected you may use 2 modes, change one 0 to 1.
      
      powerHSV(0, k);
      
      
      delay(1);
    }
  }
  else if(command=="sinerainbow"){// run rainbow programme
    for (int k=0; k<360; k++){
      // uncomment the mode (or modes) you need below.
      // with all six PWM outputs connected you may use 2 modes, change one 0 to 1.
      
      sineLED(0, k);
      
      delay(1);
    }
  }
  else if(command=="off"){// turn light off
     for(int i=0;i<NUMPIXELS;i++){

      // turns LEDs off
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 
  
      pixels.show(); // This sends the updated pixel color to the hardware.
  
      //delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }
  
}
