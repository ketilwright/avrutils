Debug
-------------------------------
Files:
	debug.cpp
	debug.h
Remarks:	
	printf style filtered debug output using usart328p or usart2560. Change the value of g_dbgFilter to reflect which
	debug messages are displayed.

TWI
-------------------------------
Files:
	i2c.c
	i2c.h
Remarks:
	Very much a work in progess, particularly the interrupt driven class. 

IO
-------------------------------
Files:
	IoDigitalPort328p.cpp
	IoDigitalPort328p.h
Remarks:
	Uses ASF to configure pins, setup interrupt functions and provide a callback mechanism to clients.
	
Ringbuffer	
Files:
-------------------------------
RingBuffer.h
Remarks:
	General purpose ring buffer object, used by usart & i2c classes
	
Timer
Files:
-------------------------------
	Timer2560.cpp
	Timer2560.h
	Timer328p.cpp
	Timer328p.h

Remarks:
	Interrupt driven timer, with callback mechanism.

USART	
-------------------------------
	Usart2560.cpp
	Usart2560.h
	Usart328p.cpp
	Usart328p.h
Remarks:
	Interrupt driven usart for data IO or tx/rx pins. Hard coded to a faster baud rate than arduino libs.