#include "terminal.h"
#include  "common.h"
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */

/*Ovo je moja verzija gameOfLife bolja je od obicne verije jer umjesto table koja je 2D imamo valjak pa se ponasao kao 3D gameOfLife(ali pravila su ista)
 Na tebi je da realizujes algoritam i time delay i ako mozes da stavis da su nam celije ZELENE boje umjesto sivih
 Funckije koje trebas da izmjenis su CellAlgoritam i napravit funkciju TimeDelay
 Kad realizujes to poslaji mi kod da jos ubacim da prikazuje koliko ima zivih celija prilikom svake iteracije
 */ 

uint16_t randomNumber(void)
{
outb(0x70, (1<<7) | 0x00); //(selected CMOS register number)     (NMI disable bit<<7)
int i=0;
for(i;i<50;i++); //delay
unsigned char seconds = inb(0x71); 
//unsigned char minute = inb(0x71); 
//uint16_t seconds=(minute)*60; //u int


unsigned char registerB;
outb(0x70, (1<<7) | 0x0B);
registerB =  inb(0x71);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            seconds
 = (seconds & 0x0F) + ((seconds / 16) * 10);}



return seconds;
}

int i2a(char *s, int n){
    int qr;
    int pos;

    if(n == 0) return 0;

    qr = n/10;
    pos = i2a(s, qr);
    s[pos] = n%10 + '0';
    return pos + 1;
}

void my_itoa(char *output_buff, int num){
    char *p = output_buff;
    if(num < 0){
        *p++ = '-';
        num *= -1;
    } else if(num == 0)
        *p++ = '0';
    p[i2a(p, num)]='\0';
    terminal_writestring(p);
}

void write_number(int number)
{

 char nizstr[12]={0};
 my_itoa(nizstr, number);
}

void CellAlgoritam( unsigned char array[][VGA_WIDTH])
{

int y=randomNumber();
write_number(y);

for (int i=0; i < VGA_HEIGHT; i++)
{
        for (int j = 0; j <VGA_WIDTH; j++)
        {
	  y = ((4*31+2)*y+1234) % 25; //mora biti mod n,n->neparan broj,za paran broj popunice sve kolone i imacemo crn ekran
	  if(y&1)
	  array[i][j] = 254;

	}
}


}

int CheckNeighborsCells(unsigned char array[][VGA_WIDTH],int i,int j)
{
    
    
     int counterOfBlocks=0;
    if (array[i][j - 1] == 254 && j > 0)
            counterOfBlocks = counterOfBlocks + 1;
    else if (j==0 && array[i][VGA_WIDTH-1]==254)
            counterOfBlocks = counterOfBlocks + 1;

    if (array[i][j + 1] == 254 && j < VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;
    else if(array[i][0] == 254 && j == VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;

    if (array[i - 1][j] == 254 && i > 0)
            counterOfBlocks = counterOfBlocks + 1;					

    if (array[i + 1][j] == 254 && i < VGA_HEIGHT - 1)
            counterOfBlocks = counterOfBlocks + 1;

    if (array[i - 1][j - 1] == 254 && i > 0 && j > 0)
            counterOfBlocks = counterOfBlocks + 1;
    else if(array[i - 1][VGA_WIDTH-1] == 254 && i > 0 && j == 0)
            counterOfBlocks = counterOfBlocks + 1;

    if (array[i - 1][j + 1] == 254 && i > 0 && j < VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;
    else if(array[i - 1][0] == 254 && i > 0 && j == VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;

    if (array[i + 1][j - 1] == 254 && j > 0 && i < VGA_HEIGHT - 1)
            counterOfBlocks = counterOfBlocks + 1;
    else if(array[i + 1][VGA_WIDTH - 1] == 254 && j == 0 && i < VGA_HEIGHT - 1)
            counterOfBlocks = counterOfBlocks + 1;
    
    if (array[i + 1][j + 1] == 254 && i < VGA_HEIGHT - 1 && j < VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;
    else if (array[i + 1][0] == 254 && i < VGA_HEIGHT - 1 && j == VGA_WIDTH - 1)
            counterOfBlocks = counterOfBlocks + 1;
    
    return counterOfBlocks;
    
    
}

void CopyArray(unsigned char array[][VGA_WIDTH],unsigned char arraysup[][VGA_WIDTH])
{    
    for (int i=0; i < VGA_HEIGHT; i++)
     {
        for (int j = 0; j < VGA_WIDTH; j++)
        {
           arraysup[i][j] = array[i][j];
        }
     }
}
void gameOfLife(void)
{
	
    unsigned char array[25][80] = { 0 };
    unsigned char arraysup[25][80] = { 0 };
    int i, j;
 
   for (i=0; i < VGA_HEIGHT; i++)
    {
        for (j = 0; j < VGA_WIDTH; j++)
        {
            arraysup[i][j] = array[i][j] = 32;
        }
    }
   //terminal_writestring("u gameOfLife metodi\n");
    CellAlgoritam(&array); //u ovoj funckiji vrsimo inicijalizaciju matrice
   //terminal_writestring("izasao iz cell algoritma\n");

   CopyArray(array,arraysup); //vrsimo inicijalizaciju kopije niza
   
    for (; ;)
    {
	for (i = 0; i < VGA_HEIGHT; i++)
	    {
		for (j = 0; j < VGA_WIDTH; j++)
		{
                    int counterOfBlocks = 0;
                    
                    counterOfBlocks=CheckNeighborsCells(array,i,j); //provjera koliko susjeda ima trenutna celija
		  
		    if (counterOfBlocks == 3 && array[i][j] == 32)
		            arraysup[i][j] = 254;
		    else if ((counterOfBlocks > 3 || counterOfBlocks < 2) && array[i][j]!=32)
		            arraysup[i][j] = 32;
		    else if((counterOfBlocks==3 || counterOfBlocks==2) && array[i][j] != 32)
		            arraysup[i][j] = 254;
		        
		}
	    }

         CopyArray(arraysup,array); //vrsimo inicijalizaciju original niza sa izmjenjenom kopijom
    
	for(i=0;i<VGA_HEIGHT;i++)
        {
            for(j=0;j<VGA_WIDTH;j++)
            terminal_putchar(array[i][j],j%15+1);  //boje od [1,15]
        }
         
        for(int i=0;i<6000;i++) //ovo prestavlja kao moju time delay funkciju 
          for(int j=0;j<6000;j++);
            asm("nop");
       
    }
	
}



 
void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
	//randomNumber();
 	gameOfLife();
	
	/* Newline support is left as an exercise. */

	//terminal_writestring("Hello, kernel World!\n");
}

