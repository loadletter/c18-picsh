/*
  arsh - arsh.c
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

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "arsh.h"
#include "usbio.h"

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

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef struct tokenstruct {
	uint8_t token;
	const rom char *keyword;
} tokenstruct;

const rom char MONITOR_TEXT[] = "\x1b[2J\x1b[1;1H" "Arsh pin monitor (esc to quit)\r\n\r\n";

// globals
char cmdbuf[SERIAL_CMDBUF_LEN];
unsigned char tokens[MAX_NUM_TOKENS + 1];
uint8_t num_tokens;
uint8_t histbuf[HISTORY_LEN + 1];
int8_t cur_histentry;
tokenstruct shelltokens[] = {
	{ TOK_HELP, "help" },
	{ TOK_PING, "ping" },
	{ TOK_RESET, "(unused)" },
	{ TOK_READ, "read" },
	{ TOK_SET, "set" },
	{ TOK_DPIN, "dpin" },
	{ TOK_APIN, "apin" },
	{ TOK_MODE, "mode" },
	{ TOK_HIGH, "HIGH" },
	{ TOK_LOW, "LOW" },
	{ TOK_IN, "IN" },
	{ TOK_OUT, "OUT" },
	{ TOK_MONITOR, "monitor" },
//	{ TOK_SERIAL, "serial" },
	{ 0, NULL }
};

/* temporary defines */
#define pinMode(a, b) ((a) + (b))
#define digitalRead(x) 0
#define digitalWrite(a, b) ((a) + (b))
#define analogRead(x) 2
#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0

/* prototypes */
uint8_t remove_oldest_history_entry(void);
void history_up(void);
void history_down(void);
void process_command(void);
void monitor(int dpins, int apins);
void show_digital_pin_status(int pin);
void show_analog_pin_status(int pin);

/* main */

void init_arsh(void)
{
	cmdbuf[0] = 0;
	num_tokens = 0;
	histbuf[0] = 0;
	cur_histentry = -1;
}


void arsh(unsigned char c)
{
	uint8_t complete, l, i;
	char *word, *suffix;
	static unsigned char prev_char = 0;
	static uint8_t escmode = 0;

	if(c == '\r' || c == '\n')
	{
		USB_endline();
		if(cmdbuf[0])
			process_command();
		cmdbuf[0] = 0;
		USB_print_ROM(PROMPT);
		escmode = 0;
		cur_histentry = -1;
	}
	else
	{
		l = strlen(cmdbuf);
		if( (c == 0x08 || c == 0x7f) && l != 0)
		{
			// backspace or rubout
			cmdbuf[l-1] = 0;
			USB_putchar(0x08);
			USB_putchar(0x20);
			USB_putchar(0x08);
		}
		else if(c == 0x09)
		{
			// tab: try to complete a keyword
			// find start of current word
			for(i = l-1; i != 0 && cmdbuf[i] != ' '; i--)
				;
			if(cmdbuf[i] == ' ')
				i++;
			word = cmdbuf + i;
			complete = try_completion(word, 0);
			if(complete && (complete & 0x80) == 0)
			{
				// found exactly one match for completion, find the token entry
				for(i = 0; shelltokens[i].token != complete; i++)
					;
				for(suffix = shelltokens[i].keyword + strlen(word); *suffix; suffix++)
				{
					if(l < SERIAL_CMDBUF_LEN)
						cmdbuf[l++] = *suffix;
					USB_putchar(*suffix);
				}
				if(l < SERIAL_CMDBUF_LEN-1)
					cmdbuf[l++] = ' ';
				cmdbuf[l] = 0;
				USB_putchar(' ');
			}
			else if(complete & 0x80 && prev_char == 0x09)
			{
				// hit tab twice, show a list of completions
				USB_println("");
				try_completion(word, 1);
				USB_print_ROM(PROMPT);
				USB_print(cmdbuf);
			}
		}
		else if(c == 0x0c)
		{
			// characters not saved, but displayed when typed (^l)
			USB_putchar(c);
		}
		else if(c == 0x1b)
		{
			// esc key, change to esc sequence capture mode
			escmode = 1;
		}
		else if(escmode > 0)
		{
			if(escmode == 1 && c == '[')
				escmode = 2;
			else
			{
				if(escmode == 2 && c == 'A')
					history_up();
				else if(escmode == 2 && c == 'B')
					history_down();

				// regardless of how we got here, we're done with the
				// escape sequence
				escmode = 0;
			}
		}
		else if(c == 0x10)
			// ^p
			history_up();
		else if(c == 0x0e)
			// ^n
			history_down();
		else if(l < SERIAL_CMDBUF_LEN-1 && c >= 0x20 && c < 0x7f)
		{
			// regular characters
			cmdbuf[l] = c;
			cmdbuf[l+1] = 0;
			USB_putchar(c);
		}
	}
	prev_char = c;
}

