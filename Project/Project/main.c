/*
 * Project.c
 *
 * Created: 26/10/2023 11:16:42
 * Author : Ambrose
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define rs
#define rw
#define enable
#define dataline
unsigned char message[] = "Hello new tourist";

int main(void)
{

	// setting up the button and interrupt
	DDRC |= (1 << 3);	  // setting 1 for output of the button when it does an interrupt
	DDRD &= ~(1 << 0);	  // setting 0 for input of the button
	EICRA |= (1 << INT3); // Set for falling edge triggering
	EIMSK |= (1 << INT3); // Enable INT0
	sei();				  // enable external interrupts globally

	// setup the LCD
	DDRG = 0xFF; // set all pins on port C to output
	DDRH = 0xFF; // set all pins on port D to output

	while (1)
	{
		commandMode();
		lunch();
		PORTH = 0x0f;
		lunch();

		// the loop to display the welcome message
	}
}

// function to write to the LCD
void lunch()
{
	PORTG |= (1 << 2); // set enable to 1
	_delay_ms(1);	   // wait 1ms
	PORTG &= ~(1 << 2);
	_delay_ms(1);
}
// function for LCD data mode
void dataMode()
{
	PORTG |= (1 << 0);	// set rs to 1- data mode
	PORTG &= ~(1 << 1); // set rw to 0- write mode
}
// function for LCD command mode
void commandMode()
{
	PORTG &= ~(1 << 0); // set rs to 0- command mode
	PORTG &= ~(1 << 1); // set rw to 0- write mode
}
void displayWelcome()
{
	// the loop to display the welcome message
	for (int i = 0; i < 18; i++)
	{
		dataMode();
		PORTH = message[i];
		lunch();
	}
}
// do the interrupt service run tine for INT0
ISR(INT3_vect)
{
	PORTC ^= (1 << 3); // activate the led on an interrupt
	_delay_ms(50);	   // Delay to denounce the button (if needed)
	displayWelcome();  // Call the function to display the welcome message
}