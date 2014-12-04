/*
* debug.cpp. Filtered debug output to a usart
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

#if defined NDEBUG
#else
	#include "debug.h"
    #include "Usart.h"
	#include <stdarg.h>
	#include <stdio.h>
	#include <string.h>

	uint32_t g_dbgFilter = dbg_error | dbg_warn | dbg_info;
	void _DbgPrint(const char* fmt, ...)
	{
		static char buf[1024];
		va_list args;
		va_start(args, fmt);
		uint16_t len = vsprintf(buf, fmt, args);
		va_end(args);
		usart.write(reinterpret_cast<uint8_t*>(buf), len);
	}
#endif