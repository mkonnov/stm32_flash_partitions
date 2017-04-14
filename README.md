# stm32-flash-partitions

#WHAT
A simple solution for STM32 flash management.

#WHY
Imagine you have a bootloader area, firmware(s) and settings ones.
Dealing with a flash adresses directly starts to be hurtable and
you always need to keep in mind all areas addresses lengths
(ok, you can note it in comments, but this is not why we're here, right ?).


So using this simple solution will keep you from direct partition addresses, lengths and
make you focus on more important tasks.

#TODO

Kill warnings


Get rid of X-macros and initialize partitions by names, start addresses etc. at init stage


Make it more generic and platform-indepentent.




