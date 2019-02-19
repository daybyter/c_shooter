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


#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>
#include "video.h"

/**
 * Array to hold the sprite mappings.
 */
unsigned char _spriteSlots[NUM_SPRITE_SLOTS];

/**
 * Load sprite data from a file into a given memory address and set the VIC data accordingly.
 *
 * @param filename Name of the file holding the sprite data.
 * @param address The memory address of the loaded sprite 
 *                (address is not(!) checked, if it's in the correct bank!).
 * @param isMultiColor Flag to indicate, if the sprite is multicolor.
 * @param spriteNumber The number of the sprite in the VIC.
 * 
 * @return true if the sprite was successfully loaded, false otherwise.
 */
int spriteLoad( char *filename, 
		unsigned char *address, 
		char isMultiColor, 
		unsigned char spriteNumber) {

  FILE *fhandle;  /* Handle for the file holding the sprite data. */
  unsigned char *bankStart;  /* Start of the current VIC bank. */
  unsigned char *spritePointer;  /* The sprite pointer of the passed sprite. */
  int numRead;  /* Number of read bytes. */

  fhandle = fopen( filename, "r");  /* Open the file for reading. */

  if( ! fhandle) { return false; }  /* Could not open the file. */

  numRead = fread( address, sizeof( unsigned char), 63, fhandle);

  fclose( fhandle);

  if( numRead != 63) { return false; }  /* Error reading the sprite. */

  spriteEnableMulticolor(isMultiColor, spriteNumber);

  /* Set the address of the sprite in the VIC's sprite pointer. */

  spritePointer = spriteGetPointer( spriteNumber);

#ifdef DEBUG
  printf("Spritepointer for sprite %d is %d\n", spriteNumber, spritePointer);
#endif

  /* Sprite index is (address - bankstart) / 64 */

#ifdef ARGUMENT_CHECK
  /* Check, if the address is valid. */
  if( ( (unsigned int)address & 63) != 0) { return false; } /* Not a 64 byte block. */
  if( ( address < bankStart) || ( ( address - bankStart) > 16383)) { return false; }  /* Not in the current VIC bank. */
#endif

  *spritePointer = (unsigned char)((address - bankStart) >> 6);

  /* Set the sprite pointer also for the 2nd video buffer. */
  *(spritePointer+1024) = (unsigned char)((address - bankStart) >> 6);
  
#ifdef DEBUG
  printf("Schreibe Blocknr %d in spritePointer", (unsigned char)((address - bankStart) >> 6));
#endif

  return true;
}


/**
 * Compute the address, where address of the block of a given sprite is stored.
 *
 * @param spriteNumber The number of the sprite (0..7).
 *
 * @return The address, where the pointer to the sprite is stored.
 */
unsigned char *spriteGetPointer( unsigned char spriteNumber) {

  /* Bank is selected in bit 0 and 1 of the cia2. Actual bank number is 3 - those 2 bits. */
  unsigned char *bankStart = (unsigned char *)(((unsigned int)getVideoBank()) << 14);

  /* Pointers are in the last 8 Byte of the screen ram block. */
  return bankStart + getVideoRamOffset() + 1016 + spriteNumber;
}

/**
 * Get a memory slot for a sprite to load.
 *
 * @return A pointer to the sprite memory or
 *         NULL, if there are no free slots.
 */
unsigned char *spriteGetMemorySlot( void) {
  
  int i;
  
  for( i = 0; i < NUM_SPRITE_SLOTS; ++i) {
    if( _spriteSlots[i] == 0xff) {
      _spriteSlots[i] = 0;
      return (unsigned char *)(SPRITE_MEM_START + ( i * 64));
    }
   }

  return NULL;  /* No free slot found. */
}

/**
 * Init the sprite system.
 */
void sprite_init( void) {

  // Set all the sprite slots to empty.
  int i;
  for( i = 0; i < NUM_SPRITE_SLOTS; ++i) {
    _spriteSlots[i] = 0xff;
  }
}
