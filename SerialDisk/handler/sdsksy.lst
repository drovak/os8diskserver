   1               /       SDSKSY OS/8 SYSTEM HANDLER
   2               
   3               /       SERIAL INTERFACE-BASED DISK SYSTEM DEVICE HANDLER [USE WITH PC SERVER].
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
  41               /       19-NOV-2016     CHARLES LASNER  BUG FIX AND OTHER THINGS.
  42               
  43               /       NOTABLE CHANGES.
  44               
  45               /       1) FIXES BUG ASSOCIATED WITH PRE-OMNIBUS SERIAL INTERFACES.
  46               
  47               /       2) REMOVES LAST REMNANTS OF CONTROL-C CHECKING CODE ALREADY DEACTIVATED.
  48               
  49               /       3) THE EMBEDDED MANUAL BOOTSTRAP CODE IS NOW ONE WORD SHORTER; A WORD THAT IS
  50               /          MERELY READ IN BY RUNNING THE BOOTSTRAP WAS INCLUDED WITHIN THE BOOTSTRAP.
  51               
  52               /       4) THE LONG AWAITED MAJOR SOURCE CODE CLEANUP TO INCLUDE [BETTER] COMMENTS AND
  53               /          CONFORMANCE WITH PDP-8 SOURCE CODE CONVENTIONS.
  54               
  55               /       5) RELEASE UPDATED TO VERSION G.
  56               
  57               /       06-AUG-2016     CHARLES LASNER  CODE CLEANUP.
  58               
  59               /       NOTABLE CHANGES.
  60               
  61               /       1) ADDS THIS EDIT HISTORY.
  62               
  63               /       2) REMOVES MINOR INCOMPATIBLE SOURCE CODE QUIRKS.  [NOTE: ALL CHANGES MADE
  64               /          ARE COSMETIC, BUT TECHNICALLY THE ORIGINAL CODE VIOLATED THE PAL LANGUAGE
  65               /          SPECIFICATION.  OS/8 PAL8 IS DEFICIENT AT FLAGGING CERTAIN ERRORS THAT WILL
  66               /          BE PROPERLY DETECTED BY OTHER ASSEMBLY PROGRAMS SUCH AS PAL III, P?S/8 PAL,
  67               /          THE LAP6-DIAL/DIAL-MS ASSEMBLER AND TOPS1O PAL10.]
  68               
  69               /       3) REMOVES INCOMPATIBILITY WITH LANGUAGE EXTENSIONS FOUND IN OTHER ASSEMBLERS
  70               /          [SUCH AS THE ERROR DIRECTIVE].
  71               /      4) REMOVAL OF CONTROL-C DETECTION.  THIS IS A DESIGN FLAW: WHEN CONTROL-C IS
  72               /          DETECTED, THE KEYBOARD INPUT FLAG IS LEFT SET [WHICH IS THE PROPER ACTION
  73               /          PER SE].  THE SYSTEM HANDLER WILL BE IMMEDIATELY ENTERED AT 07600 WITH A
  74               /          CALL TO WRITE OUT A PORTION OF MEMORY TO THE SYSTEM SCRATCH BLOCKS; WHILE
  75               /          THE KEYBOARD FLAG IS NOT CHECKED DURING THE WRITE OPERATION, THE FLAG IS
  76               /          STILL SET.  THE NEXT INTERNAL CALL TO THE SYSTEM DEVICE HANDLER IS AN
  77               /          ATTEMPT TO RELOAD THE KEYBOARD MONITOR [WHICH WOULD DISPOSE OF THE KEYBOARD
  78               /          FLAG BY VARIOUS APPROPRIATE MEANS AFTER CLEARING THE FLAG]; HOWEVER, SINCE
  79               /          THIS LATEST CALL IS A READ OPERATION, THE KEYBOARD FLAG IS DETECTED CAUSING
  80               /          ANOTHER REBOOT TO 07600.  THUS, THE KEYBOARD FLAG IS NEVER CLEARED AS THE
  81               /          CODE THAT IS DESIGNED TO HANDLE IT NEVER GAINS CONTROL; THIS RESULTS IN AN
  82               /          INFINITE LOOP SITUATION.  [NOTE: NO OFFICIAL DEC OS/8 SYSTEM HANDLERS CHECK
  83               /          FOR CONTROL-C; THIS FUNCTION IS AN OPTION FOR NON-SYSTEM HANDLERS ONLY.]
  84               
  85               /       5) DEVICE GROUP NAMES MODIFIED FOR CONSISTENCY WITH THE NON-SYSTEM HANDLERS.
  86               
  87               /       6) MAJOR SOURCE CODE CLEANUP DEFERRED PENDING TESTING OF THE PROGRAM.  [NOTE:
  88               /          AMONG OTHER PROBLEMS, THE SOURCE CODE APPEARS TO BE A RECOVERY FROM A
  89               /          LISTING FILE AND DOES NOT CONFORM TO PDP-8 SOURCE CODE STANDARDS; OTHER
  90               /          THAN MORE RECENT MINOR EDITS, THERE ARE NO HORIZONTAL TAB CHARACTERS.]
  91               
  92               /       7) RELEASE UPDATED TO VERSION F.
  93               
  94               /       16-NOV-2015     BOB ADAMSON     FIRST MAJOR UPDATE.
  95               
  96               /       NOTE: THIS RELEASE DATE IS APPROXIMATE.  IT IS MORE LIKELY AN EARLY DATE IN A
  97               /       PERIOD THAT SPANS SEVERAL MONTHS.
  98               
  99               /       NOTABLE CHANGES:
 100               
 101               /       1) REMOVES REDUNDANT CDF INSTRUCTIONS TO SHORTEN THE CODE.
 102               
 103               /       2) REPLACES BSW INSTRUCTIONS WITH THREE ROTATE INSTRUCTIONS EACH TO MAINTAIN
 104               /          COMPATIBILITY WITH ALL "FAMILY OF 8" MACHINES INSTEAD OF OMNIBUS AND NEWER.
 105               
 106               /       3) DISABLES INTERRUPTS FROM WITHIN THE HANDLER.
 107               
 108               /       4) ADDS CONTROL-C CHECKING TO ABORT TO OS/8 KEYBOARD MONITOR.
 109               
 110               /       5) LIMITS CONTROL-C CHECKING TO READ OPERATIONS ONLY TO PREVENT THE SERVER
 111               /          FROM HANGING.
 112               
 113               /       6) RELEASE UPDATED TO VERSION E.
 114               
 115               /       17-FEB-2014     KYLE OWEN       INITIAL RELEASE AND UPDATES.
 116               
 117               /       NOTABLE FEATURES:
 118               
 119               /       1) RUNS ON OMNIBUS MACHINES ONLY.
 120               
 121               /       2) HANDLER RELEASED AT VERSION D.
 122               
 123               /       3) INCLUDES CO-RESIDENT HANDLER FOR SDB0:.
 124               /      4) INCLUDES CO-RESIDENT HANDLER DUMMY FOR SDA0: [SAME AS SYS].
 125               
 126               /       NOTE: AS ALSO APPLIES TO THE OS/8 RK8E SYSTEM HANDLER, THE CO-RESIDENT ENTRY
 127               /       POINTS ONLY FUNCTION AS DESCRIBED IF THE SYSTEM IS BOOTED TO DRIVE UNIT 0.
 128               /       PRESENTLY, THIS RESTRICTION IS IRRELEVANT TO THE SERIAL DISK SYSTEM HANDLER AS
 129               /       NO EXTENDED DISK BOOTUP IS CURRENTLY SUPPORTED; HOWEVER, THE RESTRICTIONS
 130               /       SHOULD BE DOCUMENTED SHOULD FUTURE RELEASES SUPPORT ADDITIONAL BOOTUP DRIVES.
 131               
 132               /       NUMERIC LOAD DEFINITIONS.
 133               
 134                       NL7777= CLA CMA                 /LOAD AC WITH 7777.
 135               
 136               /       MISCELLANEOUS DEFINITIONS.
 137               
 138                       BLKNUM= 6260                    /COUNT OF OS/8 RECORDS PER LOGICAL DEVICE.
 139                       DEVCNT= 3                       /THREE LOGICAL DEVICES SUPPORTED.
 140                       SBOOT=  7600                    /STANDARD SYSTEM RESTART ADDRESS.
 141                       VERS=   "I&77                   /RELEASE VERSION.
 142               
 143               /       REMOTE LINE IOT DEFINITIONS.
 144               
 145                       REC=    40                      /DEVICE 40 FOR REMOTE RECEIVE.
 146                       SEN=    41                      /DEVICE CODE 41 FOR REMOTE SEND.
 147               
 148               /       RECEIVE DEFINITIONS.
 149               
 150                       RKCC=   REC^10+6002             /CLEAR AC, RECEIVE FLAG.
 151                       RKRB=   REC^10+6006             /LOAD RECEIVE DATA -> AC, CLEAR RECEIVE FLAG.
 152                       RKRS=   REC^10+6004             /LOAD RECEIVE DATA .OR. AC -> AC.
 153                       RKSF=   REC^10+6001             /SKIP IF RECEIVE FLAG SET.
 154               
 155               /       TRANSMIT DEFINITIONS.
 156               
 157                       RTCF=   SEN^10+6002             /CLEAR TRANSMIT FLAG.
 158                       RTLS=   SEN^10+6006             /SEND TRANSMIT CHARACTER, CLEAR FLAG.
 159                       RTPC=   SEN^10+6004             /SEND TRANSMIT CHARACTER.
 160                       RTSF=   SEN^10+6001             /SKIP ON TRANSMIT FLAG SET.
 161               
 162               /       TO DIFFERENTIATE BETWEEN LOGICAL DISK REGIONS, THE HANDLER SENDS AN
 163               /       INITIATING CHARACTER:
 164               
 165               /       ASCII TEXT CHARACTER    DISK REGION
 166               
 167               /               A               DISK 0 FIRST HALF.
 168               /               B               DISK 0 SECOND HALF.
 169               
 170               /               <ATSIGN>        SEND BOOT CODE BY SPECIAL PROTOCOL.
 171                      *0                              /HANDLER BLOCK STARTS HERE.
 172               
 173 000000  7775          -DEVCNT                         /DEVICE HANDLER COUNT.
 174               
 175 000001  2304          DEVICE  SDSY;DEVICE  SYS; 4640;SYSENT&177+2000;0;BLKNUM
     000002  2331  
     000003  2331  
     000004  2300  
     000005  4640  
     000006  2007  
     000007  0000  
     000010  6260  
 176 000011  2304          DEVICE  SDSY;DEVICE  SDA0;4640;SYSENT&177+1000;0;BLKNUM
     000012  2331  
     000013  2304  
     000014  0160  
     000015  4640  
     000016  1007  
     000017  0000  
     000020  6260  
 177 000021  2304          DEVICE  SDSY;DEVICE  SDB0;4640;ENTRY2&177+1000;0;BLKNUM
     000022  2331  
     000023  2304  
     000024  0260  
     000025  4640  
     000026  1056  
     000027  0000  
     000030  6260  
 178               
 179 000031  7732          BOOT-ENDB                       /BOOT CODE LENGTH [NEGATED].
 180               
 181                       RELOC   0                       /WHERE THIS LOADS.
 182               
 183               /       WHEN CONTROL IS TRANSFERRED HERE, THE SYSTEM DEVICE HANDLER IS IN 00200-00377
 184               /       AND THE CODE TO BE LOADED INTO 17647-17777 IS IN 00047-0177.  WE CAN BE
 185               /       SLOPPY AND COPY A FEW EXTRA WORDS PAST WHAT IS NEEDED [WASTES A FEW MS.].
 186               
 187 000000* 7200  BOOT,   CLA                             /CLEAN UP.
 188 000001* 1413          TAD I   BTXR13                  /GET A WORD FROM 0200 AND ONWARD.
 189 000002* 3414          DCA I   BTXR14                  /STORE INTO 07600 AND ONWARD.
 190 000003* 1415          TAD I   BTXR15                  /GET A WORD FROM 0047 AND ONWARD.
 191 000004* 6211          CDF     10                      /STORING INTO FIELD 1.
 192 000005* 3416          DCA I   BTXR16                  /STORE INTO 17647 AND ONWARD.
 193 000006* 6201          CDF     00                      /BACK TO FIELD 0.
 194 000007* 1014          TAD     BTXR14                  /GET HANDLER CODE POINTER.
 195 000010* 7640          SZA CLA                         /SKIP IF WE WENT TOO FAR.
 196 000011* 5001          JMP     BOOT+1                  /GO BACK AND DO MORE.
 197 000012* 5445          JMP I   B7605/(SBOOT+5)         /DONE, GO START UP OS/8.
 198               /      AUTO-INDEX REGISTERS; ALL MUST BE IN THE RANGE OF 0013-0017.
 199               
 200                       IFNZRO  13-.    <ERROR  .>      /POINTERS ASSEMBLED WRONG IF THIS HAPPENS.
 201               
 202 000013* 0177  BTXR13, 0200-1                          /CURRENT POINTER TO SYSTEM HANDLER CODE.
 203 000014* 7577  BTXR14, SBOOT-1                         /WHERE SYSTEM HANDLER CODE MUST GO.
 204 000015* 0046  BTXR15, 47-1                            /CURRENT POINTER TO FIELD 1 CODE [IN FIELD 0].
 205 000016* 7646  BTXR16, 7647-1                          /WHERE THE FIELD 1 CODE MUST GO.
 206 000017* 0017  STXR17, .                               /BOOTUP STORE POINTER; MUST POINT TO ITSELF.
 207               
 208                       ZBLOCK  20-.                    /EMPTY SPACE [IF ANY].
 209               
 210               /       WHAT FOLLOWS IS ACTUALLY THE BOOTUP CODE [BUT ALSO EMBEDDED HERE].
 211               
 212 000020* 7240  BUTUP,  NL7777                          /SET AC TO 0000 LESS AUTO-INDEX BACKUP FACTOR.
 213 000021* 3017          DCA     STXR17                  /STASH THE POINTER.
 214 000022* 1044          TAD     BOOTMSG/(100)           /SETUP SERVER COMMAND BOOT VALUE.
 215 000023* 6416          RTLS                            /SEND IT.
 216 000024* 6411          RTSF                            /DONE YET?
 217 000025* 5024          JMP     .-1                     /NO, WAIT FOR IT.
 218 000026* 6402  BTLP,   RKCC                            /CLEAR THE FLAG AND THE AC.
 219 000027* 6401          RKSF                            /FLAG UP?
 220 000030* 5027          JMP     .-1                     /NO, WAIT FOR IT.
 221 000031* 6406          RKRB                            /YES, READ IN THE FIRST CHARACTER.
 222 000032* 7106          CLL RTL;RTL                     /MOVE UP.
     000033* 7006  
 223 000034* 7510          SPA                             /SKIP IF NOT END OF DATA.
 224 000035* 5000          JMP     BOOT                    /ALL DATA IN, NOW GO START IT UP.
 225 000036* 7006          RTL                             /NO HAVE FIRST HALF IN HIGH-ORDER.
 226 000037* 6401          RKSF                            /FLAG UP?
 227 000040* 5037          JMP     .-1                     /NO, WAIT FOR IT.
 228 000041* 6404          RKRS                            /.OR. IN THE LOW-ORDER HALF.
 229 000042* 3417          DCA I   STXR17                  /STORE THE LATEST WORD.
 230 000043* 5026          JMP     BTLP                    /KEEP GOING.
 231               
 232 000044* 0100  BOOTMS, "A&177-1                        /BOOTUP CHARACTER.
 233               
 234               /       THE STANDALONE BOOTSTRAP IS ALL WORDS FROM 0020 THROUGH HERE.
 235               
 236                       ENDBUT= .                       /END OF MANUAL BOOTSTRAP.
 237               
 238 000045* 7605  B7605,  SBOOT+5                         /WHERE OS/8 STARTS WITHOUT WRITING.
 239               
 240                       ENDB=   .                       /END OF BOOT CODE.
 241               
 242                       RELOC                           /TURN OFF RELOCATION FOR NOW.
 243                      *200                            /THIS IS WHERE THE SYSTEM HANDLER LOADS.
 244               
 245                       RELOC   SBOOT                   /THIS IS WHERE IT EXECUTES.
 246               
 247 007600* 0000          ZBLOCK  SBOOT+7-.               /BUILD WILL FILL THIS IN.
     007601* 0000  
     007602* 0000  
     007603* 0000  
     007604* 0000  
     007605* 0000  
     007606* 0000  
 248               
 249               /       THIS IS THE ENTRY FOR THE SYSTEM DEVICE [AND THE CO-RESIDENT SDA0: HANDLER].
 250               
 251 007607* 0011  SYSENT, VERS                            /ENTRY POINT; BUILD WANTS THE VERSION HERE.
 252 007610* 7300          CLA CLL                         /CLEAN UP.
 253               / THE CURRENT VERSION DOESN'T LEAVE THE R FLAG SET, BUT SOME OLDER VESRIONS
 254               / DO.  CLEAR THE FLAG, JUST IN CASE.
 255 007611* 6402          RKCC                            /CLEAR FLAG FROM OLDER DRIVER, IF ANY
 256 007612* 1252  SETUP1, TAD     WKUP/("A&177-1)         /GET [OR ADD] INITIAL DRIVE CHARACTER.
 257 007613* 4264          JMS     SENDC                   /TELL IT TO THE SERVER.
 258 007614* 6214          RDF                             /GET CALLER'S FIELD.
 259 007615* 1334          TAD     SCDI/(CIF CDF 00)       /TURN INTO "CIF CDF" TO CALLER'S FIELD.
 260 007616* 3330          DCA     SFIELD                  /STORE IN-LINE FOR RETURN LATER.
 261 007617* 1607          TAD I   SYSENT                  /GET THE FUNCTION WORD.
 262 007620* 4273          JMS     SNDNUM                  /SEND IT TO THE SERVER.
 263 007621* 2207          ISZ     SYSENT                  /BUMP PAST FUNCTION WORD.
 264 007622* 1607          TAD I   SYSENT                  /GET THE CALLER'S BUFFER ADDRESS.
 265 007623* 4273          JMS     SNDNUM                  /TELL IT TO THE SERVER [THIS COULD GO AWAY].
 266 007624* 1607          TAD I   SYSENT                  /GET THE CALLER'S BUFFER ADDRESS AGAIN.
 267 007625* 3256          DCA     SLOC                    /STORE FOR TRANSFERS LATER.
 268 007626* 2207          ISZ     SYSENT                  /BUMP TO RECORD ARGUMENT.
 269 007627* 1607          TAD I   SYSENT                  /GET THE STARTING RECORD NUMBER.
 270 007630* 4273          JMS     SNDNUM                  /SET TO SERVER.
 271 007631* 2207          ISZ     SYSENT                  /BUMP TO ERROR RETURN.
 272 007632* 4303          JMS     GETNUM                  /GET "CDF" TO BUFFER FIELD FROM SERVER.
 273 007633* 3234          DCA     .+1                     /STORE INLINE.
 274 007634* 7402          HLT                             /CHANGE DATA FIELD TO USER'S BUFFER FIELD.
 275 007635* 4303          JMS     GETNUM                  /GET NEGATED WORD COUNT FROM SERVER.
 276 007636* 3335          DCA     WORDCT                  /STASH IT.
 277 007637* 4303  GETACK, JMS     GETNUM                  /GET STATUS FROM SERVER.
 278 007640* 7450          SNA                             /ARE WE DONE? [0000 IS GOOD COMPLETION CODE.]
 279 007641* 5327          JMP     EXIT                    /YES, TAKE GOOD EXIT NOW.
 280 007642* 7104          CLL RAL                         /MOVE UP TO LINK AND AC[0].
 281 007643* 7420          SNL                             /SKIP IF READ OR WRITE.
 282 007644* 5332          JMP     SYSERR                  /JUMP IF THERE WAS AN ERROR [CODE 2000].
 283 007645* 7640          SZA CLA                         /SKIP IF READING [4000].
 284 007646* 5320          JMP     TXLP                    /JUMP IF WE ARE WRITING [4001].
 285               /      FALLS THROUGH IF READING.  GET THE DATA FROM THE SERVER AND STORE INTO THE
 286               /       USER'S BUFFER.
 287               
 288 007647* 4303  RXLP,   JMS     GETNUM                  /GET A WORD FROM THE SERVER.
 289 007650* 3656          DCA I   SLOC                    /PUT A WORD INTO THE BUFFER.
 290 007651* 2256          ISZ     SLOC                    /BUMP UP THE BUFFER POINTER.
 291 007652* 0101  WKUP,   "A&177                          /CONSTANT 0101; ALSO HARMLESS "AND" INSTRUCTION.
 292 007653* 2335          ISZ     WORDCT                  /DONE ENOUGH WORDS?
 293 007654* 5247          JMP     RXLP                    /NO, KEEP GOING.
 294 007655* 5237          JMP     GETACK                  /GET THE FINAL STATUS BEFORE EXITING.
 295               
 296 007656* 0011  ENTRY2, VERS                            /ENTRY POINT FOR "B" SIDE.
 297               
 298                       SLOC=   .-1                     /ALSO USED AS STORAGE POINTER.
 299               
 300 007657* 7200          CLA                             /CLEAN UP.
 301 007660* 1256          TAD     ENTRY2                  /GET OUR CALLER.
 302 007661* 3207          DCA     SYSENT                  /MAKE IT THEIRS.
 303 007662* 7301          CLA CLL IAC                     /SET AC TO 1 FOR "B" SIDE OFFSET.
 304 007663* 5212          JMP     SETUP1                  /CONTINUE THERE.
 305               
 306 007664* 0000  SENDC,  .-.                             /TRANSMIT A CHARACTER ROUTINE.
 307 007665* 6416          RTLS                            /SEND THE CHARACTER IN THE AC.
 308 007666* 6411          RTSF                            /SEND FLAG UP?
 309 007667* 5266          JMP     .-1                     /NO, WAIT FOR IT.
 310 007670* 6412          RTCF                            /DON'T LEAVE THE FLAG SET (FORTRAN)
 311 007671* 3303          DCA     SNDTMP                  /CLEAN UP AND SAVE FOR SOME CALLERS.
 312 007672* 5664          JMP I   SENDC                   /YES, RETURN TO CALLER WITH AC INTACT.
 313               
 314 007673* 0000  SNDNUM, .-.                             /SEND C(AC) AS TWO CHARACTERS ROUTINE.
 315 007674* 4264          JMS     SENDC                   /SEND LOW-ORDER 8 BITS.
 316 007675* 1303          TAD     SNDTMP                  /GET THEM BACK.
 317 007676* 7012          RTR;RTR;RTR                     /MOVE DOWN HIGH-ORDER BITS.
     007677* 7012  
     007700* 7012  
 318 007701* 4264          JMS     SENDC                   /SEND HIGH-ORDER BITS [AND SOME JUNK BITS].
 319 007702* 5673          JMP I   SNDNUM                  /RETURN TO CALLER.
 320               
 321 007703* 0000  GETNUM, .-.                             /RECEIVE 12-BIT WORD IN TWO CHARACTERS ROUTINE.
 322               
 323                       SNDTMP= .-1                     /ALSO USED AS STORAGE TEMPORARY.
 324               
 325 007704* 6401          RKSF                            /RECEIVE FLAG UP?
 326 007705* 5304          JMP     .-1                     /NO, WAIT FOR IT.
 327 007706* 6406          RKRB                            /YES, READ IN FIRST SIXBIT CHARACTER.
 328 007707* 7106          CLL RTL;RTL;RTL                 /MOVE UP TO HIGH-ORDER BITS.
     007710* 7006  
     007711* 7006  
 329 007712* 3273          DCA SNDNUM                      /SAVE FIRST HALF FOR A MOMENT.
 330 007713* 6401          RKSF                            /RECEIVE FLAG UP?
 331 007714* 5313          JMP     .-1                     /NO, WAIT FOR IT.
 332 007715* 6406          RKRB                            /GET SECOND SIXBIT CHARACTER INTO AC.
 333 007716* 1273          TAD SNDNUM                      /MERGE IN FIRST HALF.
 334 007717* 5703          JMP I   GETNUM                  /RETURN TO CALLER.
 335               /      COMES HERE IF WRITING.  GET THE DATA FROM THE USER'S BUFFER AND SEND IT TO THE
 336               /       SERVER.
 337               
 338 007720* 1656  TXLP,   TAD I   SLOC                    /GET A WORD FROM THE USER'S BUFFER.
 339 007721* 2256          ISZ     SLOC                    /BUMP TO NEXT LOCATION.
 340 007722* 7000          NOP                             /HERE IN CASE IT SKIPS.
 341 007723* 4273          JMS     SNDNUM                  /SEND THE WORD TO THE SERVER.
 342 007724* 2335          ISZ     WORDCT                  /DONE ENOUGH WORDS?
 343 007725* 5320          JMP     TXLP                    /NO, KEEP GOING.
 344 007726* 5237          JMP     GETACK                  /GET THE FINAL STATUS BEFORE EXITING.
 345               
 346               /       COMES HERE FOR SUCCESSFUL EXIT TO CALLER.
 347               
 348 007727* 2207  EXIT,   ISZ     SYSENT                  /BUMP TO NORMAL RETURN.
 349 007730* 7402  SFIELD, HLT                             /THIS WILL BE "CIF CDF" TO CALLER'S FIELD.
 350 007731* 5607          JMP I   SYSENT                  /TAKE GOOD RETURN TO CALLER.
 351               
 352               /       COMES HERE IF THERE WAS AN ERROR.
 353               
 354 007732* 7130  SYSERR, STL RAR                         /FORCE ERROR CONDITION, MOVE DOWN STATUS BITS.
 355 007733* 5330          JMP     SFIELD                  /TAKE ERROR RETURN.
 356               
 357 007734* 6203  SCDI,   CIF CDF 00                      /CONSTANT 6203.
 358 007735* 0000  WORDCT, .-.                             /WORD COUNT FOR DATA TRANSFER.
 359               
 360 007736* 0000          ZBLOCK  7744-.                  /EMPTY SPACE.
     007737* 0000  
     007740* 0000  
     007741* 0000  
     007742* 0000  
     007743* 0000  
 361               
 362                       RELOC                           /TURN OFF RELOCATION.
 363               
 364                       $                               /THAT'S ALL, FOLK!

B7605   0045
BLKNUM  6260
BOOT    0000
BOOTMS  0044
BTLP    0026
BTXR13  0013
BTXR14  0014
BTXR15  0015
BTXR16  0016
BUTUP   0020 unreferenced
DEVCNT  0003
ENDB    0046
ENDBUT  0045 unreferenced
ENTRY2  7656
EXIT    7727
GETACK  7637
GETNUM  7703
NL7777  7240
REC     0040
RKCC    6402
RKRB    6406
RKRS    6404
RKSF    6401
RTCF    6412
RTLS    6416
RTPC    6414 unreferenced
RTSF    6411
RXLP    7647
SBOOT   7600
SCDI    7734
SEN     0041
SENDC   7664
SETUP1  7612
SFIELD  7730
SLOC    7656
SNDNUM  7673
SNDTMP  7703
STXR17  0017
SYSENT  7607
SYSERR  7732
TXLP    7720
VERS    0011
WKUP    7652
WORDCT  7735
