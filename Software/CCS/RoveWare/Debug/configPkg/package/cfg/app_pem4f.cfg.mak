# invoke SourceDir generated makefile for app.pem4f
app.pem4f: .libraries,app.pem4f
.libraries,app.pem4f: package/cfg/app_pem4f.xdl
	$(MAKE) -f C:\Users\Connor\Documents\Rover\Motherboard\Software\CCS\typical_TivaTM4C1294NCPDT/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Connor\Documents\Rover\Motherboard\Software\CCS\typical_TivaTM4C1294NCPDT/src/makefile.libs clean

