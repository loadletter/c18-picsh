# MPLAB IDE generated this makefile for use with GNU make.
# Project: USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.mcp
# Date: Sun Dec 22 11:31:35 2013

AS = MPASMWIN.exe
CC = /opt/microchip/mplabc18/v3.40/bin/mcc18
LD = /opt/microchip/mplabc18/v3.40/bin/mplink
AR = /opt/microchip/mplabc18/v3.40/bin/mplib
RM = rm -f

USB\ Device\ -\ CDC\ -\ Basic\ Demo\ -\ \ C18\ -\ PICDEM\ FSUSB.cof : Objects/PICDEM\ FSUSB/usb_descriptors.o Objects/PICDEM\ FSUSB/main.o Objects/PICDEM\ FSUSB/usb_function_cdc.o Objects/PICDEM\ FSUSB/usb_device.o
	$(LD) -p 18F4550 "rm18f4550 - HID Bootload.lkr" -l"/opt/microchip/mplabc18/v3.40/lib" "Objects/PICDEM FSUSB/usb_descriptors.o" "Objects/PICDEM FSUSB/main.o" "Objects/PICDEM FSUSB/usb_function_cdc.o" "Objects/PICDEM FSUSB/usb_device.o" -u_CRUNTIME -z__MPLAB_BUILD=1 -o"USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.cof" -m"USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.map" -w

