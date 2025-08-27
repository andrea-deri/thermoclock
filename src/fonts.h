#ifndef TERMOWATCH_FONTS

#include <U8x8lib.h>

const uint8_t sevensegments[869] U8X8_FONT_SECTION("sevensegments") = 
/* header      */ "\100\133\2\2"
/* zero        */ "\0\0\176\375\3\3\3\3\3\3\3\3\375\176\0\0\0\0\176\277\300\300\300\300\300\300\300\300\277\176\0\0"
/* one         */ "\0\0\0\0\0\0\0\0\0\0\0\0\376\174\0\0\0\0\0\0\0\0\0\0\0\0\0\0\177\76\0\0"
/* two         */ "\0\0\0\201\203\203\203\203\203\203\203\203\375\176\0\0\0\0\177\276\301\301\301\301\301\301\301\301\200\0\0\0"
/* three       */ "\0\0\0\201\203\203\203\203\203\203\203\203\375\176\0\0\0\0\0\200\301\301\301\301\301\301\301\301\277\176\0\0"
/* four        */ "\0\0\176\374\200\200\200\200\200\200\200\200\374\176\0\0\0\0\0\0\1\1\1\1\1\1\1\1\77\176\0\0"
/* five        */ "\0\0\376\175\203\203\203\203\203\203\203\203\1\0\0\0\0\0\0\201\301\301\301\301\301\301\301\301\277\176\0\0"
/* six         */ "\0\0\176\375\203\203\203\203\203\203\203\203\1\0\0\0\0\0\176\277\301\301\301\301\301\301\301\301\277\176\0\0"
/* seven       */ "\0\0\0\1\3\3\3\3\3\3\3\3\375\176\0\0\0\0\0\0\0\0\0\0\0\0\0\0\77\176\0\0"
/* eight       */ "\0\0\176\375\203\203\203\203\203\203\203\203\375\176\0\0\0\0\176\277\301\301\301\301\301\301\301\301\277\176\0\0"
/* nine        */ "\0\0\176\375\203\203\203\203\203\203\203\203\375\176\0\0\0\0\0\200\301\301\301\301\301\301\301\301\277\176\0\0"
/* a           */ "\0\0\0\0\0\0\60\60\60\60\300\300\0\0\0\0\0\0\0\0\70\70\306\306\306\306\71\71\0\0\0\0"
/* c           */ "\0\0\0\0\300\340\60\60\60\60\20\0\0\0\0\0\0\0\0\0\77\177\300\300\300\300\200\0\0\0\0\0"
/* d           */ "\0\0\0\0\0\0\0\0\0\0\300\340\0\0\0\0\0\0\0\0\174\272\306\306\306\306\277\171\0\0\0\0"
/* e           */ "\0\0\0\0\340\320\60\60\60\60\320\340\0\0\0\0\0\0\0\0\171\277\306\306\306\306\205\3\0\0\0\0"
/* h           */ "\0\0\0\0\340\300\0\0\0\0\0\0\0\0\0\0\0\0\0\0\171\77\6\6\6\6\72\174\0\0\0\0"
/* l           */ "\0\0\0\0\340\300\0\0\0\0\0\0\0\0\0\0\0\0\0\0\171\277\300\300\300\300\200\0\0\0\0\0"
/* n           */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\74\172\6\6\6\6\172\74\0\0\0\0"
/* s           */ "\0\0\0\0\340\320\60\60\60\60\20\0\0\0\0\0\0\0\0\0\3\205\306\306\306\306\272\174\0\0\0\0"
/* t           */ "\0\0\0\0\340\300\0\0\0\0\0\0\0\0\0\0\0\0\0\0\171\277\306\306\306\306\202\0\0\0\0\0"
/* y           */ "\0\0\0\0\340\300\0\0\0\0\300\340\0\0\0\0\0\0\0\0\3\205\306\306\306\306\277\171\0\0\0\0"
/* colon       */ "\0\0\0\0\0\0\0\60\60\0\0\0\0\0\0\0\0\0\0\0\0\0\0\14\14\0\0\0\0\0\0\0"
/* minus_sign  */ "\0\0\0\0\200\200\200\200\200\200\200\200\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\0\0\0\0"
/* percentage  */ "\0\0\0\0\140\140\0\0\0\0\340\340\0\0\0\0\0\0\0\0\374\172\6\6\6\6\145\143\0\0\0\0"
/* dot         */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\300\300\0\0\0\0\0\0\0"
/* empty       */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
/* left-arrow  */ "\0\0\200\300\340\360\220\200\200\200\200\200\200\200\0\0\0\0\1\3\7\17\11\1\1\1\1\1\1\1\0\0"
/* right-arrow */ "\0\0\200\200\200\200\200\200\200\220\360\340\300\200\0\0\0\0\1\1\1\1\1\1\1\11\17\7\3\1\0\0"
;