void print_help(void)
{
	USB_println_ROM(VERSION);
	USB_endline();
	USB_println_ROM("Commands:");
	USB_println_ROM("ping				return 'pong'");
	USB_println_ROM("read dpin <pin>			return digital state of pin");
	USB_println_ROM("read apin <pin>			return analog state of pin");
	USB_println_ROM("set dpin <pin> <state>		set pin to state ('HIGH' or 'LOW')");
	USB_println_ROM("set mode <pin> <mode>		set pin to mode ('IN' or 'OUT')");
	USB_println_ROM("monitor [dpin|apin <pin> ...]	live pin monitor");
	//USB_println_ROM("serial <rx> <tx>		establish serial connection on given pins");
}

int try_completion(char *buf, int show_matches)
{
	int token, ret, len, i;

	token = ret = 0;
	len = strlen(buf);
	for(i = 0; shelltokens[i].token; i++)
		if(!strncmpram2pgm(shelltokens[i].keyword, buf, len))
		{
			if(show_matches)
				USB_println(shelltokens[i].keyword);
			token = shelltokens[i].token;
			ret++;
		}

	if(ret == 1)
		ret = token;
	else if(ret > 1)
		// return value with bit 7 set means # of matches
		ret |= 0x80;

	return ret;
}


int isnum(char *buf)
{
	int i;

	for(i = 0; buf[i]; i++)
		if(buf[i] < '0' || buf[i] > '9')
			return 0;

	return 1;
}


void tokenize(char *buf)
{
	int val, done, i;

	done = 0;
	for(i = 0; shelltokens[i].token; i++)
	{
		if(!strcmppgm2ram(buf, shelltokens[i].keyword))
		{
			tokens[num_tokens++] = shelltokens[i].token;
			done = 1;
			break;
		}
	}

	if(!done)
	{
		if(isnum(buf))
		{
			val = atoi(buf);
			if(val > 127)
			{
				USB_println_ROM("can't parse number");
			}
			tokens[num_tokens++] = val | 0x80;
		}
		else
		{
			USB_print_ROM("don't know ");
			USB_println(buf);
		}
	}

}


void parse_cmdline(void)
{
	int i;
	char *word_start;

	// skip leading whitespace
	for(i = 0; cmdbuf[i] && cmdbuf[i] == ' '; i++);
	word_start = NULL;
	num_tokens = 0;
	while(cmdbuf[i] != 0 && num_tokens < MAX_NUM_TOKENS)
	{
		if(cmdbuf[i] != ' ')
			// got a word
			word_start = cmdbuf + i;
		while(cmdbuf[i] != 0 && cmdbuf[i] != ' ')
			// skip ahead to the next word
			i++;
		if(cmdbuf[i] == ' ')
			// delimit word
			cmdbuf[i++] = 0;
		tokenize(word_start);
	}
	// end token list
	tokens[num_tokens] = 0;

}


void add_to_history(void)
{
	uint8_t buflen, i;

	// check if the last stored entry is actually the same as the new one
	if(num_tokens == histbuf[0] && !memcmp(histbuf + 1, tokens, num_tokens))
		// same, skip it
		return;

	buflen = 0;
	while(histbuf[buflen])
	{
		buflen += 1 + histbuf[buflen];
	}

	while(HISTORY_LEN - buflen <= 1 + num_tokens)
	{
		// history buffer doesn't have enough space to store this new entry
		// shift entries off the end until it will fit
		buflen -= remove_oldest_history_entry();
	}

	// shift the entire history buffer up by the number of bytes we need
	for(i = buflen; i != 255; i--)
	{
		histbuf[i + 1 + num_tokens] = histbuf[i];
	}
	// and terminate it
	histbuf[buflen + 1 + num_tokens] = 0;

	// store the new entry
	histbuf[0] = num_tokens;
	for(i = 0; i < num_tokens; i++)
		histbuf[1 + i] = tokens[i];

}


