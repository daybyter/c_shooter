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

/*
 * Create a playground for a game.
 */

#ifndef _PLAYGROUND_H
#define _PLAYGROUND_H

#include <stdbool.h>

/*
 * Create a playground from tiles.
 */
#define TILE_WIDTH 2
#define TILE_HEIGHT 2
#define TILE_LENGTH (TILE_WIDTH*TILE_HEIGTH)

/*
 * A tile is created from characters stored in a character rom.
 */

#define PLAYGROUND_WIDTH 20 /* The number of tile in a a screen line. */

/* Initialize the playground for the next level. */
bool playgroundInitialize(unsigned char);

/* Load the playground data for the next level. */
bool playgroundLoad(unsigned char);

/* Scroll the playground by 1 pixel. */
void playgroundScroll( void);

 /* Create one screenline of the playground. */
void playgroundWriteLine(unsigned char *, int, unsigned char *, unsigned char *);

/* Write a line of a tile to the videoram. */
void playgroundWriteTile( unsigned char *, char, unsigned char *);

/* Unpack 1 character line of the current tiles to the video buffer,
   that is used after the next scolling. */
extern void _playground_unpack_tile_line( void);

#endif
