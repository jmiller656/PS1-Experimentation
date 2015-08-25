# -----------------------------------------
#       PlayStation 1 Psy-Q Makefile
# Sony Computer Entertainment Europe (SCEE)
# -----------------------------------------

all:
	del mem.map
	del main.sym
	del main.exe
	del main.cpe
	cls

	ccpsx -O3 -Xo$80010000 main.c -omain.cpe,main.sym,mem.map

	cpe2x /CE main.cpe