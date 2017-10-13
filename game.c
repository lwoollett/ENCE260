#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define P_TIME 200
#define MESSAGE_RATE 10



void display_char(char c)
{
    char to_display[2];
    to_display[0] = c;
    to_display[1] = '\0';
    tinygl_text(to_display);
}

int main (void)
{
    
    char character = 'P';

    system_init ();
    tinygl_init (P_TIME);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();


    pacer_init (P_TIME);

    while (1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();
        
        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            character = 'P';
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            character = 'S';
        }
        if (navswitch_push_event_p (NAVSWITCH_EAST)) {
            character = 'R';
        }
        display_char(character);
    }

    return 0;
}

