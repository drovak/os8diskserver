/* file:    pal.c

   purpose: a 2 pass PDP-8 pal-like assembler

   author:  Douglas Jones <jones@cs.uiowa.edu> -- built basic bits
            Rich Coon <coon@convexw.convex.com> -- added enough handle OS/278
            Bernhard Baehr <b.baehr@madsack.de> -- patch to correct checksum
            Vincent Slyngstad <vrs@msn.com> -- various fixes and enhancements
            Vincent Slyngstad <vrs@msn.com> -- added LMODE support (June 2021)

   disclaimer:  This assembler accepts a subset of the PAL 8 assembly language.
	It was written as a quick hack in order to download software into my
	bare-bones PDP-8 system, and no effort has been made to apply sound
	software engineering techniques in its design.  See the symbol table
	and code for the set of pseudo-ops supported.  See DEC's "Programming
	Languages (for the PDP/8)" for complete documentation of pseudo-ops,
	or see DEC's "Introduction to Programming (for the PDP/8)" or a
	lower level introduction to the assembly language.

   use: After assembly on a UNIX system, this program should be stored as
	the pal command.  This program uses the following file name extensions

		.pal	source code (input)
		.lst	assembly listing (output)
		.bin	assembly output in DEC's bin format (output)
		.rim	assembly output in DEC's rim format (output)

	This program takes the following command line switches

		-d	dump the symbol table at end of assembly
		-e	Define extended (Omnibus-era, EAE) instructions
		-j	Do not pad TEXT or SIXBIT
		-l	do not warn about offpage references
		-r	produce output in rim format (default is bin format)

	Needed soon:
		-c	Use the PAL8 predefined symbol table

	PQS8 assembler switches:
		-a	Include all symbols with -s
		-b	Use P?S/8 '%' area for binary output (N/A)
		-d	Include P?S/8 '$' area for binary output (N/A)
		-f	Use high speed punch if punching tape (N/A)
		-g	Load (or punch) the resulting binary (N/A)
		-h	If -g but not -w, preload memory with HLT (N/A)
		-i	If -g but not -w, reload the system handler (N/A)
		-j	Do not pad TEXT or SIXBIT
		-k	Enable title per file
		-l	Produce a listing
		-m	Generate a bitmap
		-n	Add niceties (date, etc) to listing
		-o	Enable link generation, flagged unless -q
		-p	Use wide ("printer") format (N/A)
		-q	Enable literals.  (No links unless -o.)
		-r	Output the binary in RIM format.
		-s	Dump the symbol table
		-t	List on console, not the printer. (N/A)
		-u	Choose an alternate unit for output. (N/A)
		-w	Punch the binary output. (N/A)
		-x	Generate a cross reference listing.
		-y	Change "!" operator to left shift six bits.
		-z	If -g, preload memory with 0000 (N/A)
		-0	Automatically generate *200 after FIELD directive
		-1	Set the starting address memory field (N/A)
		-2	Set the starting address memory field (N/A)
		-3	Set the starting address memory field (N/A)
		-4	Set the starting address memory field (N/A)
		-5	Set the starting address memory field (N/A)
		-6	Set the starting address memory field (N/A)
		-7	Set the starting address memory field (N/A)
		-8	Enable LINC and PDP-8 dual mode assembly at 0200
		-9	Enable LINC and PDP-8 dual mode assembly at 4020
		=nnnn	Specify starting address (N/A)
	A whole mess of these only make sense in the PQS8 environment, and
	are marked (N/A) above.  These are neither implemented, nor expected
	to be implemented in the future.  A possible exception would be if
	folks *really* wanted to generate self starting BIN/RIM output.
	Here's the short list for present or future implementation:
		-a	Include all symbols with -s
		-j	Do not pad TEXT or SIXBIT
		-k	Enable title per file
		-l	Produce a listing
		-m	Generate a bitmap
		-n	Add niceties (date, etc) to listing
		-o	Enable link generation, flagged unless -q
		-q	Enable literals.  (No links unless -o.)
		-r	Output the binary in RIM format.
		-s	Dump the symbol table
		-x	Generate a cross reference listing.
		-y	Change "!" operator to left shift six bits.
		-0	Automatically generate *200 after FIELD directive
		-8	Enable LINC and PDP-8 dual mode assembly at 0200
		-9	Enable LINC and PDP-8 dual mode assembly at 4020

   missing features:  The ASMIF[MNZ] stuff from sabr and pqs8.
	The strange undocumented IOTs to control the PDP-12 LINC registers.
	The two character (sixbit) literals ('AB).
	Many source files seem to use undefined symbols in IFNZRO.  Is there
	  a practice of assuming undefined symbols will test as zero?
	Support for FLTG or DUBL pseudo-ops (FLAP E and F?).
	Support for IFPOS, IFNEG, IFSW, IFNSW, IFFLAP or IFRALPH pseudo-ops.
	Support for IFREF, LISTON, LISTOF, REPEAT pseudo-ops.
	Several source files want to allow spaces before '='.

   known bugs:  Only a minimal effort has been made to keep the listing
	format anything like the PAL-8 listing format.  It screams too loud
	for off-page addresses (a quote mark would suffice), and it doesn't
	detect off-page addresses it can't fix (it should scream about them).

	Posix conformant systems are expected to ignore the "b" in the mode 
	argument fopen() calls, which is required by Windows and DOS when
	opening non-text files, such as the output binary.

   warrantee:  If you don't like it the way it works or if it doesn't work,
	that's tough.  You're welcome to fix it yourself.  That's what you
	get for using free software.

   copyright notice:  I wrote this and gave it away for free; if you want to
	make changes and give away the result for free, that's fine with me.
	If you can fix it enough that you can sell it, ok, but don't put any
	limits on the purchaser's right to do the same.  All the above aside,
	if you improve it or fix any bugs, it would be nice if you told me
	and offered me a copy of the new version.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define isend(c) ((c=='\0')||(c=='\n')||(c=='\r'))
#define isdone(c) ( (c == '/') || (isend(c)) || (c == ';') )

#define LINC
/* connections to command line */
#define NAMELEN 128
FILE *in;		/* input file */
char *filename = NULL;	/* input file's name */
FILE *obj = NULL;	/* object file */
FILE *objsave = NULL;	/* alternate object file (supports ENPUNCH/NOPUNCH) */
char objname[NAMELEN];	/* object file's name */
FILE *lst = NULL;	/* listing file */
FILE *lstsave = NULL;	/* alternate listing file */
char lstname[NAMELEN];	/* listing file's name */
int cflag = 0;		/* PAL8 compatibility requested */
int eflag = 0;		/* Add some more symbols */
int jflag = 0;		/* Suppress zero word after TEXT, SIXBIT */
int linkmsg = 1;	/* Suppress "off page" messages */
int dumpflag = 0;	/* dump symtab if 1 (defaults to no dump) */
int rimflag = 0;	/* generate rim format if 1 (defaults bin) */
int cksum = 0;		/* checksum generated for .bin files */
int errors = 0;		/* number of errors found so far */
int dosmode = 0;	/* Input file has CR */

/* return c as upper-case if it is lower-case; else no change */
int c2upper(c)
int c;
{
	return ((c >= 'a' && c <= 'z') ? (c - ('a'-'A')) : c);
}

