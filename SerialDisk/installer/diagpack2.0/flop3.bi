$JOB	/	Batch Stream  "FLOP4.BI"
/
/	Creates Diagnostic Floppy #4 on a BYTE mode floppy
/
/	Contains Terminal, Printer, DECtape, & Misc Diagnostics
/
/	NOTE:	This Batch Stream MUST be run on RX01 Hardware
/		but after being built, may be booted and run on either
/		RX01 or RX02 drives without modification.
/
/
$MSG	***  Mount Floppy in Drive 0 ... press [Return] when ready  ***
/
.SET HANDLER FLOP=BYTE
/
.ASSIGN RXB0 OUT
.ASSIGN SYS IN
/
.R PIP
*OUT:<IN:BYTE.SY/Y/Z/O
/
.R RXFIX
/
.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL.SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP.SV
*OUT:<IN:FOTP.SV
*OUT:<IN:SET.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:HELP.SV
*OUT:<IN:RXCOPY.SV
*OUT:<IN:RKCOPY.SV
*OUT:<IN:RKUTIL.SV
*OUT:HELP.HL<IN:FLOP.HL
*OUT:<IN:AILAB?.DG
*OUT:<IN:DHVTC?.DG
*OUT:<IN:DIKL??.DG
*OUT:<IN:DHLQ??.DG
*OUT:<IN:DHLS??.DG
*OUT:<IN:DILAC?.DG
*OUT:<IN:DILPA?.DG
*OUT:<IN:DILPE?.DG
*OUT:<IN:DHTDA?.DG
*OUT:<IN:TCBASX.DG
*OUT:<IN:TCRANX.DG
*OUT:<RKB0:TYPSET.DG
*OUT:<RKB0:DHDHA?.DG
*OUT:<RKB0:DHDHC?.DG
/
.DEASSIGN
/
/			Do magical FUTIL tricks ... Header info for FLOP3 is
/			located 5 blocks into HEADER.FU file.
/
.R FUTIL
FILE HEADER.FU
SET FILLER (A)
(F+5).0/
SET DEVICE RXB0
WR 6
M N 66.0077
1077
WR 66
M N 11.0-4
"H,"E,"L,"P," 
WR 11
EXIT
/
.SET HANDLER BYTE=FLOP
/
$END
