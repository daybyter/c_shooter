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
#include <c64.h>
#include "player.h"
#include "shot.h"
#include "sound.h"
#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>


/*
 * The delay between 2 shots.
 */
unsigned char shotDelay = 0;

/* The current data of flying shots. */
VirtualSprite shots[MAX_SHOTS];

 /* The number of currently flying shots. */
unsigned char flyingShots = 0;

/* A list of y coordinate sorted indexes of the 
   virtual sprites, so the multiplexer can access
   then quicker. */
unsigned char sortedShots[MAX_SHOTS];

/* The shot currently rendered by the multiplexing routine. */
unsigned char currentlyRenderedShot = 0;  

/**
 * Initialize the shooting code.
 *
 * @param level The next level to play.
 *
 * @return true, if the shot was successfully loaded, false otherwise.
 */
char shotInitialize( char level) {
  int i;
  
  shotInstallRasterIRQ();  /* To render the shot by sprite multiplexing. */

  /* Reset all the shots to 0. */
  for( i = 0; i < MAX_SHOTS; ++i) {
    shots[i].y=0;  /* Set this shot to available. */

    /* Set all the sorted shots to 'no shot' (0xff). */
    sortedShots[i] = 0xff;
  }
  
  return true;
}

/**
 * Install the raster IRQ routine.
 */
void shotInstallRasterIRQ( void) {
  void (*routinePointer)(void) = &shotRasterIRQ;

  SEI();
  
  CIA1.icr = 0x7f;  /* Disable timer irq from both CIAs. */
  CIA2.icr = 0x7f;

  __asm__( "lda %w", (unsigned)&CIA1.icr);  /* Delete any IRQs, that are probably pending. */
  __asm__( "lda %w", (unsigned)&CIA2.icr);

  VIC.imr |= 0x81;  /* Enable raster IRQs in the interrupt mask register. */

  VIC.rasterline = 2; /* First IRQ at rasterline 2. */
  VIC.ctrl1 &= 0x7f;  /* Bit 8 of the rasterline register is in ctrl1. */

  /* Disable basic and kernal rom to modify IRQ routine pointers. */
  *(unsigned char *)0x1 = 0x35; 
  
  *(unsigned *)0xfffe = (unsigned int)routinePointer;   /* if rom is disabled! Otherwise use 0x0314 as routine pointer. */
  /* *(unsigned *)0x0314 = routinePointer; */

  CLI();
}

/**
 * Load the shot data for the next level.
 *
 * @param level The next level to play.
 *
 * @return true, if the data were successfully loaded, false otherwise.
 */
char shotLoad( char level) {
  // Init the player sprite.
  if( ! spriteLoad( SHOT_SPRITE_FILE, (unsigned char *)(0xf040), 1, 1)) {
#ifdef DEBUG
    printf( "Cannot load sprite 1");
#endif
    return false;
  }
  spriteSetColor( 10, 1);  /* Set the color of sprite 1 to 10 (red). */

  /* Do not set multicolor colors here. Already done in the player sprite. 
   * The sprite is also not enabled yet.
   */
}

/**
 * Move the shot with the given index (o..flyingShots-1).
 *
 * @param shotNr The index of the shot.
 */
void shotMove( char shotNr) {
  if( --shots[shotNr].y < 20) {  /* If the shot has reached the top of the screen. */
    shotRemove( shotNr);
  }
}

/**
 * Move all flying shots.
 */
void shotMoveAll( void) {
  
  unsigned char index;

  /* If there's currently a delay running, decrease it. */
  /* 
  if( shotDelay > 0) {
   -- shotDelay;
  }
  */
  __asm__( "lda %v", shotDelay);  /* Optimized asm version of the C code. */
  __asm__( "bne %g", delay_done); /* If the delay is not yet 0, */
  __asm__( "dec %v", shotDelay);  /* decrement it further. */
  delay_done:

  for( index = 0; index < flyingShots; ++index) {
    shotMove( index);
  }
}

/**
 * Scheduler task to move all flying shots.
 */
void shot_move_task( void) {

  unsigned char i;
  
  for( i = 0; i < flyingShots; ) {
    unsigned char shotIndex = sortedShots[i];

    if( shotIndex != 0xff) {  /* Should never happen! */

      /* If this shot has reached the upper border. */
      if( shots[shotIndex].y <= 20) {

	/* Remove the shot. */
	shotRemove( shotIndex);

	/* Do not inc i, if shot removed, since shot[i+1] becomes
           shot[i] now. */
	
      } else {
	--(shots[shotIndex].y);
	++i;
      }
    } else {
      ++i;
    }
  }
}

/**
 * Try to create a new shot.
 *
 * @param x The initial horizontal position of the shot.
 * @param y The initial vertical position of the shot.
 * 
 * @return true, if the shot was created, false otherwise.
 */
char shotNew( int x, unsigned char y) {

  unsigned char i,k,l;
  
  if( ( flyingShots < MAX_SHOTS) && ( shotDelay == 0)) {

    /* Search for a free slot. */
    for( i = 0; i < MAX_SHOTS; ++i) {
      if( shots[i].y == 0) {  /* Free slot found. */
	break;
      }
    }
    
    if( i == MAX_SHOTS) { /* should never happen. */
      return false;
    }
    
    shots[i].x = x;
    shots[i].y = y;

    /* Insert the new shot into the list of sorted shots. */
    for( k = 0; k < flyingShots; ++k) {
      if( shots[sortedShots[k]].y > y) { /* If this shot is lower than the new shot,
			             insert the new shot here. */
	/* Shift all the shots 1 slot back. */
	for( l = k; l < flyingShots; ++l) {
	  sortedShots[l+1] = sortedShots[l];
	}

	/* Insert new shot now at pos k. */
	sortedShots[k] = i;

	break; /* Stop searching for the slot. */
      }
    }
    
    if( flyingShots++ == 0) {  /* If it's the first shot, enable the display. */
      spriteEnable( 1, SHOT_SPRITE);
    }
    shotDelay = SHOT_MIN_DELAY;

    /* Make some noise... */
    soundPlayEffect( SOUND_SHOT);

    return true;
  }
  return false;
}

