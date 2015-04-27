/*
===========================================================
                Sony PlayStation 1 Source Code
===========================================================
                         FONT EXAMPLE
Displays text on the screen using the built in GPU routines
-----------------------------------------------------------

    Developer / Programmer..............: SCEI & PSXDEV.net
    Software Ddevelopment Kit...........: PSY-Q
    Last Release........................: 30/APRIL/2014

    Original code by SCEI | Edited by PSXDEV.net

	NOTE: This example uses double buffering! I may go
	ahead and remove the double buffering to make the code
	much more simplistic in the future.

    All PlayStation 1 games are written in C. If you
	don't know how to program in C, you should follow a
	book or online tutorial and then take a look at the
	PSX C code. Above all, the PSX is an excellent system
	to program on and work with and will not dissapoint you.

    If you need help, read LIBOVR46.PDF in PSYQ/DOCS
    also join our IRC channel on EFNET #psxdev

  Copyright (C) 1994,1995 by Sony Computer Entertainment Inc.
                     All Rights Reserved.

   Sony Computer Entertainment Inc. Development Department

                    http://psxdev.net/

-----------------------------------------------------------*/

#include <stdlib.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
#include <libpad.h>

#define OT_LENGTH 1 // the ordertable length
#define PACKETMAX 18 // the maximum number of objects on the screen
#define SCREEN_WIDTH  320 // screen width
#define	SCREEN_HEIGHT 240 // screen height

GsOT myOT[2];
GsOT_TAG myOT_TAG[2][1<<OT_LENGTH];
PACKET GPUPacketArea[2][PACKETMAX];

u_long __ramsize   = 0x00200000; // force 2 megabytes of RAM
u_long __stacksize = 0x00004000; // force 16 kilobytes of stack

// --------
// INTEGERS
// --------
short CurrentBuffer = 0;
char *in = "No Input"; //Default Input
int status; //Used to store current pad status
// ----------
// PROTOTYPES
// ----------
void graphics();
void display();
void draw(char in[]);
const DEBUG = 1; // debugging


int main()
{
	graphics(); // setup the graphics (seen below)
	FntLoad(960, 256); // load the font from the BIOS into VRAM/SGRAM
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
    PadInit(1); //Initialize controller
	// should debug = 1 (as seen above), print to the debug window (should you be using one)
    if (DEBUG) printf("\n\nHello World\n");
	if (DEBUG) printf("\nhttp://psxdev.net/");

	//char in[10] = "NULL"; //make string for input


	while (1) // draw and display forever
	{
	    draw(in);
        status = PadRead(0);//Read 32 bit info from pad(s)
        if(status & PADLup){
            in = "Up";
            draw(in);
            }
        else if(status & PADLdown){
            in = "Down";
            draw(in);
            }
        else if(status & PADLleft){
            in = "Left";
            draw(in);
            }
        else if(status & PADLright){
            in = "Right";
            draw(in);
            }
        else{
            in = "No Input";
            draw(in);
            }

	}

	return 0;
}

void draw(char in[]){
    FntPrint("Current controller input:\n\n %s",in);
		display();//Display fntrprint changes
}

void graphics()
{
	if (*(char *)0xbfc7ff52=='E') SetVideoMode(1); else SetVideoMode(0); // within the BIOS, if the address 0xBFC7FF52 equals 'E', set it as PAL (1). Otherwise, set it as NTSC (0)

	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); // set the graphics mode resolutions. You may also try using 'GsNONINTER' (read LIBOVR46.PDF in PSYQ/DOCS for detailed information)
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT); // set the top left coordinates of the two buffers in video memory

	// init the ordering tables
	myOT[0].length = OT_LENGTH;
	myOT[1].length = OT_LENGTH;
	myOT[0].org = myOT_TAG[0];
	myOT[1].org = myOT_TAG[1];

	GsClearOt(0,0,&myOT[0]);
	GsClearOt(0,0,&myOT[1]);
}


void display()
{
	FntFlush(-1); // refresh the font
	CurrentBuffer=GsGetActiveBuff(); // get the current buffer
	GsSetWorkBase((PACKET*)GPUPacketArea[CurrentBuffer]); // setup the packet workbase
	GsClearOt(0,0,&myOT[CurrentBuffer]); // clear the ordering table
	DrawSync(0); // wait for all drawing to finish
	VSync(0); // wait for v_blank interrupt
	GsSwapDispBuff(); // flip the double buffers
	GsSortClear(50,50,50,&myOT[CurrentBuffer]); // clear the ordering table with a background color. RGB value 50,50,50 which is a grey background (0,0,0 would be black for example)
	GsDrawOt(&myOT[CurrentBuffer]); // Draw the ordering table for the CurrentBuffer
}
