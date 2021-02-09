$JOB	/	Batch Stream  "TC08X8.BI"
/
/	Creates a DECtape with TC08 monitor & DECX8
/
/	This Batch Stream may be run on either TD8E or TC08 hardware
/	if the proper SET HANDLER command is given prior to submission.
/
/
$MSG	***	MOUNT FORMATTED DECTAPE ON DRIVE 0 ... WRITE ENABLED	***
.ASSIGN DTA0 OUT
.ASSIGN SYS IN
.R PIP
*OUT:<IN:TC08.SY/Y/Z/O
.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL.SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP.SV
*OUT:<IN:FOTP.SV
*OUT:<IN:SET.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:RKFRMT.SV
*OUT:<IN:RKCOPY.SV
*OUT:<IN:DTCOPY.SV
*OUT:<IN:DTFRMT.SV
*OUT:<IN:RXCOPY.SV
*OUT:<IN:DECX8.SV
*OUT:<IN:*.BX
.DEASSIGN
$END
