/* 
* Usart328p.cpp
*
* Created: 7/10/2014 2:19:39 PM
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

#include "Usart2560.h"
#include "RingBuffer.h"
#include <util/delay.h>
#include <util/atomic.h>
#include <stdint.h>
//
// Set desired ring buffer sizes here.
//
static RingBuffer<127> g_tx;
static RingBuffer<127> g_rx;
//
// Set desired baud rate.
// TODO: make this a parameter of the ctor
// 
#define BAUDRATE 1000000

static const uint16_t ubrr =  ((F_CPU / (BAUDRATE * 16UL))) - 1;

Usart2560::Usart2560()
{
    g_tx.reset();
    g_rx.reset();
} 
//
// initializes USART0 with rx/tx enabled
// and interrupts enabled on receive and
// data register empty.
// 
void Usart2560::begin()
{
    //
    // USART power reduction off.
    //
    PRR0 &= ~(1 << PRUSART0);
    // set baud rate
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    //
    // enable receiver and transmitter
    //
    UCSR0B |=
        (1 << TXEN0)    | // enable transmit
        (1 << RXEN0)    | // enable receive
        (1 << RXCIE0)   | // interrupt on receive
        (1 << UDRIE0);    // interrupt on data register empty
    //
    // frame format 8 data bits, 1 stop bit
    // 
    UCSR0C =
        (0 << UMSEL01)	|
        (0 << UMSEL00)	| // async usart
        (0 << UPM01)	|
        (0 << UPM00)	| // disable parity generation and checking
        (0 << USBS0)	| // one stop bit
        (1 << UCSZ01)	|
        (1 << UCSZ00)	| // 8 bit char size
        (0 << UCPOL0);	  // when UCPOLn is zero the data will be changed at
                          // rising XCKn edge and sampled at falling XCKn edge
    
    //
    // Enable global interrupts. 
    // Perhaps this should be handled by the caller
    // afterwards.
    //                          
    sei();                          
}
//
// Pull a single byte from the g_rx ring buffer.
// returns true if a byte was available.
//
bool Usart2560::read(uint8_t *data)
{
    bool retVal = false;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        retVal = g_rx.get(data) != 0;    
        // re-enable USART_RX_vect
        UCSR0B |= _BV(RXCIE0);
    }
    return retVal;
}
//
// Pushes a single byte to g_tx ring buffer.
// If space is available, enables the receive
// buffer empty interrupt. When USART_UDRE_vect
// fires, the byte will be written to the output
// data buffer (UDR0).
//
bool Usart2560::write(uint8_t data)
{
    bool retVal= false;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        if(g_tx.put(data) != 0)
        {
            UCSR0B |= _BV(UDRIE0);
            retVal = true;
        }
    }
    return retVal;
}

//
// Pushes a buffer of bytes to the g_tx ring buffer.
// If there is insufficient space in g_tx, an incomplete
// transaction will occur.
// Enables the receive buffer empty interrupt. When USART_UDRE_vect
// fires, the byte will be written to the output
// data buffer (UDR0).
//

uint8_t Usart2560::write(uint8_t *data, uint8_t length)
{
   uint8_t bytesWritten = 0;
   ATOMIC_BLOCK(ATOMIC_FORCEON)
   {
       uint8_t *p = data;
       while(bytesWritten != length)
       {
           if(!g_tx.put(*p)) break;
           ++bytesWritten;
           p++;
       }
       UCSR0B |= _BV(UDRIE0);    
   }
   return bytesWritten;
}

//
// UART data register empty interrupt handler
// Pulls a byte from g_tx. When g_tx is not empty,
// the byte is placed in the transmit buffer.
// If this fires with g_tx empty, masks the receive 
// buffer empty interrupt, so that we don't fire repeatedly
// when there is nothing to send.
// 
ISR(USART0_UDRE_vect, ISR_BLOCK)
{
    uint8_t val;
    if (g_tx.get(&val) != 0)
    {
        UDR0 = val;
    }
    else
    {
        // transmit buffer is empty, mask the interrupt.
        UCSR0B &= ~_BV(UDRIE0);
    }
}
// UART receive data interrupt handler
ISR(USART0_RX_vect, ISR_BLOCK)
{
    uint8_t inData = UDR0;
    g_rx.put(inData);
    // clear interrupting condition
    UCSR0B &= ~_BV(RXCIE0);
}

//Usart2560 usart;