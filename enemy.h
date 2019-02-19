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

#ifndef _ENEMY_H
#define _ENEMY_H

#define ENEMY_SPRITE 2  /* Use sprite number 2. */

#define ENEMY_SPRITE_FILE1 "enemy1.spr"
#define ENEMY_SPRITE_FILE2 "enemy2.spr"
#define ENEMY_SPRITE_FILE3 "enemy3.spr"

char enemyInitialize( unsigned char);
char enemyLoad( unsigned char);
void enemyMoveAll( void);

#endif
