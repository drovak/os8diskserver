$JOB	/	Batch Stream  "BACKUP.BI"
/
/	Makes a set of Backup BYTE Mode Floppys
/
/
/	This makes the SYS Diskette on Drive 0
/
/	It will have a System, CUSPs, and Batch Steams to make
/	both Diagnostic RK05 Disks and RL01 Packs
/
/	Before starting, you need either the complete set of
/	Backup Diskettes or 12 virgin diskettes that have been
/	initialized by ZEROing or running "BUILD" on each of them.
/
/	The system diskette is made on Drive #0
/
/	All of the Backup Volumes are made on Drive #1
/
/
$MSG  ***   Please press [Return] when ready   ***

.SET HANDLER FLOP=BYTE

.R PIP
*RXB0:<SYS:BYTE.SY/Y/Z/O

.R RXFIX

.DEASSIGN
.ASSIGN SYS IN
.ASSIGN RXB0 OUT

.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL.SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP.SV
*OUT:<IN:FOTP.SV
*OUT:<IN:SET.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:BATCH.SV
*OUT:<IN:BUILD.SV
*OUT:<IN:RXFIX.SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:COMPAF.SV
*OUT:<IN:RKFRMT.SV
*OUT:<IN:RKUTIL.SV
*OUT:<IN:RLFRMT.SV
*OUT:<IN:RL2FMT.SV
*OUT:<IN:RK8EA.SY
*OUT:<IN:RK8EB.SY
*OUT:<IN:RL8A.SY
*OUT:<IN:RXSY1.SY
*OUT:<IN:RXSY2.SY
*OUT:<IN:BYTE.SY
*OUT:<IN:RKMAKE.BI
*OUT:<IN:RLMAKE.BI

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/	This makes the first of the Backup Volumes (BYTE Mode) on Drive #1
/
/	Please mount Backup Volume #1 in Drive #1
/
/
$MSG   ***   Please press [Return] when ready   ***

.DEASSIGN
.ASSIGN SYS IN
.ASSIGN RXB1 OUT

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL.SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP.SV
*OUT:<IN:FOTP.SV
*OUT:<IN:SET.SV
*OUT:<IN:PAL8.SV
*OUT:<IN:CREF.SV
*OUT:<IN:FUTIL.SV
*OUT:<IN:CONVRT.SV
*OUT:<IN:SVDUMP.SV
*OUT:<IN:HEADER.SV
*OUT:<IN:REMEM.SV
*OUT:<IN:COMPAF.SV
*OUT:<IN:RTFLOP.SV
*OUT:<IN:WPFLOP.SV
*OUT:<IN:RESORC.SV
*OUT:<IN:QUIT.SV
*OUT:<IN:BATCH.SV
*OUT:<IN:HELP.SV
*OUT:<IN:EDIT.SV
*OUT:<IN:TECO.SV
*OUT:<IN:LPTSPL.SV
*OUT:<IN:BUILD.SV
*OUT:<IN:BUILDR.SV
*OUT:<IN:BOOT.SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:RKUTIL.SV
*OUT:<IN:RKFRMT.SV
*OUT:<IN:RLFRMT.SV
*OUT:<IN:RL2FMT.SV
*OUT:<IN:TDFRMT.SV
*OUT:<IN:DTFRMT.SV
*OUT:<IN:TDCOPY.SV
*OUT:<IN:DTCOPY.SV
*OUT:<IN:RKCOPY.SV
*OUT:<IN:RXCOPY.SV
*OUT:<IN:RXREAD.SV
*OUT:<IN:RXFIX.SV
*OUT:<IN:DECX8.SV
*OUT:<IN:TECO.IN
*OUT:<IN:VTEDIT.TE
*OUT:<IN:INUSE.TE
*OUT:<IN:*.TE
*OUT:<IN:TECHLP.TX
*OUT:<IN:HEADER.FU

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/
/	This makes the second of the Backup Volumes (BYTE Mode) on Drive #1
/
/	Please mount Backup Volume #2 in Drive #1
/
$MSG   ***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:HELP.HL
*OUT:<IN:*.HL
*OUT:<IN:*.HN
*OUT:<IN:REMAKE.BI
*OUT:<IN:BACKUP.BI
*OUT:<IN:RKMAKE.BI
*OUT:<IN:RLMAKE.BI
*OUT:<IN:SUPER.BI
*OUT:<IN:DGPACK.BI
*OUT:<IN:CUSPAK.BI
*OUT:<IN:RLPACK.BI
*OUT:<IN:FLOP1.BI
*OUT:<IN:FLOP2.BI
*OUT:<IN:FLOP3.BI
*OUT:<IN:FLOPX8.BI
*OUT:<IN:TC08DG.BI
*OUT:<IN:TC08X8.BI
*OUT:<IN:TD8EDG.BI
*OUT:<IN:TD8EX8.BI
*OUT:<IN:BOOK.BI
*OUT:<IN:*.BI
*OUT:<IN:RK8EA.SY
*OUT:<IN:RK8EB.SY
*OUT:<IN:RKS8EA.SY

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/
/	This makes the third of the Backup Volumes
/
/	Please mount Backup Volume #3 in drive #1
/
$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:RKS8EB.SY
*OUT:<IN:RL8A.SY
*OUT:<IN:RXSY1.SY
*OUT:<IN:RXSY2.SY
*OUT:<IN:BYTE.SY
*OUT:<IN:TD8E.SY
*OUT:<IN:TC08.SY
*OUT:<IN:*.BX

