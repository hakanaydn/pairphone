#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "memory.h"
#include "math.h"

#include <sys/time.h> // for clock_gettime()
#include <unistd.h> // for usleep()

#include <melpe/melpe.h> // for usleep()

#define SPEECH_FRAME 540 
	
int16_t inbuf[SPEECH_FRAME]; //recording from Mike
unsigned char bits[11]; 
int16_t* sp_ptr;
int16_t outbuf[SPEECH_FRAME]; //recording from Mike

struct timeval start, end;
long secs_used,micros_used;

int main(int argc, char *argv[] )
{
	FILE *f_speech;               /* File of speech data                   */
	FILE *f_serial;               /* File of serial bits for transmission  */
  	int16_t frame = 0;             /* frame counter */

	printf("MELPE 1200 CODER TEST\n");

	if ( (f_speech = fopen(argv[1], "rb")) == NULL) {
		printf("%s - Error opening file  %s !!\n", argv[0], argv[1]);
		exit(0);
	}
	printf(" Input speech file:  %s\n", argv[1]);

	if ( (f_serial = fopen(argv[2], "wb")) == NULL) {
		printf("%s - Error opening file  %s !!\n", argv[0], argv[2]);
		exit(0);
	}
	printf(" Output bitstream file:  %s\n", argv[2]);

	/*init melp-e*/
	melpe_i();
	
	gettimeofday(&start, NULL);
	while(fread(&inbuf, sizeof(int16_t), SPEECH_FRAME, f_speech) == SPEECH_FRAME)
	{
		melpe_a(bits,&inbuf );
		melpe_s(&outbuf,bits); 

		fwrite(&outbuf, sizeof(int16_t), SPEECH_FRAME , f_serial);

		frame++;
	}
	gettimeofday(&end, NULL);

	secs_used=(end.tv_sec - start.tv_sec); //avoid overflow by subtracting first
	micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);

	printf("Frame : %d micros_used: %d\n",frame,micros_used);

	return 0;
}



//This is a main procedure of PairPhone testing software
//one-thread implementation as a infinite loop contained procedures for:
//-receiving baseband, demodulating, decrypting, decompressing, playing over earphones (RX)
//-recording voice from mike, compressing, encrypting, modulating, sending baseband into line (TX)
//-scan keyboard, processing. Suspending thread if no job (CTR)
//---------------------------------------------------------------------------
/*#ifdef _WIN32

 #include <stdlib.h>
 #include <stdio.h>
 #include <stddef.h>
 #include <basetsd.h>
 #include <stdint.h>
 #include <windows.h>
 #include <time.h>
 #include <conio.h>
 #include <string.h>
 #include "memory.h"
 #include "math.h"
  
 #else //linux
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 #include "memory.h"
 #include "math.h"
 
 #include <sys/time.h>
 
 #endif

#include "audio/audio.h"  //low-level alsa/wave audio 
#include "crypto/libcrp.h" //cryptography primitives 

#include "crp.h" //key agreement, authentication, encryption/decryption, frame synchronization
#include "ctr.h" //scan keyboard, processing. Suspending thread if no job (CTR)
#include "rx.h"  //receiving baseband, demodulating, decrypting, decompressing, playing over earphones
#include "tx.h"	 //recording voice from mike, compressing, encrypting, modulating, sending baseband into line
#include <melpe/melpe.h> // for usleep()

int main(int argc, char *argv[] )
{


 int i=0;

 printf("---------------------------------------------------------------\r\n");
 printf("   PairPhone v0.1a  Van Gegel, 2016  MailTo: torfone@ukr.net\r\n");
 printf("     P2P private talk over GSM-FR compressed voice channel\r\n");  
 printf("---------------------------------------------------------------\r\n");
   
   randInit(0,0); //init SPRNG
   if(audio_init()) return -1;  //init audio
   tty_rawmode(); //init console IO
   HangUp(); //set idle mode
   
   //main loop 
   do
   {
    i=rx(i);   //receiving
    i=tx(i);   //transmitting
    i=ctr(1);  //controlling
   }
   while(i);
   
   tty_normode(); //restore console
   audio_fin(); //close audio
   return 0;
}
//---------------------------------------------------------------------------
*/




