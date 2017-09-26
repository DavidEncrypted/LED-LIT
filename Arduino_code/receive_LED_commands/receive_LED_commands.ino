// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      50

#define BRIGHTNESS 50


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 1; // delay in ms
int rainbowcount = 0;

String command;
bool rainbowup = true;

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

void loop() {
  
  if(Serial.available() > 0)
    {
        command = Serial.readStringUntil('\n');
        //x = Serial.parseInt();
    }
  
  /*
  while(!Serial.available()) {}
  // serial read section
  while (Serial.available())
  {
    if (Serial.available() >0)
    {
      command = Serial.readStringUntil('\n');
    }
  }
  */
  Serial.print(rainbowcount);
  Serial.println(command);
  //Serial.println(count);
    
  if(command=="1"){ // turn light on
    for(int i=0;i<NUMPIXELS;i++){

      // pixels.Color takes BRG values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(50,50,50));
      
      pixels.show(); // This sends the updated pixel color to the hardware.
  
      delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }
  else if(command=="rainbow"){// run rainbow programme
      
     for(int i=0;i<NUMPIXELS;i++){

      pixels.setPixelColor(i, pixels.Color(100,rainbowcount,0)); 
  
      pixels.show(); // This sends the updated pixel color to the hardware.
  
      delay(delayval); // Delay for a period of time (in milliseconds).

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

  if(rainbowcount > 254){
    rainbowup = false;
  }
  else if (rainbowcount < 1){
    rainbowup = true;
  }

  if (rainbowup){
    rainbowcount++;
  }else{
    rainbowcount--;
  }
  
}
