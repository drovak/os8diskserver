$JOB 	Batch Stream  "RECOVR.BI"


/	Recover a destroyed directory
/	Requires that FUTIL have the EVAL A
/	Patch in order to function

/	You must have run QUIT to create the DIRECT.TS
/	Directory backup for this to work.

.R FUTIL
SET DEVICE RKB0
FILE DIRECT.TS
COMMENT Save the starting block in the filler
SET FILLER (A)
COMMENT Read the first block of the saved directory
(F).0/
SET DEVICE RKA0
COMMENT Then write it to the disk (and so forth)
WRITE 1
SET DEVICE RKB0
(F+1).0/
SET DEVICE RKA0
WRITE 2
SET DEVICE RKB0
(F+2).0/
SET DEVICE RKA0
WRITE 3
SET DEVICE RKB0
(F+3).0/
SET DEVICE RKA0
WRITE 4
SET DEVICE RKB0
(F+4).0/
SET DEVICE RKA0
WRITE 5
SET DEVICE RKB0
(F+5).0/
SET DEVICE RKA0
WRITE 6
EXIT
$END
