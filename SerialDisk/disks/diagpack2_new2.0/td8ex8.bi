$JOB	/	Batch Stream  "TD8EX8.BI"
/
/        Creates a DECtape with TD8E monitor & DECX8
/
/	This Batch Stream may be run on either TD8E or TC08 hardware
/	if the proper SET HANDLER command is given prior to submission.
/
/
$MSG	***	MOUNT FORMATTED DECTAPE ON DRIVE 0 ... WRITE ENABLED	***
.ASSIGN DTA0 OUT
.ASSIGN SYS IN
.R PIP
*OUT:<IN:TD8E.SY/Y/Z/O
.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL.SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP.SV
*OUT:<IN:FOTP.SV
*OUT:<IN:SET.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:TDCOPY.SV
*OUT:<IN:TDFRMT.SV
*OUT:<IN:RKFRMT.SV
*OUT:<IN:DECX8.SV
*OUT:<IN:*.BX
.DEASSIGN
$END
