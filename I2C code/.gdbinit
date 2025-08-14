set arch avr
set serial baud 115200
target remote /dev/ttyACM0
file main.elf
layout src
focus cmd

