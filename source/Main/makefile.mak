# ----------------------------
# PlayStation 1 Psy-Q MAKEFILE
# ----------------------------
all:
	#For Main
	ccpsx -O3 -Xo$80010000 main.c -omain.cpe main.sym mem.map
	cpe2x /ce main.cpe