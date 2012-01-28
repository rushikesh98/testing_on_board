
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4")
SEARCH_DIR("C:\Program Files\Raisonance\Ride\Lib\ARM")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\startup_stm32f10x_hd.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\main.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_it.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_desc.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_endp.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_istr.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_prop.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_pwr.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\hw_config.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_usart.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_exti.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_flash.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_fsmc.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_gpio.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_pwr.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_rcc.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32f10x_tim.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\misc.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\otgd_fs_cal.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\otgd_fs_dev.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\otgd_fs_int.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\otgd_fs_pcd.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_core.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_init.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_int.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_mem.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_regs.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\usb_sil.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\stm32_eval.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\core_cm3.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\system_stm32f10x.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\util.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\button.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\draw.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\font_spe.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\lcd.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\lcd_spe.o")
INPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\lcd_spe_IL9325.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\STM32F103_512K_64K_FLASH.ld"
OUTPUT("C:\Downloaded Files\Virtual-Com-2\Virtual-Com-2\Project\Virtual_COM_Port\RIDE\STM3210E-OPEN4\VirtualCOMPort.elf")
