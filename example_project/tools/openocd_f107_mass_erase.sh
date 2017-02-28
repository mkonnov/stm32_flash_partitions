#!/usr/bin/env bash

killall -9 openocd 1>/dev/null 2>&1
openocd -f interface/olimex-arm-usb-ocd-h.cfg -f board/olimex_stm32_h107.cfg -c "init" -c "reset halt" -c "flash erase_address 0x08000000 0x40000" -c "exit" 
killall -9 openocd 1>/dev/null 2>&1
