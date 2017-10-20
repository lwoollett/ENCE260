/*
    File     game.c
    Authors  Luke Woollett, James Coleman
    Date     15 October 2017
    Brief    PSR Game, primary file.
*/
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "ir_uart.h"
#include "utils.h"


#define P_TIME 500 //Pacer time [Hz]
#define MESSAGE_RATE 10

int main (void) {
    char PSR[3] = {'P', 'S', 'R'}; //The char array that holds our options.
    int8_t chosen = 0; //Our counter for figuring out where in the array we are.

    uint8_t start_counter = 0;
    uint8_t end_counter = 0;
    int wld[3] = {48, 48, 48}; //Win loss draw counter

    //More setup functions.
    system_init ();
    tinygl_init (P_TIME);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();

    uint8_t sentstatus = 0; //We can use these as bool values
    uint8_t recstatus = 0; //To check if we have received or sent a
    //IR, so we dont continuiosly send packets.
    char recchar = 'x';
    //Set to x so that if recchar == x then we havent received or
    //There has been an error with the receiving of the data.

    ir_uart_init();
    pacer_init(P_TIME);

    while (1) {
        //Wait and update functions.
        pacer_wait();
        tinygl_update();
        navswitch_update();

        if (start_counter == 0) { //As soon as the loop starts we want to display this message.
            display_message("READY?");
        }
        if (start_counter < 100) { //This is so that when you press the reset
            start_counter++;       //You dont immediate go both through the display message
        }                          //And straight away select a option.

        if (recstatus == 0) { //While you havent recieved a packet
            if (ir_uart_read_ready_p() != 0) { //If its ready to recieve a char
                recchar = ir_uart_getc(); //Recieve the character.
                recstatus = 1; //char has been recieved.
                if (recchar != 'P' && recchar != 'S' && recchar != 'R') { //if we've picked up some
                    recstatus = 0; //Random ir junk floating around
                    recchar = 'x'; //Drop it and try recieve again.
                }
            }
        }

        if (sentstatus == 0 && start_counter == 100) { //While you havent sent a packet
            if (navswitch_push_event_p (NAVSWITCH_WEST)) { //If the stick has been flicked to the left
                chosen -= 1; //go down in the list
                if (chosen < 0) { //Check if we've fallen off the array
                    chosen = 2; //Flip round to the back of the array
                }
            }
            else if (navswitch_push_event_p (NAVSWITCH_EAST)) { //iff the stick has been flicked to the right
                chosen += 1; //Go up in the list
                if (chosen > 2) { //Check if we've gone past the end of the array
                    chosen = 0; //Flip round to the front of the array
                }
            }
            else if (navswitch_push_event_p (NAVSWITCH_PUSH)) { //Has been pressed in, send the data.
                if (ir_uart_write_ready_p() != 0) { //If we are able to send a char
                    ir_uart_putc(PSR[chosen]); //Send the char
                    sentstatus = 1; //Send is a success
                }
            }
        }
        if (sentstatus == 1 && recstatus == 1) { //Check if you have both recieved and sent data.
            int winstatus = checkwin(PSR[chosen], recchar); //Check who won
            displaywin(winstatus);

            if (end_counter < 100) { //This has the same functionality as the start counter
                end_counter++;
            }

            if (navswitch_push_event_p (NAVSWITCH_PUSH) && end_counter == 100) { //Push in the navswitch
                sentstatus = 0; //To start a new game
                recstatus = 0; //So reset these vars
                start_counter = 0;
                chosen = 0;
                recchar = 'x';
                end_counter = 0;
            }
            if (navswitch_push_event_p (NAVSWITCH_NORTH)) { //If we're wanting to see the stats
                char buff[30] = {"\0"}; //Just a buffer, could be smaller
                wld[winstatus] += 1; //Add one to the list
                getmessage(buff, wld); //Get the string for game completion
                display_message(buff); //Display it
            }

        } else {
            display_char(PSR[chosen]); //Display the chosen char
        }

    }
    return 0;
}
