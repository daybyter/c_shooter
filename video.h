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


#ifndef _VIDEO_H
#define _VIDEO_H

#include <c64.h>  /* Get access to the VIC registers. */


/**
 * Data structure to get info on the available screen buffers.
 */
typedef struct screenInfo {
  char nScreens; /* The number of available screen buffers. */
  unsigned char **buffers;  /* Array of pointers to the buffers. */
} screenInfo;

/* Info to the current graphics mode. */
extern screenInfo *_currentScreenInfo;


/*
 * Common functions for video handling.
 */

/**
 * Get the number of the active video bank. Bank 0 (address 0) is 0, Bank 1 (address 16384) is 1 etc.
 *
 * @return The active video bank.
 */
#define getVideoBank() (3-(CIA2.pra & 3))

/**
 * Set the active video bank. 0 is Bank 0 (address 0), 1 is Bank 1 (address 16384) etc.
 *
 * @param bankNr The new video bank.
 */
#define setVideoBank(bankNr) do { \
  CIA2.pra &= 0xfc; \
  CIA2.pra |= (3-(bankNr));			\
} while(0)


/**
 * Get the video ram offset within the VIC bank. 0 = address 0. 1 = address 1024, etc.
 *
 * @return The index of the video ram within the VIC bank.
 */
#define getVideoRamOffsetIndex() (VIC.addr >> 4)


/**
 * Set the offset of the video ram within the VIC bank. 0 = address 0, 1 = address 1024, etc.
 *
 * @param index The new index of the video bank.
 */
#define setVideoRamOffsetIndex(index) do { \
  VIC.addr &= (unsigned char)0x0f; \
  VIC.addr |= (unsigned char)(index << 4); \
} while(0)


/**
 * Get the video ram offset within the VIC bank (index * 1024).
 *
 * @return The video ram offset within the VIC bank.
 */
#define getVideoRamOffset() (((unsigned int)getVideoRamOffsetIndex()) << 10)


/**
 * Get the physical address of the video ram.
 *
 * @return The physical address of the video ram.
 */
#define getVideoRamAddress() ((((unsigned int)getVideoBank()) << 14) + getVideoRamOffset())


/**
 * Get the 2kb bank of the character generator within the VIC bank ( 0= address 0, 1=address 2048 etc).
 *
 * @return The 2kb bank of the character generator within the VIC bank.
 */
#define getCharacterGenerator() ((((unsigned char)VIC.addr) > 1) & (unsigned char)7)


#define setCharacterGenerator(bank) do { \
  VIC.addr &= (unsigned char)0xf1; \
  VIC.addr |= (unsigned char)(bank << 1); \
} while(0)


/**
 * Select 38 or 40 column mode.
 * 
 * @param enable 1 for 40 column mode, 0 for 38 column mode.
 */
#define setColumnSelect(enable) do { \
  VIC.ctrl2 &= ~((unsigned char)8); \
  if(enable) { VIC.ctrl2 |= ((unsigned char)8); } \
  } while(0)


/**
 * Select 24 or 25 column mode.
 *
 * @param enable 1 for 25 row mode, 0 for 24 row mode.
 */
#define setRowSelect(enable) do { \
  VIC.ctrl1 &= ~((unsigned char)8); \
  if(enable) { VIC.ctrl1 |= ((unsigned char)8); } \
  } while(0)

/**
 * Clear the video screen.
 */
void video_clear( void);

/**
 * Copy the character set to a ram address.
 *
 * @param target the address to copy to.
 */
void video_copy_characterset( unsigned char *);

/**
 * Get info on the screen buffers.
 *
 * @return A struct with info on the available screen buffers.
 */
screenInfo *video_getScreenInfo( void);

/**
 * Init the video system.
 */
void video_init( void);


/**
 * Scroll the video screen by 1 pixel.
 */
void video_scroll( void);

/**
 * Scroll the video screen by 1 pixel.
 * Assembler version.
 */
extern void video_scroll_asm2( void);

/**
 * Wait for the end of the screen.
 */
void video_wait4screen_end( void);

#endif