// returns the number of bytes chopped off the end of the history buffer
uint8_t remove_oldest_history_entry(void)
{
	uint8_t prev, len, i;

	prev = len = i = 0;
	while(histbuf[i])
	{
		prev = i;
		i += 1 + histbuf[i];
	}

	len = 1 + histbuf[prev];
	histbuf[prev] = 0;

	return len;
}


int8_t find_cur_histentry(void)
{
	uint8_t pos, i;

	for(pos = i = 0; i != cur_histentry; )
	{
		if(histbuf[pos] == 0)
			break;
		pos += 1 + histbuf[pos];
		i++;
	}

	if(histbuf[pos] == 0)
		return -1;
	else
		return pos;
}


void load_histentry(uint8_t pos)
{
	uint8_t i;
	char buf[8];

	cmdbuf[0] = 0;
 	for(i = pos + 1; i < pos + 1 + histbuf[pos]; i++)
	{
 		if(histbuf[i] & 0x80)
 		{
 			// it's a number
 			btoa(histbuf[i] & 0x7f, buf);
 			strncat(cmdbuf, buf, SERIAL_CMDBUF_LEN - strlen(cmdbuf) - 1);
 		}
 		else
 		{
 			strncatpgm2ram(cmdbuf, shelltokens[histbuf[i] - 1].keyword, SERIAL_CMDBUF_LEN - strlen(cmdbuf) - 1);
 			strncatpgm2ram(cmdbuf, " ", SERIAL_CMDBUF_LEN - strlen(cmdbuf) - 1);
 		}
	}
	USB_print_ROM("\r\x1b[0K");
	USB_print_ROM(PROMPT);
	USB_print(cmdbuf);

}


void history_up(void)
{
	int8_t i;

	cur_histentry++;
	i = find_cur_histentry();
	if(i == -1)
	{
		// tried to go up past the stored entries... ignore
		cur_histentry--;
	}
	else
	{
		load_histentry(i);
	}

}


void history_down(void)
{
	int8_t i;

	if(cur_histentry == -1)
		// already at the bottom
		return;

	if(--cur_histentry == -1)
	{
		// stepped down out of history again, show a blank line
		USB_print_ROM("\r\x1b[0K");
		USB_print_ROM(PROMPT);
		cmdbuf[0] = 0;
		num_tokens = 0;
	}
	else
	{
		i = find_cur_histentry();
		load_histentry(i);
	}

}


