# Copyright (C) 2010 Enrico Rossi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

MCU = atmega164p
PWD = $(shell pwd)
INC = -I/usr/avr/include/
OPTLEV = 2
FCPU = 1000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)

AR = avr-ar
CC = avr-gcc

DUDEPORT = /dev/ttyUSB0
DUDEDEV = stk500v2
PRGNAME = pwm
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c $(DUDEDEV) -p $(MCU) -P $(DUDEPORT) -e -U flash:w:$(PRGNAME).hex
REMOVE = rm -f

objects = counter.o main.o

GITRELEASE = "\"$(shell git describe --tags)\""

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(objects)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

program:
	$(DUDE)

clean:
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex $(objects)