/**
 * Raster IRQ service routine to multiplex shot sprites.
 */
void shotRasterIRQ( void) {

  __asm__( "lda #$ff");  /* Clear the interrupt request register, to clear the interrupt condition. */
  __asm__( "sta %w", (unsigned)&VIC.irr);  /* Otherwise, the VIC will trigger another IRQ immediately regardless of the rasterline. */

  /* __asm__( "inc %w", (unsigned)&VIC.bordercolor); */ /* Just for debugging. */

  /* if currentlyRenderedShot == flyingShots goto nextFrame // All shots are rendered or there are no flying shots. */ 
  __asm__( "ldy %v", currentlyRenderedShot);
  __asm__( "cpy %v", flyingShots);
  
  __asm__( "beq %g", nextFrame);
  /* else // Set sprite coordinates for <currentlyRenderedShot> */

  /* Compute the starting address of the virtual sprite in the array. 
    Since I padded the sizeof(VirtualSprite) to 8, it is 
    y * 8, which is y << 3 .*/

  __asm__( "ldx _multiply_by5_table,y"); /* x = 5*y . 5 is sizeof(VirtualSprite). */

  __asm__( "lda %v+3,x", shots);  /* AC = shots[currentlyRenderedShot].y */
  __asm__( "sta %w", (unsigned)&VIC.spr1_y);
  __asm__( "lda %v+1,x", shots);  /* Get the lsb of shots[currentlyRenderedShot].x */
  __asm__( "sta %w", (unsigned)&VIC.spr1_x);
  __asm__( "lda %v+2,x", shots); /* Get the msb of the x coordinate. */
  __asm__( "and #1");  /* Use only the lowest bit. */
  __asm__( "tax");
  __asm__( "lda %w", (unsigned)&VIC.spr_hi_x); /* Get the sprite msb from the VIC. */
  __asm__( "and #%b", ~(1 << SHOT_SPRITE));  /* Clear the msb for the shot sprite. */
  __asm__( "cpx #0");
  __asm__( "beq %g", no_msb);
  __asm__( "ora #%b", (1 << SHOT_SPRITE));
 no_msb:
  __asm__( "sta %w", (unsigned)&VIC.spr_hi_x);
  __asm__( "inc %v", currentlyRenderedShot);  /* Increment the index to the next shot. */
  __asm__( "ldy %v", currentlyRenderedShot);  
  __asm__( "cpy %v", flyingShots);
  __asm__( "beq %g", nextFrame);  /* All shots for this frame are rendered. */

  /* Get the y coordinate of the next shot. */
  __asm__( "ldx _multiply_by5_table,y"); /* x = 5 * y. 5 = sizeof(VirtualSprite) */
  __asm__( "ldy %v+3,x", shots); /* Get the y coordinate of the next shot. */
  __asm__( "dey");  /* substract one and set this value for the next raster IRQ. */
  __asm__( "sty %w", (unsigned)&VIC.rasterline);
  __asm__( "rti"); 
  /* __asm__( "jmp $ea31"); */  /* Do normal service routine. */
 nextFrame:
  __asm__( "lda #0");
  __asm__( "sta %v", currentlyRenderedShot);  /* Start again with shot 0. */
  __asm__( "lda #2");
  __asm__( "sta %w", (unsigned)&VIC.rasterline);  /* And raster line 2. */
  __asm__( "rti");
  /* __asm__( "jmp $ea31"); */  /* Do normal service routine. */
}

/**
 * Remove a given shot from the flying shots.
 *
 * @param shotNr The index of the shot to remove.
 */
void shotRemove( char shotNr) {
  int curShot;

  /* Reduce the number of flying shots and disable the sprite, 
     if it was the last flying shot. */
  if( --flyingShots == 0) {
    spriteEnable( 0, SHOT_SPRITE);
  }

  /* Shift the following shots by 1 to keep the shots sorted, which is important for sprite multiplexing. */
  for( curShot = shotNr; curShot < flyingShots; curShot++) {
    shots[curShot] = shots[curShot + 1];
  }
}

/**
 * Render the flying shots.
 */
void shotRender( void) {
  unsigned char curShot;

  /* For now just set the position to the only flying shot and enable the sprite. */
  for( curShot = 0; curShot < flyingShots; ++curShot) {
    spriteSetPosition( shots[curShot].x, shots[curShot].y, SHOT_SPRITE);
  }
}

/**
 * Resort the sorted_shots array to reflect the current data
 * of the virtual sprite array.
 */
void shotResortShots(void) {

  unsigned char i,j,temp;
  
  /* Start with an unsorted array of the sprites. */
  for( i = 0; i < flyingShots; ++i) {
    sortedShots[i] = i;
  }
  
  /* Just do a simple swap sort. */
  for( i = flyingShots - 1; i > 0; --i) {
    for( j = 0; j < i; ++j) {

      /* Test, if we have to swap the sorted indexes. */
      if( shots[ sortedShots[j]].y > shots[ sortedShots[j+1]].y) {

	/* Sort the sprites according to their y coordinate. */
	temp = sortedShots[j];
	sortedShots[j] = sortedShots[j+1];
	sortedShots[j+1] = temp;
      }
    }
  }
}
