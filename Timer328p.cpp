/* 
* Timer328p.cpp
*
* Created: 7/14/2014 4:12:26 PM
* Author: Ketil Wright
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

#include "Timer328p.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/*
 *  AVR timer notes 
 *
 * 
 * Timer0 
 *      8 bits
 *      2 independent output compare registers (OCRnn
 *      PRR.PRTIM0 must be written to zero to enable
 *
 * Timer1 
 *      16 bits
 *      PRR.PRTIM1 must be cleared to disable power save mode
 *      "Special procedures" required to access 16 bit registers. See p 115
 *      TCNT1 Timer counter (16 bit) 
 *          TCNT1H upper 8 bits
 *          TCNT1L lower 8 bits
 *      OCR1A Output Compare register (16 bit)
 *      OCR1B Output Compare register (16 bit)
 *          continuously compares TCNT1 with OCR1x
 *          when equal, signals a match by setting OCF1x at the next timer clock cycle
 *          IF OXIEx = 1, generates output compare interrupt.
 *          OCF1x is auto cleared when interrupt is executed.
 *          Software may chose to manually clear OCF1x (by writing 1).
 *          Waveform generator uses match signal to generate an output set by WGM13:0
 *          and COM1x1:0. see page 125
 *      ICR1  Input capture register (16 bit)
 *          Can capture the timer/counter value at a given external edge
 *          triggered event on ICP1 (input capture pin, PB0)
 *      No access restrictions on 8 bit registers
 *      TCCR1A Timer counter control register (8 bit) no cpu access restriction
 *      TCCR1B Timer counter control register (8 bit) no cpu access restriction
 *              ClockSelect CS12:0 see page 141
 *              Waveform generation mode bits WGM13:0 (unclear if this is in a/b counter control register).
 *                  WGN13:0 = 4 Clear timer on compare (CTC)
 *                      OCR1A or ICR1 manipulate counter resolution
 *                      counter cleared to 0 when TCNT1 matches either OCR1A or
 *              TODO: p 125 addresses this?
 *              ICNC1 input capture noise canceler. Not affected by prescaler. Uses system clock directly.
 *              COM1x1:0 (2 function)
 *                  1) define output compare state at the next compare match
 *                  2) control output compare pin output source
 *      TIFR1 Timer interrupt flag register
 *      TIMSK Timer interrupt mask register
 *
 */


Timer328p::Timer328p(){} 
void Timer328p::init(void *ctx, timerCallback callback)
{
     // assume the caller invokes sei()
     m_cb = callback;
     m_ctx = ctx;
     // Make sure timer is powered up. 
     // 2560 -> page 133
     // 328p -> page 113
     PRR &=  ~(1 << PRTIM1);
     
     // Use Waveform Generation Mode Bit Description
     // to setup CTC (Clear timer on compare match)
     // of OCR1A. 
     TCCR1A &= ~( _BV(WGM10) | _BV(WGM11));
     TCCR1B |= _BV(WGM12);
     TCCR1B &= ~(_BV(WGM13));
     
     
     // Target Timer Count = (((Input Frequency / Prescaler) / Target Frequency) - 1)
     
     const uint16_t preScaler = 64;
     
     
     // 16000000 / 64 = 250000
     // so 1 millsecond 
     OCR1A = 249;
     
     
     // CSn2    CSn1    CSn0
     // 0       0       0       No clock source (Timer/Counter stopped).
     // 0       0       1       clkI/O/1 (No prescaling)
     // 0       1       0       clkI/O/8 (From prescaler)
     // 0       1       1       clkI/O/64 (From prescaler)
     // 1       0       0       clkI/O/256 (From prescaler)
     // 1       0       1       clkI/O/1024 (From prescaler)
     // 1       1       0       External clock source on T1 pin. Clock on falling edge.
     // 1       1       1       External clock source on T1 pin. Clock on rising edge.
     
     switch(preScaler)
     {
         case 1: 
         {
             TCCR1B |= _BV(CS10);
             break;
         }
         case 8:
         {
             TCCR1B |= _BV(CS11);
             break;
         }
         case 64:
         {
             TCCR1B |= _BV(CS10) | _BV(CS11);
             break;
         }
         case 256:
         {
             TCCR1B |= _BV(CS12);
             break;
         }
         case 1024:
         {
             TCCR1B |= _BV(CS12) | _BV(CS10);
             break;
         }
     }
     
     m_millis = 0;
     // configure the interrupt to fire
     // on compare of output compare register 1A
     TIMSK1 |= _BV(OCIE1A);
     
     
     
} 

uint32_t Timer328p::millis() const
{
    cli();
    uint32_t m = m_millis;
    sei();
    return m;
    
}

void Timer328p::reset()
{
    m_millis = 0;
}

void Timer328p::isr()
{
    ++m_millis;
    if(m_cb) m_cb(m_ctx);
}

// Timer328p timer;
#include "Timer.h"

// Timer1 compare A interrupt handler
ISR(TIMER1_COMPA_vect) //.., ISR_BLOCK)
{
    timer.isr();
    //++timer.m_millis;
    //if(timer.m_cb) timer.m_cb(timer.m_ctx);
}