/* symbol table */
#define SYMBOLS  2048
#define SYMLEN 6
struct symbol {
	char sym[SYMLEN+1]; /* the textual name of the symbol, zero filled */
	short int val;	  /* the value associated with the symbol */
	short int refs;   /* the number of references to the symbol */
} pmode[] = {  /* values = 01xxxx indicate MRI */
		       /* values = 02xxxx indicate LINC mode comma def */
		       /* values = 04xxxx indicate pseudo-ops */
		       /* values = 05xxxx indicate LINC mode ops */
	{ "DECIMA", 040000 }, /* read literal constants in base 10 */
	{ "OCTAL" , 040001 }, /* read literal constants in base 8 */
	{ "ZBLOCK", 040002 }, /* zero a block of memory */
	{ "PAGE"  , 040003 }, /* advance origin to next page or page x (0..37) */
	{ "TEXT"  , 040004 }, /* pack 6 bit trimmed ASCII into memory */
	{ "EJECT" , 040005 }, /* eject a page in the listing */
	{ "FIELD" , 040006 }, /* set origin to memory field */
	{ "NOPUNC", 040007 }, /* turn off object code generation */
	{ "ENPUNC", 040010 }, /* turn on object code generation */
	{ "XLIST" , 040011 }, /* toggle listing generation */
	{ "IFZERO", 040012 }, /* unsupported */
	{ "IFNZRO", 040013 }, /* unsupported */
	{ "IFDEF" , 040014 }, /* unsupported */
	{ "IFNDEF", 040015 }, /* unsupported */
	{ "RELOC" , 040016 }, /* assemble for execution at a different address */
	{ "SEGMNT", 040017 }, /* like page, but with page size = 1K words */
	{ "BANK"  , 040020 }, /* like field, select a different 32K out of 128K */
	{ "FIXMRI", 040021 }, /* like =, but creates mem ref instruction */

	{ "DEVICE", 040022 }, /* creates sixbit device name */
	{ "FILENA", 040023 }, /* creates sixbit file name, with extension */
	{ "FIXTAB", 040024 }, /* Fix symbol table */
	{ "ERROR" , 040035 }, /* output an error message */
	{ "EXPUNG", 040036 }, /* lose the standard symbols */
	{ "PAUSE" , 040042 }, /* wait for tape to be mounted */
	{ "SIXBIT", 040045 }, /* output sixbit character data */
#ifdef PQS8
	{ "ASMIFM", 040025 }, /* assemble next if minus */
	{ "ASMIFN", 040026 }, /* assemble next if nonzero */
	{ "ASMIFZ", 040027 }, /* assemble next if zero */
	{ "ASMSKP", 040028 }, /* skip assembling the next N lines */
	{ "CONSOL", 040029 }, /* read the front panel switches */
	{ "DATE"  , 040030 }, /* get the low date word */
	{ "DATEHI", 040031 }, /* get the high date word */
	{ "DTORG" , 040032 }, /* set the output media block */
	{ "ENBITS", 040033 }, /* ?? */
	{ "ENDBIN", 040034 }, /* ?? */
	{ "IFREF" , 040037 }, /* ?? */
	{ "IFNREF", 040038 }, /* ?? */
	{ "NOBITS", 040043 }, /* ?? */
	{ "PQS"   , 040044 }, /* ?? */
	{ "SKIP"  , 040046 }, /* ?? */
#endif
	{ "TITLE" , 040047 }, /* set the listing title */

	{ "AND", 010000 }, /* mainline instructions */
	{ "TAD", 011000 },
	{ "ISZ", 012000 },
	{ "DCA", 013000 },
	{ "JMS", 014000 },
	{ "JMP", 015000 },
	{ "I",   010400 },
	{ "Z",   010000 },

	{ "OPR", 007000 },

	{ "NOP", 007000 }, /* group 1 */
	{ "CLA", 007200 },
	{ "CIA", 007041 },
	{ "CLL", 007100 },
	{ "CMA", 007040 },
	{ "CML", 007020 },
	{ "IAC", 007001 },
	{ "BSW", 007002 },
	{ "RAR", 007010 },
	{ "RAL", 007004 },
	{ "RTR", 007012 },
	{ "RTL", 007006 },
	{ "STA", 007240 },
	{ "STL", 007120 },
	{ "GLK", 007204 },
	{ "LAS", 007604 },

	{ "SMA", 007500 }, /* group 2 */
	{ "SZA", 007440 },
	{ "SNL", 007420 },
	{ "SKP", 007410 },
	{ "SPA", 007510 },
	{ "SNA", 007450 },
	{ "SZL", 007430 },
	{ "OSR", 007404 },
	{ "HLT", 007402 },
	{ "KCC", 006032 }, /* actually iots, but haven't fixed iots yet */
	{ "KSF", 006031 },
	{ "KRS", 006034 },
	{ "KRB", 006036 },
	{ "IOT", 006000 },
	{ "ION", 006001 },
	{ "IOF", 006002 },
	{ "CDF", 006201 },
	{ "CIF", 006202 },
	{ "RDF", 006214 },
	{ "RIF", 006224 },
	{ "RIB", 006234 },
	{ "RMF", 006244 },
	{ "SMP", 006101 },
	{ "CMP", 006104 },
/*	{ "TFL", 006040 },*/
	{ "TSF", 006041 },
	{ "TCF", 006042 },
	{ "TPC", 006044 },
	{ "TLS", 006046 },
	{ "RSF", 006011 },
	{ "RRB", 006012 },
	{ "RFC", 006014 },
	{ "RCC", 006016 },
	{ "PSF", 006021 },
	{ "PCF", 006022 },
	{ "PPC", 006024 },
	{ "PLS", 006026 },
	{ "DCX", 006051 },
	{ "DXL", 006053 },
	{ "DIX", 006054 },
	{ "DXS", 006057 },
	{ "DCY", 006061 },
	{ "DYL", 006063 },
	{ "DIY", 006064 },
	{ "DYS", 006067 },
	{ "DSF", 006071 },
	{ "DCF", 006072 },
	{ "DSB", 006074 },
	{ "PLSF", 006501 },
	{ "PLCF", 006502 },
	{ "PLPU", 006504 },
	{ "PLPR", 006511 },
	{ "PLDU", 006512 },
	{ "PLDD", 006514 },
	{ "PLPL", 006521 },
	{ "PLPD", 006524 },
	{ "DCMA", 006601 },
	{ "DMAR", 006603 },
	{ "DMAW", 006605 },
	{ "DCEA", 006611 },
	{ "DSAC", 006612 },
	{ "DEAL", 006615 },
	{ "DEAC", 006616 },
	{ "DFSE", 006621 },
	{ "DFSC", 006622 },
	{ "DMAC", 006626 },
	{ "MMLS", 006751 },
	{ "MMLM", 006752 },
	{ "MMLF", 006754 },
	{ "MMMF", 006756 },
	{ "MMMM", 006757 },
	{ "MMSF", 006761 },
	{ "MMML", 006766 },
	{ "MMSC", 006771 },
	{ "MMCF", 006772 },
	{ "MMRS", 006774 },
	{ "DTRA", 006761 },
	{ "DTCA", 006762 },
	{ "DTXA", 006764 },
	{ "DTLA", 006766 }, //BUGBUG: PUSHJ Conflict?
	{ "DTSF", 006771 },
	{ "DTRB", 006772 },
	{ "DTLB", 006774 },
	{ "LCD", 006751 },
	{ "XDR", 006752 },
	{ "STR", 006753 },
	{ "SER", 006754 },
	{ "SDN", 006755 },
	{ "INTR", 006756 },
	{ "INIT", 006757 },
/* My personal view is that these are not "family of eight",	*/
/* and shouldn't be here.  Alas, most of the code in the world	*/
/* seems to disagree.  As a compromise, I have added the -e	*/
/* option which defines these. VRS				*/
#ifdef LINC
	{ "LINC" , 046141 }, /* Also implied LMODE pseudo-op */
	{ "LMODE", 040040 }, /* set the LINC mode */
	{ "PMODE", 040041 }, /* set the PDP-8 mode */
//BUGBUG: Sixteen IOTs apparently exist in the LINC-8 to access the
//LINC-side registers.  The documentation survives in the 1967 edition
//of the Small Computer Handbook (but not later editions), and if your
//code is expected to run on a LINC-8, you may need these.
/* LINC-8 IOTs! */
	{ "ICON", 06141 }, // 16 of these
	{ "IBAC", 06143 },
	{ "ILES", 06145 },
	{ "INTS", 06147 },
	{ "ICS1", 06151 },
	{ "LMR",  06151 }, // ??
	{ "ICS2", 06153 },
	{ "IMBS", 06155 },
	{ "ITAC", 06157 }, // ??
	{ "IACB", 06161 },
	{ "IACS", 06163 },
	{ "ISSP", 06165 },
	{ "IACA", 06167 },
	{ "IAAC", 06171 },
	{ "IZSA", 06173 },
	{ "IACF", 06175 },
//	{ "????", 06177 }, // ITAC?
#endif
	{ "SCL", 007403 }, /* group 3 */
	{ "ASC", 007403 },
	{ "MUY", 007405 },
	{ "DVI", 007407 },
	{ "NMI", 007411 },
	{ "SHL", 007413 },
	{ "ASR", 007415 },
	{ "LSR", 007417 },
	{ "MQL", 007421 },
	{ "SCA", 007441 },
	{ "SWAB", 007431 },
	{ "DAD", 007443 },
	{ "DST", 007445 },
	{ "SWBA", 007447 },
	{ "DPSZ", 007451 },
	{ "SAM", 007457 },
	{ "DPIC", 007473 },
	{ "DCM", 007475 },
	{ "MQA", 007501 },
	{ "SWP", 007521 },
	{ "DPIC", 007573 },
	{ "DCM", 007575 },
	{ "CAM", 007621 },
	{ "ACL", 007701 },
	{ "DLD", 007763 },
	{ "SKON",006000 }, /* IOTs */
	{ "SRQ", 006003 },
	{ "GTF", 006004 },
	{ "RTF", 006005 },
	{ "SGT", 006006 },
	{ "CAF", 006007 },
	{ "RPE", 006010 },
	{ "PCE", 006020 },
	{ "KCF", 006030 },
	{ "KIE", 006035 },
	{ "TFL", 006040 },
	{ "TSK", 006045 },
	{ "CLSK", 006131 }, /* PDP-12 or DK8-EP */
	{ "CLLR", 006132 },
	{ "CLAB", 006133 },
	{ "CLEN", 006134 },
	{ "CLSA", 006135 },
	{ "CLBA", 006136 },
	{ "CLCA", 006137 },
	{ "SEL", 006750 },
	{ "PDP", 050002 }, /* Kludge for PQS8 */
	{ "COM", 050017 }, /* Kludge for PQS8 */
#define TSS
#ifdef TSS
	{ "RRS", 006010 },
	{ "PST", 006020 },
	{ "KSR", 006030 },
	{ "SAS", 006040 },
	{ "CKS", 006200 },
	{ "KSB", 006400 },
	{ "SBC", 006401 },
	{ "DUP", 006402 },
	{ "UND", 006403 },
	{ "CLS", 006405 },
	{ "SEGS", 006406 },
	{ "URT", 006411 },
	{ "TOD", 006412 },
	{ "RCR", 006413 },
	{ "DATE", 006414 },
	{ "SYN", 006415 },
	{ "STM", 006416 },
	{ "SRA", 006417 },
	{ "TSS", 006420 },
	{ "USE", 006421 },
	{ "CON", 006422 },
	{ "SSW", 006430 },
	{ "SEA", 006431 },
	{ "ASD", 006440 },
	{ "REL", 006442 },
	{ "REN", 006600 },
	{ "OPEN", 006601 },
	{ "CLOS", 006602 },
	{ "RFILE",006603 },
	{ "PROT", 006604 },
	{ "WFILE",006605 },
	{ "CRF", 006610 },
	{ "EXT", 006611 },
	{ "RED", 006612 },
	{ "FINF", 006613 },
	{ "SIZE", 006614 },
	{ "WHO", 006616 },
	{ "ACT", 006617 },
#define pdp8e (46+19+38) /* Number of extensions to the symbol table */
#else
#ifdef LINC
#define pdp8e (46+19)	/* Number of extensions to the symbol table */
#else
#define pdp8e 46	/* Number of extensions to the symbol table */
#endif
#endif
};
#define pcount ((sizeof pmode)/(sizeof *pmode))

