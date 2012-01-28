#!bin/bash
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c main.c gpio.c vector.c rcc.c flash.c 
arm-none-eabi-ld -T linker.ld main.o gpio.o vector.o rcc.o flash.o -o main.elf
arm-none-eabi-objcopy -O binary main.elf main.bin
cp main.bin /home/swatee/CodeSourcery/openocd-0.5.0/src/ 
cp main.elf /home/swatee/CodeSourcery/openocd-0.5.0/src/  
modprobe ftdi_sio vendor=0x15ba product=0x002a
gnome-terminal --tab -e "sudo openocd -f /home/mandar/CodeSourcery/openocd-0.5.0/tcl/interface/olimex-arm-usb-tiny-h.cfg -f /home/mandar/CodeSourcery/openocd-0.5.0/tcl/board/olimex_stm32_h107.cfg" --tab -e "sudo telnet localhost 4444" --tab -e "arm-none-eabi-gdb"

