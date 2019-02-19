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

#include "enemy.h"
#include "sprite.h"
#include <stdbool.h>
#include "video.h"


/*
 * The current animation frame of the enemy.
 */
unsigned char enemyAnimationFrame; 
unsigned char enemyAnimationFrameDelay;

/*
 * Pointer to the sprite index.
 */
unsigned char *enemy_spritePointer;

/**
 * Animate the enemy sprite.
 */
void enemyAnimate( void) {

  /* Count the delay. */
  enemyAnimationFrameDelay++;
  enemyAnimationFrameDelay &= 63;

  if( enemyAnimationFrameDelay == 0) {  /* Display the next frame. */
    
    if( ( enemyAnimationFrame & 1) == 0) { /* frame 0 or 2 */
      *enemy_spritePointer = 194;
    } else {
      if( enemyAnimationFrame & 2) {
	spriteSetColor( 1, 2);
	*enemy_spritePointer = 195;
      } else {
	spriteSetColor( 4, 2);
	*enemy_spritePointer = 196;
      }
    }
    enemyAnimationFrame++;
    enemyAnimationFrame &= 3;  /* There are only frames 0..3 . */
  }
}

/**
 * Initialize the enemy for the next level.
 *
 * @param level The next level.
 *
 * @return true, if the enemy was successfully initialized, false otherwise.
 */
char enemyInitialize( unsigned char level) {

  enemyAnimationFrame = 0;
  enemyAnimationFrameDelay = 0;

  spriteSetPosition( 160, 60, 2); /* Set a initial position. */
  spriteEnable(true, 2);  /* Make the sprite visible. */

  return true;
}

/**
 * Load the data for the enemies of the next level.
 *
 * @param level The next level to play.
 *
 * @return true, if the data were successfully loaded, false otherwise.
 */
char enemyLoad( unsigned char level) {

  if( ! spriteLoad( ENEMY_SPRITE_FILE1, (unsigned char *)(0xf080), 1, ENEMY_SPRITE)
      || !  spriteLoad( ENEMY_SPRITE_FILE2, (unsigned char *)(0xf0c0), 1, ENEMY_SPRITE)
      || !  spriteLoad( ENEMY_SPRITE_FILE3, (unsigned char *)(0xf100), 1, ENEMY_SPRITE)) {
    return false;
  }

  enemy_spritePointer = spriteGetPointer( ENEMY_SPRITE);

  return true;
}

/**
 * Move all visible enemies.
 */
void enemyMoveAll( void) {
  /* For now, there's only one... */
  enemyAnimate();
}
