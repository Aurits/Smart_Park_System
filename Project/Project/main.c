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
unsigned char message[] = "Hello new tourist!";

int keypadGate()
{
	PORTA = 0b11110111;				 // set column 1 to 0
	PINA |= (1 << 4);				 // set column 1 to 1
	if ((PINA & 0b00001000) == 0x00) // check if row 1 is 1
	{
		return 1; // display 1
		_delay_ms(50);
	}
	else if ((PINA & 0b00000100) == 0x00)
	{
		return 4; // display 4
		_delay_ms(50);
	}
	else if ((PINA & 0b00000010) == 0x00)
	{
		return 7; // display 7
		_delay_ms(50);
	}
	else if ((PINA & 0b00000001) == 0x00)
	{
		return 200; // display *
	}

	PORTA &= ~(1 << 5);				 // set column 2 to 0
	if ((PINA & 0b00001000) == 0x00) // check if row 1 is 1
	{
		return 2; // display 2
		_delay_ms(50);
	}
	else if ((PINA & 0b00000100) == 0x00)
	{
		return 5; // display 5
		_delay_ms(50);
	}
	else if ((PINA & 0b00000010) == 0x00)
	{
		return 8; // display 8
		_delay_ms(50);
	}
	else if ((PINA & 0b00000001) == 0x00)
	{
		return 0; // display 0
		_delay_ms(50);
	}

	PORTA &= ~(1 << 6);				 // set column 3 to 0
	if ((PINA & 0b00001000) == 0x00) // check if row 1 is 1
	{
		return 3; // display 3
		_delay_ms(50);
	}
	else if ((PINA & 0b00000100) == 0x00)
	{
		return 6; // display 6
		_delay_ms(50);
	}
	else if ((PINA & 0b00000010) == 0x00)
	{
		return 9; // display 9
		_delay_ms(50);
	}
	else if ((PINA & 0b00000001) == 0x00)
	{
		return 100; // display #
	}

	return 2000;
	_delay_ms(50);
}

int kepadFridge()
{
	PORTB &= ~(1 << 5);				// set column 1 to 0
	if ((PINB & 0b000010000) == 00) // check if row 1 is 1
	{
		PORTB = (1 << 0); // display 1
	}
	else if ((PINB & 0b00001000) == 00)
	{
		PORTB = (1 << 0); // display 4
	}
	else if ((PINB & 0b00000100) == 00)
	{
		PORTB = (1 << 0); // display 7
	}
	else if ((PINB & 0b00000010) == 00)
	{
		PORTB = (1 << 0); // display *
	}

	PORTB &= ~(1 << 6);				// set column 2 to 0
	if ((PINB & 0b000010000) == 00) // check if row 1 is 1
	{
		PORTB = (1 << 0); // display 2
	}
	else if ((PINB & 0b00001000) == 00)
	{
		PORTB = (1 << 0); // display 5
	}
	else if ((PINB & 0b00000100) == 00)
	{
		PORTB = (1 << 0); // display 8
	}
	else if ((PINB & 0b00000010) == 00)
	{
		PORTB = (1 << 0); // display 0
	}

	PORTB &= ~(1 << 7);				// set column 3 to 0
	if ((PINB & 0b000010000) == 00) // check if row 1 is 1
	{
		PORTB = (1 << 0); // display 3
	}
	else if ((PINB & 0b00001000) == 00)
	{
		PORTB = (1 << 0); // display 6
	}
	else if ((PINB & 0b00000100) == 00)
	{
		PORTB = (1 << 0); // display 9
	}
	else if ((PINB & 0b00000010) == 00)
	{
		PORTB = (1 << 0); // display #
	}
}

int main(void)
{

	// setting up the button and interrupt
	DDRB |= (1 << 0);  // setting 1 for output of the button when it does an interrupt
	DDRD &= ~(1 << 3); // setting 0 for input of the button
	DDRD &= ~(1 << 2); // setting 0 for input of the button
	// for 1st btn
	EICRA |= (1 << INT3); // Set for falling edge triggering
	EIMSK |= (1 << INT3); // Enable INT0
	// for 2nd btn
	EICRA |= (1 << INT2);
	EIMSK |= (1 << INT2);
	sei(); // enable external interrupts globally

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

		// keypadGate

		// keypadFridge

		// rotateFridge();
		rotateFridge();
	}
}

// function to write to the LCD
void lunch()
{
	PORTG |= (1 << 2); // set enable to 1
	_delay_ms(50);	   // wait 1ms
	PORTG &= ~(1 << 2);
	_delay_ms(50);
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

	commandMode();
	lunch();
	PORTH = 0x0f;
	lunch();
	// the loop to display the welcome message
	for (int i = 0; i < 18; i++)
	{
		dataMode();
		PORTH = message[i];
		lunch();
	}
	_delay_ms(500);
	clearScreen();
}

void lcd_data_print(char info[])
{
	int len = strlen(info);
	for (int a = 0; a < len; a++)
	{
		lcd_data(info[a]);
	}
}

void lcd_init()
{
	lcd_cmd(0x38); // all lines and a 5x7 matrix
	lcd_cmd(0x0c); // display on, cursor off
	lcd_cmd(0x06); // increment cursor
	lcd_cmd(0x01); // clear screen
}

void lcd_cmd(unsigned char cmd)
{
	PORTH = cmd;
	PORTG &= ~(1 << 0); // set rs to 0- command mode
	PORTG &= ~(1 << 1); // set rw to 0- write mode
	lunch();
}

void lcd_data(unsigned char data)
{
	PORTH = data;
	PORTG |= (1 << 0);	// set rs to 1- data mode
	PORTG &= ~(1 << 1); // set rw to 0- write mode
	lunch();
}
// function to do car registration
void registerCar()
{

	lcd_init();
	lcd_cmd(0x01); // clear screen
	lcd_cmd(0x80); // row 1
	lcd_data_print("REGISTRATION");
	lunch();
	// enter number plate
	lcd_cmd(0xC0); // row 2
	lcd_data_print("Number Plate: ");
	int plate = keypadGate();
	while (plate == 2000)
	{
		plate = keypadGate();
	}
	char plates[1];
	sprintf(plates, "%d", plate);
	lcd_cmd(0xCD);
	lcd_data_print(plates);
	lunch();
}
// function to do adult registration
void registerAdults()
{
	lcd_cmd(0x94); // row 3
	lcd_data_print("10 years below: ");
	int belowTen = keypadGate();
	while (belowTen == 200)
	{
		belowTen = keypadGate();
	}
	char belowTens[1];
	sprintf(belowTens, "%d", belowTen);
	lcd_cmd(0xA4);
	lcd_data_print(belowTens);
}
void registerChildren()
{
	lcd_cmd(0xD4); // row
	lcd_data_print("10 years above: ");
	int aboveTen = keypadGate();
	while (aboveTen == 200)
	{
		aboveTen = keypadGate();
	}
	char aboveTens[1];
	sprintf(aboveTens, "%d", aboveTens);
	lcd_cmd(0xE4);
	lcd_data_print(aboveTens);
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
	PORTB ^= (1 << 0); // activate the led on an interrupt
	_delay_ms(1000);
	displayWelcome(); // Call the function to display the welcome messagel
}

ISR(INT2_vect)
{
	registerCar();
	_delay_ms(1000);
	registerAdults();
	_delay_ms(1000);
	registerChildren();
}