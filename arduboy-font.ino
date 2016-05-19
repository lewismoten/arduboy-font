#include "Arduboy.h"

Arduboy arduboy;

uint8_t value = 0xEC;
bool atFirstHex = true;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(8);
  redraw();
}

void loop() {

  // mocking debounce...
  if(!arduboy.nextFrame()) {
    return;
  }

  uint8_t buttons = arduboy.getInput();

  // assume only 1 button pressed
  switch(buttons) {

    case LEFT_BUTTON:
      atFirstHex = true;
      break;
    case RIGHT_BUTTON:
      atFirstHex = false;
      break;
    case DOWN_BUTTON:
    case UP_BUTTON:
      change(buttons == UP_BUTTON);
      break;
    default:
      return;
  }

  redraw();
}

void change(bool isAdding) {
  int8_t delta = atFirstHex ? 0x10 : 0x1;
  if(!isAdding) {
    delta *= -1;
  }
  value += delta;
}

void redraw() {

  // Play with these values to scale and position input/output
  
  uint16_t outputX = 78;
  uint16_t outputY = 11;
  uint8_t outputSize = 6;

  uint16_t inputX = 11;
  uint16_t inputY = 18;
  uint8_t inputSize = 4;

  uint8_t glyphWidth = 6;
  uint8_t glyphHeight = 8;

  arduboy.clear();

  // Background
  arduboy.fillRoundRect(0, 10, 128, 54, 8, WHITE);

  // Header
  arduboy.setCursor(0, 0);
  arduboy.setTextWrap(true);
  arduboy.setTextSize(1);
  arduboy.print(" Arduboy Font: ");
  arduboy.write(value);
  arduboy.write(value);
  arduboy.write(value);

  // Hex box
  arduboy.drawRoundRect(inputX, inputY, 8 + (2 * glyphWidth * inputSize), 6 + (glyphHeight * inputSize), 4, BLACK);
  arduboy.fillRoundRect(inputX + 2, inputY + 2, 4 + (2 * glyphWidth * inputSize), 2 + (glyphHeight * inputSize), 4, BLACK);
  
  // Selected Hex
  int16_t arrowX = atFirstHex ? inputX + 5 : inputX + 5 + (inputSize * glyphWidth);
  arduboy.fillRoundRect(arrowX, inputY + (glyphHeight * inputSize) + 8, (inputSize * (glyphWidth - 1)), inputSize, 2, BLACK);

  // Hex
  char hex[2];
  sprintf(hex, "%02X", value);

  arduboy.setTextSize(inputSize);
  arduboy.setCursor(inputX + 4, inputY + 3);
  arduboy.print(hex);
  
  // Draw out the character
  arduboy.drawRect(outputX, outputY, (outputSize * glyphWidth) + 4, (outputSize * glyphHeight) + 4, BLACK);
  arduboy.setTextSize(outputSize);
  arduboy.setCursor(outputX + 2, outputY + 2);
  arduboy.write(value);

  // Grid!
  for(uint8_t x = 0; x < glyphWidth; x++) {
    arduboy.drawFastVLine(outputX + 2 + (x * glyphWidth), outputY + 2, (outputSize * glyphHeight), BLACK);
  }
  for(uint8_t y = 0; y < glyphHeight; y++) {
    arduboy.drawFastHLine(outputX + 2, outputY + 2 + (y * outputSize), (glyphWidth * outputSize), BLACK);
  }

  arduboy.display();
}