void process_command(void)
{
	unsigned int mond, mona;
	uint8_t pin, mode, t;

	parse_cmdline();
	add_to_history();
	if(tokens[0] == TOK_HELP)
		print_help();
	else if(tokens[0] == TOK_PING)
		USB_println_ROM("pong");
	else if(tokens[0] == TOK_RESET)
	{
		USB_println_ROM("Not implemented.");
	}
	else if(tokens[0] == TOK_READ)
	{
		if(num_tokens == 3)
		{
			if(tokens[2] < 0x80)
				print_help();
			else
			{
				pin = tokens[2] & 0x7f;
				if(tokens[1] == TOK_DPIN)
					show_digital_pin_status(pin);
				else if(tokens[1] == TOK_APIN)
					show_analog_pin_status(pin);
			}
		}
		else
			print_help();
	}
	else if(tokens[0] == TOK_SET)
	{
		if(num_tokens == 4)
		{
			if(tokens[1] == TOK_DPIN)
			{
				if(tokens[2] < 0x80)
					print_help();
				else
				{
					pin = tokens[2] & 0x7f;
					if(tokens[3] == TOK_HIGH)
						digitalWrite(pin, HIGH); //TODO
					else if(tokens[3] == TOK_LOW)
						digitalWrite(pin, LOW); //TODO
					else
						print_help();
				}
			}
			else if(tokens[1] == TOK_MODE)
			{
				if(tokens[2] < 0x80)
					print_help();
				else
				{
					pin = tokens[2] & 0x7f;
					if(tokens[3] == TOK_IN) //TODO
						pinMode(pin, INPUT);
					else if(tokens[3] == TOK_OUT)
						pinMode(pin, OUTPUT); //TODO
					else
						print_help();
				}
			}
			else
				print_help();
		}
		else
			print_help();
	}
	else if(tokens[0] == TOK_MONITOR)
	{
		mode = mond = mona = 0;
		t = 1;
		while(tokens[t])
		{
			if(mode == 0)
			{
				// top
				if(tokens[t] == TOK_DPIN)
					mode = 1;
				else if(tokens[t] == TOK_APIN)
					mode = 2;
				else
					print_help();
			}
			else
			{
				// in dpin or apin mode, expect pin numbers...
				if(tokens[t] & 0x80)
				{
					pin = 1 << (tokens[t] & 0x7f);
					if(mode == 1)
						mond |= pin;
					else
						mona |= pin;
				}
				else
				{
					// not a number, so back to top mode and parse this
					// keyword again the next time around the loop
					mode = 0;
					t--;
				}
			}
			t++;
		}
		if(mond == 0 && mona == 0)
			mond = mona = 0xffff;
		monitor(mond, mona);
	}
//	else if(tokens[0] == TOK_SERIAL)
//	{
//		if(num_tokens == 3 && tokens[1] & 0x80 && tokens[2] & 0x80)
//		{
//			// two numeric arguments found
//			connect_serial(tokens[1] & 0x7f, tokens[2] & 0x7f);
//		}
//		else
//			serial_print_P(HELP_TEXT);
//	}
	else
		USB_println_ROM("unknown command");

}


//void connect_serial(uint8_t rxpin, uint8_t txpin)
//{
//	char c, prev_char, d, buf[16];
//	int i;
//
//	serial_println_P(PSTR("Arsh serial passthrough mode (esc twice to quit)\r\n"));
//	afss_begin(rxpin, txpin, 9600);
//	pinMode(rxpin, INPUT);
//	digitalWrite(rxpin, HIGH);
//
//	prev_char = 0;
//	i = 0;
//	while(1)
//	{
//		// softserial
//		if(afss_available())
//		{
//			c = afss_read();
//			if(c > 0x20 && c < 0x7f)
//				d = c;
//			else
//				d = 0x2e;
//			snprintf(buf, 16, "[%.2x %c ] ", c, d);
//			serial_println(buf);
//		}
//
//		// hardware serial (shell console)
//		if(serialAvailable())
//		{
//			c = serialRead();
//			if(c == 0x1b && prev_char == 0x1b)
//				break;
//			afss_write(c);
//			prev_char = c;
//		}
//	}
//	serial_endline();
//
//}


//uint8_t softserial_read(uint8_t rxpin)
//{
//	uint8_t result, i;
//
//	result = 0;
//	while(digitalRead(rxpin));
//	if(digitalRead(rxpin) == LOW)
//	{
//		delayMicroseconds(42);
//		for (i = 0; i < 8; i++) {
//	     delayMicroseconds(84);
//	     result |= digitalRead(rxpin) << i;
//	    }
//		//wait for stop bit + extra
//		delayMicroseconds(84);
//		delayMicroseconds(84);
//	}
//
//	return result;
//}


