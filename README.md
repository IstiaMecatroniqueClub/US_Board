# Istia Mecatronique Club - US Board

## Synopsis

This repository contains the source codes and the schematics of our US board. This board has an Ultra-Sound sensor and a CAN bus interface, controlled by an ATMega 32m1.

## Repository architecture

This repository contains a "board" folder with the schematics of the board, a "code" folder with the source code of the ATMega and a "documentation" folder with the doxygen documentation of the source code.

## The source code

### How does it work

The board waits for a remote request CAN message with the IDMSGCAN identifier (0x0C0), then returns a data CAN message with the same identifier and the sensor value (the distance over 2 bytes). Note that the board can also return an error value (if the returned distance is 0, 1 or 2 it means that an error occured).

### The files

The "code/include" folder contains 
 - a Pin interface class to handle a pin on the ATMega (this class sould not be instanciated)
 - an Input class to handle an input pin
 - an Ouput class to handle an output pin
 - a Led class to handle an led connected to the ATMega
 - a Hcrs04 class to handle the US sensor
 - a Timer class to handle timers in the ATMega

### The ATMega output.hex file
In the code folder you can find a Makefile to generate the binary *.hex file and upload it to the ATMega. The makefile is configured for
 - avrdude compiler ( $ sudo apt-get install avrdude avr-libc)
 - AVRispmkII programmer

Then do
 - $ make hex # to generate the binary ouput.hex
 - $ make upload # to upload the output.hex file to the ATMega
 - $ make all  # to compile and then upload 

This has been tester with Ubuntu and Lubuntu on a raspberry pi 3.

## The documentation
We use Doxygen to generate the documentation of the source code. By using the makefile (in the code folder) you can do
 - $ make documentation
To generate the documentation files.

Note that you need to have doxygen and graphviz (for the graphs) installed
 - $ sudo apt-get install doxygen
 - $ sudo apt-get install graphviz


## Tests

When powering the board, the LED should blink a few times and then stop.

## License

GNU General Public License v3.0.