#ifdef LINC
struct symbol lmode[] = {
		       /* values = 04xxxx indicate pseudo-ops */
	{ "DECIMA", 040000 }, /* read literal constants in base 10 */
	{ "OCTAL" , 040001 }, /* read literal constants in base 8 */
	{ "ZBLOCK", 040002 }, /* zero a block of memory */
	{ "PAGE"  , 040003 }, /* advance to next page or page x (0..37) */
	{ "TEXT"  , 040004 }, /* pack 6 bit trimmed ASCII into memory */
	{ "EJECT" , 040005 }, /* eject a page in the listing */
	{ "FIELD" , 040006 }, /* set origin to memory field */
	{ "NOPUNC", 040007 }, /* turn off object code generation */
	{ "ENPUNC", 040010 }, /* turn on object code generation */
	{ "XLIST" , 040011 }, /* toggle listing generation */
	{ "IFZERO", 040012 }, /* unsupported */
	{ "IFNZRO", 040013 }, /* unsupported */
	{ "IFDEF" , 040014 }, /* unsupported */
	{ "IFNDEF", 040015 }, /* unsupported */
	{ "RELOC" , 040016 }, /* assemble for execution at a different address */
	{ "SEGMNT", 040017 }, /* like page, but with page size = 1K words */
	{ "BANK"  , 040020 }, /* like field, select a different 32K out of 128K */
	{ "FIXMRI", 040021 }, /* like =, but creates mem ref instruction */

	{ "DEVICE", 040022 }, /* creates sixbit device name */
	{ "FILENA", 040023 }, /* creates sixbit file name, with extension */
	{ "FIXTAB", 040024 }, /* Fix symbol table */
	{ "ERROR" , 040035 }, /* output an error message */
	{ "EXPUNG", 040036 }, /* lose the standard symbols */
	{ "PAUSE" , 040042 }, /* wait for tape to be mounted */
	{ "SIXBIT", 040045 }, /* output sixbit character data */
	{ "TITLE" , 040047 }, /* set the listing title */
	{ "LMODE" , 040040 }, /* set the LINC mode */
	{ "PMODE" , 040041 }, /* set the PDP-8 mode */
	{ "PDP"   , 040043 }, /* Also implies PMODE */

	{ "U",   000010 }, /* "U" bit */
	{ "I",   000020 }, /* "I" bit */

	{ "HLT", 050000 }, /* LINC A-class and others */
	{ "MSC", 050000 },
	{ "AXO", 050001 },
	{ "TAC", 050003 },
	{ "ESF", 050004 },
	{ "QAC", 050005 },
//	{ "ZTA", 050005 },
	{ "DJR", 050006 },
	{ "CLR", 050011 },
	{ "ATR", 050014 },
	{ "RTA", 050015 },
	{ "NOP", 050016 },
	{ "COM", 050017 },
	{ "XOA", 050021 },
	{ "TMA", 050023 },
	{ "SFA", 050024 },
	{ "SET", 050040 },
	{ "SAM", 050100 },
	{ "DIS", 050140 },
	{ "XSK", 050200 },
	{ "ROL", 050240 },
	{ "ROR", 050300 },
	{ "SCR", 050340 },
	{ "SXL", 050400 },
	{ "KST", 050415 },
	{ "STD", 050416 },
	{ "TWC", 050417 },
	{ "SNS", 050440 },
	{ "AZE", 050450 },
	{ "APO", 050451 },
	{ "LZE", 050452 },
	{ "IBZ", 050453 },
	{ "FLO", 050454 },
	{ "QLZ", 050455 },
//	{ "ZZZ", 050455 },
//	{ "SWD", 050457 },
	{ "SKP", 050467 },
	{ "IOB", 050500 },
/* 0501 to 0515 are Undefined */
//	{ "TYP", 050514 },
//	{ "KBD", 050515 },
	{ "RSW", 050516 },
	{ "LSW", 050517 },
/* 0521 to 0535 are Undefined */
/* 0540 to 0577 are Undefined */
	{ "LIF", 050600 },
	{ "LDF", 050640 },
//	{ "UMB", 050640 },
	{ "RDC", 050700 },
	{ "RCG", 050701 },
	{ "RDE", 050702 },
	{ "MTB", 050703 },
	{ "WRC", 050704 },
	{ "WCG", 050705 },
	{ "WRI", 050706 },
	{ "CHK", 050707 },
//	{ "EXC", 050740 },
/* 0740 to 0747 are Undefined */
	{ "LDA", 051000 }, /* LINC B-class */
	{ "STA", 051040 },
	{ "ADA", 051100 },
	{ "ADM", 051140 },
	{ "LAM", 051200 },
	{ "MUL", 051240 },
	{ "LDH", 051300 },
	{ "STH", 051340 },
	{ "SHD", 051400 },
	{ "SAE", 051440 },
	{ "SRO", 051500 },
	{ "BCL", 051540 },
	{ "BSE", 051600 },
	{ "BCO", 051640 },
//	{ "???", 051700 }, /* Undefined, B-class */
	{ "DSC", 051740 },

	{ "ADD", 052000 }, /* LINC Direct addressing */
	{ "STC", 054000 },
	{ "JMP", 056000 },
//	{ "JSR", 056000 },
	{ "LINC",006141 }, /* Kludge */
};
#define lcount ((sizeof lmode)/(sizeof *lmode))
#endif

struct symbol pal8[] = {  /* values = 01xxxx indicate MRI */
		       /* values = 02xxxx indicate LINC mode comma def */
		       /* values = 04xxxx indicate pseudo-ops */
		       /* values = 05xxxx indicate LINC mode ops */
	{ "DECIMA", 040000 }, /* read literal constants in base 10 */
	{ "OCTAL" , 040001 }, /* read literal constants in base 8 */
	{ "ZBLOCK", 040002 }, /* zero a block of memory */
	{ "PAGE"  , 040003 }, /* advance origin to next page or page x (0..37) */
	{ "TEXT"  , 040004 }, /* pack 6 bit trimmed ASCII into memory */
	{ "EJECT" , 040005 }, /* eject a page in the listing */
	{ "FIELD" , 040006 }, /* set origin to memory field */
	{ "NOPUNC", 040007 }, /* turn off object code generation */
	{ "ENPUNC", 040010 }, /* turn on object code generation */
	{ "XLIST" , 040011 }, /* toggle listing generation */
	{ "IFZERO", 040012 }, /* unsupported */
	{ "IFNZRO", 040013 }, /* unsupported */
	{ "IFDEF" , 040014 }, /* unsupported */
	{ "IFNDEF", 040015 }, /* unsupported */
	{ "RELOC" , 040016 }, /* assemble for execution at a different address */
	{ "FIXMRI", 040021 }, /* like =, but creates mem ref instruction */

	{ "DEVICE", 040022 }, /* creates sixbit device name */
	{ "FILENA", 040023 }, /* creates sixbit file name, with extension */
	{ "FIXTAB", 040024 }, /* Fix symbol table */
	{ "EXPUNG", 040036 }, /* lose the standard symbols */
	{ "PAUSE" , 040042 }, /* wait for tape to be mounted */
	{ "DTORG" , 040032 }, /* set the output media block */
	{ "AND", 010000 }, /* mainline instructions */
	{ "TAD", 011000 },
	{ "ISZ", 012000 },
	{ "DCA", 013000 },
	{ "JMS", 014000 },
	{ "JMP", 015000 },
	{ "I",   010400 },
	{ "Z",   010000 },

	{ "OPR", 007000 },

	{ "NOP", 007000 }, /* group 1 */
	{ "CLA", 007200 },
	{ "CIA", 007041 },
	{ "CLL", 007100 },
	{ "CMA", 007040 },
	{ "CML", 007020 },
	{ "IAC", 007001 },
	{ "BSW", 007002 },
	{ "RAR", 007010 },
	{ "RAL", 007004 },
	{ "RTR", 007012 },
	{ "RTL", 007006 },
	{ "STA", 007240 },
	{ "STL", 007120 },
	{ "GLK", 007204 },
	{ "LAS", 007604 },