void monitor(int dpins, int apins)
{
	unsigned long stamp;
	int val, done, init_done, acol, prevd, preva[6], row, drow, arow, i;
	char buf[6];

	// set up the display, values will be filled in later
	USB_print_ROM(MONITOR_TEXT);/*
	drow = arow = 0;
	for(i = 0; i <= 13; i++)
	{
		if(dpins & (1 << i))
		{
			USB_print_ROM("Digital pin "); //"%2d:", i
			if(i < 10)
				USB_putchar(' ');
			itoa(i, buf);
			USB_println(buf);
			USB_putchar(':'); 
			drow++;
		}
	}
	if(dpins == 0)
		acol = 0;
	else
		acol = 40;
	for(i = 0; i <= 5; i++)
	{
		if(apins & (1 << i))
		{
			USB_print_ROM("\x1b["); //"\x1b[%d;%dH", arow + 3, acol
			itoa(arow + 3, buf);
			USB_print(buf);
			USB_putchar(';');
			itoa(acol, buf);
			USB_print(buf);
			USB_putchar('H');
			
			USB_print_ROM("Analog pin "); //"Analog pin %d:", i
			itoa(i, buf);
			USB_print(buf);
			USB_putchar(':');
			
			arow++;
		}
	}
	if(arow > drow)
		drow = arow;

	done = init_done = prevd = 0;
	while(!done)
	{
		// digital pins
		row = 3;
		for(i = 0; i <= 13; i++)
		{
			if(dpins & (1 << i))
			{
				val = digitalRead(i); //TODO
				if(!init_done || val != (prevd & (1 << i)) >> i)
				{
					USB_print_ROM("\x1b["); //"\x1b[%d;17H", row
					itoa(row, buf);
					USB_print(buf);
					USB_print_ROM(";17H");
					
					if(val == LOW)
					{
						USB_println_ROM("LOW ");
						prevd &= !(1 << i);
					}
					else
					{
						USB_println_ROM("HIGH");
						prevd |= 1 << i;
					}
				}
				row++;
 			}
		}

		// analog pins
		row = 3;
		for(i = 0; i <= 5; i++)
		{
			if(apins & (1 << i))
			{
				val = analogRead(i); //TODO
				if(!init_done || val != preva[i])
				{
					unsigned char numlen;
					
					USB_print_ROM("\x1b["); //"\x1b[%d;%dH%4d", row, acol + 14, val
					itoa(row, buf);
					USB_print(buf);
					USB_putchar(';');
					itoa(acol + 14, buf);
					USB_print(buf);
					USB_putchar('H');
					itoa(val, buf);
					for(numlen = strlen(buf); numlen < 4; numlen++)
						USB_putchar(' ');
					USB_print(buf);
					
					preva[i] = val;
				}
				row++;
			}
		}

		USB_print_ROM("\x1b["); //"\x1b[%d;1H", drow + 3
		itoa(drow + 3, buf);
		USB_print(buf);
		USB_print_ROM(";1H");
		
		stamp = millis();
		while(millis() < stamp + MONITOR_FREQUENCY) //TODO: this needs to be modiefied to not block (set a flag the first time and execute it until the char == 0x1b)
		{
			delay(10);
			if(serialAvailable() > 0)
				if(serialRead() == 0x1b)
					done = 1;
		}

		init_done = 1;
	}

	// move down past the display area before returning to the prompt
	
	USB_print_ROM("\x1b["); //"\x1b[%d;1H", drow + 4
	itoa(drow + 4, buf);
	USB_print(buf);
	USB_print_ROM(";1H");*/
}


void read_all_pins(void)
{
	int i;

	for(i = 0; i <= 13; i++)
		show_digital_pin_status(i);

	for(i = 0; i <= 5; i++)
		show_analog_pin_status(i);

}


void show_digital_pin_status(int pin)
{
	char buf[4];
	
	USB_print_ROM("dpin "); //"dpin %d = %s", pin, state
	itoa(pin, buf);
	USB_print(buf);
	USB_print_ROM(" = ");
	
	if(digitalRead(pin) == LOW) //TODO
		USB_println_ROM("LOW");
	else
		USB_println_ROM("HIGH");
}

/*
void set_digital_pin(int pin) //set ???, it's the same as show_digital_pin_status and not used
{
	char buf[32], *state;

	if(digitalRead(pin) == LOW) //TODO
		state = "LOW";
	else
		state = "HIGH";
	snprintf(buf, 32, "dpin %d = %s", pin, state);
	USB_println(buf);

}
*/

void show_analog_pin_status(int pin)
{
	char buf[7]; //16bit

	USB_print_ROM("apin "); //"apin %d = %d", pin, analogRead(pin)
	itoa(pin, buf);
	USB_print(buf);
	USB_print_ROM(" = ");
	itoa(analogRead(pin), buf); //TODO
	USB_println(buf);
}


