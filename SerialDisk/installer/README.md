There are two "installers" here.

The 'handler_installer' does a bare bones kludge to allow booting with the
new handler for SYS:.  The handling of DSK: (if different from SYS:) is not
updated, and the table of co-resident handlers is not updated.  Attempts
to access those devices will likely corrupt the new system.

The 'sdsk' script is fancier, but it requires Perl, the tools in ../tools,
'socat', 'expect', and SIMH.  With minor edits to point to the image you
want to upgrade (assuming it's not diagpack2), it will fire up SIMH with
pdp8.ini, and the SerialDisk server, run 'BUILD', and attempt to reconfigure
it to install the system and non-system drivers, and save a new copy of
'BUILD' as 'BUILT'.  See SDSK.md and SIMH.md for more details.

The 'sdsk' script does not patch 'RESORC' or 'PIP', so you are still on
your own there.

You can also run SIMH (by just typing 'pdp8') and interactively play with
your new system.

	Vince
