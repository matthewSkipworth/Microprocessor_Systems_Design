//Matthew Skipworth
//Microprocessor Systems
//8 November, 2018 (start date)


//@author Matthew Skipworth
//@version 9 November, 2018


//******************************************Problem Statement******************************************//
//2.
//Controlling sequential LEDs with SW1.

//In this exercise the LEDs on board will light in the order described below:

//When you press SW1 -Initially, nothing should happen... 
//The  first  press and release of  SW1 should  turn  on  D1.  

//The  second  press and  release  of SW1  will turn  off  D1  and  make 
//D2 start  blinking.  The  third  press and  release of  SW1 will keep
//D2 on and start blinking D3. The fourth press of SW1 will not change the status 
//of D2 and D3, and meanwhile turn on D4. When you release SW1 all the LEDs on board 
//should be off and start the process over.
//****************************************End Problem Statement****************************************//



//*****************************Included header files and macro definitions*****************************//
#include<stdio.h>
#include<stdlib.h>

# define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608 ))
	
# define  DELAY_DEBOUNCE             SYSTEM_CLOCK_FREQUENCY /1000

# define SYSCTL_DC4_GPION        0x00001000  // GPIO Port N Present
# define GPIO_PORTN_DATA_R       (*((volatile unsigned long *)0x400643FC))
# define GPIO_PORTN_DIR_R        (*((volatile unsigned long *)0x40064400))
# define GPIO_PORTN_DEN_R        (*((volatile unsigned long *)0x4006451C))
# define GPIO_PORTN_AFSEL_R      (*((volatile unsigned long *)0x40064420))
# define GPIO_PORTN_AMSEL_R      (*((volatile unsigned long *)0x40064528))
	
# define GPIO_PORTN_PIN1 0x02 // PF4, LED D3 on board
# define GPIO_PORTN_PIN0 0x01


# define SYSCTL_DC4_GPIOJ        0x00000100  // GPIO Port J Present
# define GPIO_PORTJ_AHB_DATA_R   (*((volatile unsigned long *)0x400603FC))
# define GPIO_PORTJ_AHB_DIR_R    (*((volatile unsigned long *)0x40060400))
# define GPIO_PORTJ_AHB_AFSEL_R  (*((volatile unsigned long *)0x40060420))	
# define GPIO_PORTJ_AHB_DEN_R    (*((volatile unsigned long *)0x4006051C))
# define GPIO_PORTJ_AHB_PUR_R    (*((volatile unsigned long *)0x40060510))
# define GPIO_PORTJ_AHB_AMSEL_R  (*((volatile unsigned long *)0x40060528))
# define GPIO_PORTJ_AHB_PCTL_R   (*((volatile unsigned long *)0x4006052C))

# define GPIO_PORTJ_PIN1 0x02 // PJ1
# define GPIO_PORTJ_PIN0 0x01 // PJ0

# define SYSCTL_RCGC2_GPIOF  0x0020 // Port F clock gating control
# define GPIO_PORTF_AHB_DATA_R   (*((volatile unsigned long *)0x4005D3FC))
# define GPIO_PORTF_AHB_DIR_R    (*((volatile unsigned long *)0x4005D400))
# define GPIO_PORTF_AHB_DEN_R    (*((volatile unsigned long *)0x4005D51C))
# define GPIO_PORTF_AHB_PUR_R    (*((volatile unsigned long *)0x4005D510))
# define GPIO_PORTF_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005D528))
# define GPIO_PORTF_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005D420))	
# define GPIO_PORTF_AHB_PCTL_R   (*((volatile unsigned long *)0x4005D52C))
	
# define  GPIO_PORTF_PIN0  0x01
# define  GPIO_PORTF_PIN4  0x10
	

# define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
# define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY/10
# define GPIO_PORTF_AHB_AFSEL_DIS 0x00 //disable alternate function



# define INIT 0;
# define INTERMEDIATE 1;
# define SOLID 2;
# define BLINK 3;


//********************************end header files and macro definitions*******************************//




/**
 * Function delay - runs the system clock to wait. This function is used to make the LEDs blink consistently.
 *
 * @param - This function takes in an unsigned long as an amount of time for the program to wait. 
 */
void delay ( unsigned long counter ){
    unsigned long i = 0;
    for (i = 0; i < counter ; i++) ;
}

/**
 *Function update - updates the state of the state machine.
 *
 *@param - This function takes in an unsigned int representing the current 
 *state of the state machine.
 *
 *@return - This function determines what the next state of the state machine 
 *should be based on whether or not the button (SW1) is currently pressed.
 */

