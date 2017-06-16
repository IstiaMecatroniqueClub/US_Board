# Istia Mecatronique Club - US Board

## Synopsis

This repository contains the source codes and the schematics of our US board. This board has an Ultra-Sound sensor and a CAN bus interface, controlled by an ATMega 32m1.

## Repository architecture

This repository contains a "board" folder with the schematics of the board, a "code" folder with the source code of the ATMega and a "documentation" folder with the doxygen documentation of the source code.

## The source code

A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.

## Installation

### The ATMega .hex file
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
