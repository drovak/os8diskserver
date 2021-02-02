   1               /       SDSKNS OS/8 NON-SYSTEM HANDLER
   2               
   3               /       SERIAL INTERFACE-BASED DISK NON-SYSTEM DEVICE HANDLER [USE WITH PC SERVER].
   4               
   5               /       LAST EDIT: 01-FEB-2021 VRS
   6               
   7               /       EDIT HISTORY.
   8               
   9               /       01-FEB-2021     VINCE SLYNGSTAD
  10               
  11               /       1) REMOVED RKIE INSTRUCTION, REPLACING IT WITH A FAMILY-OF-EIGHT
  12               /          COMPATIBLE SOLUTION TO THE FORTRAN HANG ISSUE.
  13               /       2) REMOVED IOT DEFINITIONS INCONSISTENT WITH USAGE ACROSS THE ENTIRE
  14               /          FAMILY-OF-EIGHT LINE.
  15               /       A WORD ABOUT THE FORTRAN ISSUE IS PROBABLY APROPRIATE.  FORTRAN (AND
  16               /       POSSIBLY OTHER SOFTWARE) WORKS BY DISABLING INTERRUPTS, CALLING US
  17               /       (THE OS/8 DRIVER), THEN RE-ENABLING INTERRUPTS ON RETURN.  EVEN IF
  18               /       THEY DON'T EXPLICITLY CALL US, BUT WE'VE RUN SINCE THE SYSTEM BOOT,
  19               /       THE FORTRAN INTERRUPT SERVICE ROUTINE KNOWS NOTHING ABOUT OUR DEVICE,
  20               /       SO IF WE LEFT A FLAG SET ON RETURN, THAT ISR WILL BE CALLED, WILL
  21               /       FAIL TO CLEAR OUR DEVICE FLAG, WILL RETURN, AND IMMEDIATELY TAKE
  22               /       ANOTHER INTERRUPT, ESSENTIALLY HANGING THERE. THE FLAG REMAINS
  23               /       SET, AND NO FORWARD PROGRESS IS MADE.  EARLIER VERSIONS OF THIS
  24               /       DRIVER USED MASKING TO PREVENT THESE INTERRUPTS, BUT ONLY OMNIBUS
  25               /       AND LATER MACHINES IMPLEMENT MASKING INSTRUCTIONS.  IT IS THEREFORE
  26               /       ESSENTIAL THAT THIS DRIVER NOT LEAVE FLAGS SET, IF IT IS TO RUN ON
  27               /       OLDER SERIAL PORTS (OR NEWER ONES THAT DON'T IMPLEMENT MASKING).
  28               
  29               /       08-FEB-2018     CHARLES LASNER  SMALL CHANGES IN PREPARATION FOR SPEED-UP.
  30               
  31               /       NOTABLE CHANGES.
  32               
  33               /       1) MODIFIES THE 12-BIT WORD SEND AND RECEIVE ROUTINES IN PREPARATION FOR A
  34               /          POTENTIAL FUTURE CHANGE TO SPEED UP THE HANDLER AT THE EXPENSE OF REQUIRING
  35               /          OMNIBUS MACHINES.
  36               
  37               /       2) MINOR COSMETIC CHANGES TO CONFORM TO SOURCE CODE WIDTH LIMITATIONS OF PAL8.
  38               
  39               /       3) RELEASE UPDATED TO VERSION H.
  40               
  41               /       13-NOV-2016     CHARLES LASNER  BUG FIX AND MAJOR SOURCE CODE CLEANUP.
  42               
  43               /       NOTABLE CHANGES.
  44               
  45               /       1) FIXES BUG ASSOCIATED WITH PRE-OMNIBUS SERIAL INTERFACES BY REORDERING THE
  46               /          CODE THUS OVERALL LENGTH IS UNCHANGED.
  47               
  48               /       2) THE LONG AWAITED MAJOR SOURCE CODE CLEANUP TO INCLUDE [BETTER] COMMENTS AND
  49               /          CONFORMANCE WITH PDP-8 SOURCE CODE CONVENTIONS.
  50               
  51               /       6) RELEASE UPDATED TO VERSION G.
  52               
  53               /       06-AUG-2016     CHARLES LASNER  CODE CLEANUP.
  54               
  55               /       NOTABLE CHANGES.
  56               
  57               /       1) ADDS THIS EDIT HISTORY.
  58               
  59               /       2) REMOVES MINOR INCOMPATIBLE SOURCE CODE QUIRKS.  [NOTE: ALL CHANGES MADE
  60               /          ARE COSMETIC, BUT TECHNICALLY THE ORIGINAL CODE VIOLATED THE PAL LANGUAGE
  61               /          SPECIFICATION.  OS/8 PAL8 IS DEFICIENT AT FLAGGING CERTAIN ERRORS THAT WILL
  62               /          BE PROPERLY DETECTED BY OTHER ASSEMBLY PROGRAMS SUCH AS PAL III, P?S/8 PAL,
  63               /          THE LAP6-DIAL/DIAL-MS ASSEMBLER AND TOPS1O PAL10.]
  64               
  65               /       3) REMOVES INCOMPATIBILITY WITH LANGUAGE EXTENSIONS FOUND IN OTHER ASSEMBLERS
  66               /          [SUCH AS THE ERROR DIRECTIVE].
  67               
  68               /       4) DEVICE GROUP NAMES ARE MODIFIED FOR CONSISTENCY WITH THE SYSTEM HANDLER.
  69               
  70               /       5) MAJOR SOURCE CODE CLEANUP DEFERRED PENDING TESTING OF THE PROGRAM.  [NOTE:
  71               /          AMONG OTHER PROBLEMS, THE SOURCE CODE APPEARS TO BE A RECOVERY FROM A
  72               /          LISTING FILE AND DOES NOT CONFORM TO PDP-8 SOURCE CODE STANDARDS; OTHER
  73               /          THAN MORE RECENT MINOR EDITS, THERE ARE NO HORIZONTAL TAB CHARACTERS.]
  74               
  75               /       6) RELEASE UPDATED TO VERSION F.
  76               /      16-NOV-2015     BOB ADAMSON     FIRST MAJOR UPDATE.
  77               
  78               /       NOTE: THIS RELEASE DATE IS APPROXIMATE.  IT IS MORE LIKELY AN EARLY DATE IN A
  79               /       PERIOD THAT SPANS SEVERAL MONTHS.
  80               
  81               /       NOTABLE CHANGES:
  82               
  83               /       1) REMOVES REDUNDANT CDF INSTRUCTIONS TO SHORTEN THE CODE.
  84               
  85               /       2) REPLACES BSW INSTRUCTIONS WITH THREE ROTATE INSTRUCTIONS EACH TO MAINTAIN
  86               /          COMPATIBILITY WITH ALL "FAMILY OF 8" MACHINES INSTEAD OF OMNIBUS AND NEWER.
  87               
  88               /       3) DISABLES INTERRUPTS FROM WITHIN THE HANDLER.
  89               
  90               /       4) ADDS CONTROL-C CHECKING TO ABORT TO OS/8 KEYBOARD MONITOR.
  91               
  92               /       5) LIMITS CONTROL-C CHECKING TO READ OPERATIONS ONLY TO PREVENT THE SERVER
  93               /          FROM HANGING.
  94               
  95               /       6) SUPPORT NOW EXTENDED TO FOUR DISKS [REQUIRES UPDATED SERVER SOFTWARE].
  96               
  97               /       7) RELEASE UPDATED TO VERSION E.
  98               
  99               /       17-FEB-2014     KYLE OWEN       INITIAL RELEASE AND UPDATES.
 100               
 101               /       NOTABLE FEATURES:
 102               
 103               /       1) RUNS ON OMNIBUS MACHINES ONLY.
 104               
 105               /       2) HANDLES TWO DISKS.
 106               
 107               /       3) HANDLER RELEASED AT VERSION D.
 108               
 109               /       NUMERIC LOAD DEFINITIONS.
 110               
 111                       NL2000= CLA CLL CML RTR         /LOAD AC WITH 2000.
 112               
 113               /       MISCELLANEOUS DEFINITIONS.
 114               
 115                       BLKNUM= 6260                    /COUNT OF OS/8 RECORDS PER LOGICAL DEVICE.
 116                       DEVCNT= 10                      /EIGHT LOGICAL DEVICES SUPPORTED.
 117                       VERS=   "I&77                   /RELEASE VERSION.
 118               /      REMOTE LINE IOT DEFINITIONS.
 119               
 120                       REC=    40                      /DEVICE 40 FOR REMOTE RECEIVE.
 121                       SEN=    41                      /DEVICE CODE 41 FOR REMOTE SEND.
 122               
 123               /       RECEIVE DEFINITIONS.
 124               
 125                       RKCC=   REC^10+6002             /CLEAR AC, RECEIVE FLAG.
 126                       RKRB=   REC^10+6006             /LOAD DATA -> AC, CLEAR RECEIVE FLAG.
 127                       RKRS=   REC^10+6004             /LOAD RECEIVE DATA .OR. AC -> AC.
 128                       RKSF=   REC^10+6001             /SKIP IF RECEIVE FLAG SET.
 129               
 130               /       TRANSMIT DEFINITIONS.
 131               
 132                       RTCF=   SEN^10+6002             /CLEAR TRANSMIT FLAG.
 133                       RTLS=   SEN^10+6006             /SEND TRANSMIT CHARACTER, CLEAR FLAG.
 134                       RTPC=   SEN^10+6004             /SEND TRANSMIT CHARACTER.
 135                       RTSF=   SEN^10+6001             /SKIP ON TRANSMIT FLAG SET.
 136               
 137               /       TO DIFFERENTIATE BETWEEN LOGICAL DISK REGIONS, THE HANDLER SENDS AN
 138               /       INITIATING CHARACTER:
 139               
 140               /       ASCII TEXT CHARACTER    DISK REGION
 141               
 142               /               A               DISK 0 FIRST HALF.
 143               /               B               DISK 0 SECOND HALF.
 144               /               C               DISK 1 FIRST HALF.
 145               /               D               DISK 1 SECOND HALF.
 146               /               E               DISK 2 FIRST HALF.
 147               /               F               DISK 2 SECOND HALF.
 148               /               G               DISK 3 FIRST HALF.
 149               /               H               DISK 3 SECOND HALF.
 150                      *0                              /HANDLER BLOCK STARTS HERE.
 151               
 152 000000  7770          -DEVCNT                         /DEVICE HANDLER COUNT.
 153               
 154 000001  2304          DEVICE  SDNS;DEVICE  SDA0;4640;SDA0&177;0;0
     000002  1623  
     000003  2304  
     000004  0160  
     000005  4640  
     000006  0056  
     000007  0000  
     000010  0000  
 155 000011  2304          DEVICE  SDNS;DEVICE  SDB0;4640;SDB0&177;0;0
     000012  1623  
     000013  2304  
     000014  0260  
     000015  4640  
     000016  0055  
     000017  0000  
     000020  0000  
 156 000021  2304          DEVICE  SDNS;DEVICE  SDA1;4640;SDA1&177;0;0
     000022  1623  
     000023  2304  
     000024  0161  
     000025  4640  
     000026  0054  
     000027  0000  
     000030  0000  
 157 000031  2304          DEVICE  SDNS;DEVICE  SDB1;4640;SDB1&177;0;0
     000032  1623  
     000033  2304  
     000034  0261  
     000035  4640  
     000036  0053  
     000037  0000  
     000040  0000  
 158 000041  2304         DEVICE  SDNS;DEVICE  SDA2;4640;SDA2&177;0;0
     000042  1623  
     000043  2304  
     000044  0162  
     000045  4640  
     000046  0052  
     000047  0000  
     000050  0000  
 159 000051  2304          DEVICE  SDNS;DEVICE  SDB2;4640;SDB2&177;0;0
     000052  1623  
     000053  2304  
     000054  0262  
     000055  4640  
     000056  0051  
     000057  0000  
     000060  0000  
 160 000061  2304          DEVICE  SDNS;DEVICE  SDA3;4640;SDA3&177;0;0
     000062  1623  
     000063  2304  
     000064  0163  
     000065  4640  
     000066  0050  
     000067  0000  
     000070  0000  
 161 000071  2304          DEVICE  SDNS;DEVICE  SDB3;4640;SDB3&177;0;0
     000072  1623  
     000073  2304  
     000074  0263  
     000075  4640  
     000076  0047  
     000077  0000  
     000100  0000  
 162                      *200                            /CODE DEFINED HERE.
 163               
 164 000200  0000  SENDC,  .-.                             /TRANSMIT A CHARACTER ROUTINE.
 165 000201  6416          RTLS                            /SEND THE CHARACTER IN THE AC.
 166 000202  6411          RTSF                            /SEND FLAG UP?
 167 000203  5202          JMP     .-1                     /NO, WAIT FOR IT.
 168 000204  6412          RTCF                            /DON'T LEAVE THE FLAG SET (FORTRAN)
 169 000205  3217          DCA     SNDTMP                  /CLEAN UP AND SAVE FOR SOME CALLERS.
 170 000206  5600          JMP I   SENDC                   /YES, RETURN TO CALLER WITH AC INTACT.
 171               
 172 000207  0000  SNDNUM, .-.                             /SEND C(AC) AS TWO CHARACTERS ROUTINE.
 173 000210  4200          JMS     SENDC                   /SEND LOW-ORDER 8 BITS.
 174 000211  1217          TAD     SNDTMP                  /GET THEM BACK.
 175 000212  7012          RTR;RTR;RTR                     /MOVE DOWN HIGH-ORDER BITS.
     000213  7012  
     000214  7012  
 176 000215  4200          JMS     SENDC                   /SEND HIGH-ORDER BITS [AND SOME JUNK BITS].
 177 000216  5607          JMP I   SNDNUM                  /RETURN TO CALLER.
 178               
 179 000217  0000  GETNUM, .-.                             /RECEIVE 12-BIT WORD IN TWO CHARACTERS ROUTINE.
 180               
 181                       SNDTMP= .-1                     /ALSO USED AS STORAGE TEMPORARY.
 182               
 183 000220  6401          RKSF                            /RECEIVE FLAG UP?
 184 000221  5220          JMP     .-1                     /NO, WAIT FOR IT.
 185 000222  6406          RKRB                            /YES, READ IN FIRST SIXBIT CHARACTER.
 186 000223  7106          CLL RTL;RTL;RTL                 /MOVE UP TO HIGH-ORDER BITS.
     000224  7006  
     000225  7006  
 187 000226  3207          DCA SNDNUM                      /SAVE IT FOR A MOMENT.
 188 000227  6401          RKSF                            /RECEIVE FLAG UP?
 189 000230  5227          JMP     .-1                     /NO, WAIT FOR IT.
 190 000231  6406          RKRB                            /GET SECOND SIXBIT CHARACTER INTO AC.
 191 000232  1207          TAD SNDNUM                      /MERGE IN THE OTHER HALF.
 192 000233  5617          JMP I   GETNUM                  /RETURN TO CALLER.
 193               
 194 000234  0000  CTRLC,  .-.                             /CONTROL-C CHECK ROUTINE.
 195 000235  7600  S7600,  CLA!400                         /CLEAR AC; ALSO CONSTANT 7600.
 196 000236  6031          KSF                             /KEYBOARD FLAG UP?
 197 000237  5634          JMP I   CTRLC                   /NO, RETURN NOW.
 198 000240  6034          KRS                             /YES, GET THE LATEST CHARACTER.
 199 000241  0346          AND     S177/(177)              /REMOVE PARITY BIT.
 200 000242  1363          TAD     M3/(-3)                 /COMPARE TO CONTROL-C.
 201 000243  7640          SZA CLA                         /SKIP IF IT MATCHES.
 202 000244  5634          JMP I   CTRLC                   /RETURN IF DIFFERENT FROM CONTROL-C.
 203 000245  6203  SCDI,   CIF CDF 00                      /GOING TO FIELD 0 ON ABORT.
 204 000246  5635          JMP I   S7600/(7600)            /EXIT TO OS/8.
 205               /      HANDLER ENTRY POINTS.
 206               
 207               /       NOTE: ALL HANDLER ENTRY POINTS FOLLOW IN REVERSE ORDER.
 208               
 209 000247  0011  SDB3,   VERS                            /FIRST ENTRY POINT CONTAINS VERSION NUMBER.
 210 000250  2360  SDA3,   ISZ     SDCNT                   /SECOND ENTRY POINT.
 211 000251  2360  SDB2,   ISZ     SDCNT                   /THIRD ENTRY POINT.
 212 000252  2360  SDA2,   ISZ     SDCNT                   /FOURTH ENTRY POINT.
 213 000253  2360  SDB1,   ISZ     SDCNT                   /FIFTH ENTRY POINT.
 214 000254  2360  SDA1,   ISZ     SDCNT                   /SIXTH ENTRY POINT.
 215 000255  2360  SDB0,   ISZ     SDCNT                   /SEVENTH ENTRY POINT.
 216 000256  2360  SDA0,   ISZ     SDCNT                   /EIGHTH ENTRY POINT.
 217               
 218               /       AT THIS POINT, "SDCNT" HAS BEEN BUMPED 0 THROUGH 7 TIMES DEPENDING ON WHICH
 219               /       ENTRY POINT WAS USED.  WE USE THIS COUNT TO DETERMINE WHICH ENTRY WAS USED.
 220               
 221               /       THE NEXT WORD EXECUTES AS A HARMLESS "AND" INSTRUCTION TO PROVIDE PARTIAL
 222               /       PROTECTION FROM CALLS MADE FROM LOCATIONS NEAR THE END OF THE CALLING FIELD.
 223               /       WHILE THIS IS NOT FOOLPROOF, CALLS IN OS/8 ARE SELDOM MADE FROM LOCATIONS PAST
 224               /       X7600 FOR ANY FIELD X IN THE RANGE OF 0-7.
 225               
 226               /       ADDITIONALLY, "WKUP" MUST BE JUST AFTER THE ENTRY POINT CHAIN TO HELP DEFINE
 227               /       REFERENCES TO THE PROPER ENTRY POINT.
 228               
 229                       IFNZRO  SDA0+1-. <ERROR .>      /ASSEMBLES ONLY IF THE LOGIC IS BUNGLED.
 230               
 231 000257  0101  WKUP,   "A&177                          /CONSTANT 0101; ALSO HARMLESS "AND" INSTRUCTION.
 232 000260  7300          CLA CLL                         /CLEAN UP.
 233 000261  1360          TAD     SDCNT                   /GET ENTRY POINT COUNTER
 234 000262  7040          CMA                             /INVERT
 235 000263  1300          TAD     SDTAD/(TAD WKUP)        /NOW HAVE "TAD" TO THE PROPER ENTRY POINT.
 236 000264  3273          DCA     SDGET                   /STORE INLINE FOR USE LATER.
 237 000265  7332          NL2000                          /SET AC TO "DCA" - "TAD" OFFSET.
 238 000266  1273          TAD     SDGET                   /NOW HAVE "DCA" TO THE PROPER ENTRY POINT.
 239 000267  3276          DCA     SRESTR                  /STORE INLINE TO RESTORE CALLED ENTRY POINT.
 240 000270  6214          RDF                             /GET THE CALLER'S FIELD.
 241 000271  1245          TAD     SCDI/(CIF CDF)          /TURN INTO "CIF CDF" RETURN FIELD INSTRUCTION.
 242 000272  3354          DCA     SFIELD                  /STORE INLINE FOR RETURN LATER.
 243 000273  7402  SDGET,  HLT                             /THIS IS NOW "TAD" TO THE CHOSEN ENTRY POINT.
 244 000274  3361          DCA     SDENT                   /SAVE IT TO GET THE INLINE ARGUMENTS.
 245 000275  1362          TAD     SDISZ/(ISZ SDCNT)       /GET THE NORMAL CONTENTS
 246 000276  7402  SRESTR, HLT                             /SAVE OVER THE CALLED ENTRY POINT.
 247 000277  4234          JMS     CTRLC                   /CHECK FOR CONTROL-C ABORT NOW.
 248 000300  1257  SDTAD,  TAD     WKUP/("A&177)           /GET THE DRIVE BASE CHARACTER.
 249 000301  1360          TAD     SDCNT                   /ADD OFFSET TO THE DESIRED [HALF] DRIVE.
 250 000302  4200          JMS     SENDC                   /TELL IT TO THE SERVER.
 251 000303  3360          DCA     SDCNT                   /RESET THE ENTRY COUNTER FOR NEXT TIME.
 252 000304  1761         TAD I   SDENT                   /GET THE FUNCTION WORD.
 253 000305  4207          JMS     SNDNUM                  /SEND IT TO THE SERVER.
 254 000306  2361          ISZ     SDENT                   /BUMP PAST FUNCTION WORD.
 255 000307  1761          TAD I   SDENT                   /GET THE CALLER'S BUFFER ADDRESS.
 256 000310  4207          JMS     SNDNUM                  /TELL IT TO THE SERVER [THIS COULD GO AWAY].
 257 000311  1761          TAD I   SDENT                   /GET THE CALLER'S BUFFER ADDRESS AGAIN.
 258 000312  3364          DCA     SLOC                    /STORE FOR TRANSFERS LATER.
 259 000313  2361          ISZ     SDENT                   /BUMP TO RECORD ARGUMENT.
 260 000314  1761          TAD I   SDENT                   /GET THE STARTING RECORD NUMBER.
 261 000315  4207          JMS     SNDNUM                  /SET TO SERVER.
 262 000316  2361          ISZ     SDENT                   /BUMP TO ERROR RETURN.
 263 000317  4217          JMS     GETNUM                  /GET "CDF" TO BUFFER FIELD FROM SERVER.
 264 000320  3321          DCA     .+1                     /STORE INLINE.
 265 000321  7402          HLT                             /CHANGE DATA FIELD TO USER'S BUFFER FIELD.
 266 000322  4217          JMS     GETNUM                  /GET NEGATED WORD COUNT FROM SERVER.
 267 000323  3365          DCA     WORDCT                  /STASH IT.
 268 000324  4217  GETACK, JMS     GETNUM                  /GET STATUS FROM SERVER.
 269 000325  7450          SNA                             /ARE WE DONE? [0000 IS GOOD COMPLETION CODE.]
 270 000326  5352          JMP     EXIT                    /YES, TAKE GOOD EXIT NOW.
 271 000327  7104          CLL RAL                         /MOVE UP TO LINK AND AC[0].
 272 000330  7420          SNL                             /SKIP IF READ OR WRITE.
 273 000331  5356          JMP     DSKERR                  /JUMP IF THERE WAS AN ERROR [CODE 2000].
 274 000332  7640          SZA CLA                         /SKIP IF READING [4000].
 275 000333  5343          JMP     TXLP                    /JUMP IF WE ARE WRITING [4001].
 276               
 277               /       FALLS THROUGH IF READING.  GET THE DATA FROM THE SERVER AND STORE INTO THE
 278               /       USER'S BUFFER.
 279               
 280 000334  4217  RXLP,   JMS     GETNUM                  /GET A WORD FROM THE SERVER.
 281 000335  3764          DCA I   SLOC                    /PUT A WORD INTO THE BUFFER.
 282 000336  2364          ISZ     SLOC                    /BUMP UP THE BUFFER POINTER.
 283 000337  4234          JMS     CTRLC                   /CHECK FOR CONTROL-C [MIGHT BE SKIPPED].
 284 000340  2365          ISZ     WORDCT                  /DONE ENOUGH WORDS?
 285 000341  5334          JMP     RXLP                    /NO, KEEP GOING.
 286 000342  5324          JMP     GETACK                  /GET THE FINAL STATUS BEFORE EXITING.
 287               
 288               /       COMES HERE IF WRITING.  GET THE DATA FROM THE USER'S BUFFER AND SEND IT TO THE
 289               /       SERVER.
 290               
 291 000343  1764  TXLP,   TAD I   SLOC                    /GET A WORD FROM THE USER'S BUFFER.
 292 000344  4207          JMS     SNDNUM                  /SEND THE WORD TO THE SERVER.
 293 000345  2364          ISZ     SLOC                    /BUMP TO NEXT LOCATION.
 294 000346  0177  S177,   177                             /CONSTANT 0177; HERE IN CASE THE PREVIOUS SKIPS.
 295 000347  2365          ISZ     WORDCT                  /DONE ENOUGH WORDS?
 296 000350  5343          JMP     TXLP                    /NO, KEEP GOING.
 297 000351  5324          JMP     GETACK                  /GET THE FINAL STATUS BEFORE EXITING.
 298               /      COMES HERE FOR SUCCESSFUL EXIT TO CALLER.
 299               
 300 000352  2361  EXIT,   ISZ     SDENT                   /BUMP TO NORMAL RETURN.
 301 000353  4234          JMS     CTRLC                   /CHECK FOR CONTROL-C ONE LAST TIME.
 302 000354  7402  SFIELD, HLT                             /THIS WILL BE "CIF CDF" TO CALLER'S FIELD.
 303 000355  5761          JMP I   SDENT                   /TAKE GOOD RETURN TO CALLER.
 304               
 305               /       COMES HERE IF THERE WAS AN ERROR.
 306               /BUGBUG: IF THERE IS AN ERROR, AND WE ARE CALLED FROM FORTRAN, WE CAN
 307               /ESCAPE WITH THE INPUT FLAG SET!
 308               
 309 000356  7130  DSKERR, STL RAR                         /FORCE ERROR CONDITION, MOVE DOWN STATUS BITS.
 310 000357  5354          JMP     SFIELD                  /TAKE ERROR RETURN.
 311               
 312 000360  0000  SDCNT,  0                               /THIS IS USED TO DETERMINE THE ACTIVE CALLER.
 313 000361  0000  SDENT,  .-.                             /POINTER TO INLINE ARGUMENTS.
 314 000362  2360  SDISZ,  ISZ     SDCNT                   /INSTRUCTION CONSTANT NEEDED FOR RESTORATION.
 315 000363  7775  M3,     -3                              /CONSTANT 7775.
 316 000364  0000  SLOC,   .-.                             /POINTER TO USER'S BUFFER.
 317 000365  0000  WORDCT, .-.                             /WORD COUNT FOR DATA TRANSFER.
 318               
 319                       $                               /THAT'S ALL, FOLK!

BLKNUM  6260 unreferenced
CTRLC   0234
DEVCNT  0010
DSKERR  0356
EXIT    0352
GETACK  0324
GETNUM  0217
M3      0363
NL2000  7332
REC     0040
RKCC    6402 unreferenced
RKRB    6406
RKRS    6404 unreferenced
RKSF    6401
RTCF    6412
RTLS    6416
RTPC    6414 unreferenced
RTSF    6411
RXLP    0334
S177    0346
S7600   0235
SCDI    0245
SDA0    0256
SDA1    0254
SDA2    0252
SDA3    0250
SDB0    0255
SDB1    0253
SDB2    0251
SDB3    0247
SDCNT   0360
SDENT   0361
SDGET   0273
SDISZ   0362
SDTAD   0300
SEN     0041
SENDC   0200
SFIELD  0354
SLOC    0364
SNDNUM  0207
SNDTMP  0217
SRESTR  0276
TXLP    0343
VERS    0011
WKUP    0257
WORDCT  0365
