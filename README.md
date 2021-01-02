# MbrOverWritter

Here is a simple malware that overrides the MBR sector of the disk.
The malware was written for a school project. It is supposed to demonstrate in a simple way how a spyware works.

## Architecture
The malware was written for a windows 10 operating system.

## Contents
1. asmCode.asm
	Contains the code for the MBR. 
	It was written for x86 architecture as it is available on most machines.
2. data.bin 
	Binary file for asmCode - compiled with nasm tool.
3. mbrOverwriter.cpp - Source file for the malware

