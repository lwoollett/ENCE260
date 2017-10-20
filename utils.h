/*
    File     utils.h
    Authors  Luke Woollett, James Coleman
    Date     15 October 2017
    Brief    Utilities for the PSR game.
*/




#ifndef UTILS_H
#define UTILS_H

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


//Small function to display a character on the screen
void display_char(char c);

//Function to display some scrolling text
void display_message (char* message);

//Function for checking if a player wins
int checkwin(char me, char you);

//Function to display whether the player won, lost or drew
void displaywin(int winstatus);

//Function to display the scores
void getmessage(char* buff, int wld[]);

#endif
