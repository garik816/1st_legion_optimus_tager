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

	DDRB&=~(trigger);
	PORTB|=(trigger);

	DDRB |= (IR);
	PORTB |= IR;

	FreqOff();

while(1){
	if (~PINB&(trigger) && !buttonFlag && 0){
		buttonFlag = 1;
		TransmitCmnd(command);
	}
	else if (!(~PINB&(trigger)) && buttonFlag)  {
		buttonFlag = 0;
	}
}
}

void TransmitCmnd (uint16_t command){
	//стартовый импульс
	FreqOn ();
	_delay_us (2400000);

	FreqOff ();
	PORTB &= ~IR;
	_delay_us (600000);

	//передача команды
	for (char i=0; i<14; i++){
      if (command & (1<<i)) {
  		FreqOn ();
  		_delay_us (120000);
      }
      else {
  		FreqOn ();
  		_delay_us (60000);
      }
      	FreqOff();
      	PORTB &= ~IR;
      	_delay_us (600);
	}




}
