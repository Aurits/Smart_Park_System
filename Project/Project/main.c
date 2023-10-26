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
	DDRB |= (1 << 0);	  // setting 1 for output of the button when it does an interrupt
	DDRD &= ~(1 << 0);	  // setting 0 for input of the button
	EICRA |= (1 << INT3); // Set for falling edge triggering
	EIMSK |= (1 << INT3); // Enable INT0
	sei();				  // enable external interrupts globally

	// setup the LCD
	DDRG = 0xFF; // set all pins on port G to output
	DDRH = 0xFF; // set all pins on port H to output

	// setting up motor
	DDRJ = 0xFF; // set all pins on port J to output

	// keypads DDR setup
	DDRA = 0b11110000;
	DDRB = 0b11110001;

	while (1)
	{
		commandMode();
		lunch();
		PORTH = 0x0f;
		lunch();

		// keypadGate
		PORTA &= ~(1 << 4);			   // set column 1 to 0
		if ((PINA & 0b00001000) == 00) // check if row 1 is 1
		{
			PORTB = (1 << 0); // display 1
		}
		else if ((PINA & 0b00000100) == 00)
		{
			PORTB = (1 << 0); // display 4
		}
		else if ((PINA & 0b00000010) == 00)
		{
			PORTB = (1 << 0); // display 7
		}
		else if ((PINA & 0b00000001) == 00)
		{
			PORTB = (1 << 0); // display *
		}

		PORTA &= ~(1 << 5);			   // set column 2 to 0
		if ((PINA & 0b00001000) == 00) // check if row 1 is 1
		{
			PORTB = (1 << 0); // display 2
		}
		else if ((PINA & 0b00000100) == 00)
		{
			PORTB = (1 << 0); // display 5
		}
		else if ((PINA & 0b00000010) == 00)
		{
			PORTB = (1 << 0); // display 8
		}
		else if ((PINA & 0b00000001) == 00)
		{
			PORTB = (1 << 0); // display 0
		}

		PORTA &= ~(1 << 6);			   // set column 3 to 0
		if ((PINA & 0b00001000) == 00) // check if row 1 is 1
		{
			PORTB = (1 << 0); // display 3
		}
		else if ((PINA & 0b00000100) == 00)
		{
			PORTB = (1 << 0); // display 6
		}
		else if ((PINA & 0b00000010) == 00)
		{
			PORTB = (1 << 0); // display 9
		}
		else if ((PINA & 0b00000001) == 00)
		{
			PORTB = (1 << 0); // display #
		}

		// keypadFridge
		PORTB &= ~(1 << 5); // set column 1 to 0

		PORTB &= ~(1 << 6); // set column 2 to 0

		PORTB &= ~(1 << 7); // set column 3 to 0

		// rotateFridge();
		rotateFridge();
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
	_delay_ms(1500);
	clearScreen();
}

// function to rotate the motor
void rotateFridge()
{
	PORTJ = 0b00000010; // do rotate clockwise
	_delay_ms(1000);
	PORTJ = 0b00000001; // do rotate anti clockwise
	_delay_ms(1000);
}
// function to always clear the screen of the LCD
void clearScreen()
{
	PORTH = 0x01;
	lunch();
}

// do the interrupt service run tine for INT0
ISR(INT3_vect)
{
	// PORTB ^= (1 << 0); // activate the led on an interrupt
	_delay_ms(50);	  // Delay to denounce the button (if needed)
	displayWelcome(); // Call the function to display the welcome message
}