.DIR OUT:/E=3-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/
/	This makes the fourth of the Backup Voulmes
/
/	Please mount Backup Volume #4 in Drive #1
/
$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:ALIGNX.DG
*OUT:<IN:AHKLAB.DG
*OUT:<IN:AILABE.DG
*OUT:<IN:AIRXAE.DG
*OUT:<IN:AIRXBF.DG
*OUT:<IN:AJEXCC.DG
*OUT:<IN:AJKTAB.DG
*OUT:<IN:AJRLAC.DG
*OUT:<IN:AJRLBB.DG
*OUT:<IN:AJRLCA.DG
*OUT:<IN:AJRLDA.DG
*OUT:<IN:AJRLEA.DG
*OUT:<IN:AJRLGA.DG
*OUT:<IN:AJRLHA.DG
*OUT:<IN:AJRLIA.DG
*OUT:<IN:AJRLJA.DG
*OUT:<IN:AJRLLA.DG
*OUT:<IN:AJRLKA.DG
*OUT:<IN:AKVTBB.DG
*OUT:<IN:DGMCAB.DG
*OUT:<IN:DHKAAB.DG
*OUT:<IN:DHKABA.DG
*OUT:<IN:DHKACA.DG
*OUT:<IN:DHKADA.DG
*OUT:<IN:DHKAFA.DG
*OUT:<IN:DHKAGA.DG
*OUT:<IN:DHKEBB.DG
*OUT:<IN:DHKECA.DG
*OUT:<IN:DHKLDA.DG
*OUT:<IN:DHKMAD.DG
*OUT:<IN:DHKMCC.DG

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/
/	This makes the fifth of the Backup Volumes
/
/	Please mount Backup Volume #5 in Drive #1

$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:DHKMCC.DG
*OUT:<IN:DHKPAB.DG
*OUT:<IN:DHLQAB.DG
*OUT:<IN:DHLSAC.DG
*OUT:<IN:DHMCAB.DG
*OUT:<IN:DHMPAA.DG
*OUT:<IN:DHPCAA.DG
*OUT:<IN:DHRKAE.DG
*OUT:<IN:DHRKBG.DG
*OUT:<IN:DHRKCH.DG
*OUT:<IN:DHRKEA.DG
*OUT:<IN:DHTDAB.DG
*OUT:<IN:DHVTCD.DG
*OUT:<IN:DIKLAC.DG
*OUT:<IN:DIKLBA.DG
*OUT:<IN:DILACB.DG
*OUT:<IN:DILPAC.DG
*OUT:<IN:DILPEC.DG
*OUT:<IN:DJDKAD.DG
*OUT:<IN:DJFPAB.DG
*OUT:<IN:DJFPBC.DG
*OUT:<IN:DJKKBA.DG
*OUT:<IN:DJKLAD.DG
*OUT:<IN:DJKMAC.DG
*OUT:<IN:DKVTAA.DG
*OUT:<IN:TCBASX.DG
*OUT:<IN:TCRANX.DG

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/
/
/	This completes the backup of RKA0
/
/
/	More of "BACKUP.BI"
/
/	This makes the sixth of the Backup Volumes
/	and begins the backup of RKB0:
/
/	Please mount Backup Volume #6 in Drive #1
/
$MSG	***   Please press [Return] when ready   ***

.DEASSIGN
.ASSIGN RKB0 IN
.ASSIGN RXB1 OUT

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:ABSLDR.SV
*OUT:<IN:CCL   .SV
*OUT:<IN:DIRECT.SV
*OUT:<IN:PIP   .SV
*OUT:<IN:FOTP  .SV
*OUT:<IN:SET   .SV
*OUT:<IN:FUTIL .SV
*OUT:<IN:BATCH .SV
*OUT:<IN:HELP  .SV
*OUT:<IN:FRTS  .SV
*OUT:<IN:RKBOOT.SV
*OUT:<IN:BASIC .SV
*OUT:<IN:BLOAD .SV
*OUT:<IN:BCOMP .SV
*OUT:<IN:BRTS  .SV
*OUT:<IN:CHESS .SV
*OUT:<IN:ADVENT.SV
*OUT:<IN:BASIC .OV
*OUT:<IN:BASIC .UF
*OUT:<IN:ADVENT.DA

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/	This makes the seventh of the Backup Volumes
/
/	Please mount Backup Volume #7 in Drive #1
/
/
$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:ADVENT.LD
*OUT:<IN:ADVENT.TX
*OUT:<IN:HELP  .HL
*OUT:<IN:GAMES .HL
*OUT:<IN:*.BH
*OUT:<IN:RECOVR.BI

