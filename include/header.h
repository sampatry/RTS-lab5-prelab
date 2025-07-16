#ifndef HEADER_H
#define HEADER_H

// Include necessary standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//stores availible opperations of program
char operations[7][20] = {
    "add",
    "subtract",
    "multiply",
    "triangle number",
    "factorial",
    "join 2 strings",
    "end"
};

typedef struct {
    char type;
    char text[50];
} LogData;

typedef struct{
    char type;
    char text[50]; //this char array will store the user inputs
    char time[25];
}user_log;

#endif // HEADER_H