	{ "SMA", 007500 }, /* group 2 */
	{ "SZA", 007440 },
	{ "SNL", 007420 },
	{ "SKP", 007410 },
	{ "SPA", 007510 },
	{ "SNA", 007450 },
	{ "SZL", 007430 },
	{ "OSR", 007404 },
	{ "HLT", 007402 },
	{ "KCC", 006032 }, /* actually iots, but haven't fixed iots yet */
	{ "KSF", 006031 },
	{ "KRS", 006034 },
	{ "KRB", 006036 },
	{ "IOT", 006000 },
	{ "ION", 006001 },
	{ "IOF", 006002 },
	{ "CDF", 006201 },
	{ "CIF", 006202 },
	{ "RDF", 006214 },
	{ "RIF", 006224 },
	{ "RIB", 006234 },
	{ "RMF", 006244 },
	{ "TSF", 006041 },
	{ "TCF", 006042 },
	{ "TPC", 006044 },
	{ "TLS", 006046 },
	{ "RSF", 006011 },
	{ "RRB", 006012 },
	{ "RFC", 006014 },
	{ "PSF", 006021 },
	{ "PCF", 006022 },
	{ "PPC", 006024 },
	{ "PLS", 006026 },
/* My personal view is that these are not "family of eight",	*/
/* and shouldn't be here.  Alas, most of the code in the world	*/
/* seems to disagree.  As a compromise, I have added the -e	*/
/* option which defines these. VRS				*/
	{ "MQL", 007421 },
	{ "MQA", 007501 },
	{ "SWP", 007521 },
	{ "SKON",006000 }, /* IOTs */
	{ "SRQ", 006003 },
	{ "GTF", 006004 },
	{ "RTF", 006005 },
	{ "SGT", 006006 },
	{ "CAF", 006007 },
	{ "RPE", 006010 },
	{ "PCE", 006020 },
	{ "KCF", 006030 },
	{ "KIE", 006035 },
	{ "TFL", 006040 },
	{ "TSK", 006045 },
};
#define palcount ((sizeof pal8)/(sizeof *pal8))

struct symbol *cmode = pmode; /* Start in PMODE */
/* The nmode normally tracks cmode, except after IOB. */
struct symbol *nmode = pmode; /* Start in PMODE */

struct symbol symtab[SYMBOLS]; /* User symbols */


/* Now the user symbol table starts at entry 0 */
/* Predefined symbols are in pmode[] and lmode[] */
#define firstsym 0

/* command line argument processing */
void
getargs(argc, argv)
int argc;
char *argv[];
{
	int i,j;

        for (i=1; i < argc; i++) {
		if (argv[i][0] == '-') { /* a flag */
			for (j=1; argv[i][j] != 0; j++) {
				if (argv[i][1] == 'c') {
					cflag = 1;
				} else if (argv[i][1] == 'd') {
					dumpflag = 1;
				} else if (argv[i][1] == 'e') {
					eflag = 1;
				} else if (argv[i][1] == 'j') {
					jflag = 1;
				} else if (argv[i][1] == 'l') {
					linkmsg = 0;
				} else if (argv[i][1] == 'r') {
					rimflag = 1;
				} else {
					fprintf( stderr,
						 "%s: unknown flag: %s\n",
						 argv[0], argv[i] );
					fprintf( stderr,
						 " -c -- use PAL8 symtab\n" );
					fprintf( stderr,
						 " -d -- dump symtab\n" );
					fprintf( stderr,
						 " -e -- predefine Omnibus era instructions\n" );
					fprintf( stderr,
						 " -j -- don't pad TEXT\n" );
					fprintf( stderr,
						 " -r -- output rim file\n" );
					exit(-1);
				}
			}
		} else { /* no - at front of argument, must be file name */
			if (filename != NULL) {
				fprintf( stderr, "%s: too many input files\n",
					 argv[0] );
				exit(-1);
			}
			filename = &argv[i][0];
		} /* end if */
        } /* end for loop */

	if (!eflag)
		pmode[pcount-pdp8e].sym[0] = 0;

        if (filename == NULL) { /* no input file specified */
		fprintf( stderr, "%s: no input file specified\n", argv[0] );
		exit(-1);
	}

	/* now, fix up the file names */
	{
		int dot; /* location of last dot in filename */
		dot = 0;
		for (j = 0; filename[j] != 0; j++) {
			if (j > (NAMELEN - 5)) {
				fprintf( stderr,
					 "%s: file name too long %s\n",
					 argv[0], filename );
				exit(-1);
			}
			lstname[j] = objname[j] = filename[j];
			if (filename[j]=='.') {
				dot = j;
			} else if (filename[j]=='/') {
				dot = 0;
			}
		}
		if (dot == 0) {
			dot = j;
			lstname[dot] = objname[dot] = '.';
		}
		lstname[dot + 1] = 'l';
		lstname[dot + 2] = 's';
		lstname[dot + 3] = 't';
		lstname[dot + 4] = '\0';

		if (rimflag == 1) {
			objname[dot + 1] = 'r';
			objname[dot + 2] = 'i';
			objname[dot + 3] = 'm';
		} else {
			objname[dot + 1] = 'b';
			objname[dot + 2] = 'i';
			objname[dot + 3] = 'n';
		}
		objname[dot + 4] = '\0';
        }

	/* now, open up the input file */
	if ((in = fopen(filename, "r")) == NULL) {
		fprintf( stderr, "%s: cannot open %s\n", argv[0], filename );
		exit(-1);
	}
}

#define LINELEN 132
char line[LINELEN];
int pos;    /* position on line */
int listed; /* has line been listed to listing yet (0 = no, 1 = yes) */
int lineno; /* line number of current line */

char title[LINELEN]; /* Title for listing */

void
listline()
/* generate a line of listing if not already done! */
{
	char *p;
	int tabstop;

	if ((lst != NULL) && (listed == 0)) {
		fprintf( lst, "%4d               ", lineno );
		tabstop = 8;
		for (p = line; *p; p++) {
			 if (*p == '\t') {
				 while (tabstop-- > 0)
					 putc(' ', lst);
				 tabstop = 8;
			 } else {
				 putc(*p, lst);
				 if (--tabstop == 0)
					 tabstop = 8;
			 }
		}
	}
	listed = 1;
}

void
error( msg )
char *msg;
/* generate a line of listing with embedded error messages */
{
	FILE *tlst;
// BUGBUG: When XLIST is in effect, error messages print, but the source
// line does not!
        tlst = lst;
	if (lst == NULL) {
		/* force error messages to print despite XLIST */
		lst = lstsave;
	}
	if (lst != NULL) {
		listline();
		fprintf( lst, "%-15.15s ", msg );
		{
			int i;
			for (i = 0; i < (pos-1); i++) {
				if (line[i] == '\t') {
					putc('\t', lst );
				} else {
					putc( ' ', lst );
				}
			}
		}
		fputc( '^', lst );
		if (dosmode)
			fputc( '\r', lst );
		fputc( '\n', lst );
		fprintf( stderr, "%4d  %s\n", lineno, msg );
	}
	listed = 1;
	lst = tlst; /* Restore lst */
	errors++;
}

/* Some input files use NUL as a padding character, particularly
 * after form feed.  The use of fgets() here would preclude us
 * from seeing any character on the line after a NUL.
*/
char *myfgets(line, n, in)
char *line;
int n;
FILE *in;
{
	int i, c;
	for (i = 0; i < n; ) {
        	c = fgetc(in);
		if (c == EOF) {
			if (i)
				return line;
			return NULL;
		}
		if (c) /* Don't store NUL */
			line[i++] = c;
		if (c == '\n') {
			line[i++] = 0;
			return line;
		}
	}
	return line;
}

void
readline()
/* read one input line, setting things up for lexical analysis */
{
	listline(); /* if it hasn't been listed yet, list it! */
	lineno = lineno + 1;
	listed = 0;
	pos = 0;
	
	if (myfgets( line, LINELEN-1, in ) == NULL) {
		line[0] = '$';
		line[1] = '\n';
		line[2] = '\000';
		error( "end of file" ); errors--; /* VRS: warning only */
	}
        /* At strlen-1 is presumably '\n' */
        dosmode = (line[strlen(line)-2] == '\r');
}

/* dump symbol table */
void
dump()
{
	struct symbol tmp;
	int i, again;
	fputc( '\f', lst );
	if (dosmode)
		fputc( '\r', lst );
	fputc( '\n', lst );
	/* Sort the symbol table with a ripple sort */
sort:
	again = 0;
	for (i = firstsym; symtab[i+1].sym[0] != '\0'; i++) {
		if (strncmp(symtab[i].sym, symtab[i+1].sym, SYMLEN) > 0) {
			tmp = symtab[i];
			symtab[i] = symtab[i+1];
			symtab[i+1] = tmp;
			again = 1;
		}
	}
	if (again)
		goto sort;
	/* Dump the symbol table */
	for (i = firstsym; symtab[i].sym[0] != '\0'; i++) {
		fprintf( lst, "%-6s  %04o%s",
			 symtab[i].sym, 07777 & symtab[i].val,
			 symtab[i].refs? "" : " unreferenced");
		if (dosmode)
			fputc( '\r', lst );
		fputc( '\n', lst );
	}
}

/* define symbol, returns old value, if any */
int
define( sym, val )
char sym[SYMLEN];
short int val;
{
	int i,j, old;
	for (i = 0; symtab[i].sym[0] != '\0'; i++) {
		for (j = 0; j < SYMLEN; j++) {
			if (symtab[i].sym[j] != sym[j]) {
				goto mismatch;
			}
		}
                old = symtab[i].val;
		goto match;
		mismatch:;
	}
	/* if it ever gets here, a new symbol must be defined */
	if (i < (SYMBOLS - 1)) {
		for (j = 0; j < SYMLEN; j++) {
			symtab[i].sym[j] = sym[j];
		}
	} else {
                error("full");
	}
	symtab[i+1].sym[0] = 0; /* VRS: Don't allow deleted symbols to creep back in */
	symtab[i].refs = 0;
        old = -1;
	match:;

	symtab[i].val = val;
        return old;
}

short int lookup1( symtab, sym )
struct symbol *symtab;
char sym[SYMLEN];
{
	int i,j;
	for (i = 0; symtab[i].sym[0] != '\0'; i++) {
		for (j = 0; j < SYMLEN; j++) {
			if (symtab[i].sym[j] != sym[j]) {
				goto mismatch;
			}
		}
		goto match;
		mismatch:;
	}
	/* if it ever gets here, the symbol is undefined */
	return -1;

	match:;
	symtab[i].refs++;
	return symtab[i].val;
}

