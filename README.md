Mupen64plus-pandora
===================

Mupen64plus for Pandora

OpenPandora
===========

This fork is aimed at OpenPandra, so you get:
 * ARM compatibilty
 * Some NEON optimized code
 * GLES v1.1 or v2.0 renderer
 
Most code come from mupen64plus-ae, from PAULSCODE, the Android Edition of Mupen64plus.
 
For the Pandora version, I used some Hardwired modif in the Construct file
To compile (using code::blocks PND), go inside each prject/unix and build with "make all"
There are 2 exeption:
for the core, you need to type "make all OSD=0" because OSD is not supported for now.
For Notaz sound plugin, there is no project/unix, so the "make all" has to be typed it the base folder, where everything is.

For more info on the OpenPandora go here: http://boards.openpandora.org/
