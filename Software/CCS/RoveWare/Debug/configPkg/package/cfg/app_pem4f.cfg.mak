# invoke SourceDir generated makefile for app.pem4f
app.pem4f: .libraries,app.pem4f
.libraries,app.pem4f: package/cfg/app_pem4f.xdl
	$(MAKE) -f C:\Users\Connor\Documents\Rover\Motherboard\Software\CCS\RoveWare/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Connor\Documents\Rover\Motherboard\Software\CCS\RoveWare/src/makefile.libs clean

