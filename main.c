#define _CRT_SECURE_NO_WARNINGS
/* headers */
#include <DAQlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* TODO: define symbolic constants */
#define DISPLAY_SIMULATOR  6
#define DISPLAY0     0
#define SEQLEN  5
#define TRUE 1
#define FALSE 0
#define DELAY 400

#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3

#define BUT0 0
#define BUT1 1
#define BUT2 2
#define BUT3 3


/* function prototypes */
void genSeq(int seq[] );  
void simonGame(void); 
int randInt(int lower, int upper);
void blinkSeq(int seq[],int len);
int checkSeq(int seq[], int len);
int readIn(int input[]);
void blink(int led);

/* main function */
int main(void)
{
	if (setupDAQ(DISPLAY_SIMULATOR)) {
		srand((unsigned)time(NULL));
		simonGame();
	}
	else {
		printf("ERROR: failed to initialize the DAQ\n");
	}
	return 0;
}

/* control loop */
void simonGame(void)
{
	int seq[SEQLEN];
	genSeq(seq);
	int lost=0;


	while (continueSuperLoop()) {
		printf("\n\nGame has begun!");


		for (int i = 0; i < SEQLEN; i++) {
			printf("%d",seq[i]);
		}

		printf("\n");
		printf("\n");

		for (int i = 0; i < SEQLEN ; i++) {
			
			blinkSeq(seq, i);
			lost = checkSeq(seq, i);
			if (lost == 1) {
				blink(1);
				blink(1);
				blink(1);
				break;
			}

		}
		if (lost == 0) {
			blink(0);
			blink(0);
			blink(0);
		}
		
		printf("\n\nGame has ended!");
		Sleep(100000);
	}
	
}

/* writes the integer number to the displays */
void genSeq(int seq[SEQLEN])
{
	
	for (int i = 0; i < SEQLEN; i++) {
		seq[i] = randInt(LED0,LED3);
	}

	

}

int randInt(int lower, int upper) {

	
	return lower + (rand() % (upper - lower+1));
}
void blinkSeq(int seq[],int len) {

	for (int i = 0; i <= len; i++) {
		printf("\n \n blinking>> %d\n", seq[i]);
		blink(seq[i]);
	}

}
int checkSeq(int seq[],int len) {
	int input[1];
	int step = 0;
	
	 while (step<=len){
		while (readIn(input) == -1) {
			
		}
		
		
		
		if (input[0]==seq[step]) {
			printf("\n step %d is a success", step);
		}
		else {
			printf("\n game lost at %d\n",step);
			return 1;
		}
		
		step++;
	 } 
	 if (step == SEQLEN - 1) {
		 return 0;
	 }
	 
	 return 0;
}
int readIn(int input[]) {
	for (int i = 0; i <4; i++) {
		if (digitalRead(i)==1) {
			while (digitalRead(i) == 1) {
				
				}
			
			input[0] = i;
			return 1;
		}
		
	}

	return -1;
}

void blink(int led) {

	Sleep(DELAY);
	digitalWrite(led, 1);
	Sleep(DELAY);
	digitalWrite(led, 0);
}