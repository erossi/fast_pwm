/* Copyright (C) 2010 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>

/* from 50 to 950 */
uint16_t duty_cycle;

/* IRQ routine at 500hz this is called 500 times x second */
ISR(TIMER1_OVF_vect)
{
	/* if the duty is at 95% the restart from 5% */
	if (duty_cycle > 950)
		duty_cycle = 50;

	/* increment the duty cycle by 1 step at the time wich is
	   0.1%, in this way I will complete 100% in 2 seconds (1000 step) */
	duty_cycle++;

	/* set the level where the output pin OC1A will toggle */
	OCR1A = duty_cycle;

	/* set the level where the output pin OC1B will toggle
	   this will be the inverse of OC1A level */
	OCR1B = 1000 - duty_cycle;
}

void counter_setup(void)
{
	/* Pin OC1A & B set to output */
	DDRD |= _BV(PD4) | _BV(PD5);

	/* Clear OC1A & B on compare match, set it at BOTTOM.
	   Waveform generation mode on 14, see datasheet */
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
	TCCR1B = _BV(WGM13) | _BV(WGM12);

	/* TOP value*/
	ICR1 = 1000;

	/* start with a duty of 50% */
	OCR1A = 500;
	OCR1B = 500;

	/* enable interrupt on timer overflow */
	TIMSK = _BV(TOIE1);
}

int main(void)
{
	/* prepare everything */
	counter_setup();

	/* start with a duty cycle of 50% */
	duty_cycle = 500;

	/* enable interrupt */
	sei();

	/* start the counter with prescaler = 8 */
	TCCR1B |= _BV(CS11);

	/* Infinite loop doing nothing, everything is
	 handled by the IRQ routine called 500 times x second. */
	for (;;);

	/* just for correct looking code, disable irq and terminate */
	cli();
	return(0);
}
