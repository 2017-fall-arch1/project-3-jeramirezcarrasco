#include "BitsInit.h"
#include "msp430g2553.h"
void buzzer_init()
// this is use to set up the buzzer
{
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

