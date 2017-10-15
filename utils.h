/*  File     utils.h
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


void display_char(char c);

void display_message (char* message);

int checkwin(char me, char you);

void displaywin(int winstatus);

void getmessage(char* buff, uint8_t wld[]);

#endif
