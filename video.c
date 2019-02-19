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

#include <6502.h>
#include <stdlib.h>
#include <string.h>
#include "video.h"


/* A pointer to the video ram. */
unsigned char *videoram;

/* 2nd version: 2 pointers now for double buffering. */
unsigned char *video_buffer1 = (unsigned char *)0xe400;
unsigned char *video_buffer2 = (unsigned char *)0xe800;

/* Info on the screen buffers. */
screenInfo *_currentScreenInfo = NULL;

/**
 * Clear the video screen.
 */
void video_clear( void) {
  char currentBuffer;
  screenInfo *currentScreenInfo = video_getScreenInfo();

  if( currentScreenInfo != NULL) {
    for( currentBuffer = 0; currentBuffer < currentScreenInfo->nScreens; ++currentBuffer) {
      memset( currentScreenInfo->buffers[currentBuffer], 32, 1000);  // Fill with spaces
    }
  }
}

/**
 * Copy the character set to a ram address.
 *
 * @param target the address to copy to.
 */
void video_copy_characterset( unsigned char *target) {

  unsigned char *mem_config = (unsigned char *)1;
  unsigned old_config;
  unsigned char *src = (unsigned char *)0xd000;
 
  /* Store the old mem bank config. */
  old_config = *mem_config;

  SEI();  /* disable IRQs, while we copy. */

  *mem_config = 0x33;  /* Make cpu see the character ROM. */

  /* Do the actual copy loop. */
  while( src < (unsigned char*)0xd800) {
    *target++ = *src++;
  }

  /* Restore old mem banking. */
  *mem_config = old_config;
  CLI();
}

/**
 * Get info on the screen buffers.
 *
 * @return A struct with info on the available screen buffers.
 */
screenInfo *video_getScreenInfo( void) {

  if( _currentScreenInfo == NULL) {

    _currentScreenInfo = malloc( sizeof( screenInfo));
    if( _currentScreenInfo != NULL) {
      _currentScreenInfo->nScreens = 2;
      _currentScreenInfo->buffers = malloc( 2 * sizeof( unsigned char *));
      if( _currentScreenInfo->buffers != NULL) {
	_currentScreenInfo->buffers[0] = video_buffer1;
	_currentScreenInfo->buffers[1] = video_buffer2;
      } else {
	free( _currentScreenInfo); /* Release memory again. */
	_currentScreenInfo = NULL;
      }
    }
  }

  return _currentScreenInfo;
}

/**
 * Init the video system.
 */
void video_init( void) {

  /* Set a pointer to the current video ram. */
  videoram = (unsigned char *)(getVideoRamAddress());

  /* Set the border and background color to black. */
  /* VIC.bordercolor = 0;
     VIC.bgcolor0 = 0; */
}


/**
 * Scroll the video screen by 1 pixel.
 */
void video_scroll( void) {

  unsigned char *src;
  unsigned char *dest;
  unsigned char *bank_switcher;
  
  unsigned char cur_y = VIC.ctrl1 & 7;
  ++cur_y;
  cur_y &= 7; 
  VIC.ctrl1 = ( (VIC.ctrl1 & (unsigned char)248) | cur_y); 
  /* #pragma optimize( push, off) 
  asm( "lda $d011");
  asm( "tax");
  asm( "clc");
  asm( "adc #$01");
  asm( "and #$07");
  asm( "sta %g+1", or_label);
  asm( "txa");
  asm( "and #248");
 or_label:
  asm( "ora #0");    Modify this argument. 
  asm( "sta $d011");
  #pragma optimize( pop);  */
  
  if( ( VIC.ctrl1 & 7) == 0) {

  /* Scroll the screen by 1 character line. */

    ++VIC.bordercolor;

    SEI();
    
    /* Make RAM under kernal accessible. */
    bank_switcher = (unsigned char *)1;
    *bank_switcher &= 253;
    
    src = (unsigned char *)(videoram + 999 - 40);
    dest = (unsigned char *)(videoram + 999); 
    
    while( src >= videoram) {
      *dest = *src;
      dest--;
      src--;
    }

    /* Activate kernal rom again. */
    *bank_switcher |= 2;

    CLI();
  } 
}


/**
 * Wait for the end of screen.
 */
void video_wait4screen_end( void) {

  while( VIC.rasterline != 250) {
  }
}
