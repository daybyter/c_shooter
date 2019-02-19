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

#include <string.h>
#include "video.h"


/**
 * A pointer to the current cursor position.
 */
unsigned char *cursorpos;

/**
 * A pointer to the current screen ram.
 */
unsigned char *screenmemory;

/**
 * Convert a character to c64 screen code.
 *
 * @param c The character to convert.
 * @return The converted character or a space, if there is
 *         no proper conversion possible.
 */
char loadScreenConvert2c64screencode( char c) {

  if( c >= 'A' && c <= 'Z') {
    return c - 'A' + 1;
  }

  if( c >= 'a' && c <= 'z') {
    return c - 'a' + 1;
  }

  if( c >= '0' && c <= '9') {
    return c -'0' + 48;
  }
  
  return ' '; /* Return a space as default. */
}

/**
 * Set the cursor to a given column and row.
 *
 * @param column The column to set to (0 - 39).
 * @param row The row to set to (0 - 24).
 */
void loadScreenGotoXY( int column, int row) {

  cursorpos = screenmemory + 40 * row + column;
}

/**
 * Init a load screen (for the game or a level.
 */
void loadScreenInit() {

  screenmemory = cursorpos = (unsigned char *)getVideoRamAddress();
  video_clear();
}

/**
 * Print a character at the current cursor position.
 *
 * @param c The character to print.
 */
void loadScreenPrintChar( char c) {

  /* Just copy the character to the screen position. Do a 
     petscii -> screencode conversion here? */
  *cursorpos++ = loadScreenConvert2c64screencode( c);

  /* Check, if we reached the end of the screen memory. */
  if( cursorpos > (screenmemory + 999)) {
    cursorpos = screenmemory + 999; /* Set it to the end of the screen. */
  }
}

/**
 * Print a number at the current cursor position.
 *
 * @param n The number to print.
 */
void loadScreenPrintNumber( int n) {
  char result[5];
  short len = 0;

  if( n == 0) {
    loadScreenPrintChar('0');
  } else {
  
    if( n < 0) {
      loadScreenPrintChar('-');
      n = -n;
    }

    while( n > 0) {      
      result[4-len++] = '0' + (n % 10);
      n /= 10;
    }

    while( len > 0) {
      loadScreenPrintChar( result[4-len--]);
    }
  }
}

/**
 * Print a 0-terminated string to the current cursor position.
 *
 * @param s The string to print.
 */
void loadScreenPrintString( char *s) {

  while( *s != 0) {  /* While we are not at the end of the string. */
    loadScreenPrintChar( *s++); /* Print the current character. */
  }
}

/**
 * Update the percentage of the progress and add some
 * text, what is currently going on.
 *
 * @param percentage The current progress.
 * @param statustext Some info on current activity.
 */
void loadScreenUpdateStatus( int percentage, char *statustext) {

  int i;
  
  loadScreenGotoXY( 2, 5);
  loadScreenPrintString( "Progress: ");
  loadScreenPrintNumber( percentage);
  loadScreenPrintString( " %");
  loadScreenGotoXY( 2, 8);
  loadScreenPrintString( statustext);
  
  /* Delete until the end of the line, in case the previous text
     was longer, so it is overwritten. */
  for( i=0; i < ( 37 - strlen( statustext)); ++i) {
    loadScreenPrintChar( ' ');
  }
}
