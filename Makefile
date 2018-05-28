
CC = avr-gcc
OBJCOPY = avr-objcopy

MCU = atmega328p

TARGET = audio

SRC = $(TARGET).c i2c.c ssd1306.c tpa2016.c yamaha_acc5.c timing.c

OBJ = $(SRC:.c=.o)

ALL_CFLAGS = -mmcu=$(MCU) -Os $(CFLAGS) -DF_CPU=1000000UL

all: gccversion $(TARGET).elf $(TARGET).hex size

gccversion:
	$(CC) --version

size:
	avr-size --mcu=$(MCU) -C $(TARGET).elf

%.hex: %.elf
	$(OBJCOPY) -O ihex -j .text -j .data $< $@

%.elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) -o $@

%.o : %.c
	$(CC) $(ALL_CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET).hex
	rm -f $(TARGET).elf
	rm -f $(OBJ)

program: all
	avrdude -c linuxspi -p m328p -P /dev//spidev0.0 -U flash:w:$(TARGET).hex:a

test: all
	avrdude -n -c linuxspi -p m328p -P /dev//spidev0.0 -U flash:w:$(TARGET).hex:a


.PHONY: all gccversion size clean program
