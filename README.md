# atari_jaguar_240p_test_suite

## NOTE
This software is a work in progress. When compared with other ports of the 240p Test Suite, some functionality is missing. There are also Jaguar specific functions that still need to be implemented. Please see the issues section for more details.

## Introduction
_As described on https://github.com/ArtemioUrbina/240pTestSuite_

The 240p test suite is a homebrew software suite for video game consoles developed to help in the evaluation of upscalers, upscan converters and line doublers.

It has tests designed with the processing of 240p signals in mind, although when possible it includes other video modes and specific tests for them. These have been tested with video processors on real hardware and a variety of displays, including CRTs and Arcade monitors via RGB.

As a secondary target, the suite aims to provide tools for calibrating colors, black and white levels for specific console outputs and setups.

This is free software, with full source code available under the GPL.

Learn more about the 240p Test Suite by Artemino at http://junkerhq.net/240p/

## Compiling & Running
Compiling requires the following tools/libraries:
- m68k-atari-mint cross-tools (cross compiler - Needed for building and using the Removers Library) - https://tho-otto.de/crossmint.php
- RMAC (compiler) - http://rmac.is-slick.com/
- RLN (linker) - http://rmac.is-slick.com/
- Removers Library (C Library) - https://github.com/theRemovers/rmvlib/
- Ray's lz77 Packer (Optional: Only if you plan on modifying graphics) - http://s390174849.online.de/ray.tscc.de/files/lz77_v13.zip
- JCP (Optional: If you are flashing the program to a Skunkboard) - http://harmlesslion.com/software/skunkboard

If you are running a linux environment that has access to the latest Ubuntu repositories, I have a set of bash scripts that automates the setup of a Jaguar devolopment environment on your linux system.  See the following for more details: https://github.com/BitJag/ubuntu-rmvlib-install-scripts

Running the 240p Test Suite without crashing on real hardware requires a working ROM file.  Currently, if running the program from RAM space by using the .COF file, this works for most tests, but will crash for some tests that need to unpack assets that will exceed the available space in RAM, crashing the program.

A ROM file can be produced by passing the file through Jiffi, or adding a universal cart header to the .BIN file manually or using makefastboot.

- Jiffi - https://reboot.untergrund.net/new-reboot/jiffi.html
- makefastboot - https://github.com/tursilion/makefastboot/

Finally, you can run your ROM file by using an emulator, a flash card reader, Skunkboard, or by creating your own physical cartridge. Currently there isn't anyone I'm aware of that is producing carts of the Jaguar 240p Test Suite.

## Contributors
```
* Code: Artemio Urbina
* Code: William Thorup
* Main menu graphics: Asher
* Extra patterns and collaboration: Konsolkongen & shmups regulars
```
=============================================================================== Copyright 2011-2022 Artemio Urbina

Atari Jaguar Version Copyright 2022 William Thorup (BitJag)

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
