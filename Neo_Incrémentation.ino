#include "Adafruit_NeoTrellis.h"
#define Y_DIM 8 //number of rows of key
#define X_DIM 20  //number of columns of keys

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {
  {Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x34), Adafruit_NeoTrellis(0x36), Adafruit_NeoTrellis(0x38) },
  {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x33), Adafruit_NeoTrellis(0x35), Adafruit_NeoTrellis(0x37) },
};
//pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
// x -= y; // équivaut à l'expression x = x - y;  x : variable; y : variable ou constante
// exemple
// int x = 20;
// x -= 2; x au moins égal au max (20) - 2
// x contient 18
// resultat : x = 18
uint32_t Wheel(byte WheelPos) {
  //uint32_t Wheel(byte WheelPos) {
  // WheelPos = 255
  if (WheelPos < 85) {
    //if(WheelPos < 85) { //couleurs de 0 à 85
    return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    //return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    //} else if(WheelPos < 170) {
    WheelPos -= 85;
    //WheelPos -= 85; //couleurs de 85 à 170
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    //return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    //WheelPos -= 170; //couleurs de 170 à 255
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
    //return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
//Basically the the code breaks the possible 255 choices of the wheel input into 3 sections of 85.
//Then the first 'if' does the math to fade two of the 3 colors (red and blue) from all one to all the other over 85 steps.
//The next 85 steps fade between another pair of colors (blue and green) and the last 85 steps fade between the final pair (green and red).
//So to get rid of most of the red you would pass the wheel function values 42 to 213 instead of 0 to 255.

//define a callback for key presses
TrellisCallback Keyz(keyEvent evt) {
  
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    Serial.print(evt.bit.NUM);
    Serial.print(" ");
    Serial.println("1"); 
    
    // for(int x=1; x<5; x++){
    //  int animation = 0;
    //  int animation2 = 0;
    //  animation = evt.bit.NUM + x;
    //  animation2 = evt.bit.NUM - x;
    //  trellis.setPixelColor(animation, 0xFFFFFF);
    //  trellis.setPixelColor(animation2, 0xFFFFFF);
    //  trellis.show();
    //  delay(10);
    //  trellis.setPixelColor(animation, 0x000000);
    //  trellis.setPixelColor(animation2, 0x000000);
    //  trellis.show();
    //  delay(10); 
    // }
    
    for(int i=0; i<Y_DIM*X_DIM; i++){
      if (evt.bit.NUM == 159) {
        trellis.setPixelColor(i, 0x000000);
      }
    }
    for (int i=16; i<157; i=i+20){
      if (evt.bit.NUM == 139) {
        trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
      }
    }
  }
  
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    Serial.print(evt.bit.NUM);
    Serial.print(" ");
    Serial.println("0");
    
    for(int y=0; y<4; y++){
      for(int x=16 + y; x<160; x=x+20){
        trellis.setPixelColor(x, 0x020000); //addressed with x,y
      }
    }  
  }
   
  trellis.show();
  return 0;
}

void setup() {
  Serial.begin(9600);
  //initialise le port série qui permet à l'Arduino d'envoyer et de recevoir des informations à l'ordinateur.
  //Pour communiquer avec l'ordinateur, utiliser l'un de ces débits : 300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200
  //C'est recommandé, mais cela fonctionne aussi sans
  //while(!Serial) delay(1);
  trellis.begin();
  
  if(!trellis.begin()){
    Serial.println("failed to begin trellis");
    while(1) delay(1);
  }

  /* the array can be addressed as x,y or with the key number */
  for(int i=0; i<Y_DIM*X_DIM; i++){
      trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
      trellis.show();
      delay(2);
  }
  
  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){
      trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
      trellis.show(); //show all LEDs
      delay(2);
    }
  }
  
  for(int y=0; y<4; y++){
    for(int x=16 + y; x<160; x=x+20){
      trellis.setPixelColor(x, 0x020000); //addressed with x,y
      trellis.show(); //show all LEDs
      delay(2);
    }
  }
  
}

void loop() {
  
  char buffer1[20];
  char buffer2[20];
  int index_buffer1 = 0;
  int index_buffer2 = 0;
  static bool ligne_maxmsp1 = false;
  uint16_t pixel_on = 160;

  char buffer3[20];
  char buffer4[20];
  int index_buffer3 = 0;
  int index_buffer4 = 0;
  static bool ligne_maxmsp2 = false;
  uint32_t couleur;

  
  trellis.read();
  for(int j=0; j<Y_DIM*X_DIM; j++){
    trellis.activateKey(j, SEESAW_KEYPAD_EDGE_RISING, true);
    trellis.activateKey(j, SEESAW_KEYPAD_EDGE_FALLING, true);
    trellis.registerCallback(j, Keyz);
  }
    
  if (Serial.available() > 0 && Serial.read() == 171) { // Début d'une ligne

    ligne_maxmsp1 = true;        
  } 
  while (ligne_maxmsp1) { // Une ligne est en cours de lecture
    uint16_t ReadS = Serial.read();
    if (ReadS == 172) { // Fin d'une ligne
      ligne_maxmsp1 = false;
      buffer2[index_buffer2] = '\0';

      index_buffer2 = 0;
      pixel_on = atoi(buffer2);
    } else {

      buffer2[index_buffer2++] = ReadS;
    }
  }
  
  if (Serial.available() > 0 && Serial.read() == 175) { // Début d'une ligne

    ligne_maxmsp2 = true;        
  }
  while (ligne_maxmsp2) { // Une ligne est en cours de lecture
    uint32_t ReadS2 = Serial.read();
    if (ReadS2 == 174) { // Fin d'une ligne
      ligne_maxmsp2 = false;
      buffer4[index_buffer4] = '\0';

      index_buffer4 = 0;
      couleur = strtoul(buffer4, NULL, 16);
    } else {
      
      buffer4[index_buffer4++] = ReadS2;
    }
  }

  trellis.setPixelColor(pixel_on, couleur);
  trellis.show();
  
  delay(1);
}
