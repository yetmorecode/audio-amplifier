
CC = avr-gcc
OBJCOPY = avr-objcopy

MCU = atmega328p

TARGET = button

SRC = $(TARGET).c

OBJ = $(SRC:.c=.o)

ALL_CFLAGS = -mmcu=$(MCU) -Os $(CFLAGS)

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

program: $(TARGET).hex
	avrdude -c linuxspi -p m328p -P /dev//spidev0.0 -U flash:w:$(TARGET).hex:a
