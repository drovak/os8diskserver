$JOB	/	Batch Stream  "ADVFLP.BI"
/
/
/	Creates a ADVENTURE floppy on a BYTE mode Diskette
/
/	NOTE:	This Batch Stream MUST be run on RX01 hardware
/		but after it is built, may be run on either
/		RX01 (single density) or RX02 (double density)
/		drives with no modification.
/
/
$MSG	***  Mount New BYTE MODE Floppy Diskette in Drive 0  ***
.SET HANDLER FLOP=BYTE
.R PIP
*RXB0:<BYTE.SY/Y/Z/O
.R RXFIX
.R FOTP
*RXB0:<CCL.SV
*RXB0:<DIRECT.SV
*RXB0:<PIP.SV
*RXB0:<FOTP.SV
*RXB0:<SET.SV
*RXB0:<RXCOPY.SV
*RXB0:<BOOT.SV
*RXB0:ADV.SV<RKB0:ADVENT.SV
*RXB0:<RKB0:ADVENT.DA
*RXB0:<LPT.HN
*RXB0:<LQP.HN
.R FUTIL
SET DEVICE SYS
FILE HEADER.FU
SET FILLER (A)
(F+1).0/
SET DEVICE RXB0
WR 6
M N 66.0077
1077
WR 66
M N 11.0-4
"R, " ,"A,"D,"V
WR 11
EXIT

.SET HANDLER BYTE=FLOP
$END
