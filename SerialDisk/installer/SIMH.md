It has come to my attention that the 3.x versions of SIMH (Classic)
which come with some versions of Linux or are downloaded from
	http://simh.trailing-edge.com/
are not compatible with the code in pdp8.ini.

If your version of 'pdp8' announces as version 3.x (up to at least 3.11), it
won't do, and you'll need to install a 4.x version from 
	https://github.com/simh/simh

Likewise, if you have no SIMH at all installed yet.

You will also need to get this version of pdp8 on your search path.
Perhaps the easiest way to do this is to copy the binary into ../tools.

Here's a possible command sequence:

	$ cd ../simh
	$ git clone https://github.com/simh/simh
	$ cd simh
	$ make
	$ cd -
	$ cp simh/BIN/pdp8 ../tools
	$ cd ../installer

These scripts were tested with the SIMH that identifies as
	PDP-8 simulator V4.0-0 Current        git commit id: 4c44975e

The file pdp8.ini is used as a startup script to boot OS/8 from SerialDisk.
You may need to edit it to correct the server invocation paths to point to
the images you desire.

Vince
