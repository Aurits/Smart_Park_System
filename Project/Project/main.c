/*
 * Project.c
 *
 * Created: 26/10/2023 11:16:42
 * Author : Ambrose
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>

int main(void)
{
	DDRC |= (1<<3); //setting 1 for output of the button when it does an interrupt
	DDRD &= ~(1<<0); //setting 0 for input of the button
	
	EICRA |= (1<<INT3); // Set for falling edge triggering
	EIMSK |= (1<<INT3); // Enable INT0
	sei(); //enable external interrupts globally
	
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
	
}
	// do the interrupt service run tine for INT0
	ISR(INT3_vect){
		PORTC ^= (1<<3); //activate the led on an interrupt
	}


