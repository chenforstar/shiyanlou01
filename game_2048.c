#include<stdio.h>
#include<stdlib.h>
//used by linux system 
#include<cures.h>//zhi fu jie mian  zhi zuo
#include<unistd.h>//Contains function prototypes for many system services
#include<signal.h>//Signal header file that defines signal symbol constants, signal structure, and signal operation function prototypes.
//end
#include<time.h>


/*
	global variable
*/

//main interface
int a[4][4]={0};
//numbers of spaces
int empty;
int old_y,old_x;

/*
	functions
*/

void draw();//draw the game interface
void play();//the logical body of game
void init();
void draw_one(int y,int x);
void cnt_value(int *new_y,int *new_x);
int game_over();
int cnt_one(int y,int x);
