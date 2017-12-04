  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <shape.h>

#define GREEN_LED BIT6

int SCORE = 0;
AbRect rect10 = {abRectGetBounds, abRectCheck, {20,2}}; 
Abmap rectput = {abRectGetBoundsm, abRectCheckm, {160,120}};
Abbak recbak = {abRectGetBoundsb, abRectCheckb, {160,120}};
Abcat recat = {abRectGetBoundsC, abRectCheckC, {160,120}};
AbRectOutline fieldOutline = {abRectOutlineGetBounds, abRectOutlineCheck,{screenWidth/2, screenHeight/2}};
//Layer used for triple score
Layer layer4 = {
  (AbShape *)&recat,
  {(screenWidth/2)+10, (screenHeight/2)+5}, 
  {0,0}, {0,0},				   
  COLOR_BLUE,
  0
};
  
//Layer for double score
Layer layer3 = {		
  (AbShape *)&recbak,
  {(screenWidth/2)+10, (screenHeight/2)+5}, 
  {0,0}, {0,0},				   
  COLOR_BLUE,
  &layer4,
};
//Layer for single score
Layer layer2 = {
  (AbShape *)&rectput,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_YELLOW,
  &layer3,
};
//Layer for the region where the game is played
Layer fieldLayer = {	       
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},
  {0,0}, {0,0},				   
  COLOR_BLACK,
  &layer2
};
// this is the moving block which we keep track of for sxore
Layer layer0 = {	
  (AbShape *)&rect10,
  {screenWidth/2,30 },
  {0,0}, {0,0},				    
  COLOR_RED,
  &fieldLayer,
};

typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;
 
MovLayer ml0 = { &layer0, {0,2}, 0 }; 

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  


void delay_ms(unsigned int ms)
{
  unsigned int i;
  for(i = 0;i <=ms; i++)
    __delay_cycles(500);

}
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) )
      {
	//if the region is touch the score goes down 
	SCORE--;
	int velocity = ml->velocity.axes[axis];
	//this check for score AND the current velozity , if the if-statemnet is true then the velozity is increased/decreased and layer colors are changed
 	if((SCORE > 10 && SCORE < 25) && (velocity == 2 || velocity ==4))
	  {
	    layer0.color = COLOR_CYAN;
	    velocity = ml->velocity.axes[axis] = 3;
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] += (2*velocity);
	  }
 	if((SCORE > 25 && SCORE < 35) && (velocity == 3 || velocity ==5))
	  {
	    layer0.color = COLOR_SIENNA;
	    layer3.color = COLOR_BLUE;
	    layer4.color = COLOR_BLUE;
	    velocity = ml->velocity.axes[axis] = 4;
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] += (2*velocity);
	  }
        if((SCORE > 35 && SCORE < 65) && (velocity == 4 || velocity ==7))
	  {
	    layer0.color = COLOR_VIOLET;
	    layer3.color = COLOR_BLUE;
	    layer4.color = COLOR_CHOCOLATE;
	    velocity = ml->velocity.axes[axis] = 5;
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] += (2*velocity);
	  }
	if(SCORE > 65 && velocity == 5)
	  {
	    layer0.color = COLOR_BLACK;
	    layer4.color = COLOR_FOREST_GREEN;
	    velocity = ml->velocity.axes[axis] = 7;
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] += (2*velocity);
	  }
	else if(SCORE < 10 && velocity == 3)
	  {
	    layer0.color = COLOR_RED;
	    velocity = ml->velocity.axes[axis] = 3;
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] += (2*velocity);
	  }
	else
	  {
	    velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	    newPos.axes[axis] +=(2*velocity);
	  }	
      }	
    } 
    ml->layer->posNext = newPos;
  } 
}
u_int bgColor = COLOR_BLUE;     
int redrawScreen = 1;          

Region fieldFence;		

void main()
{
  //bunch of bits starters
  buzzer_init();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
  P1DIR |= GREEN_LED;				
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  shapeInit();
  shapeInit();
  layerInit(&layer0);
  layerDraw(&layer0);
  layerGetBounds(&fieldLayer, &fieldFence);
  enableWDTInterrupts();     
  or_sr(0x8);	             
  P2REN |= BIT0 & BIT1 & BIT2 & BIT3;
  P2OUT |= BIT0 & BIT1 & BIT2 & BIT3;
  P2IE |= BIT0 & BIT1 & BIT2 & BIT3;
  P2IES |= BIT0 & BIT1 & BIT2 & BIT3;
  _BIS_SR(CPUOFF + GIE);
  for(;;) { 
    while (!redrawScreen) {
      P1OUT &= ~GREEN_LED; 
      or_sr(0x10);	   
    }
    P1OUT |= GREEN_LED;    
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  }
}

void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  // this using the layer 0 block, checks for the current axis and if the if-statemnet is true it increase the score
  // also it adds sound
  if(P2IFG & BIT0 & ((layer0.pos.axes[1] >8 && layer0.pos.axes[1] <20)|| (layer0.pos.axes[1] >74 && layer0.pos.axes[1] <86)
		     || (layer0.pos.axes[1] >140 && layer0.pos.axes[1] <152)))
    {
      CCR0 = 1000;
      SCORE +=2;
      P2IFG &= ~BIT0;
      CCR1 = 1000 >> 1;
    }
  else if(P2IFG & BIT0 & ((layer0.pos.axes[1] >47 && layer0.pos.axes[1] <55)|| (layer0.pos.axes[1] >113 && layer0.pos.axes[1] < 121)))
    {
      CCR0 = 2000;
      SCORE +=4;
      P2IFG &= ~BIT0;
      CCR1 = 2000 >> 1;
    }
  else if(P2IFG & BIT0 & ((layer0.pos.axes[1] >33 && layer0.pos.axes[1] <38)|| (layer0.pos.axes[1] >99 && layer0.pos.axes[1] < 104)))
    {
      CCR0 = 3000;
      SCORE +=8;
      P2IFG &= ~BIT0;
      CCR1 = 3000 >> 1;
    }
  else if(P2IFG & BIT0)
    {
      CCR0 = 500;
      SCORE--;
      P2IFG &= ~BIT0;
      CCR1 = 500 >> 1;
    }
  else if(P2IFG & BIT3)
    {
      CCR0 = 5000;
      SCORE ++;
      P2IFG &= ~BIT3;
      CCR1 = 5000 >> 1;
    }
  else if(P2IFG & BIT2)
    {
      CCR0 = 350;
      SCORE ++;
      P2IFG &= ~BIT2;
      CCR1 = 350 >> 1;
    }
  else if(CCR0 !=1)
    {
      delay_ms(1000);
      CCR0 = 1;
      CCR1 = 1 >> 1;
    }
  if (count == 15) {
    mlAdvance(&ml0, &fieldFence);
    redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
