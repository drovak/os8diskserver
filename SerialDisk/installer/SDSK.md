As mentioned in the installer/README.md, the "sdsk" script is the fancy
"does it all" installer.  It requires SIMH (pdp8, anyway), perl, a couple
of other packages as mentioned, and the tools from the "../tools" directory.

As mentioned in the tools/README.md, the tools directory is a subset of
the "8tools", a suite of software forensics tools which are targeted at
the PDP-8.  (Instructions in tools/README.md on where to get those.)
The 8tools do come with a "doc" sub-directory with documentation for
each tool.

The tools, with the exception of the cross-assembler (C source provided),
are perl scripts, and contain source code but not really any documentation.
If you don't have perl, try your system's equivalent of:
	$ sudo apt install perl

socat is used to tie the invokation of ../server/server to the running
SIMH instance.  If you don't have socat, try your system's equivalent of:
	$ sudo apt install socat
The text file server.log will be over-written with the diagnostic output
from ../server/server.

This version no longer uses xterm, so X windows is no longer required.

installer/runbuild is an expect script, which is why you needed to install
expect.  If you don't have expect, try your system's equivalent of
	$ sudo apt install expect

Here are the other tools, as used by "./sdsk":

pal is a cross-assembler, which will take PAL source and generate a listing
(.lst) and a binary (.bin).  It's main use is to convert Serialdisk source 
in ../handler to .bin files.  (Do the "make" in ../handler *before* doing
"make" here!)

bin2bn converts .bin files to proper OS/8 format by padding them with
trailer bytes to a multiple of the OS/8 block size.  The result can be 
included with the files used to build OS/8 images.

bincmp is a comparison tool which will diff .bin (or .bn) files.  (Sdsk
doesn't actually use it, but it's so handy that I included it.)

os8xplode takes a ".dsk" file and explodes it into directories containing
the individual files for each file-system found.  It also writes an ".xml"
description of the OS/8 volume so that "mkdsk" can re-construct it.

mkdsk reads the .xml mentioned above, and the various individual files, and
reconstructs the specified OS/8 volume.

os8implode is a tools that scans the (probably modified) directories
associated with a disk volume, and writes a new .xml description suitable
for mkdsk.  The magic "dot" files will be used to reconstruct the system
areas, and all files that match the OS/8 "6.2" naming convention will be
included in the new image.


./sdsk, then, rounds up the SerialDisk drivers from ../handler for inclusion
in the new volume, then runs os8implode and mkdsk to construct a volume that
If you don't have perl, try your system's equivalent of:
	$ sudo apt install perl

Here are the tools, as used by "./sdsk":

pal is a cross-assembler, which will take PAL source and generate a listing
(.lst) and a binary (.bin).  It's main use is to convert Serialdisk source 
in ../handler to .bin files.  (Do the "make" in ../handler *before* doing
"make" here!)

bin2bn converts .bin files to proper OS/8 format by padding them with
trailer bytes to a multiple of the OS/8 block size.  The result can be 
included with the files used to build OS/8 images.

bincmp is a comparison tool which will diff .bin (or .bn) files.  (Sdsk
doesn't actually use it, but it's so handy that I included it.)

os8xplode takes a ".dsk" file and explodes it into directories containing
the individual files for each file-system found.  It also writes an ".xml"
description of the OS/8 volume so that "mkdsk" can re-construct it.

mkdsk reads the .xml mentioned above, and the various individual files, and
reconstructs the specified OS/8 volume.

os8implode is a tools that scans the (probably modified) directories
associated with a disk volume, and writes a new .xml description suitable
for mkdsk.  The magic "dot" files will be used to reconstruct the system
areas, and all files that match the OS/8 "6.2" naming convention will be
included in the new image.


./sdsk, then, rounds up the SerialDisk drivers from ../handler for inclusion
in the new volume, then runs os8implode and mkdsk to construct a volume that
If you don't have perl, try your system's equivalent of:
	$ sudo apt install perl

Here are the tools, as used by "./sdsk":

pal is a cross-assembler, which will take PAL source and generate a listing
(.lst) and a binary (.bin).  It's main use is to convert Serialdisk source 
in ../handler to .bin files.  (Do the "make" in ../handler *before* doing
"make" here!)

bin2bn converts .bin files to proper OS/8 format by padding them with
trailer bytes to a multiple of the OS/8 block size.  The result can be 
included with the files used to build OS/8 images.

bincmp is a comparison tool which will diff .bin (or .bn) files.  (Sdsk
doesn't actually use it, but it's so handy that I included it.)

os8xplode takes a ".dsk" file and explodes it into directories containing
the individual files for each file-system found.  It also writes an ".xml"
description of the OS/8 volume so that "mkdsk" can re-construct it.

mkdsk reads the .xml mentioned above, and the various individual files, and
reconstructs the specified OS/8 volume.

os8implode is a tools that scans the (probably modified) directories
associated with a disk volume, and writes a new .xml description suitable
for mkdsk.  The magic "dot" files will be used to reconstruct the system
areas, and all files that match the OS/8 "6.2" naming convention will be
included in the new image.


./sdsk, then, rounds up the SerialDisk drivers from ../handler for inclusion
in the new volume, then runs os8implode and mkdsk to construct a volume that
has the .bn files for the drivers.  Then it runs SIMH (see SIMH.md,
"installer/runbuild", and "installer/pdp8.ini").  SIMH will then be fed
the commands to convert your OS/8 volume into a SerialDisk volume.

For this to work correctly, you need to install the packages mentioned above.
Then a simple ./sdsk should work.  (You can edit sdsk to select another image
from ../disks.)  Your new volume will have a name like "diagpack2.new", and
can be served with the server from ../server.

Hope that helps!

    Vince
