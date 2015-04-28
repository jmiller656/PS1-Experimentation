/*
===========================================================
                Sony PlayStation 1 Source Code
===========================================================
                         GAMEPAD EXAMPLE
              Displays on screen input from gamepad
-----------------------------------------------------------
    Developer / Programmer..............: Josh Miller
    Software Ddevelopment Kit...........: PSY-Q
    Last Release........................: 30/APRIL/2014
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
int status; //Used to store current pad status
int i;
int j;
int index;
int pos[2];
int blinker;
// -------
// STRINGS
// -------
char *in = "No Input"; //Default Input
char board[3][3];
char b_string[35];
// ----------
// PROTOTYPES
// ----------
void graphics();
void display();
void setBoard();
void controllerListener(int status);
void draw();
const DEBUG = 1; // debugging


int main()
{
	graphics(); // setup the graphics (seen below)
	FntLoad(960, 256); // load the font from the BIOS into VRAM/SGRAM
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
    PadInit(1); //Initialize controller
	setBoard();
    pos[0]=0;
    pos[1]=0;
    blinker = 0;
	while (1) // Game Loop
	{
	    draw();//Draws Board
       //Read 32 bit info from pad(s) and process input
        if(blinker%4==0)
                board[pos[0]][pos[1]]='#';
        else
                board[pos[0]][pos[1]]=' ';
        blinker = blinker++%33;
        if(blinker%8==0)
             controllerListener(PadRead(0));

	}

	return 0;
}

/*
* Make new Blank Board
*/
void setBoard(){
    for(i = 0; i<3;i++){
        for(j =0; j<3;j++){
            board[i][j]=' ';
        }
    }
}
/*
* Process input from controller
*/
void controllerListener(int status){
    if(status & PADLup){
            if(pos[0]>0)
                pos[0]--;
            }
        else if(status & PADLdown){
                if(pos[0]<2)
                    pos[0]++;
            }
        else if(status & PADLleft){
                if(pos[1]>0)
                    pos[1]--;
            }
        else if(status & PADLright){
                if(pos[1]<2)
                    pos[1]++;
            }

}
/*
* Draw contents of board to screen
*/
void draw(){

    index = 0;
    b_string[index]='\t';
    index++;
    b_string[index]='\t';
    index++;
    i = 0;
    for(j = 0; j<3;j++){
        b_string[index]=board[i][j];
        index++;
        if(j<2){
        b_string[index]='!';
        index++;}
    }

    i++;
    b_string[index]='\n';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='+';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='+';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='\n';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='\t';
    index++;

    for(j = 0; j<3;j++){
        b_string[index]=board[i][j];
        index++;
        if(j<2){
        b_string[index]='!';
        index++;}
    }

    i++;
    b_string[index]='\n';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='+';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='+';
    index++;
    b_string[index]='-';
    index++;
    b_string[index]='\n';
    index++;
    b_string[index]='\t';
    index++;
    b_string[index]='\t';
    index++;

    for(j = 0; j<3;j++){
        b_string[index]=board[i][j];
        index++;
        if(j<2){
        b_string[index]='!';
        index++;}
    }
    b_string[index]='\0';
    FntPrint(b_string);
    display();
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
