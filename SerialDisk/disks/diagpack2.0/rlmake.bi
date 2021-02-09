$JOB	/	Batch Stream "RLMAKE.BI"
/
/
/	This makes an RL01 Diagnostic Pack from the
/	BYTE Mode Floppy Backup volumes.
/
/	The SYSTEM floppy should be mounted in Drive 0
/	and booted; the other eleven floppys are fed to
/	Drive 1 sequencially upon prompting.
/
/	To make an RL01 Diagnostic Pack, you should have a
/	"FORMATTED"  RL01 pack mounted in  RL01  Drive 0.
/	The various formatting programs are located on the
/	SYSTEM floppy. This batch stream, "RLMAKE.BI", will
/	control the building process.
/
/	Please mount the necessary media and
/
/
$MSG	Press [Return] when ready



.R PIP
*RL0A:<SYS:RL8A.SY/Y/Z/O
*RL0B:</Z/O


/
/
/	This begins the building of RL0A:
/
/	It will use Backup Volumes 1-5
/
/	Please make sure that Backup Volume #1 is mounted in Floppy Drive #1
/
/

$MSG	Press [Return] when ready


.DEASSIGN
.ASSIGN RXB1 IN
.ASSIGN RL0A OUT

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

/	More of "RLMAKE.BI"
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
/	This completes the building of RL0A on the RL01
/
/	RL0B will be built in a like manner using Backup Volumes 6-10
/
/
/	Now please mount Backup Volume #6 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.DEASSIGN
.ASSIGN RXB1 IN
.ASSIGN RL0B OUT


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
/	More of "RLMAKE.BI"
/
/
/	Now please mount Backup Volume #10 in Floppy Drive #1 and
/

$MSG	Press [Return] when ready

.COPY OUT:<IN:
.COMPARE OUT:,IN:

/
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
SET DEVICE RL0A
M N 66.0077
1077
WR 66
M N 11.0-4
"H,"E,"L,"P
WR 11
EXIT

/
/
/	Now booting to the RL01  ! ! !
/
/
.BO/RL

$END
