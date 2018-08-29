# BIOS programming quickstart guide

This is a quick introduction to writing assembly programs for BIOS.

After the computer turns on, BIOS firmware performs hardware initialization
and starts executing the program at the [Master Boot Record] (MBR) sector of the disk.
Usually this program is a bootloader, which takes care of the operating system 
initialization. In our case, we are going to write our own programs and overwrite
the Master Boot Record. This way, every time the computer turns on it will
execute our program instead of loading the operating system.

[Master Boot Record]: https://wiki.osdev.org/MBR_(x86) 


## Basics

The programs executed from the MBR are running in the [x86 Real mode]. This is a 16-bit mode of operation,
present on all modern computers to maintain backwards compatibility. In Real mode we can use BIOS interrupts
and up to 1 MB of memory. Working with memory in this mode is a bit tedious, due to the [segmented addressing mode].
This memory addressing mode enables us to read/write into 20-bit address memory space, using 16-bit registers.

Example of reading two bytes at memory address 0x80100
```
; DS = 0x8000
mov al, ds:[0x0100]
```

[x86 Real mode]: https://wiki.osdev.org/Real_Mode
[segmented addressing mode]: https://en.wikipedia.org/wiki/X86_memory_segmentation 


## BIOS Interrupts

For input/output programming, we can use BIOS interrupts. The best resource for BIOS interrupts is 
[Ralph Brown's interrupt list](http://ctyme.com/intr/int.htm). The website looks funny and old, but
it contains the most valuable documentation of x86 BIOS interrupts.

For the list of most popular and usable interrupts, check [this page](https://en.wikipedia.org/wiki/BIOS_interrupt_call)  

Example of using a BIOS interrupt, to set the video mode.
```
    ; Set video mode 80x25, 16 color
    mov ah, 0x00
    mov al, 0x03    ; video mode 0x03
    int 0x10        ; interrupt 0x10
```


## Literature

There are some books that offer a great introduction to x86 assembly programing with BIOS.  
[Writing a Simple Operating System from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf) by Nick Blundell.  
[The Art of Assembly Language](http://www.ic.unicamp.br/~pannain/mc404/aulas/pdfs/Art%20Of%20Intel%20x86%20Assembly.pdf)  


Web resources and examples:  
[http://www.c-jump.com/CIS77/ASM/Memory/lecture.html]  
[https://hackaday.com/2017/10/23/write-your-own-x86-bootloader/]  
[https://github.com/cirosantilli/x86-bare-metal-examples/tree/d217b180be4220a0b4a453f31275d38e697a99e0]  
[https://wiki.osdev.org/Main_Page]  



## Tools

The programs are written in assembly language. We are using NASM assembler with
Atom text editor. Makefile is used to assemble the program, to make an disk image file.
The disk image file contains our program written in MBR sector of the disk and is set
to be bootable.
