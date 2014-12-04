/* 
* Usart.h
*
* Created: 12/04/2014 1:42:00 PM
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


#pragma once
#include <stdint.h>
#if defined __AVR_ATmega328P__
    #include "Usart328p.h"
    typedef Usart328p UsartImpl;
#elif defined __AVR_ATmega2560__
    #include "Usart2560.h"
    typedef Usart2560 UsartImpl;
#endif

template<typename McuImpl>
class Usart
{
    McuImpl m_impl;
public:
	Usart(){}
    // initialize with g_rx/g_tx enabled
    // and interrupts enabled on receive and
    // data register empty.
    void begin() { m_impl.begin(); }
    // pulls a single byte from g_rx, if one is available. 
    // returns true if successful. False if the receive
    // buffer is empty
    bool read(uint8_t *data) { return m_impl.read(data); }
    // writes a single byte
    bool write(uint8_t data) { return m_impl.write(data); }
    // writes a buffer, successfully when return value
    // equals length param.
    uint8_t write(uint8_t *data, uint8_t length) {return m_impl.write(data, length);}
    uint8_t available() { return m_impl.available(); }
}; 

extern Usart<UsartImpl> usart;
