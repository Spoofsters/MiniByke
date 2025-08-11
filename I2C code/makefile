CC      = avr-gcc
CFLAGS += \
    -DF_CPU=16000000ULL \
    -O0 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -mmcu=atmega328p \
    -std=gnu11 \

.PHONY: all
all: main.elf

.PHONY: clean
clean:
	@rm -rvf *.d *.elf *.o

main.elf: main.o twi.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MM -MF $*.d $<
	$(CC) $(CFLAGS) -c $<

-include $(wildcard *.d)