short int lookup( sym )
char sym[SYMLEN];
{
	short int val;
	/* Look in user symbols first */
	val = lookup1(symtab, sym);
//BUGBUG: LINC references to PMODE symbols need 10 bits too!
// Should set 020000 for all comma defs.
	if ((cmode == lmode) && ((val & ~07777) == 020000))
		val &= 01777;
	if (val < 0)
		val = lookup1(nmode, sym);
	return val;
}

int lc; /* the location counter */
int reloc; /* the relocation distance (see RELOC) */
int pzlc; /* the page zero location counter for page zero constants */
int cplc; /* the current page location counter for current page constants */
int radix; /* the default number radix */
int sixbit = 0; /* Only set during SIXBIT */

/*
 * New Literal storage mechanism (proposed):
 * As before, pz[] and cp[] store literal values for page zero
 * of the current field and for the current page, respectively.
 * The pzlc and cplc indices record the "next" literal to be allocated.
 *
 * In addition, pzend[] and cpend[] arrays note which locations
 * have previously been allocated.  Literals involved in the current
 * FIELD and PAGE settings then, range from pzloc to pzend[field] and
 * from cploc to cpend[page].
*/
int pz[0200]; /* storehouse for page zero constants */
int cp[0200]; /* storehouse for current page constants */
#define FIELDS	(128/4)		/* 128K Max, 4K per field */
#define PAGES (128*1024/128)	/* 128K Max, 128 per page */
int pzend[FIELDS];
int cpend[PAGES];

/* single pass of the assembler */

void
putleader()
/* generate 2 feet of leader on the object file, as per DEC documentation */
{
	if (obj != NULL) {
		int i;
		for (i = 1; i < 240; i++) {
 			fputc( 0200, obj );
		}
	}
}

void
puto(c)
int c;
/* put one character to obj file and include it in checksum */
{
	c &= 0377;
	fputc(c, obj);
	cksum += c;
}

int field; /* the current field */

void
putorg( loc )
short int loc;
{
	/* VRS: Use "reloc" here, as we are setting a load address */
	puto( (((loc+reloc) >> 6) & 0077) | 0100 );
	puto( (loc+reloc) & 0077 );
}

void
putout( loc, val )
short int loc;
short int val;
/* put out a word of object code */
{
	char *p;
	int tabstop;

	if (lst != NULL) {
		if (listed == 0) {
			tabstop = reloc? '*' : ' '; /* VRS */
			fprintf( lst, "%4d %2.2o%4.4o%c %4.4o  ",
				 lineno, field, loc, tabstop, val);
			tabstop = 8;
			for (p = line; *p; p++) {
				 if (*p == '\t') {
					 while (tabstop-- > 0) {
						 putc(' ', lst);
					 }
					 tabstop = 8;
				 } else {
					 putc(*p, lst);
					 if (--tabstop == 0)
						 tabstop = 8;
				 }
			}
		} else {
			tabstop = reloc? '*' : ' '; /* VRS */
			fprintf( lst, "     %2.2o%4.4o%c %4.4o  ",
				 field, loc, tabstop, val);
                        if (dosmode)
				putc( '\r', lst );
			putc( '\n', lst );
		}
	}
	if (obj != NULL) {
		if (rimflag == 1) { /* put out origin in rim mode */
			putorg( loc );
		}
		puto( (val >> 6) & 0077 );
		puto( val & 0077 );
	}
	listed = 1;
}


char lexstart; /* index of start of the current lexeme on line */
char lexterm;  /* index of character after the current lexeme on line */

#define IsEOF(c) ((c=='$')||(c==032))
#define IsBlank(c) ((c==' ')||(c=='\t')||(c==0177)||(c=='\f')||(c=='>')||(c=='\r'))

/*
 * The nextlex() function assumes 'pos' is the beginning of a token,
 * sets lexstart to that, finds the end of the token, and sets lexterm
 * and 'pos' to the start of the next token.
 * The exception is '/' and end-of-line, which are considered to be of
 * length 1, since their actual length should never be relevant.
*/
void
nextlex()
/* get the next lexeme into lex */
{
	while (IsBlank(line[pos])) {
		pos++;
	}

	lexstart = pos;

	if (isalpha(line[pos])) { /* identifier */
		while (isalnum(line[pos])) {
			pos++;
		}
	} else if (isdigit(line[pos])) { /* number */
		while (isdigit(line[pos])) {
			pos++;
		}
//BUGBUG: Should allow six bit dual character constants: 'AB
	} else if (line[pos] == '"') { /* quoted letter */
		pos++;
		pos++;
//BUGBUG: Should ignore the closing '"' if present
	} else if (isend(line[pos])) { /* end of line */
		/* don't advance pos! */
	} else if (line[pos] == '/') { /* comment */
		/* don't advance pos! */
	} else { /* all punctuation is handled here! */
		pos++;
	}

	lexterm = pos;
}

int
deflex( start, term, val )
int start; /* start of lexeme to be defined */
int term; /* character after end of lexeme to be defined */
int val; /* value of lexeme to be defined */
{
	char sym[SYMLEN];
	int from, to;

	from = start;
	to = 0;
	while ((from < term) && (to < SYMLEN)) {
		sym[to++] = c2upper(line[from++]);
	}
	while (to < SYMLEN) {
		sym[to++] = '\000';
	}

	return define( sym, val );
}

void
condtrue()
/* called when a true conditional has been evaluated */
/* lex should be the opening <; skip it and setup for normal assembly */
{
	if (line[lexstart] == '<') {
		nextlex(); /* skip the opening bracket */
	} else {
		error( "< expected" );
	}
}

void
condfalse()
/* called when a false conditional has been evaluated */
/* lex should be the opening <; ignore all text until the closing > */
{
	if (line[lexstart] == '<') {
		int level = 1;
		/* invariant: line[pos] is the next unexamined character */
		while (level > 0) {
			if (isend(line[pos])) { /* need to get a new line */
				readline();
			} else if (isalpha(line[pos])) { /* identifier */
				lexstart = pos;
				while (isalnum(line[pos])) {
					pos++;
				}
				if ((pos-lexstart == 4) &&
				    ((strncmp(line+lexstart, "TEXT", 4) == 0) ||
				     (strncmp(line+lexstart, "SIXBIT", 6) == 0))) {
					char delimiter;
					/* Ignore the text */
					nextlex();
					delimiter = line[lexstart];
					/* Because nextlex() didn't */
					if (delimiter == '/') pos++;
					while ((line[pos] != delimiter) &&
					       !isend(line[pos])       ) {
						pos++;
					}
					pos++;
				}
			} else if (line[pos] == '"') {
				pos++;
				if (!isend(line[pos]))
					pos++;
			} else if (line[pos] == '/') {
				/* Weirdly, > after / does count! */
				while (!isend(line[pos])) {
					if (line[pos] == '<') level++;
					if (line[pos] == '>') level--;
					pos++;
				}
			} else if (line[pos] == '>') {
				level --;
				pos++;
			} else if (line[pos] == '<') {
				level ++;
				pos++;
			} else if (IsEOF(line[pos])) {
				level = 0;
				pos++;
			} else {
				pos++;
			}
		}
		nextlex();
	} else {
		error( "< expected" );
	}
}
 
/*
 * When we leave a field or page, putpz() or putcp() are called.
 * We put the literals which are new, as before, but also update
 * pzend[field] or cpend[page] to record that some literals have
 * already been output (and forgotten).  This keeps up from putting
 * literals on top of each other, though we can still duplicate a
 * literal, if it is needed on multiple trips through the field/page.
 *
 * Best practice, of course, is not to revisit fields or pages which
 * already have literals and then define new ones.  Most assemblers
 * will get this horribly wrong, and without warning.
*/

void
putpz()
/* put out page zero data */
{
	int loc;
	if (pzlc < pzend[field]) {
		if (rimflag != 1) { /* put out origin if not in rim mode */
			if (obj != NULL) {
				putorg( pzlc+1 );
			}
		}
		for (loc = pzlc+1; loc <= pzend[field]; loc ++) {
			putout( loc, pz[loc] );
		}
	}
	pzend[field] = pzlc;
// MUST CALL putcp() BEFORE putpz() if calling both!
	for (cplc = 0; cplc < PAGES; cplc++)
		cpend[cplc] = 0177;
	cplc = 00177; /* points to end of page for () operands */
//VRS	pzlc = 00177;	/* CALLER IS EXPECTED TO RESET PZLC! */
	pzlc = 00177;	/* CALLER IS EXPECTED TO RESET PZLC! */
}

void
putcp()
/* put out current page data */
{
	int loc;
	if (cplc < cpend[lc>>7]) {
		if (lc-1 > (cplc + (lc & 07600))) { /* overrun constant pool */
			error( "overrun" );
		}
		if (rimflag != 1) { /* put out origin if not in rim mode */
			if (obj != NULL) {
				putorg( cplc+1 + (lc & 07600) );
			}
		}
		for (loc = cplc+1; loc <= cpend[lc>>7]; loc ++) {
			putout( loc + (lc & 07600), cp[loc] );
		}
	}
	cpend[lc>>7] = cplc;
//VRS	cplc = 00177;	/* CALLER IS EXPECTED TO RESET CPLC! */
}

int getexprs(); /* forward declaration */

int evalsym()
/* get the value of the current identifier lexeme; don't advance lexeme */
{
	char sym[SYMLEN];
	int from = lexstart;
	int to = 0;

	/* assert isalpha(line[lexstart]) */

	/* copy the symbol */
	while ((from < lexterm) && (to < SYMLEN)) {
		sym[to++] = c2upper(line[from++]);
	}
	while (to < SYMLEN) {
		sym[to++] = '\000';
	}
	return (lookup( sym ));
}