unsigned int update(unsigned int state) {
	
  unsigned int next_state;                        //return variable
	
	switch (state) {
		case 0: //Initial
    			if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					    next_state = 1; 	
					} else {
					    next_state = state; //do nothing
					} break;
					
		case 1: //Intermediate/Wait
					 if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					     next_state = state;	//do nothing
		       } else {
					     next_state = 2; 
					 } break;
					 
		case 2: //Solid
					 if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
		           next_state = 3;
		       } else {
					     next_state = state;
					 } break;
					 
		case 3: //Intermediate/wait
					  if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					     next_state = state;	//do nothing
		       } else {
					     next_state = 4; 
					 } break;
					 
		case 4: //D1 off, D2 Blink			 
		       if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					     next_state = 5;
					 } else {
					     next_state = state;
					 } break;
		
		case 5: //Intermediate/wait
					  if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					      next_state = state;	//do nothing
		       } else {
					      next_state = 6; 
					 } break;
					 
		case 6: //D2 solid, D3 blink
					  if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
						    next_state = 7;
						} else {
						    next_state = state;
						} break;
						
		case 7: //D2 solid, D3 solid, D4 blink
					  if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PORTJ_PIN0)) {
					      next_state = state;	//do nothing
		       } else {
					      next_state = 0; 
					 } break;
		default:
						next_state = 0;
	}
	return next_state;
}


//**********************************************Begin main*********************************************//

int main (){
	
/****************************************pin setup and configuration****************************************/	
SYSCTL_RCGC2_R |= SYSCTL_DC4_GPION;
SYSCTL_RCGC2_R |= SYSCTL_DC4_GPIOJ; 
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
// disable the alternate functionality
GPIO_PORTF_AHB_AFSEL_R &= 0x00;
GPIO_PORTF_AHB_AMSEL_R &= 0x00;
	
GPIO_PORTJ_AHB_AFSEL_R &= 0x00;
GPIO_PORTJ_AHB_AMSEL_R &= 0x00;

GPIO_PORTJ_AHB_PCTL_R = 0x00;	

GPIO_PORTN_AMSEL_R &= 0x00;	
GPIO_PORTN_AFSEL_R &= 0x00;
	
// PJ0 & PJ1 as input	
GPIO_PORTJ_AHB_DIR_R &= 0x00;
GPIO_PORTJ_AHB_DEN_R |= GPIO_PORTJ_PIN1 + GPIO_PORTJ_PIN0;

// PN0 & PN1 as output
	
GPIO_PORTN_DIR_R |= GPIO_PORTN_PIN1 + GPIO_PORTN_PIN0;	
GPIO_PORTN_DEN_R |= GPIO_PORTN_PIN1 + GPIO_PORTN_PIN0;	

// PF0 & PF4 as output
GPIO_PORTF_AHB_DIR_R |= GPIO_PORTF_PIN0 + GPIO_PORTF_PIN4;
GPIO_PORTF_AHB_DEN_R |= GPIO_PORTF_PIN0 + GPIO_PORTF_PIN4;
GPIO_PORTJ_AHB_PUR_R |= 0x01 + 0x02;

/****************************************end setup and configuration****************************************/


unsigned int state;

    while (1) {

				if (state == 0 ) {
						GPIO_PORTF_AHB_DATA_R &= 0;
					  GPIO_PORTN_DATA_R &= 0;
					
				} 	else if (state == 1 || state == 3) {
						; // do nothing
					
				}	else if (state == 2) {
					  GPIO_PORTF_AHB_DATA_R &= 0;
				    
					  GPIO_PORTN_DATA_R |= GPIO_PORTN_PIN1;
					
				}	else if (state == 4 || state == 5) {
						GPIO_PORTN_DATA_R &= 0;
					  GPIO_PORTF_AHB_DATA_R &= 0;
					
						GPIO_PORTN_DATA_R |= GPIO_PORTN_PIN0;
						delay(SYSTEM_CLOCK_FREQUENCY/16);
					  GPIO_PORTN_DATA_R &= 0;
					  delay(SYSTEM_CLOCK_FREQUENCY/16);
					
				}	else if (state == 6) {
					  GPIO_PORTN_DATA_R &= 0;
					  GPIO_PORTF_AHB_DATA_R &=0;
					
				    GPIO_PORTN_DATA_R |= GPIO_PORTN_PIN0;        // turn LED2 on solid
					  GPIO_PORTF_AHB_DATA_R |= GPIO_PORTF_PIN4;    // turn LED3 on
						delay(SYSTEM_CLOCK_FREQUENCY/16);            // delay
					  GPIO_PORTF_AHB_DATA_R &= 0;                  // turn LED3 off 
					  delay(SYSTEM_CLOCK_FREQUENCY/16);            // delay
					
				}	else if (state == 7) {
				    GPIO_PORTN_DATA_R |= GPIO_PORTN_PIN0; 
					
					  GPIO_PORTF_AHB_DATA_R |= GPIO_PORTF_PIN4;
					
					  GPIO_PORTF_AHB_DATA_R ^= GPIO_PORTF_PIN0;    //XOR PF0. This allows
					                                               //pin 0 to blink without 
						delay(SYSTEM_CLOCK_FREQUENCY/16);            //affecting the other 
					  GPIO_PORTF_AHB_DATA_R ^= GPIO_PORTF_PIN0;    //pins on port F.
					  delay(SYSTEM_CLOCK_FREQUENCY/16);
				}	
		    state = update(state);
    }
}

//***********************************************End main**********************************************//




