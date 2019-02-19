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

#include <c64.h>
#include "sound.h"


/**
 * Play a note of a given frequency and length.
 *
 * @param notep Pointer to the current note.
 */
void soundPlayNote( struct note *notep) {
}

/**
 * Play a given sound effect.
 *
 * @param effectNumber ID of the sound effect.
 */
void soundPlayEffect( char effectNumber) {
  int timeIndex;

  switch( effectNumber) {
    case SOUND_SHOT: 
      for( timeIndex = 15; timeIndex >= 0; timeIndex--) {
	SID.amp = timeIndex;
	SID.v1.freq = 10440;
	SID.v1.pw = 129;
	SID.v1.ctrl = 15;
	SID.v1.ad = 0;
      }
      SID.amp = 0;
      SID.v1.pw = 0;
    break;
  }
}
