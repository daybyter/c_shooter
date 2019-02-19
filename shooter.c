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
#include <c64.h>  // c64 hardware access.
#include <conio.h>
#include "enemy.h"
#include "gameplay.h"
#include "joystick.h"
#include "loadscreen.h"
#include "player.h"
#include "playground.h"
#include "shooter.h"
#include "shot.h"
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include "video.h"


/* The current level. */
short level = 0;

/* The currently loaded level (might differ from the next level to play. */
short loadedLevel = -1;

/**
 * Initialize the entire game.
 *
 * return true, if the game was successfully initialized. false otherwise.
 */
bool gameInitialize( void) {

  SEI();

  setVideoBank( 3);             /* Set the VIC to use memory from $c000. */
  setVideoRamOffsetIndex( 9);   /* Set the video memory to $e400. */

  video_copy_characterset( (unsigned char *)0xd000);  /* Copy the character rom to 0xd000 in RAM. */
  setCharacterGenerator(2);

  video_init();  /* Init video subsystem. */

  /* Turn off timer IRQs */
  asm( "lda #$7f");
  asm( "sta $dc0d");
  asm( "sta $dd0d");
  asm( "lda $dc0d");  /* Delete potential pending IRQs. */
  asm( "lda $dd0d"); 
  
  CLI(); 

  return true;
}

/**
 * Init a level.
 *
 * @param currentLevel
 *
 * @return true, if the level was successfully initialized. False otherwise.
 */
bool levelInitialize( unsigned char currentLevel) {

  if( loadedLevel != currentLevel) {  /* If we have to load the data for the next level. */

    loadScreenInit();

    /* Reset the sprite subsystem, because the sprites
       weren't most likely removed after the last level. */
    sprite_init();

    if( ! playerLoad( currentLevel)) {
      return false;
    }

    if( ! shotLoad( currentLevel)) {
      return false;
    }

    if( ! enemyLoad( currentLevel)) {
      return false;
    }
    
    loadScreenUpdateStatus( 30, "Loaded sprites");
    
    if( ! playgroundLoad( currentLevel)) {
      return false;
    }

    loadScreenUpdateStatus( 60, "Loaded level data");
    
    loadedLevel = currentLevel; 
  }

  if( ! playerInitialize( currentLevel)) {
    return false;
  }
  
  if( ! shotInitialize( currentLevel)) {
    return false;
  }
  
  if( ! enemyInitialize( currentLevel)) {
    return false;
  }

  if( ! playgroundInitialize( currentLevel)) {
    return false;
  }

  loadScreenUpdateStatus( 100, "Level initialized");

  video_clear();  /* Clear the screen. */

  return true;
}

/**
 * Play the current level.
 *
 * @param currentLevel The current level.
 *
 * @return true, if the level was completed. False in case of an error.
 */
bool levelPlay( unsigned char currentLevel) {
  /*  while( ! kbhit()) { */
  /*
  while( true) {
    video_wait4screen_end();
    //playgroundScroll( getVideoRamAddress());
    video_scroll_asm();
    //video_scroll();

    // Fill the last line with dummy graphics. 
    playgroundScroll();
    
    playerMove();
    enemyMoveAll();
    shotMoveAll();
    shotRender();  // Render all flying shots.
  } */
  switch( currentLevel) {
  case 0: play_level0(); break;
  }

  return true;
}

/**
 * Main entry point.
 */
int main( void) {
  if( ! gameInitialize()) { return 1; }

  while( level < MAX_LEVEL) {
    
    if( ! levelInitialize( level)) {
      return 1;
    }

    if( ! levelPlay( level)) {
      /* Restart from 1st level. */
    } else {
      level++;
    }
  }

  return 0;
}

