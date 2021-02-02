$JOB	/	Batch Stream "RKMAKE.BI"
/
/
/	This makes an RK05 Diagnostic Pack from the
/	BYTE Mode Floppy Backup volumes.
/
/	The SYSTEM floppy should be mounted in Drive 0
/	and booted; the other ten floppys are fed to
/	Drive 1 sequencially upon prompting.
/
/	To make an RK05 Diagnostic Pack, you should have a
/	FORMATTED 16 sector pack mounted in RK05 Drive 0.
/	The various formatting programs are located on the
/	SYSTEM floppy. This batch stream, "RKMAKE.BI", will
/	control the building process.
/
/	Please mount the necessary media and
/
/
$MSG	Press [Return] when ready



.R PIP
*RKA0:<SYS:RK8EA.SY/Y/Z/O
*RKB0:<SYS:RK8EB.SY/Y/Z/O


/
/
/	This begins the building of RKA0
/
/	It will use Backup Volumes 1-5
/
/	Please make sure that Backup Volume #1 is mounted in Floppy Drive #1
/
/

$MSG	Press [Return] when ready


.DEASSIGN
.ASSIGN RXB1 IN
.ASSIGN RKA0 OUT

.COPY OUT:<IN:
.COMPARE OUT:,IN:
/
/	Now please mount Backup Volume #2 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	Now please mount Backup Volume #3 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/	More of "RKMAKE.BI"
/
/
/
/	Now please mount Backup Volume #4 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	Now please mount Backup Volume #5 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	This completes the building of RKA0 on the RK05
/
/	RKB0 will be built in a like manner using Backup Volumes 6-10
/
/
/	Now please mount Backup Volume #6 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.DEASSIGN
.ASSIGN RXB1 IN
.ASSIGN RKB0 OUT


.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	Now please mount Backup Volume #7 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	Now please mount Backup Volume #8 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/	Now please mount Backup Volume #9 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:
/	More of "RKMAKE.BI"
/
/
/	Now please mount Backup Volume #10 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
/
/	Now please mount Backup Volume #11 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:
.DEASSIGN

/
/	This completes the building of your new, latest - best
/
/
/	NEW ENGLAND DISTRICT
/
/	PDP-8 Diagnostic System   ...   Version 5
/
/
/
/	Now for a bit of Magic...
/
/

.R FUTIL
SET DEVICE RKA0
M N 0.0077
1077
WR 0
M N 11.0-4
"H,"E,"L,"P
WR 11
EXIT

/
/
/	Now booting to the RK05  ! ! !
/
/
.R RKBOOT/


$END
