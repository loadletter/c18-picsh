/*
  arsh - arsh.h
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

#ifndef UTIL_H_
#define UTIL_H_

void serial_endline(void);
void serial_print(char *msg);
void serial_println(char *msg);
void serial_print_P(const char *msg);
void serial_println_P(const char *msg);
#define PSTR(x) (x)

#endif /* UTIL_H_ */
