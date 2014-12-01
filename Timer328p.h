/* 
* Timer328p.h
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

#pragma once
#include <stdint.h>

class Timer328p
{
    
public:
    volatile uint32_t m_millis;
    typedef void (*timerCallback)(void*);
    timerCallback m_cb;
    void *m_ctx;

	Timer328p();
    void init(void *ctx, timerCallback callback);
    uint32_t millis() const;
    void reset();
}; 
extern Timer328p timer;