Objects/PICDEM\ FSUSB/usb_descriptors.o : usb_descriptors.c /opt/microchip/mplabc18/v3.40/h/stdio.h /opt/microchip/mplabc18/v3.40/h/stdlib.h /opt/microchip/mplabc18/v3.40/h/string.h ../../Microchip/Include/USB/usb_common.h ../../Microchip/Include/USB/usb_device.h ../../Microchip/Include/USB/usb_hal.h ../../Microchip/Include/USB/usb_function_cdc.h usb_descriptors.c ../../Microchip/Include/USB/usb.h ../../Microchip/Include/GenericTypeDefs.h /opt/microchip/mplabc18/v3.40/h/stddef.h ../../Microchip/Include/Compiler.h /opt/microchip/mplabc18/v3.40/h/p18cxxx.h /opt/microchip/mplabc18/v3.40/h/p18f4550.h /opt/microchip/mplabc18/v3.40/h/stdarg.h usb_config.h /opt/microchip/mplabc18/v3.40/h/limits.h ../../Microchip/Include/USB/usb_ch9.h ../../Microchip/Include/USB/usb_hal_pic18.h
	mkdir -p "Objects/PICDEM FSUSB"
	$(CC) -p=18F4550 -I"/opt/microchip/mplabc18/v3.40/h" -I"../../Microchip/Include" -I"." "usb_descriptors.c" -fo="./Objects/PICDEM FSUSB/usb_descriptors.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Objects/PICDEM\ FSUSB/main.o : main.c /opt/microchip/mplabc18/v3.40/h/stdio.h /opt/microchip/mplabc18/v3.40/h/stdlib.h /opt/microchip/mplabc18/v3.40/h/string.h ../../Microchip/Include/USB/usb_common.h ../../Microchip/Include/USB/usb_device.h ../../Microchip/Include/USB/usb_hal.h ../../Microchip/Include/USB/usb_function_cdc.h main.c ../../Microchip/Include/USB/usb.h ../../Microchip/Include/GenericTypeDefs.h /opt/microchip/mplabc18/v3.40/h/stddef.h ../../Microchip/Include/Compiler.h /opt/microchip/mplabc18/v3.40/h/p18cxxx.h /opt/microchip/mplabc18/v3.40/h/p18f4550.h /opt/microchip/mplabc18/v3.40/h/stdarg.h usb_config.h /opt/microchip/mplabc18/v3.40/h/limits.h ../../Microchip/Include/USB/usb_ch9.h ../../Microchip/Include/USB/usb_hal_pic18.h HardwareProfile.h HardwareProfile\ -\ PICDEM\ FSUSB.h
	mkdir -p "Objects/PICDEM FSUSB"
	$(CC) -p=18F4550 -I"/opt/microchip/mplabc18/v3.40/h" -I"../../Microchip/Include" -I"." "main.c" -fo="./Objects/PICDEM FSUSB/main.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Objects/PICDEM\ FSUSB/usb_function_cdc.o : ../../Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c /opt/microchip/mplabc18/v3.40/h/stdio.h /opt/microchip/mplabc18/v3.40/h/stdlib.h /opt/microchip/mplabc18/v3.40/h/string.h ../../Microchip/Include/USB/usb_common.h ../../Microchip/Include/USB/usb_device.h ../../Microchip/Include/USB/usb_hal.h ../../Microchip/Include/USB/usb_function_cdc.h ../../Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c ../../Microchip/Include/USB/usb.h ../../Microchip/Include/GenericTypeDefs.h /opt/microchip/mplabc18/v3.40/h/stddef.h ../../Microchip/Include/Compiler.h /opt/microchip/mplabc18/v3.40/h/p18cxxx.h /opt/microchip/mplabc18/v3.40/h/p18f4550.h /opt/microchip/mplabc18/v3.40/h/stdarg.h usb_config.h /opt/microchip/mplabc18/v3.40/h/limits.h ../../Microchip/Include/USB/usb_ch9.h ../../Microchip/Include/USB/usb_hal_pic18.h HardwareProfile.h HardwareProfile\ -\ PICDEM\ FSUSB.h
	mkdir -p "Objects/PICDEM FSUSB"
	$(CC) -p=18F4550 -I"/opt/microchip/mplabc18/v3.40/h" -I"../../Microchip/Include" -I"." "../../Microchip/USB/CDC Device Driver/usb_function_cdc.c" -fo="./Objects/PICDEM FSUSB/usb_function_cdc.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Objects/PICDEM\ FSUSB/usb_device.o : ../../Microchip/USB/usb_device.c /opt/microchip/mplabc18/v3.40/h/stdio.h /opt/microchip/mplabc18/v3.40/h/stdlib.h /opt/microchip/mplabc18/v3.40/h/string.h ../../Microchip/Include/USB/usb_common.h ../../Microchip/Include/USB/usb_device.h ../../Microchip/Include/USB/usb_hal.h ../../Microchip/USB/usb_device.c ../../Microchip/Include/USB/usb.h ../../Microchip/Include/GenericTypeDefs.h /opt/microchip/mplabc18/v3.40/h/stddef.h ../../Microchip/Include/Compiler.h /opt/microchip/mplabc18/v3.40/h/p18cxxx.h /opt/microchip/mplabc18/v3.40/h/p18f4550.h /opt/microchip/mplabc18/v3.40/h/stdarg.h usb_config.h /opt/microchip/mplabc18/v3.40/h/limits.h ../../Microchip/Include/USB/usb_ch9.h ../../Microchip/Include/USB/usb_hal_pic18.h HardwareProfile.h HardwareProfile\ -\ PICDEM\ FSUSB.h ../../Microchip/USB/usb_device_local.h
	mkdir -p "Objects/PICDEM FSUSB"
	$(CC) -p=18F4550 -I"/opt/microchip/mplabc18/v3.40/h" -I"../../Microchip/Include" -I"." "../../Microchip/USB/usb_device.c" -fo="./Objects/PICDEM FSUSB/usb_device.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "Objects/PICDEM FSUSB/usb_descriptors.o" "Objects/PICDEM FSUSB/main.o" "Objects/PICDEM FSUSB/usb_function_cdc.o" "Objects/PICDEM FSUSB/usb_device.o" "USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.cof" "USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.hex" "USB Device - CDC - Basic Demo -  C18 - PICDEM FSUSB.map"

