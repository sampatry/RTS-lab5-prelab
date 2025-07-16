#ifndef HEADER_H
#define HEADER_H

// Include necessary standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//stores availible opperations of program
char operations[8][20] = {
    "add",
    "subtract",
    "multiply",
    "triangle number",
    "factorial",
    "magic 8 ball",
    "join 2 strings",
    "end"
};

//structure for 8ball option (creates alias for struct Eightball)
typedef struct {
    char magic8ball[10][50]; // this char array will store the 8ball options
} EightBall;

// Function to initialize the magic 8 ball using pointer address
void initialize_eightball(EightBall *ball) { 
    //Uses strcpy to copy strings into the array
    strcpy(ball->magic8ball[0], "It is certain."); //the arrow (->) is used since ball is a pointer
    strcpy(ball->magic8ball[1], "It is decidedly so.");
    strcpy(ball->magic8ball[2], "Without a doubt.");
    strcpy(ball->magic8ball[3], "Yes definitely.");
    strcpy(ball->magic8ball[4], "You may rely on it.");
    strcpy(ball->magic8ball[5], "Don't count on it.");
    strcpy(ball->magic8ball[6], "My reply is no.");
    strcpy(ball->magic8ball[7], "My sources say no.");
    strcpy(ball->magic8ball[8], "Outlook not so good.");
    strcpy(ball->magic8ball[9], "Very doubtful.");
}

typedef struct{
    char type;
    char text[50]; //this char array will store the user inputs
    char time[25];
}user_log;

#endif // HEADER_H