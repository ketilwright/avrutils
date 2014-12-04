/*
* Timer.cpp
*
* Created: 12/04/2014 1:49:00 PM
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


#include "Timer.h"
#include <stdint.h>
#if defined __AVR_ATmega328P__
    #include "Timer328p.cpp"
#elif defined __AVR_ATmega2560__
    #include "Timer2560.cpp"
#endif

Timer<TimerImpl> timer;