int delimiter; /* the character immediately after this eval'd term */

#if 0
void
nextlexblank()
/* used only within eval, getexpr, this prevents illegal blanks */
{
	nextlex();
	if (IsBlank(delimiter)) {
		error("illegal blank");
	}
	delimiter = line[lexterm];
}
#endif

/*
 * Try to keep track of where we are, lexically (debug).
*/
void
debuglex(caller)
char *caller;
{
	fprintf(stderr, "%d %s; delim = '%c'\n", lineno, caller, delimiter);
	fprintf(stderr, "    pos = %s", line+pos);
	fprintf(stderr, "    term = %s", line+lexterm);
	fprintf(stderr, "    start = %s", line+lexstart);
}

short int
negative(arg)
short int arg;
{
	if (cmode == lmode) return 07777 & ~arg;
	return 07777 & -arg;
}

short int
sum(a, b)
short int a, b;
{
	int s = 017777 & (a + b);
        if ((s > 07777) && (cmode == lmode)) s++;
	return s & 07777;
}

/*
 * The current lexeme spans from lexstart to lexterm-1.
 * Determine a value for it, and return it.
 * If we are called for something that doesn't have a meaningful
 * value, report "illegal char".
 * As side effects, set 'delimiter' to the first character of the
 * subsequent lexeme, then advance to that lexeme.
 * Note: This means that on return delimiter should match lexstart,
 * not lexterm!
 *
 * VRS 5/27/2023 Modified to return -1 if undefined symbol encountered.
 *     Be sure to check for it!
*/
int
eval()
{
	int val;

	delimiter = line[lexterm];
	/* Hacks for unary operators */
	if (line[lexstart] == '-') {
		nextlex();
		return negative(eval());
	} else if (line[lexstart] == '+') {
		nextlex();
		return eval();

	} else if (isalpha(line[lexstart])) {
		val = evalsym();

		if (val == -1) {
			error( "undefined" );
		}

	} else if (isdigit(line[lexstart])) {
		int from = lexstart;

		val = 0;
		while (from < lexterm) {
			int digit = (int)line[from++] - (int)'0';
			if (digit < radix) {
				val = (val * radix) + digit;
			} else {
				error("d > radix");
			}
		}

	} else if (line[lexstart] == '"') {
		val = line[lexstart+1] | 0200;
		/* lexterm points at lexstart+1, so fix things up. */
		delimiter = line[lexstart+2];
		lexterm = pos = lexstart+2;

	} else if (line[lexstart] == '.') {
		val = lc & 07777;
		if (cmode == lmode)
			val &= 01777;

	} else if (line[lexstart] == '[') {
		int loc;

		nextlex(); /* skip bracket */
		val = getexprs(); /* May return -1 for undefined */
		/* If we see a [ for an undefined symbol during pass 1, it is 
		 * a quandry whether (and what) to allocate for it.  If don't 
		 * allocate, we sometimes mis-align literals during pass 2
		 * because we didn't allocate enough in pass 1.  If we
		 * allocate every time, we mis-align literals because we
		 * create too many in pass 1.  Here we kludge a non-zero 
		 * allocation, creating a single such literal.  This seems
		 * to give the same answer as PAL8.
		*/
		if (val < 0)
			val = 0200;
		val &= 07777;
		if (pos == lexstart)
			nextlex(); /* advance */
		if (line[lexstart] == ']') {
			nextlex(); /* skip end bracket */
			//delimiter = line[lexterm];
		} else {
			/* error("parens") */;
			//pos = lexterm = lexstart;
		}
		/* Now rig for caller to see the terminator */
		pos = lexterm = lexstart;
		delimiter = line[lexterm];

		loc = pzend[field];
		while ((loc > pzlc) && (pz[loc] != val)) {
			loc--;
		}
		if (loc == pzlc) {
			pz[pzlc] = val;
			pzlc--;
		}
		val = loc;

	} else if (line[lexstart] == '(') {
		int loc;
		int *lit = cp;
		int *plc = &cplc;

		if ((lc & 07600) == 0) {
			error("page zero"); errors--; /* VRS: warning only */
			lit = pz;
			plc = &pzlc;
		}
		nextlex(); /* skip left paren */
		val = getexprs(); /* May return -1 for undefined */
		if (pos == lexstart) {
			nextlex(); /* wtf?? */
		}
		if (line[lexstart] == ')') {
			nextlex(); /* skip end paren */
		} else {
			/* error("parens") */ ;
			//pos = lexterm = lexstart;
		}
		if (val < 0)
			return val;
		val &= 07777;
		/* Now rig for caller to see the terminator */
		pos = lexterm = lexstart;
		delimiter = line[lexterm];

		loc = cpend[lc>>7];
		while ((loc > *plc) && (lit[loc] != val)) {
			loc--;
		}
		if (loc == *plc) {
			lit[*plc] = val;
			(*plc)--;
		}
		val = loc + (lc & 07600);

	} else {
fprintf(stderr, "The illegal character is '%c'\n", line[lexstart]);
		error("illegal char");
		val = 0;
	}
        if (!IsBlank(delimiter)) {
		nextlex();
	} else {
		lexstart = lexterm;
		lexterm++;
	}
	return val;
}

/* VRS:
 * Process an expression.  Whitespace is interpreted as an "OR" function,
 * modified if an MRI has been seen.  Evaluation is strictly left to
 * right, with no operator having precedence.
 * We are called with the first lexeme already found, so begin (and loop)
 * inspecting the operator and combining with the following operand.
 * Return is with the current lexeme being the terminator.
 * This replaces the former code, which incorrectly evaluated from
 * right to left, and also considered "OR" lower precedence than the
 * rest of the operators..
 * Currently "!" functions as an OR that pays no attention to MRI'ness.
 * I gather some assemblers find it useful as a way of forming sixbit.
 *
 * VRS 5/27/23 This function may now return undefined (-1).  Be sure to
 *     check for it!
*/
int
getexpr()
{
	int value = eval();
	int op, temp, ostart, oterm;

	/* We have set 'value' to the current token at entry, aka
	 * the left operand, and advanced the token to the operator.
	 * We need to keep things that way every time through the loop.
         *
         * VRS 5/17/23 The value may be undefined (-1), but we continue
         *     because we want to advance over the expression.
        */
	while (1) {
		op = delimiter; /* remember the operator */

		/* End of statement is also end of expression */
		if (isdone(op)) { /* end of statement/expression */
			return value;
		}
		/* These next terminate expressions, but not statements */
		if (op == '<') {  /* end of expression */
			return value;
		}
		if (op == ')') {  /* end of expression */
			return value;
		}
		if (op == ']') {  /* end of expression */
			return value;
		}

// BUGBUG: Want to allow 'X = 2':
// If the operator is followed by another operator, the first better be
// a blank.  Also, this would change '5 -2' to evaluate to 3, not -2.
// TBD.
		/*
		 * Not done, so interpret the next operand.
		*/
		nextlex(); /* advance over the operator */
		/* We may have advanced over space to the end */
		if (isdone(line[lexstart])) { /* end of statement/expression */
			return value;
		}
		/* These next terminate expressions, but not statements */
		if (line[lexstart] == '<') {  /* end of expression */
			return value;
		}
		if (line[lexstart] == ')') {  /* end of expression */
			return value;
		}
		if (line[lexstart] == ']') {  /* end of expression */
			return value;
		}
		ostart = lexstart;
		oterm = lexterm;
		temp = eval();	/* Evaluate second operand */
		/*
		 * VRS 5/27/23 Regardless of op, if either side is undef,
                 *     result should be undef.
                */
//fprintf(stderr, "%d: value=%o, temp=%o, op='%c'\n", lineno, value, temp, op);
		if ((value < 0) || (temp < 0)) {
			value = -1;
		} else if (IsBlank(op)) {
			/* interpret space as logical or */
			if (value <= 07777) { /* normal 12 bit value */
				value = value | temp;
			} else if (temp > 07777 & (020000 != (temp&~07777))) { /* or together MRI opcodes */
				value = value | temp;
			} else {
				/* The left operand is MRI, the right isn't. */
				/* Rescan the right, as an expression rather */
				/* than a term. */
/* BUGBUG: This rescan causes error messages to repeat! */
				lexstart = ostart;
				lexterm = pos = oterm;
//BUGBUG: Check for undef (-1) from getexpr()?
				temp = getexpr() & 07777;
				/* Now proceed, dealing with offpage, etc. */
				if (temp < 0200) { /* page zero MRI */
					value = value | temp;
				} else if (   ((lc & 07600) <= temp)
					   && (temp <= (lc | 00177)) ) {
					/* current page MRI */
					value = value | 00200 | (temp & 00177);
				} else if (value & 0400) {
					error("offpage ind");
				} else {
					/* off page MRI */
					int loc;
					int *lit = cp;
					int *plc = &cplc;

					if ((lc & 07600) == 0) {
						lit = pz;
						plc = &pzlc;
					}
					if (linkmsg) {
						error("off page");
						errors--; /* warning only */
					}
					/* having complained, fix it up */
					loc = pzend[field];
					while ((loc>*plc) && (lit[loc]!=temp)) {
						loc--;
					}
					if (loc == *plc) {
						lit[*plc] = temp;
						(*plc)--;
					}
					// Fatal if already indirect */
					if (value & 0400)
						error("offpage ind");
					value = value | 00600 | loc;
				}
				pos = lexterm = lexstart;
				delimiter=line[lexstart];
			}
		} else if (op == '+') { /* add */
			value = sum(value, temp);
		} else if (op == '-') { /* subtract */
			value = sum(value, negative(temp));
		} else if (op == '^') { /* multiply */
			value = value * temp;
		} else if (op == '%') { /* divide */
			if (temp == 0) {
				error("divide by 0");
			} else {
				value = value / temp;
			}
		} else if (op == '&') { /* and */
			value = value & temp;
		} else if (op == '!') { /* or */
			/* OPTIONAL PATCH 2
			*/
			/* value = (value << 6) | temp; */
			value = value | temp;
		} else {
			error("expression");
			return 0;
		}
	}
}

