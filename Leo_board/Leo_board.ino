#include <LEDMatrixDriver.hpp>
// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are arduino specific SPI pins (MOSI=DIN of the LEDMatrix and CLK) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 9;

// Define LED Matrix dimensions (0-n) - eg: 32x8 = 31x7
const int LEDMATRIX_WIDTH = 31;  
const int LEDMATRIX_HEIGHT = 7;
const int LEDMATRIX_SEGMENTS = 4;

int marq = 0;
int sprt = 0;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

void setup() {
  // initialize the display
  lmd.setEnabled(true);
  lmd.setIntensity(10);   // 0 = low, 10 = high
  Serial.begin(9600);
}

int x=0, y=0, x2=0;   // start top left

// This is the font definition. You can use http://gurgleapps.com/tools/matrix to create your own font or sprites.
// If you like the font feel free to use it. I created it myself and donate it to the public domain.
byte font[95][8] = { {0,0,0,0,0,0,0,0}, // SPACE
                     {0x10,0x18,0x18,0x18,0x18,0x00,0x18,0x18}, // EXCL
                     {0x28,0x28,0x08,0x00,0x00,0x00,0x00,0x00}, // QUOT
                     {0x00,0x0a,0x7f,0x14,0x28,0xfe,0x50,0x00}, // #
                     {0x10,0x38,0x54,0x70,0x1c,0x54,0x38,0x10}, // $
                     {0x00,0x60,0x66,0x08,0x10,0x66,0x06,0x00}, // %
                     {0,0,0,0,0,0,0,0}, // &
                     {0x00,0x10,0x18,0x18,0x08,0x00,0x00,0x00}, // '
                     {0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x04}, // (
                     {0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20}, // )
                     {0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10}, // *
                     {0x00,0x08,0x08,0x08,0x7f,0x08,0x08,0x08}, // +
                     {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08}, // COMMA
                     {0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00}, // -
                     {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06}, // DOT
                     {0x00,0x04,0x04,0x08,0x10,0x20,0x40,0x40}, // /
                     {0x00,0x38,0x44,0x4c,0x54,0x64,0x44,0x38}, // 0
                     {0x04,0x0c,0x14,0x24,0x04,0x04,0x04,0x04}, // 1
                     {0x00,0x30,0x48,0x04,0x04,0x38,0x40,0x7c}, // 2
                     {0x00,0x38,0x04,0x04,0x18,0x04,0x44,0x38}, // 3
                     {0x00,0x04,0x0c,0x14,0x24,0x7e,0x04,0x04}, // 4
                     {0x00,0x7c,0x40,0x40,0x78,0x04,0x04,0x38}, // 5
                     {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38}, // 6
                     {0x00,0x7c,0x04,0x04,0x08,0x08,0x10,0x10}, // 7
                     {0x00,0x3c,0x44,0x44,0x38,0x44,0x44,0x78}, // 8
                     {0x00,0x38,0x44,0x44,0x3c,0x04,0x04,0x78}, // 9
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00}, // :
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x08}, // ;
                     {0x00,0x10,0x20,0x40,0x80,0x40,0x20,0x10}, // <
                     {0x00,0x00,0x7e,0x00,0x00,0xfc,0x00,0x00}, // =
                     {0x00,0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // >
                     {0x00,0x38,0x44,0x04,0x08,0x10,0x00,0x10}, // ?
                     {0x00,0x30,0x48,0xba,0xba,0x84,0x78,0x00}, // @
                     {0x00,0x1c,0x22,0x42,0x42,0x7e,0x42,0x42}, // A
                     {0x00,0x78,0x44,0x44,0x78,0x44,0x44,0x7c}, // B
                     {0x00,0x3c,0x44,0x40,0x40,0x40,0x44,0x7c}, // C
                     {0x00,0x7c,0x42,0x42,0x42,0x42,0x44,0x78}, // D
                     {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x7c}, // E
                     {0x00,0x7c,0x40,0x40,0x78,0x40,0x40,0x40}, // F
                     {0x00,0x3c,0x40,0x40,0x5c,0x44,0x44,0x78}, // G
                     {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
                     {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x7e}, // I
                     {0x00,0x7e,0x02,0x02,0x02,0x02,0x04,0x38}, // J
                     {0x00,0x44,0x48,0x50,0x60,0x50,0x48,0x44}, // K
                     {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7c}, // L
                     {0x00,0x82,0xc6,0xaa,0x92,0x82,0x82,0x82}, // M
                     {0x00,0x42,0x42,0x62,0x52,0x4a,0x46,0x42}, // N
                     {0x00,0x3c,0x42,0x42,0x42,0x42,0x44,0x38}, // O
                     {0x00,0x78,0x44,0x44,0x48,0x70,0x40,0x40}, // P
                     {0x00,0x3c,0x42,0x42,0x52,0x4a,0x44,0x3a}, // Q
                     {0x00,0x78,0x44,0x44,0x78,0x50,0x48,0x44}, // R
                     {0x00,0x38,0x40,0x40,0x38,0x04,0x04,0x78}, // S
                     {0x00,0x7e,0x90,0x10,0x10,0x10,0x10,0x10}, // T
                     {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3e}, // U
                     {0x00,0x42,0x42,0x42,0x42,0x44,0x28,0x10}, // V
                     {0x80,0x82,0x82,0x92,0x92,0x92,0x94,0x78}, // W
                     {0x00,0x42,0x42,0x24,0x18,0x24,0x42,0x42}, // X
                     {0x00,0x44,0x44,0x28,0x10,0x10,0x10,0x10}, // Y
                     {0x00,0x7c,0x04,0x08,0x7c,0x20,0x40,0xfe}, // Z
                      // (the font does not contain any lower case letters. you can add your own.)
                  };    // {}, // 

// Marquee speed
const int ANIM_DELAY =80;

// Marquee text 
char text[] = "LEO";
int len = strlen(text);

char text2[] = "ISLA";
int len2 =strlen(text2);

//-----------------------  running sprite
byte a[8]={ B00011000,
            B00100100,
            B00100100,
            B00011000,
            B01111110,
            B00011000,
            B00100100,
            B01000010};

byte b[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B01111100,
            B00011000,
            B01100100,
            B00000010};

byte c[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00110100,
            B00000100};

byte d[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00011000,
            B00011000};

byte e[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00010100,
            B00010000};

byte f[8]={ B00011000,
            B00100100,
            B00100100,
            B00011000,
            B00111110,
            B01011000,
            B00010100,
            B00010100};


const int ANIM_DELAY2 = 100;

void loop() 
{
 while(marq==0){
  // ----------Draw the text to the current position
 drawString(text, len, x, 0);
   // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...
 
 // Toggle display of the new framebuffer
 lmd.display();

 // Wait to let the human read the display
 delay(ANIM_DELAY);

 // Advance to next coordinate
 if( --x < len * -8 )
   x = LEDMATRIX_WIDTH;
if (x ==31){
  marq=1;
  sprt=0;
}
}
//------------------
 while(sprt==0){
  drawSprite( (byte*)&a, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&b, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&c, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&d, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&e, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&f, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  if( x2 > LEDMATRIX_WIDTH )
    x2= -1;
Serial.println(x2); 
if (x2 ==29){
  sprt=1;
  marq=0;
  x2=31;
}
 }   
 while(marq==0){
  // ----------Draw the text to the current position
 drawString(text2, len2, x, 0);
   // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...
 // Toggle display of the new framebuffer
 lmd.display();
 // Wait to let the human read the display
 delay(ANIM_DELAY);
 // Advance to next coordinate
 if( --x < len2 * -8 )
   x = LEDMATRIX_WIDTH;
if (x ==31){
  marq=1;
  sprt=0;
}
}
 while(sprt==0){
  drawSprite( (byte*)&a, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&b, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&c, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&d, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&e, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  lmd.clear();
  drawSprite( (byte*)&f, x2++, 0, 8, 8 );
  lmd.display();
  delay(ANIM_DELAY2);

  if( x2 > LEDMATRIX_WIDTH )
    x2= -1;
Serial.println(x2); 
if (x2 ==29){
  sprt=1;
  marq=0;
  x2=31;
}
 }   
}


/**
 * This function draws a string of the given length to the given position.
 */
void drawString(char* text, int len, int x, int y )
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > LEDMATRIX_WIDTH )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite( font[c], x + idx * 8, y, 8, 8 );
  }
}

/**
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite( byte* sprite, int x2, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;
  
  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(x2 + ix, y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}