#define SEVENSEGMENTS_FONT_START_OFFSET 64

#define SEVENSEGMENTS_FONT_ZERO SEVENSEGMENTS_FONT_START_OFFSET + 0
#define SEVENSEGMENTS_FONT_ONE SEVENSEGMENTS_FONT_START_OFFSET + 1
#define SEVENSEGMENTS_FONT_TWO SEVENSEGMENTS_FONT_START_OFFSET + 2
#define SEVENSEGMENTS_FONT_THREE SEVENSEGMENTS_FONT_START_OFFSET + 3
#define SEVENSEGMENTS_FONT_FOUR SEVENSEGMENTS_FONT_START_OFFSET + 4
#define SEVENSEGMENTS_FONT_FIVE SEVENSEGMENTS_FONT_START_OFFSET + 5
#define SEVENSEGMENTS_FONT_SIX SEVENSEGMENTS_FONT_START_OFFSET + 6
#define SEVENSEGMENTS_FONT_SEVEN SEVENSEGMENTS_FONT_START_OFFSET + 7
#define SEVENSEGMENTS_FONT_EIGHT SEVENSEGMENTS_FONT_START_OFFSET + 8
#define SEVENSEGMENTS_FONT_NINE SEVENSEGMENTS_FONT_START_OFFSET + 9

#define SEVENSEGMENTS_FONT_A SEVENSEGMENTS_FONT_START_OFFSET + 10
#define SEVENSEGMENTS_FONT_C SEVENSEGMENTS_FONT_START_OFFSET + 11
#define SEVENSEGMENTS_FONT_D SEVENSEGMENTS_FONT_START_OFFSET + 12
#define SEVENSEGMENTS_FONT_E SEVENSEGMENTS_FONT_START_OFFSET + 13
#define SEVENSEGMENTS_FONT_H SEVENSEGMENTS_FONT_START_OFFSET + 14
#define SEVENSEGMENTS_FONT_L SEVENSEGMENTS_FONT_START_OFFSET + 15
#define SEVENSEGMENTS_FONT_N SEVENSEGMENTS_FONT_START_OFFSET + 16
#define SEVENSEGMENTS_FONT_S SEVENSEGMENTS_FONT_START_OFFSET + 17
#define SEVENSEGMENTS_FONT_T SEVENSEGMENTS_FONT_START_OFFSET + 18
#define SEVENSEGMENTS_FONT_Y SEVENSEGMENTS_FONT_START_OFFSET + 19

#define SEVENSEGMENTS_FONT_COLON SEVENSEGMENTS_FONT_START_OFFSET + 20
#define SEVENSEGMENTS_FONT_MINUS_SIGN SEVENSEGMENTS_FONT_START_OFFSET + 21
#define SEVENSEGMENTS_FONT_PERCENTAGE SEVENSEGMENTS_FONT_START_OFFSET + 22
#define SEVENSEGMENTS_FONT_DOT SEVENSEGMENTS_FONT_START_OFFSET + 23
#define SEVENSEGMENTS_FONT_SPACE SEVENSEGMENTS_FONT_START_OFFSET + 24
#define SEVENSEGMENTS_FONT_LEFTARROW SEVENSEGMENTS_FONT_START_OFFSET + 25
#define SEVENSEGMENTS_FONT_RIGHTARROW SEVENSEGMENTS_FONT_START_OFFSET + 26



