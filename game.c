#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "ir_uart.h"


#define P_TIME 500 //50Hz Master race. 144 is overrated.
#define MESSAGE_RATE 10

void display_char(char c) //Small function to display a character on the screen
{ //It takes a character, adds a list end to it, and then pushes it to the display.
    char to_display[2];
    to_display[0] = c;
    to_display[1] = '\0';
    tinygl_text(to_display);
}

void display_message (char* message)
{
    
    uint8_t counter = 0;
    
    tinygl_init(P_TIME);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);


    tinygl_text(message);

    pacer_init (P_TIME);
    
    while(counter == 0)
    {
        pacer_wait();
        

        navswitch_update ();
        tinygl_update();
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            counter++;
        }
    }
    
}


int checkwin(char me, char you){ //Function for checking if a player wins.
    if (me == you) { //Draw scenario
      return 2;
    } else if (me == 'P' && you == 'R') { //Paper beats rock
      return 1;
    } else if (me == 'S' && you == 'P') { //Scissors beats paper
      return 1;
    } else if (me == 'R' && you == 'S') { //Rock beats scissors
      return 1;
    } else if (you == 'x'){
      return -1;
    } else { //You lost
      return 0;
    }
}

int main (void)
{
    char PSR[3] = {'P', 'S', 'R'}; //The char array that holds our options.
    int chosen = 0; //Our counter for figuring out where in the array we are.
    
    uint8_t start_counter = 0;
    uint8_t end_counter = 0;
    
    //More setup functions.
    system_init ();
    tinygl_init (P_TIME);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();

    int sentstatus = 0; //We can use these as bool values
    int recstatus = 0; //To check if we have received or sent a
    //IR, so we dont continuiosly send packets.
    char recchar = 'x';
    //Set to x so that if recchar == x then we havent received or
    //There has been an error with the receiving of the data.

    ir_uart_init();
    

    pacer_init (P_TIME);

    while (1) {
        //Wait and update functions.
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();


        if (start_counter == 0) {
            display_message("READY?");
        }
        if (start_counter < 100) {
            start_counter++;
        }



        if (recstatus == 0){ //While you havent recieved a packet
            if (ir_uart_read_ready_p() != 0) { //If its ready to recieve a char
                recchar = ir_uart_getc(); //Recieve the character.
                recstatus = 1; //char has been recieved.
                if (recchar != 'P' && recchar != 'S' && recchar != 'R') { //if we've picked up some
                    recstatus = 0; //Random ir junk floating around
                    recchar = 'x'; //Drop it and try recieve again.
                }
            }
        }

        if (sentstatus == 0 && start_counter == 100) //While you havent sent a packet
        {
            if (navswitch_push_event_p (NAVSWITCH_WEST)) { //If the stick has been flicked to the left
                chosen -= 1;
                if (chosen < 0) { //Check if we've fallen off the array
                  chosen = 2; //Flip round to the back of the array
                }
            }
            else if (navswitch_push_event_p (NAVSWITCH_EAST)) { //iff the stick has been flicked to the right
                chosen += 1;
                if (chosen > 2) { //Check if we've gone past the end of the array
                  chosen = 0; //Flip round to the front of the array
                }
            }
            else if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                //Has been pressed in, send the data.
                if (ir_uart_write_ready_p() != 0) { //If we are able to send a char
                  ir_uart_putc(PSR[chosen]); //Send the char
                  sentstatus = 1; //Send is a success... Hopefully.
                }
            }
        }
        if (sentstatus == 1 && recstatus == 1) {
            //Check if you have both recieved and sent data.
            int winstatus = checkwin(PSR[chosen], recchar); //Check who won
            if (winstatus == 0) { //You've lost
              display_char('L');
            } else if (winstatus == 1) { //You've won
              display_char('W');
            } else if (winstatus == 2) { //Drawn games suck
              display_char('D');
            }
            end_counter += 1;
            //Display char stuff is temp code.
            if (navswitch_push_event_p (NAVSWITCH_PUSH) && end_counter > 100) { //Push in the navswitch
                sentstatus = 0; //To start a new game
                recstatus = 0; //So reset these vars
                start_counter = 0;
                chosen = 0;
                recchar = 'x';
                end_counter = 0;
            }

        } else {
          display_char(PSR[chosen]); //Display the chosen char
        }

    }

    return 0;
}