.DIR OUT:/E=3-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/	This makes the eighth of the Backup Volumes
/
/	Please mount Backup Volume #8 in Drive #1
/
/
$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:*.SY
*OUT:<IN:FUTIL.TX
*OUT:<IN:ALIGN.TX
*OUT:<IN:DECX8.TX

.DIR OUT:/E-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/	This makes the ninth of the Backup Volumes
/
/	Please mount Backup Volume #9 in Drive #1
/
/
$MSG	***   Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:OS8.TX
*OUT:<IN:DIAG.TX
*OUT:<IN:TITLE.TX
*OUT:<IN:*.BA

.DIR OUT:/E=3-L

.COMPARE OUT:,IN:
/	More of "BACKUP.BI"
/
/	This makes the tenth of the Backup Volumes
/
/	Please mount Backup Volume #10 in drive #1
/
/
$MSG	***    Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:TYPSET.DG
*OUT:<IN:SHELLY.DG
*OUT:<IN:AUV???.DG
*OUT:<IN:DGV5A?.DG
*OUT:<IN:DHADA?.DG
*OUT:<IN:DHCMAA.DG
*OUT:<IN:DHCRAA.DG
*OUT:<IN:DHDHAA.DG
*OUT:<IN:DHDHCA.DG
*OUT:<IN:DHDKAA.DG
*OUT:<IN:DHDPAD.DG
*OUT:<IN:DHDRAB.DG
*OUT:<IN:DHDRHA.DG
*OUT:<IN:DHICAA.DG
*OUT:<IN:DHKGAB.DG
*OUT:<IN:DHKLBB.DG
*OUT:<IN:DHKLCD.DG
*OUT:<IN:DHLAAB.DG
*OUT:<IN:DHTAAC.DG
*OUT:<IN:DHTABC.DG
*OUT:<IN:DHTMAB.DG
*OUT:<IN:DHTMBB.DG
*OUT:<IN:DHTMCA.DG
*OUT:<IN:DHTMDB.DG
*OUT:<IN:DHTMEB.DG
*OUT:<IN:DHTMFC.DG
*OUT:<IN:DHTSAB.DG
*OUT:<IN:DHTSBB.DG
*OUT:<IN:DHTSCB.DG
*OUT:<IN:DHTSDB.DG
*OUT:<IN:DHTSEA.DG
*OUT:<IN:DHTSFA.DG
*OUT:<IN:DHVCAA.DG
*OUT:<IN:DHVTAB.DG
*OUT:<IN:DHVTBA.DG
*OUT:<IN:DHVTDA.DG

.DIR OUT:/E=2-L

.COMPARE OUT:,IN:

/	More of "BACKUP.BI"
/
/	This makes the eleventh of the Backup Volumes
/
/	Please mount Backup Volume #11 in drive #1
/
/
$MSG	***    Please press [Return] when ready   ***

.R FOTP
*OUT:*.*/D
.SQU OUT:/OK

.R FOTP
*OUT:<IN:DIADFA.DG
*OUT:<IN:DIAFBA.DG
*OUT:<IN:DIDBAA.DG
*OUT:<IN:DIDFAC.DG
*OUT:<IN:DIDFBA.DG
*OUT:<IN:DIDFCA.DG
*OUT:<IN:DILTCA.DG
*OUT:<IN:DILTDA.DG
*OUT:<IN:DILVAA.DG
*OUT:<IN:DIRTAB.DG
*OUT:<IN:DITCBA.DG
*OUT:<IN:DITCCA.DG
*OUT:<IN:DITCDA.DG
*OUT:<IN:DIUDAC.DG
*OUT:<IN:DIVTBA.DG
*OUT:<IN:DIVTCA.DG
*OUT:<IN:DJADAC.DG
*OUT:<IN:DJVKAB.DG

.DIR OUT:/E-L

.COMPARE OUT:,IN:

$MSG	***    Please press [Return] when ready   ***

.DEASSIGN
.SET HANDLER BYTE=FLOP

/
/
/	*************************************************
/	*						*
/	*	This completes the Backup Process	*
/	*						*
/	*	These BYTE Mode Floppys contain a	*
/	*	complete  backup  of  Version   5 	*
/	*	of the  N.E.D.  Diagnostic System	*
/	*						*
/	*************************************************
/
/
/
$END
