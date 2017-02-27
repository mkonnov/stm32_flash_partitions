#!/usr/bin/env bash

function flash_dfu {
	dfu-util --device , -a 0 -s "$2" -D "$1"
}

function flash_dfu_leave {
	dfu-util --device , -a 0 -s "$2":leave -D "$1"
}

flash_dfu_leave ../build/partitions.bin 0x08000000
