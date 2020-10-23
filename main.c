#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "Timer.h"

#define IR			(1<<0)
#define trigger		(1<<1)
#define UART		(1<<4)

#define UART_DELAY 104


//Обявляєм функції
void TransmitCmnd (uint16_t);
void FreqOn  (void);
void program_uart_send(uint8_t *data);
void player_play(uint8_t track_number);
void player_pause(void);
void player_volume(uint8_t volume_level);



uint16_t command = 0b00000000111111;
uint8_t buttonFlag = 0;

int main(void){

	DDRB &= ~(trigger);
	PORTB |= (trigger);

	DDRB |= (IR);
	PORTB &= ~(IR);

	DDRB |= (UART);
	PORTB |= (UART);

	FreqOff();

	_delay_ms (50);
	player_volume(29);
	_delay_ms (50);

while(1){

	if (!(PINB&(trigger))){
		buttonFlag = 1;
		TransmitCmnd(command);
		if (!buttonFlag){
			player_play(1);
		}
	}
	else if (PINB&(trigger) && buttonFlag)  {
		player_play(2);
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

void program_uart_send(uint8_t *data) {
	for (int byte_counter = 0; byte_counter < 8; byte_counter++) {
		PORTB &= ~(UART);
		_delay_us(UART_DELAY);
		for (int i = 0; i < 8; i++) {
			if (data[byte_counter]&(1<<i)) PORTB |= (UART);
			else PORTB &= ~(UART);
			_delay_us(UART_DELAY);
		}
		 PORTB |= (UART);
		_delay_us(UART_DELAY);
	}

}

void player_play(uint8_t track_number) {
	uint8_t command[8] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, track_number, 0xEF};
	program_uart_send(command);
}

void player_pause(void) {
	uint8_t command[8] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
	program_uart_send(command);
}

void player_volume(uint8_t volume_level) {
	uint8_t command[8] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, volume_level, 0xEF};
	program_uart_send(command);
}
