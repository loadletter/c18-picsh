#include <string.h>
#include "./USB/usb_function_cdc.h"

/* USB communication functions */

void USB_endline(void)
{
	if(mUSBUSARTIsTxTrfReady())
		putrsUSBUSART("\r\n");
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
}

void USB_print(char *c)
{
	if(mUSBUSARTIsTxTrfReady())
		putsUSBUSART(c);
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
}

void USB_print_ROM(const char *c)
{
	if(mUSBUSARTIsTxTrfReady())
		putrsUSBUSART(c);
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
}

void USB_println(char *c)
{
	if(mUSBUSARTIsTxTrfReady())
		putsUSBUSART(c);
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
	USB_endline();
}

void USB_println_ROM(const char *c)
{
	if(mUSBUSARTIsTxTrfReady())
		putrsUSBUSART(c);
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
	USB_endline();
}

void USB_putchar(char c)
{
	char buf[] = "x\0";
	buf[0] = c;
	if(mUSBUSARTIsTxTrfReady())
		putsUSBUSART(buf);
	while(!USBUSARTIsTxTrfReady())
		CDCTxService(); 
}
