/* 
* Usart328p.h
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


#pragma once
#include <stdint.h>
class Usart2560
{
public:
	Usart2560();
    // initializes USART0 with g_rx/g_tx enabled
    // and interrupts enabled on receive and
    // data register empty.
    void begin();
    // pulls a single byte from g_rx, if one is available. 
    // returns true if succesful. False if the recieve
    // buffer is empty
    bool read(uint8_t*);
    // writes a single byte
    bool write(uint8_t);
    // writes a buffer, successfully when return value
    // equals length param.
    uint8_t write(uint8_t *data, uint8_t length);
    uint8_t available();
}; 

extern Usart2560 usart;