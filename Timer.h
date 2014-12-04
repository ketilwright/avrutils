/* 
* Timer.h
*
* Created: 12/04/2014 1:42 PM
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
    #include "Timer328p.h"
    typedef Timer328p TimerImpl;
#elif defined __AVR_ATmega2560__
    #include "Timer2560.h"
    typedef Timer2560 TimerImpl;
#endif

template<typename McuImpl>
class Timer
{
    McuImpl m_impl;
public:
    typedef void (*timerCallback)(void*);
	Timer(){}
    void init(void *ctx, timerCallback callback) { m_impl.init(ctx, callback);}
    uint32_t millis() const { return m_impl.millis(); }
    void reset() { m_impl.reset(); }
    void isr() { m_impl.isr(); }        
}; 

extern Timer<TimerImpl> timer;