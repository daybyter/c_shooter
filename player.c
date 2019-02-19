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

#include "player.h"
#include "shot.h"
#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>


/*
 * Horizontal position of the player.
 */
unsigned int playerXpos = PLAYER_X_POS_DEFAULT;

/*
 * Vertical position of the player.
 */
unsigned char playerYpos = PLAYER_Y_POS_DEFAULT;


/**
 * Initialize the player.
 *
 * @param level The current level.
 *
 * @return true, if the player was initialized. False otherwise.
 */
char playerInitialize( unsigned char level) {
  spriteSetPosition( 160, playerYpos, 0); /* Set a initial position. */
  spriteEnable(true, 0);  /* Make the sprite visible. */
  return true;
}

/**
 * Load the player data for a given level.
 *
 * @param level The next level to play.
 *
 * @return true, if the data were successfully loaded, false otherwise.
 */
char playerLoad( unsigned char level) {
  // Init the player sprite.
  /* if( ! spriteLoad( PLAYER_SPRITE_FILE, (unsigned char *)(0xf000), 1, 0)) { */
  if( ! spriteLoad( PLAYER_SPRITE_FILE, spriteGetMemorySlot(), 1, 0)) {
#ifdef DEBUG
    printf( "Cannot load sprite 0");
#endif
    return false;
  }
  spriteSetColor( 10, 0);  /* Set the color of sprite 0 to 10 (red). */
  spriteSetMultiColor( 15, 0);  /* Set sprite multicolor 0 to 15 (light grey). */
  spriteSetMultiColor( 12, 1);  /* Set sprite multicolor 1 to 12 (darker grey). */
}

/**
 * Move the player sprite.
 */
void playerMove(void) {
  if( joystick1isLeft() && (playerXpos > 22)) { spriteSetXposition(--playerXpos,0); } 
  if( joystick1isRight() && (playerXpos < 324)) { spriteSetXposition(++playerXpos,0); } 
  if( joystick1isUp() && (playerYpos > 80)) { spriteSetYposition(--playerYpos,0); }
  if( joystick1isDown() && (playerYpos < 228)) { spriteSetYposition(++playerYpos,0); }
  if( joystick1isFire()) { shotNew( playerXpos, playerYpos); }
}
