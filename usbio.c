#include <string.h>
/* USB communication functions */

char *usbbuf = NULL;

void init_usbio(char *c)
{
	usbbuf = c;
}

void USB_endline(void)
{
	strcatpgm2ram(usbbuf, "\r\n");
}

void USB_print(char *c)
{
	strcat(usbbuf, c);
}

void USB_print_ROM(const char *c)
{
	strcatpgm2ram(usbbuf, c);
}

void USB_println(char *c)
{
	strcat(usbbuf, c);
	USB_endline();
}

void USB_println_ROM(const char *c)
{
	strcatpgm2ram(usbbuf, c);
	USB_endline();
}

void USB_putchar(char c)
{
	unsigned char buflen = strlen(usbbuf);
	usbbuf[buflen + 1] = 0;
	usbbuf[buflen] = c;
}
