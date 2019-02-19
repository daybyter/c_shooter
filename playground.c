/*****************************************************************
 * Project: c64 shooter
 *
 * $Revision$
 * $Date$
 * $Author$
 *****************************************************************
 *
 * Copyright(c) 2016 Andreas Rueckert <arueckert67@t-online.de>
 *
 * This file may be licensed under the terms of of the
 * GNU General Public License Version 2 (the ``GPL'').
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the GPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the GPL along with this
 * program. If not, go to http://www.gnu.org/licenses/gpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *****************************************************************/

#include <c64.h>
#include "playground.h"


/**
 * Load the playground data for the next level.
 *
 * @param level The next level to play.
 *
 * @return true, if the level was successfully loaded, false otherwise. 
 */
bool playgroundLoad( unsigned char level) {
  return true;
}

/**
 * Initialize the playground for the next level.
 *
 * @param level The next level to play.
 *
 * @return true, if the level was successfully initialized. false otherwise.
 */
bool playgroundInitialize( unsigned char level) {
  return true;
}

/**
 * Scroll the playground by 1 pixel. 
 * @see http://www.zimmers.net/cbmpics/cbm/c64/vic-ii.txt
 */
void playgroundScroll( void) {

  /*  unsigned char cur_y = VIC.ctrl1 & 7;
  ++cur_y;
  cur_y &= 7;
  VIC.ctrl1 = (VIC.ctrl1 & (unsigned char)248) | cur_y; */
  /*
  if( cur_y == 0) {  // Scroll the screen by 1 character line.
    unsigned char *src = videoram;
    unsigned char *dest = videoram + 40;

    while( dest < videoram + 1000 ) {
      *dest++ = *src++;
    }
    } */

   /* Write a new line at the beginning of the video ram. */
  unsigned char *cur_ram;
  for( cur_ram = (unsigned char *)0xe400; cur_ram < (unsigned char *)0xe428; ++cur_ram) {
    *cur_ram = 47;
  }
  /* Set the color ram to red. */
  for( cur_ram = (unsigned char *)0xd800; cur_ram < (unsigned char *)0xd828; ++cur_ram) {
    *cur_ram = 2;
  }
}

/* 
 * Create one screenline of the playground. 
 *
 * @param playgroundTiles Entire list of tiles of the playground.
 * @param linenr Line number of the rendered playground to create.
 * @param availableTile List of tiles used in the playground.
 * @param videoram Pointer of the videoram, where the rendered playground is created.
 */
void playgroundWriteLine(unsigned char *playgroundTiles, int linenr, unsigned char *availableTiles, unsigned char *videoram) {
  char tileLine = linenr % TILE_HEIGHT;
  unsigned char *currentTile = &playgroundTiles[PLAYGROUND_WIDTH * (linenr / TILE_HEIGHT)];
  int tileIndex;

  for( tileIndex = 0; tileIndex < PLAYGROUND_WIDTH; ++tileIndex) {
    playgroundWriteTile( availableTiles[ currentTile[ tileIndex]], tileLine, videoram);
    videoram += TILE_WIDTH;
  }
}


/** 
 * Write a line of a tile to the videoram. 
 *
 * @param tile Pointer to the 6 characters of the tile to write.
 * @param tileLine Line of the tile to be written (0..TILE_HEIGHT-1).
 * @param videoram Pointer into the videoram position, where the tile should be written.
 */
void playgroundWriteTile( unsigned char *tile, char tileLine, unsigned char *videoram) {
  short index;
  tile += tileLine * TILE_WIDTH;  /* Get the position within the tile. */

  for( index = 0; index < TILE_WIDTH; index++) {
    videoram[index] = tile[index];
  }
}