/* BUGBUG: Just punt for now. */
int
getexprs()
{
	return getexpr();
}

void
onepass()
/* do one assembly pass */
{
	lc = 0200; /* Some code expects this! */
	if (obj != NULL) { /* ... and on the tape */
		putorg( lc );
	}
	reloc = 0;
	field = 0;
	for (pzlc = 0; pzlc < FIELDS; pzlc++)
		pzend[pzlc] = 0177;
	pzlc = 00177; /* points to end of page for [] operands */
	for (cplc = 0; cplc < PAGES; cplc++)
		cpend[cplc] = 0177;
	cplc = 00177; /* points to end of page for () operands */
	radix = 8;
	listed = 1;
	lineno = 0;
	if (cflag) {
		nmode = cmode = pal8; /* Start in PAL mode */
	} else {
		nmode = cmode = pmode; /* Start in PMODE */
	}

getline:
	readline();
	nextlex();

restart:
	if (line[lexstart] == '/') {
		goto getline;
	}
	if (isend(line[lexstart])) {
		goto getline;
	}
	if (line[lexstart] == ';') {
		nextlex();
		goto restart;
	}
	if (IsEOF(line[lexstart])) {
		putcp();
		putpz(); /* points to end of page for () operands */
		listline(); /* if it hasn't been listed yet, list it! */
		return;
	}
	if (line[lexstart] == '*') {
		int newlc;
		nextlex(); /* skip * (set origin symbol) */
//BUGBUG: Check for undef (-1) from getexpr()?
		newlc = getexpr() & 07777;
		if (cmode == lmode) /* Adjust if LMODE */
			newlc = (lc & 06000) + (newlc & 01777);
		if ((newlc & 07600) != (lc & 07600)) { /* we changed pages */
			putcp();
			cplc = cpend[newlc>>7];
		}
		lc = newlc;
		/* reloc = 0; /* VRS: Retain existing RELOC */
		if (rimflag != 1) { /* put out origin if not in rim mode */
			if (obj != NULL) {
				putorg( lc );
			}
		}
		goto restart;
	}
	if (line[lexterm] == ',') {
		if (isalpha(line[lexstart])) {
                        /* Can't call evalsym() without mangling
                         * the reference count.
                        */
                        int ev;
			/* Remember if done in LINC mode. */
			if (0 || (cmode == lmode)) {
				ev = deflex( lexstart, lexterm, lc | 020000 );
				if ((ev >= 0) && (cmode == lmode))
					ev &= 01777;
			} else
				ev = deflex( lexstart, lexterm, lc & 07777 );
			if ((ev != -1) && (ev != (lc & ((cmode == lmode)? 01777 : 07777) ))) {
				error("redefined");
			}
		} else {
			error("label");
		}
		nextlex(); /* skip label */
		nextlex(); /* skip comma */
		goto restart;
	}
	if (line[lexterm] == '=') {
//BUGBUG: This isn't allowing space before '=' ("A = 2")
//Something to do with ' ' as OR operator?
		if (isalpha(line[lexstart])) {
			int start = lexstart;
			int term = lexterm;
			nextlex(); /* skip symbol */
			nextlex(); /* skip trailing = */
			deflex( start, term, 07777 & getexprs() );
		} else {
			error("symbol");
			nextlex(); /* skip symbol */
			nextlex(); /* skip trailing = */
			(void) getexprs(); /* skip expression */
		}
		goto restart;
	}
	if (isalpha(line[lexstart])) {
		int val;
		val = evalsym();
		if (val > 047777) { /* LINC op */
			nextlex(); /* skip symbol */
			val = val & 07777;
			/* Top two bits select instruction format */
			if (val & 06000) { /* Direct addressing */
				/* interpret 10 bit address */
				if (!isdone(line[lexstart]))
					val += getexprs() & 01777; /* 10 bit address */
				putout( lc, val );
				lc = (lc+1) & 07777;
				if ((lc+1 & 07600) != (lc & 07600))
					putcp();
				cplc = cpend[lc>>7];
				goto restart;
// BUGBUG: remove	} else if (val & 01000) { /* "B" class */
// Are A and B format instructions actually parsed differently?
                        } else { /* "A" class */
				if (!isdone(line[lexstart]))
					val += getexprs() & 037; /* 4 bit address */
				putout( lc, val );
				lc = (lc+1) & 07777;
				if ((lc+1 & 07600) != (lc & 07600))
					putcp();
				cplc = cpend[lc>>7];
				if (val == 00500) /* IOB */
					nmode = pmode; /* 1-off PMODE */
				goto restart;
			}
		} else if (val > 037777) { /* pseudo op */
			nextlex(); /* skip symbol */
			val = val & 07777;
			switch (val) {
			case 0: /* DECIMAL */
				radix = 10;
				break;
			case 1: /* OCTAL */
				radix = 8;
				break;
			case 2: /* ZBLOCK */
				val = getexpr();
//BUGBUG: Check for undef from getexpr()
				val &= 07777;
				if (cmode == lmode)
					val &= 01777;
				if (val+(lc&07777)-1 > 07777) {
					error("too big");
				} else {
					for ( ;val > 0; val--) {
						putout( lc, 0 );
						lc++;
					}
				}
				break;
			case 3: { /* PAGE */
				    int newlc;
				    if (isdone(line[lexstart])) { /* no arg */
					newlc = ((lc-1) & 07600) + 00200;
                                    } else {
                                        val = getexpr();
//BUGBUG: Check for undef from getexpr()
					newlc = (val & 037) << 7;
                                    }
				    if ((newlc & 07600) != (lc & 07600)) {
					/* we changed pages */
					putcp();
					cplc = cpend[newlc>>7];
				    }
				    lc = newlc;
				}
				if (rimflag != 1) {
					if (obj != NULL) {
						putorg( lc );
					}
				}
				break;
			case 045: /* SIXBIT */
				sixbit = 040;
				/* FALL THROUGH */
			case 4: /* TEXT */
				{
					char delim = line[lexstart];
					int pack = 0;
					int count = 0;
					int index = lexstart + 1;
					while ((line[index] != delim) &&
					       !isend(line[index])       ) {
						pack = (pack << 6)
						     | (line[index]+sixbit & 077);
						count++;
						if (count > 1) {
							putout( lc, pack );
							lc++;
							count = 0;
							pack = 0;
						}
						index++;
					}
					sixbit = 0;
					if (count != 0) {
						putout( lc, pack << 6 );
						lc++;
					} else if (!jflag) {
						putout( lc, 0 );
						lc++;
					}
					if (isend(line[index])) {
						lexterm = index;
						pos = index;
						error("parens");
						errors--; /* Warning */
						nextlex();
					} else {
						lexterm = index + 1;
						pos = index + 1;
						nextlex();
					}
				}
				break;
			case 5: /* EJECT */
				if (lst != NULL) {
					/* this will do for now */
					if (dosmode)
						fputc( '\r', lst );
					fprintf( lst, "\n\f" );
					if (dosmode)
						fputc( '\r', lst );
					fputc( '\n', lst );
				}
				goto getline;
				break;
			case 6: /* FIELD */
				if (isdone(line[lexstart])) {
					/* blank FIELD directive */
					val = field + 1;
				} else {
					/* FIELD with an argument */
// PQS8 apparently doesn't dump page zero literals if 4000 bit set.
// That seems wildly unsafe, as well as undocumented, so we don't
// do that here.
					val = getexpr() & 07777;
//BUGBUG: Is this OK for fields > 7?
					if ((val & 07) == 0)
						val /= 010;
				}
				// Force dump these for compatibility. */
				putcp();
				putpz();
				if ((lc&07600) != 0200) {
					putcp();
                                        cplc = cpend[0200>>7];
				}
				if (val != field) {
					putcp();
                                        cplc = cpend[0200>>7];
					putpz();
				        pzlc = pzend[val];
				}
				if (rimflag == 1) { /* can't change fields */
					error("rim mode");
				} else if ((val > 7) || (val < 0)) {
					error("field");
				} else if (obj != NULL) { /* change field */
		/*	b.baehr	patch	puto( ((val & 0007)<<3) | 0300 ); */
					fputc( ((val & 0007)<<3) | 0300, obj );
				}
				field = val; /* VRS: does not depend on obj */
				lc = 0;
				/* OPTIONAL PATCH 4 -- delete next line */
				lc = 0200;
				cplc = cpend[lc>>7];
				if (rimflag != 1) {
					if (obj != NULL) {
						putorg( lc );
					}
				}
				break;
			case 7: /* NOPUNCH */
				obj = NULL;
				break;
			case 010: /* ENPUNCH */
				obj = objsave;
				break;
			case 011: /* XLIST */
				if (isdone(line[lexstart])) {
					/* blank XLIST directive */
					FILE *temp = lst;
					lst = lstsave;
					lstsave = temp;
				} else {
					/* XLIST with an argument */
					if (getexpr() == 0) { /* ON */
						if (lst == NULL) {
							lst = lstsave;
							lstsave = NULL;
						}
					} else { /* OFF */
						if (lst != NULL) {
							lstsave = lst;
							lst = NULL;
						}
					}
				}
				break;
			case 012: /* IFZERO */
				if ((getexprs() & 07777) == 0) {
					condtrue();
				} else {
					condfalse();
				}
				break;
			case 013: /* IFNZRO */
				if ((getexprs() & 07777) == 0) {
					condfalse();
				} else {
					condtrue();
				}
				break;
			case 014: /* IFDEF */
				if (isalpha(line[lexstart])) {
					int val = evalsym();
					nextlex();
					if (val >= 0) {
						condtrue();
					} else {
						condfalse();
					}
				} else {
					error( "identifier" );
				}
				break;
			case 015: /* IFNDEF */
				if (isalpha(line[lexstart])) {
					int val = evalsym();
					nextlex();
					if (val >= 0) {
						condfalse();
					} else {
						condtrue();
					}
				} else {
					error( "identifier" );
				}
				break;
			case 016: /* RELOC */
				/* VRS: Rework the treatment of "reloc" and "lc".	*/
				/* We are almost exclusively interested for "lc" to	*/
				/* Refer to the address for code generation, not the	*/
				/* actual load address.  So, RELOC now sets "lc" to	*/
				/* the requested value (similar to "*"), and calculates	*/
				/* "reloc" so that "lc+reloc" gives the address we're	*/
				/* loading at.						*/
				/* NOTE: Combining "*" with RELOC is not advised.  For	*/
				/* compatibility with existing (PQS8) assemblers, "*"	*/
				/* does NOT alter the current relocation offset.	*/
				/* NB: "lc" does not include field, and must wrap.	*/

				/* RELOC is documented to always push out */
				/* literals */
				putcp();
				/* putcp() may have disturbed "." */
				if (obj != NULL)
					putorg(lc);

				if (isdone(line[lexstart])) {
					/* RELOC without arg */
					val = (lc + reloc) & 07777;
				} else {
					/* RELOC with an argument */
//BUGBUG: Check for undef from getexpr()
					val = getexpr() & 07777;
				}
				/* Cancel old RELOC. *.
				 * NOTE: Must have already done getexpr()
                                 * since "." may have been used there.
				*/
				lc += reloc;
				lc &= 07777;
				reloc = 0;
				cplc = cpend[val>>7];
				/* Implement new RELOC. */
				if ((val & 07600) != (lc & 07600)) {
					cplc = cpend[val>>7];
				}
				reloc = (lc - val) & 07777;
				lc = val;
				break;
			case 017: /* SEGMNT */
				putcp();
				if (isdone(line[lexstart])) { /* no arg */
//BUGBUG: SEGMNT doesn't dump literals!
					lc = lc+01777 & 06000;
                                } else {
                                        val = getexpr();
//BUGBUG: SEGMNT doesn't dump literals!
//BUGBUG: Check for undef from getexpr()
					lc = (val & 003) << 10;
                                }
				cplc = cpend[lc>>7];
				if (rimflag != 1) {
					if (obj != NULL) {
						putorg( lc );
					}
				}
				break;
			case 020: /* BANK   */
				error("unsupported");
				/* should select a different 32K out of 128K */
				break;
			case 021: /* FIXMRI */
				if ((line[lexterm] == '=')
				&& (isalpha(line[lexstart]))) {
					int start = lexstart;
					int term = lexterm;
					nextlex(); /* skip symbol */
					nextlex(); /* skip trailing = */
					if (cmode == lmode) {
						deflex( start, term, 050000 | getexprs() );
					} else {
						deflex( start, term, 010000 | getexprs() );
					}
				} else {
					error("symbol");
					nextlex(); /* skip symbol */
					nextlex(); /* skip trailing = */
					(void) getexprs(); /* skip expr */
				}
				break;
			case 022: /* DEVICE */
				{	int count = 0, p;
					unsigned long d = 0;
					for (p = lexstart; p < lexterm; p++) {
						if (++count > 4)
							break;
						d = (d << 6) | (line[p] & 077);
					}
					while (++count <= 4) {
						d = d << 6;
					}
					putout(lc, (d >> 12) & 07777); lc++;
					putout(lc,  d        & 07777); lc++;
					nextlex(); /* skip device name */
				}
				break;
			case 023: /* FILENAME */
				{	int p = lexstart, d;
					/* VRS: nextlex() probably saw an    */
					/* identifier, but in the case where */
					/* it saw a number, fix things up.   */
					while (isalnum(line[pos])) {
						pos++; /* keep going */
					}
					lexterm = pos;
					if (p < lexterm)
						d = line[p++] & 077;
					else
						d = 0;
					if (p < lexterm)
						d = (d << 6) | (line[p++] & 077);
					else
						d = d << 6;
					putout(lc, d & 07777); lc++;
					if (p < lexterm)
						d = line[p++] & 077;
					else
						d = 0;
					if (p < lexterm)
						d = (d << 6) | (line[p++] & 077);
					else
						d = d << 6;
					putout(lc, d & 07777); lc++;
					if (p < lexterm)
						d = line[p++] & 077;
					else
						d = 0;
					if (p < lexterm)
						d = (d << 6) | (line[p++] & 077);
					else
						d = d << 6;
					putout(lc, d & 07777); lc++;
					nextlex(); /* skip filee name */
					if (line[lexstart] == '.') {
						nextlex(); /* skip dot */
						p = lexstart;
						/* VRS: probably saw an    */
						/* identifier, but in case */
						/* we saw a number, fix up.*/
						while (isalnum(line[pos])) {
							pos++; /* keep going */
						}
						lexterm = pos;
						if (p < lexterm)
							d = line[p++] & 077;
						else
							d = 0;
						if (p < lexterm)
							d = (d << 6) | (line[p++] & 077);
						else
							d = d << 6;
						putout(lc, d & 07777); lc++;
						nextlex(); /* skip file extension */
					} else {
						putout(lc, 0); lc++;
					}
				}
				break;
			case 024: /* FIXTAB */
				/* Not implemented.                        */
				/* There is an opportunity to move IOTs to */
				/* symtab, then make EXPUNGE/FIXTAB behave */
				/* more traditionally.                     */
				break;
			case 035: /* ERROR */
				/* Error allocates a word here so that the */
				/* symbol may be redefined in old code     */
				/* during pass 1.                          */
				lc = (lc+1) & 07777;
cplc = cpend[lc>>7];
				error("error");
				break;
			case 036: /* EXPUNG/E */
				/* The fixed symbols are fixed. */
				/* No need to free memory by deleting them. */
				/* Delete the user symbols, though */
//VRS Mustn't do this in pass 2, so just never mind for now.
//VRS				symtab[firstsym].sym[0] = 0;
				break;
#ifdef LINC
			case 06141: /* LINC */
				/* interpret line load value */
				putout( lc, 06141);
				if ((lc+1 & 07600) != (lc & 07600))
					putcp();
				lc = (lc+1) & 07777;
//BUGBUG: Don't change cplc unless page break.  Also need to emit literals.
				cplc = cpend[lc>>7];
				break;
			case 040: /* LMODE */
				nmode = cmode = lmode;
				break;
			case 043: /* PDP */
				putout( lc, 00002);
				if ((lc+1 & 07600) != (lc & 07600))
					putcp();
				lc = (lc+1) & 07777;
				cplc = cpend[lc>>7];
				/* FALL THROUGH */
			case 041: /* PMODE */
				nmode = cmode = pmode;
				break;
#endif
			case 042: /* PAUSE */
				/* Not implemented.                       */
				/* There is an opportunity to make this   */
				/* move on to the next source input file. */
				break;
			case 047: /* TITLE */
//BUGBUG: TITLE is accepted, but does nothing.
				{ /* Copy the argument text into the title. */
					int count = 0;
					pos = lexstart + 1;
					while (!isend(line[pos])) {
						title[count++] = line[pos++];
					}
					title[count] = 0;
					nextlex();
				}
				break;
			} /* end switch */
			goto restart;
		}
		/* fall through here if ident is not LINC or pseudo-op */
	}
	{ /* default -- interpret line as load value */
		putout( lc, getexprs() & 07777); /* interpret line load value */
		if ((lc+1 & 07600) != (lc & 07600)) { /* last in page */
			/* This error case is not detected by putcp(), */
			/* as it expects lc to be empty. */
			/* This seems to detect quite late, when leaving. */
			if (cplc != 0177)
				error( "overrun" ); /* overrun constant pool */
			putcp();
			cplc = cpend[(lc+1)>>7];
		}
		lc = (lc+1) & 07777;
		nmode = cmode; /* Not an IOB while in LMODE */
		goto restart;
	}
}


/* main program */
int
main(argc, argv)
int argc;
char *argv[];
{
	getargs(argc, argv);

	onepass();

	rewind(in);
	obj = fopen(objname, "wb"); /* must be "wb" under DOS */
	objsave = obj;
	lst = fopen(lstname, "w");
	lstsave = NULL;
	putleader();
	errors = 0;
	cksum = 0;

	onepass();

	if (lst == NULL) { /* undo effects of XLIST for any following dump */
		lst = lstsave;
	}
	if (dumpflag != 0) dump();
	obj = objsave; /* undo effects of NOPUNCH for any following checksum */
	if ((rimflag == 0) && (obj != NULL)) { /* checksum */
		/* for now, put out the wrong value */
		fputc( (cksum >> 6) & 0077, obj );
		fputc( cksum & 0077, obj );
	}
	putleader();
	exit (errors != 0);
	return 0;
}
