#ifndef USBIO_H_
#define USBIO_H_

void init_usbio(char *c);
void USB_endline(void);
void USB_print(char *c);
void USB_print_ROM(const char *c);
void USB_println(char *c);
void USB_println_ROM(const char *c);
void USB_putchar(char c);
#endif
