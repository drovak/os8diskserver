$JOB	/	Batch Stream "FLOPX8.BI"
/
/	Creates DECX8 Floppy on a BYTE Mode Floppy
/
/	Contains DECX8 Monitor/Builder & Exerciser Modules
/
$MSG	***  Mount DECX8 Floppy in Drive 0 ... press [RETURN] when ready ***
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
*OUT:<IN:RXCOPY.SV
*OUT:<IN:RKUTIL.SV
*OUT:<IN:HELP.SV
*OUT:HELP.HL<IN:FLOP.HL
*OUT:<IN:DECX8.SV
*OUT:<IN:*.BX
*OUT:<RKB0:DECX8.TX
/
.DEASSIGN
/
/		Do the magical FUTIL tricks ... Header info for DECX8 floppy
/		is located 6 blocks into HEADER.FU file.
/
.R FUTIL
FILE HEADER.FU
SET FILLER (A)
(F+6).0/
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
