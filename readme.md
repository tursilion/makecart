20130330

This program takes a TI program image file, broken into
the usual 8k file chunks and saved as RAW, TIDATA or V9T9
FIAD files, and combines them into a single GROM file 
to work on emulators.

The resulting file is probably not too useful on a real TI,
because (a) GROMs are hard to write to, and (b) the result
still requires that the TI have memory expansion attached.

Browse for the first file in the load chain. Type in a name
to appear on the "Press to Select" menu (max 21 chars).

'Use 8k GROMs' may be useful if you need a little more memory.
Real GROMs are only >1800 bytes long, but nothing in the system
prevents >2000 byte GROMs from working. In fact, some GROM
emulators did so in the real machine, and so far as I know, all
emulators can use them. I guess I was just being purist.

Enable Editor/Assembler Utilities, see below. You will probably
need to enable this for C99 programs. You should only need it
for programs that directly access the Editor/Assembler cart or
rely on it's utilities without providing their own.

Load character set adds code to ensure the default character set
is correctly loaded into memory before starting. This code is
99/4 aware and will simulate the Editor/Assembler setup on that
machine (ie: two copies of the uppercase letters, however, the 
two dots are not added to the lowercase versions).

Details Wnd will pop up a console window that will provide
additional detailed information about the process - press it
before you press Begin.

Begin will perform the conversion then ask for a filename to 
save to. Normally you won't use an extension, and for full
compatibility, type no more than 7 characters (mostly for V9T9,
and this is not enforced). If the filename does not end with G,
then 'G' will be appended before saving.

Editor/Assembler cartridge GROM is (C) Texas Instruments,
and distributed under license from Texas Instruments.

You might wonder what the large warning when you enable
Editor/Assembler Utilities is all about? In order to support
C99 programs using the standard C99PFI file, the Editor/
Assembler utility routines must be in GROM at >7000 for it
to copy them into low memory. Otherwise, C99 programs will
hang. 

This switch copies the entire cartridge over, mostly because
it's too much trouble to squeak code in around it (and most
programs won't need to). Whether the whole cartridge or only
a portion, that code is owned by TI and due to my license I'm
obligated to remind you of that.

The best way to work around it is, in your own C99 programs,
write your own VDP utilities, and replace C99PFI with a 
simple stub that just jumps to the START label. Some people
have also snuck around it by saving low memory into a separate
file and including that in the loader chain (but, that's
technically still distributing TI's code ;) ).

As of version 1.1, you can specify which GROM you want the
image assembled for. Valid GROMs are 1-7 (addresses >2000
through >E000). GROM 0 is available in the program but it
is unlikely you will be able to use it unless you really
know what you are doing.

If you enable Editor/Assembler, the GROM index will be forced
to 3 for the correct load address. If you know you don't need
that, you can hold shift while clicking to override that behaviour.

TROUBLESHOOTING
The basics are:
- Program too large - try enabling 2k GROMs
- Corrupted or missing graphics - try loading the character set
- Program crashes or misbehaves - try enabling Editor/Assembler 
 (this means program probably wouldn't work under XB either)
- Too many segments - this program can't condense segments, you 
 will need to merge the files some!
- E/A #3 - convert to E/A #5 with the SAVE utility first
- Cart doesn't work at selected base - check if it works at #3 first.
 if it does, let me know! :)
