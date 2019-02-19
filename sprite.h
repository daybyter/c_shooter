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


#ifndef _SPRITE_H
#define _SPRITE_H

#include <c64.h>
#include "config.h"


/* Number of available sprite slots. */
#define NUM_SPRITE_SLOTS 40

/* Start address of the sprites in memory. */
#define SPRITE_MEM_START 0xf000


/**
 * Enable or diable the multicolor mode for a given sprite.
 *
 * @param enable 1, if the sprite is multicolor, 0 if not.
 * @param spriteNumber The number of the sprite (0..7).
 */
#define spriteEnableMulticolor( enable, spriteNumber) do { \
    VIC.spr_mcolor &= (unsigned char)~((unsigned char)1 << spriteNumber); \
  if( enable) { VIC.spr_mcolor |= (unsigned char)(1 << spriteNumber); } \
} while(0)

/**
 * Get a pointer to some free memory slot for a sprite.
 *
 * @return A pointer to a free sprite slot, or NULL if
 *         all slots are taken.
 */
unsigned char *spriteGetMemorySlot( void);

/**
 * Init the sprite system.
 */
void sprite_init( void);

/**
 * Set the color of a given sprite number.
 *
 * @param color The index of the color (0..15).
 * @param spriteNumber The number of the sprite (0..7).
 */
#define spriteSetColor( color, spriteNumber) do { \
   VIC.spr ## spriteNumber ## _color = (unsigned char)color; \
} while(0)

/**
 * Set the colors for multicolor sprites.
 *
 * @param color The index of the color (0..15).
 * @param multicolorIndex The multicolor index (0 or 1).
 */
#define spriteSetMultiColor( color, multicolorIndex) do { \
  VIC.spr_mcolor ## multicolorIndex = (unsigned char)color; \
} while(0)

/**
 * Enable or disable the sprite with the given sprite number (0-7).
 *
 * @param enable 1, if the sprite should be displayed. 0 if it should be hidden.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteEnable(enable,spriteNumber) do { \
    VIC.spr_ena &= (unsigned char)~((unsigned char)1 << spriteNumber); \
  if( enable) { VIC.spr_ena |= (unsigned char)(1 << spriteNumber); } \
} while(0)

unsigned char *spriteGetPointer( unsigned char);

/**
 * Set the expand flag of a given sprite in horizontal direction.
 *
 * @param enable 1, if the sprite is supposed to be expanded. 0 otherwise.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteSetExpandX(enable,spriteNumber) do { \
    VIC.spr_exp_x &= (unsigned char)~((unsigned char)1 << spriteNumber);	\
  if( enable) { VIC.spr_exp_x |= (unsigned char)(1 << spriteNumber); }	\
} while(0)

/**
 * Set the expand flag of a given sprite in vertical direction.
 * 
 * @param enable 1, if the sprite is supposed to be expanded. 0 otherwise.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteSetExpandY(enable,spriteNumber) do { \
  VIC.spr_exp_y &= (unsigned char)~((unsigned char)1 << spriteNumber); \
  if( enable) { VIC.spr_exp_y |= (unsigned char)(1 << spriteNumber); }	\
  } while(0)

int spriteLoad( char *name, 
		unsigned char *address, 
		char isMultiColor, 
		unsigned char spriteNumber);

/**
 * Set the horizontal position of a sprite.
 *
 * @param x The new horizontal position.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteSetXposition(x,spriteNumber) do { \
    VIC.spr ## spriteNumber ## _x = (unsigned char)( (x) & 0xff);	\
    VIC.spr_hi_x = ( VIC.spr_hi_x & ~((unsigned char)1 << spriteNumber)) | ((unsigned char)( ( (x) & 0x100) >> (8 - spriteNumber))); \
  } while(0)

/**
 * Set the vertical position of a sprite.
 *
 * @param y The new vertical position.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteSetYposition(y,spriteNumber) do { \
  VIC.spr ## spriteNumber ## _y = (unsigned char)y; \
} while(0)

/**
 * Set the position of a sprite.
 *
 * @param x The new horizontal position.
 * @param y The new vertical position.
 * @param spriteNumber The number of the sprite (0-7).
 */
#define spriteSetPosition(x,y,spriteNumber) do { \
  spriteSetXposition(x,spriteNumber); \
  spriteSetYposition(y,spriteNumber); \
} while(0)


/**
 * Data structure for a virtual sprite. To be mapped to a hardware
 * sprite by a sprite multiplexer.
 */
typedef struct {
  unsigned char  datap;        /* Byte 0: index of the sprite data block. */
  unsigned int   x:9;          /* Byte 1 & 2: first 9 bits: x coordinate. */
  unsigned int   active:1;     /* bit 10: 1 for visible sprite. */
  unsigned int   width:1;      /* bit 11: 1 for double width sprite. */
  unsigned int   height:1;     /* bit 12: 1 for double height. */
  unsigned int   priority:1;   /* bit 13: 1 if sprite has priority over background. */
  unsigned int   multicolor:1; /* bit 14: 1 if sprite is a multicolor sprite. */
  unsigned char  y;            /* Byte 3: y coordinate. */
  unsigned char  color;        /* Byte 4: color of the sprite. */
} VirtualSprite;

#endif
