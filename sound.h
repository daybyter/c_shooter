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


#ifndef _SOUND_H
#define _SOUND_H

/*
 * Common sould related functions.
 */

#define SOUND_SHOT 1


/**
 * Struct to hold the info on a note.
 */
struct note {
  int frequency;  /* Frequency of the note. */
  int length;     /* Duration of the note. */
}; 


void soundPlayNote( struct note *);
void soundPlayEffect( char);

#endif
