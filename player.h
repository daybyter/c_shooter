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

#ifndef _PLAYER_H
#define _PLAYER_H

#include "joystick.h"


#define PLAYER_SPRITE_FILE "player2.spr"  /* Name of the file holding the player sprite data. */
#define PLAYER_SPRITE 0  /* The index of the sprite used for the player. */
#define PLAYER_X_POS_DEFAULT 160  /* Horizontal start position of the player. */
#define PLAYER_Y_POS_DEFAULT 210  /* Vertical start position of the player. */

extern unsigned int playerXpos;  /* Horizontal position of the player. */
extern unsigned char playerYpos;  /* Vertical position of the player. */

char findFreeSlot(); /* Find a slot for a new shot. */
char playerFireShot(); /* Try to fire a shot. */
void playerAnimateShots(); /* Animate the flying shots. */
char playerInitialize(unsigned char);  /* Initialize the player. */
char playerLoad(unsigned char); /* Load the player data for a given level. */
void playerMove(void); /* Animate the player. */

extern void player_move_asm( void);  /* Assembler version of animation. */
#endif
