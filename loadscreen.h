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


#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_

/**
 * This module defines a simple status screen, that should be
 * shown while the game and a level is loaded and initialized.
 */

/**
 * Convert a character to c64 screen code.
 *
 * @param c The character to convert.
 * @return The converted character or a space, if there is
 *         no proper conversion possible.
 */
char loadScreenConvert2c64screencode( char c);

/**
 * Set the cursor to a given column and row.
 *
 * @param column The column to set to (0 - 39).
 * @param row The row to set to (0 - 24).
 */
void loadScreenGotoXY( int column, int row);

/**
 * Init a load screen (for the game or a level.
 */
void loadScreenInit();

/**
 * Print a character at the current cursor position.
 *
 * @param c The character to print.
 */
void loadScreenPrintChar( char c);

/**
 * Print a number at the current cursor position.
 *
 * @param n The number to print.
 */
void loadScreenPrintNumber( int n);

/**
 * Print a 0-terminated string to the current cursor position.
 *
 * @param s The string to print.
 */
void loadScreenPrintString( char *s);

/**
 * Update the percentage of the progress and add some
 * text, what is currently going on.
 *
 * @param percentage The current progress.
 * @param statustext Some info on current activity.
 */
void loadScreenUpdateStatus( int percentage, char *statustext);

#endif
