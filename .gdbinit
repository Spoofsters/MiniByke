set serial baud 115200
target remote /dev/ttyACM0
file main.elf
load
layout src