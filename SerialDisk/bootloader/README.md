There are actually two boot loaders here:

The original boot loader written by Kyle Owen, toggled into locations
20-44 (bootloader.pal).

A newer bootloader written by Doug Ingraham, which toggles into
locations 30-40 (helpbt.pal, helpbt.txt).

I've also added SIMH scripting here to test the newer boot loader.

The hlpgen.c program can be used to assist with hand encoding of HELP
bytecodes, but isn't really needed since the server has code to do
that on the fly.
