# 8tools directory
TOOLS	= ../tools
# Cross assembler.  Palbart should also work.
PAL	= ../tools/pal

all:	sdskns.bin sdsksy.bin

clean:

%.bin:	%.pal
	$(PAL) -d $<
