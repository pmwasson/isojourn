#include <Arduboy2.h>
#include <ArduboyFX.h>
#include "isojourn.h"

#define FX_DATA_PAGE 0xFF0A
#define FRAME_RATE 60
#define ISOWIDTH 40
#define ISOHEIGHT 20

Arduboy2 arduboy;

uint8_t playerFrame = 0;

uint8_t mapTile[8][4] = {
  {TILE_grass,    TILE_water,   TILE_grass,   TILE_grass},
  {       TILE_grass,    TILE_water,   TILE_weeds,      TILE_grass2},
  {TILE_rock1,   TILE_grass,   TILE_pondrock,   TILE_treePine},
  {       TILE_logL,    TILE_grass,   TILE_water,   TILE_treePine},
  {TILE_grass2,   TILE_campfire,   TILE_grass2,  TILE_reed},
  {       TILE_grass,    TILE_grass/*p*/,   TILE_grass,      TILE_water},
  {TILE_treePine,    TILE_grass,   TILE_grass,   TILE_grass},
  {       TILE_treePine, TILE_grass,   TILE_treePine,      TILE_grass2},
};

int8_t isoX(int8_t x, int8_t y) {
  return x*ISOWIDTH+(y%2)*(ISOWIDTH/2) - 20;
}

int8_t isoY(int8_t y) {
  return y*(ISOHEIGHT/2) - 46;
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(FRAME_RATE);
  FX::disableOLED(); // OLED must be disabled before cart can be used. OLED display should only be enabled prior updating the display.
  FX::begin(FX_DATA_PAGE); // wakeup external flash chip, initialize datapage, detect presence of external flash chip
}

void loop() {
  if (!arduboy.nextFrame()) return;
  arduboy.pollButtons();

  // 8 way directions
  if (arduboy.justPressed(DOWN_BUTTON)) {
    playerFrame = 0;
    if (arduboy.pressed(RIGHT_BUTTON)) {
      playerFrame = 1;
    }
    else if (arduboy.pressed(LEFT_BUTTON)) {
      playerFrame = 7;
    }      
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    playerFrame = 4;
    if (arduboy.pressed(RIGHT_BUTTON)) {
      playerFrame = 3;
    }      
    else if (arduboy.pressed(LEFT_BUTTON)) {
      playerFrame = 5;
    }      
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)) {
    playerFrame = 2;
    if (arduboy.pressed(UP_BUTTON)) {
      playerFrame = 3;
    }      
    else if (arduboy.pressed(DOWN_BUTTON)) {
      playerFrame = 1;
    }      
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    playerFrame = 6;
    if (arduboy.pressed(UP_BUTTON)) {
      playerFrame = 5;
    }      
    else if (arduboy.pressed(DOWN_BUTTON)) {
      playerFrame = 7;
    }      
  }

  // Test
  arduboy.setCursor(0,0);
  arduboy.print("If you can read\nthis, the tiles\ndidn't draw.");

  // Fill screen
  for (int8_t y = 0; y < 8; y++) {
    int8_t iy = isoY(y);
    for (int8_t x = 0; x < 4; x++) {
      uint8_t tile = mapTile[y][x];
      uint8_t frame = 0;
      int8_t ix = isoX(x,y);
      
      if ((tile == TILE_water)  ||
          (tile == TILE_reed)   ||
          (tile == TILE_pondrock)) {
          frame = (((arduboy.frameCount) >> 4)+ (x*2) + (y%2)) & 0x7;
         if (frame > 3) {
            frame = 7 - frame;
         }
      }
      else if (tile == TILE_campfire) {
        frame = ((arduboy.frameCount) >> 5) % 3;
      }

      FX::drawBitmap(ix,iy,TILE_START,tile+frame,dbmMasked);

      // draw player on top of map
      if ((x==1) && (y==5)) {
        if ((playerFrame == 0) &&
            ((((arduboy.frameCount) >> 3) & 0x3f) == 0)) { 
          FX::drawBitmap(ix,iy,TILE_START,TILE_boy+1,dbmMasked);
        }
        else {
          FX::drawBitmap(ix,iy,TILE_START,TILE_player+playerFrame,dbmMasked);
        }
      }
    }
  }


  FX::enableOLED();// only enable OLED for updating the display
  arduboy.display(CLEAR_BUFFER);
  FX::disableOLED();// disable so flash cart can be used at any time
}
