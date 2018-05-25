# Yamaha Natural Sound Amplifier A-CC5 makeover with an Atmega328P and TPA2016

This repository contains firmware code for my personal Atmega based sound amplifier.

The idea was to replace the my old Yamaha A-CC5 amplifier which consumed huge amounts of
electricity even in standby. The code drives an Atmega328P and the below listed peripherals.

See datasheets directory for IC datasheets.

## TPA2016 amplifier 

 * Controlled through I2C

## SSD1306 OLED display 

 * Controlled through I2C

## Yamaha A-CC5 frontplate 

 * Comes with 9 LEDs
 * .. 2 push buttons 
 * .. 3 potentiometers
 * .. IR diode
 * .. audio jack
 * Controlled through single pins / reverse engineered connector 
 
 Some LEDs on the frontplate are controlled by an MC14052B analog multiplexer.
