#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "Timer.h"

#define IR			(1<<0)
#define trigger		(1<<1)

//Обявляєм функції
void TransmitCmnd (uint16_t);
void FreqOn  (void);

uint16_t command = 0b00000000111111;
uint8_t buttonFlag = 0;

int main(void){

	DDRB &= ~(trigger);
	PORTB |= (trigger);

	DDRB |= (IR);
//	PORTB |= IR;
	PORTB &= ~(IR);

	FreqOff();

while(1){

	if (!(PINB&(trigger)) && !buttonFlag){
		buttonFlag = 1;
		TransmitCmnd(command);
	}
	else if (PINB&(trigger) && buttonFlag)  {
		buttonFlag = 0;
		_delay_ms (50);
	}
	}
}

void TransmitCmnd (uint16_t command){
	//стартовый импульс
	FreqOn ();
	_delay_us (24000);

	FreqOff ();
	PORTB &= ~IR;
	_delay_us (6000);

	//передача команды
	for (char i=15; i>1; i--){
      if (command & (1<<i)) {
  		FreqOn ();
  		_delay_us (12000);
      }
      else {
  		FreqOn ();
  		_delay_us (6000);
      }
      	FreqOff();
      	PORTB &= ~IR;
      	_delay_us (6000);
	}




}
