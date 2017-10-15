#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
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

    while(counter == 0) {
        pacer_wait();
        navswitch_update ();
        tinygl_update();
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            counter++;
        }
    }
}

int checkwin(char me, char you) { //Function for checking if a player wins.
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

void displaywin(int winstatus) {
    if (winstatus == 0) { //You've lost
      display_char('L');
    } else if (winstatus == 1) { //You've won
      display_char('W');
    } else if (winstatus == 2) { //Drawn games suck
      display_char('D');
    }
}
