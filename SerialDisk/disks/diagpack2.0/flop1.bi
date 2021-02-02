$JOB	/	Batch Stream  "FLOP1.BI"
/
/	Creates Diagnostic Floppy #1 on a BYTE mode floppy
/
/	Contains VT78, PDP-8A / 8E  Diagnostics
/
/	NOTE:	This batch Stream must be run on RX01 Hardware
/		but after building, can be booted and run on either
/		RX01 or RX02 drives with modification.
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
*OUT:<IN:RESORC.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:HELP.SV
*OUT:<IN:RXCOPY.SV
*OUT:HELP.HL<IN:FLOP.HL
*OUT:<IN:LPT.HN,LQP.HN
*OUT:<IN:DKVTA?.DG
*OUT:<IN:AKVTB?.DG
*OUT:<IN:DJKKB?.DG
*OUT:<IN:AJEXC?.DG
*OUT:<IN:DJDKA?.DG
*OUT:<IN:DJKMA?.DG
*OUT:<IN:AJKTA?.DG
*OUT:<IN:DHKMA?.DG
*OUT:<IN:DHKMC?.DG
*OUT:<IN:DHMCA?.DG
*OUT:<IN:DJKLA?.DG
*OUT:<IN:DIKLB?.DG
*OUT:<IN:DHKLD?.DG
*OUT:<IN:DHKAA?.DG
*OUT:<IN:DHKAF?.DG
*OUT:<IN:DHKAG?.DG
.DEASSIGN

/
/			Do the magical FUTIL tricks ... Header info for FLOP1
/			is located 3 blocks into the HEADER.FU file.
/
/
.R FUTIL
FILE HEADER.FU
SET FILLER (A)
(F+3).0/
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
