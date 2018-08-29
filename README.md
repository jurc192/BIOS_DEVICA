# BIOS DEVICA README

This is a program intended for creating and checking-out various BIOS hacks made by students.
It is an text-user-interface for checking out the repository of various assembly programs for BIOS,
for writing new programs and burning them to floppy disks.


## How it works

The program is written in C, using the [ncurses] library.
Compile with: `make`
Run the program with: `make run`


## Prerequisites

BIOS DEVICA depends on few external programs. It is written and tested on Ubuntu 16.04 LTS and Ubuntu 18.04 LTS.
It requires: nasm, qemu, libncurses5-dev and make. 
All of the programs can be installed in Ubuntu, using `sudo apt install <name of the program>`


Program also uses [Atom text editor], for the "write a hack" part of the program.

[ncurses]: https://en.wikipedia.org/wiki/Ncurses 
[Atom text editor]: https://atom.io/ 


## Configuration

In order to run the program successfully, you need to edit the configuration file in: `inc/config.h`.
HACKS_DIR is a path to a folder, which contains all the BIOS hacks.
SKELETON_DIR is a path to a folder with default files, which are copied when creating a new hack.

BIOS_DEVICA searches the HACKS_DIR and looks for folders containing `README.md`. 
All subfolders of HACKS_DIR containing theese files are listed in the "HACKS" part of the program.