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


#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include <c64.h>

/* Bits indicating a certain direction. */
#define JOYSTICK_UP     1
#define JOYSTICK_DOWN   2
#define JOYSTICK_LEFT   4
#define JOYSTICK_RIGHT  8
#define JOYSTICK_FIRE  16

/* Check if a joystick port is triggered in a give direction. */
#define joystickIsDirection(joystickPort,direction) (((joystickPort) & direction)==0)

/* Check a given joystick port for a certain direction. */
#define joystickIsUp(joystickPort) joystickIsDirection(joystickPort,JOYSTICK_UP)
#define joystickIsDown(joystickPort) joystickIsDirection(joystickPort,JOYSTICK_DOWN)
#define joystickIsLeft(joystickPort) joystickIsDirection(joystickPort,JOYSTICK_LEFT)
#define joystickIsRight(joystickPort) joystickIsDirection(joystickPort,JOYSTICK_RIGHT)
#define joystickIsFire(joystickPort) joystickIsDirection(joystickPort,JOYSTICK_FIRE)

/* Joystick 1 flags. */
#define joystick1isUp() joystickIsUp(CIA1.prb)
#define joystick1isDown() joystickIsDown(CIA1.prb)
#define joystick1isLeft() joystickIsLeft(CIA1.prb)
#define joystick1isRight() joystickIsRight(CIA1.prb)
#define joystick1isFire() joystickIsFire(CIA1.prb)

/* Joystick 2 flags. */
#define joystick2isUp() joystickIsUp(CIA1.pra)
#define joystick2isDown() joystickIsDown(CIA1.pra)
#define joystick2isLeft() joystickIsLeft(CIA1.pra)
#define joystick2isRight() joystickIsRight(CIA1.pra)
#define joystick2isFire() joystickIsFire(CIA1.pra)

#endif