/*
The method permits to retrieve the offset related to the glyph
included in the font charset. 
*/
int getGlyphOffset(uint8_t character) {

  // Setting a default offset, in order to print placeholder
  // character if not included in charset.  
  int offset = SEVENSEGMENTS_FONT_MINUS_SIGN;
  
  // Passed character is an integer: convert as digit glyph
  if (character >= 48 && character <= 57) {
    offset = SEVENSEGMENTS_FONT_START_OFFSET + (character - 48); 
  } 
  
  // Passed character is not an integer: convert as other glyph, if possible
  else {
    switch (character) {

      // ' ' (space) character 
      case 32:
        offset = SEVENSEGMENTS_FONT_SPACE;
        break;

      // '%' (percentage) character
      case 37:
        offset = SEVENSEGMENTS_FONT_PERCENTAGE;
        break;

      // '.' (dot) character
      case 46:
        offset = SEVENSEGMENTS_FONT_DOT;
        break;

      // ':' (colon) character
      case 58:
        offset = SEVENSEGMENTS_FONT_COLON;
        break;

      // 'A' character, either uppercase and lowercase
      case 65:
      case 97:
        offset = SEVENSEGMENTS_FONT_A;
        break;

      // 'C' character, either uppercase and lowercase
      case 67:
      case 99:
        offset = SEVENSEGMENTS_FONT_C;
        break;

      // 'D' character, either uppercase and lowercase
      case 68:
      case 100:
        offset = SEVENSEGMENTS_FONT_D;
        break;

      // 'E' character, either uppercase and lowercase
      case 69:
      case 101:
        offset = SEVENSEGMENTS_FONT_E;
        break;

      // 'H' character, either uppercase and lowercase      
      case 72:
      case 104:
        offset = SEVENSEGMENTS_FONT_H;
        break;

      // 'L' character, either uppercase and lowercase      
      case 76:
      case 108:
        offset = SEVENSEGMENTS_FONT_L;
        break;

      // 'N' character, either uppercase and lowercase
      case 78:
      case 110:
        offset = SEVENSEGMENTS_FONT_N;
        break;

      // 'S' character, either uppercase and lowercase
      case 83:
      case 115:
        offset = SEVENSEGMENTS_FONT_S;
        break;

      // 'T' character, either uppercase and lowercase
      case 84:
      case 116:
        offset = SEVENSEGMENTS_FONT_T;
        break;

      // 'Y' character, either uppercase and lowercase
      case 89:
      case 121:
        offset = SEVENSEGMENTS_FONT_Y;
        break;

      // '{' character converted as left arrow glyph
      case 123:
        offset = SEVENSEGMENTS_FONT_LEFTARROW;
        break;

      // '}' character converted as right arrow glyph        
      case 125:
        offset = SEVENSEGMENTS_FONT_RIGHTARROW;
        break;
    }
  }

  return offset;
}


/*
The method permits to draw a single glyph in standard size.
*/
void drawGlyph(U8X8* u8x8, int x, int y, byte character) {
  u8x8->drawGlyph(x, y, getGlyphOffset(character));
}


/*
The method permits to draw a single glyph enlarged in 1x2 size.
*/
void draw1x2Glyph(U8X8* u8x8, int x, int y, byte character) {
  u8x8->draw1x2Glyph(x, y, getGlyphOffset(character));
}


/*
The method permits to draw a string in standard sized font, using
x-axis and y-axis offset for positioning characters.
*/
void printInMediumFont(U8X8* u8x8, int x, int y, char* value) {

  int arraySize = strlen(value);
  for (int i = 0; i < arraySize; i ++) {
    drawGlyph(u8x8, x + (2 * i), y, value[i]);
  }
}


/*
The method permits to draw a string in enlarged-sized font, using
x-axis and y-axis offset for positioning characters.
*/
void printInLargeFont(U8X8* u8x8, int x, int y, char* value) {

  int arraySize = strlen(value);
  for (int i = 0; i < arraySize; i ++) {
    draw1x2Glyph(u8x8, x + (2 * i), y, value[i]);
  }
}

#endif
