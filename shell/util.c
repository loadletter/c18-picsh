/*
  arsh - util.c
  Copyright (C) 2009 Bert Vermeulen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "WProgram.h"
#include <avr/pgmspace.h>


void serial_endline(void)
{

	serialWrite('\r');
	serialWrite('\n');

}


void serial_print(char *msg)
{
	int i;

	for(i = 0; msg[i]; i++)
	{
		serialWrite(msg[i]);
	}

}


void serial_println(char *msg)
{

	serial_print(msg);
	serial_endline();

}


void serial_print_P(const char *msg)
{
	char c;

	while( (c = pgm_read_byte(msg++)) )
		serialWrite(c);

}


void serial_println_P(const char *msg)
{

	serial_print_P(msg);
	serial_endline();

}

