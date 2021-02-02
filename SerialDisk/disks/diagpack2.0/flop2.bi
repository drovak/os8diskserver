$JOB	/	Batch Stream  "FLOP2.BI"
/
/	Creates Diagnostic Floppy #2 on a BYTE mode floppy
/
/	Contains RX01, RK05, RL01, & RL02  Diagnostics
/
/	NOTE:	This Batch Stream MUST be run on RX01 Hardware
/		but after building may be booted and run on either
/		RX01 or RX02 drives without modification.
/
/
$MSG	***  Mount floppy in Drive 0 ... press [Return] when ready  ***
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
*OUT:<IN:FUTIL.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:RKCOPY.SV
*OUT:<IN:RKUTIL.SV
*OUT:<IN:RXCOPY.SV
*OUT:<IN:RXREAD.SV
*OUT:<IN:RKFRMT.SV
*OUT:<IN:RLFRMT.SV
*OUT:<IN:RL2FMT.SV
*OUT:<IN:HELP.SV
*OUT:HELP.HL<IN:FLOP.HL
*OUT:<IN:LPT.HN,LQP.HN
*OUT:<IN:AIRX??.DG
*OUT:<IN:ALIGNX.DG
*OUT:<IN:DHRKA?.DG
*OUT:<IN:DHRKB?.DG
*OUT:<IN:DHRKC?.DG
*OUT:<IN:AJRL??.DG
/
.DEASSIGN
/
/			Do the magical FUTIL tricks ... Header info for FLOP2
/			is located 4 blocks into the HEADER.FU file.
/
.R FUTIL
FILE HEADER.FU
SET FILLER (A)
(F+4).0/
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
