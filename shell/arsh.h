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

#ifndef ARSH_H_
#define ARSH_H_

#include <avr/pgmspace.h>

#define SERIAL_CMDBUF_LEN	64
#define MAX_NUM_TOKENS		10
#define MONITOR_FREQUENCY	200  // monitor refresh frequency in ms
#define HISTORY_LEN		16

#define VERSION			"Arsh v0.1"
#define PROMPT				"] "
#define TOK_HELP			1
#define TOK_PING			2
#define TOK_RESET			3
#define TOK_READ			4
#define TOK_SET			5
#define TOK_DPIN			6
#define TOK_APIN			7
#define TOK_MODE			8
#define TOK_HIGH			9
#define TOK_LOW			10
#define TOK_IN				11
#define TOK_OUT			12
#define TOK_MONITOR		13
#define TOK_SERIAL			14

typedef struct tokenstruct {
	uint8_t token;
	char *keyword;
} tokenstruct;


void init_arsh(unsigned char c);
void arsh(void);
int try_completion(char *buf, int show_matches);
void tokenize(char *buf);
void parse_cmdline(void);
void add_to_history(void);
uint8_t remove_oldest_history_entry(void);
int8_t find_cur_histentry(void);
void load_histentry(uint8_t pos);
void history_up(void);
void history_down(void);
void process_command(void);
void connect_serial(uint8_t rxpin, uint8_t txpin);
uint8_t softserial_read(uint8_t rxpin);
void monitor(int dpins, int apins);
void read_all_pins(void);
void show_digital_pin_status(int pin);
void show_analog_pin_status(int pin);

#endif /* ARSH_H_ */
