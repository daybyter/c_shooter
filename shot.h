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

#ifndef _SHOT_H
#define _SHOT_H

#include "sprite.h"


/*
 * Handling a shot from the player.

 */

#define MAX_SHOTS 6  /* The max number of shots flying at the same time. */

extern unsigned char shotDelay; /* The delay between 2 shots. */

#define SHOT_MIN_DELAY 30 /* Minimum number of frames between 2 shots. */

extern unsigned char spread_factor;  /* Add spreading as another extra, when rapid fire is already activated. */

#define MAX_SPREADING 40  /* 40% spreading max. */

#define SHOT_START_Y 170  /* The start y coordinate for shots. */

#define SHOT_SPRITE_FILE "missile.spr"  /* Name of the file holding the sprite data. */

#define SHOT_SPRITE 1  /* The index of the sprite to use for the shot rendering. */

/* The number of currently flying shots. */
extern char flyingShots;

/* The current data of flying shots. */
extern VirtualSprite shots[MAX_SHOTS];

/* A list of y coordinate sorted indexes of the 
   virtual sprites, so the multiplexer can access
   then quicker. */
extern unsigned char sortedShots[MAX_SHOTS];


char shotInitialize( char);
void shotInstallRasterIRQ( void);
char shotLoad( char);
void shotMove( char);
void shotMoveAll( void);
void shot_move_task( void);
char shotNew( int, unsigned char);
void shotRasterIRQ(void);
void shotRemove( char);
void shotRender( void);
void shotResortShots(void);

extern void shot_add_new( void);
extern void shot_check_fire_asm( void);
extern void shot_search_free_slot( void);

#endif
