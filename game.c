#include "system.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "./fonts/font5x7_1.h"

#define P_TIME = 100;
#define PSR = ["P", "S", "R"]


void display_char(char c)
{
  char to_display[2];
  to_display[0] = c;
  to_display[1] = '\0';
  tinygl_text(to_display);
}

int main (void)
{
  system_init();
  navswitch_init();

  while (1)
  {
    display_char(42);
    tinygl_update();
  }
  return 0;
}
