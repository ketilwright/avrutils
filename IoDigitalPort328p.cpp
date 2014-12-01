/* 
* IoPort328p.cpp
*
* Created: 11/30/2014 11:02:38 AM
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



#include "IoDigitalPort328p.h"
#include <debug.h>

/*
 *   328p Data sheet Chapter 12. External Interrupts
 *   
 *   The External Interrupts are triggered by the INT0 and INT1 pins or any of the PCINT23..0 pins.
 *   Observe that, if enabled, the interrupts will trigger even if the INT0 and INT1 or PCINT23..0 pins
 *   are configured as outputs. This feature provides a way of generating a software interrupt. 
 *   
 *   The pin change interrupt PCI2 will trigger if any enabled PCINT23..16 pin toggles. 
 *   The pin change interrupt PCI1 will trigger if any enabled PCINT14..8 pin toggles. 
 *   The pin change interrupt PCI0 will trigger if any enabled PCINT7..0 pin toggles. 
 *   
 *   The PCMSK2, PCMSK1 and PCMSK0 Registers control which pins contribute to the pin change interrupts.
 *   
 *   Register  7        6      5       4        3      2        1      0
 *   ------------------------------------------------------------------------
 *   PCMSK2:       
 *   
 *           PCINT23 PCINT22 PCINT21 PCINT20 PCINT19 PCINT18 PCINT17 PCINT16
 *            (PD7)   (PD6)   (PD5)   (PD4)   (PD3)   (PD2)   (PD1)   (PD0)
 *            
 *   328pPin    13      12      11       6       5       4       3       2          
 *   ------------------------------------------------------------------------             
 *   PCMSK1:
 *                   PCINT14 PCINT13 PCINT12 PCINT11 PCINT10 PCINT9  PCINT8
 *                    (PC6)   (PC5)   (PC4)   (PC3)   (PC2)   (PC1)   (PC0)
 *   328pPin             1      28      27      26      25      24      23
 *   ------------------------------------------------------------------------                
 *   PCMSK0:
 *           PCINT7  PCINT6  PCINT5  PCINT4  PCINT3  PCINT2  PCINT1  PCINT0
 *            (PB7)   (PC6)   (PC5)   (PB4)   (PB3)   (PB2)   (PB1)   (PB0)
 *   328pPin    10       9      19      18      17      16      15      14         
 *   
*/


static const IoPortInfo pinIndexToIoPort[IoPort328p::nPins] =
{
    // pin 1
    { IOPORT_CREATE_PIN(PORTC, PORTC6), PCINT14, PCIE1, PCMSK1 },
    // pin 2
    { IOPORT_CREATE_PIN(PORTD, PORTD0), PCINT16, PCIE2, PCMSK2 },
    // pin 3
    { IOPORT_CREATE_PIN(PORTD, PORTD1), PCINT17, PCIE2, PCMSK2 },
    // pin 4
    { IOPORT_CREATE_PIN(PORTD, PORTD2), PCINT18, PCIE2, PCMSK2 },
    // pin 5
    { IOPORT_CREATE_PIN(PORTD, PORTD3), PCINT19, PCIE2, PCMSK2 },
    // pin 6
    { IOPORT_CREATE_PIN(PORTD, PORTD4), PCINT20, PCIE2, PCMSK2 },
    // pin 9
    { IOPORT_CREATE_PIN(PORTB, PORTB6), PCINT6,  PCIE0, PCMSK0 },
    // pin 10
    { IOPORT_CREATE_PIN(PORTB, PORTB7), PCINT7,  PCIE0, PCMSK0 },
    // pin 11
    { IOPORT_CREATE_PIN(PORTD, PORTD5), PCINT21, PCIE2, PCMSK2 },
    // pin 12
    { IOPORT_CREATE_PIN(PORTD, PORTD6), PCINT22, PCIE2, PCMSK2 },
    // pin 13
    { IOPORT_CREATE_PIN(PORTD, PORTD7), PCINT23, PCIE2, PCMSK2 },
    // pin 14
    { IOPORT_CREATE_PIN(PORTB, PORTB0), PCINT0,  PCIE0, PCMSK0 },
    // pin 15
    { IOPORT_CREATE_PIN(PORTB, PORTB1), PCINT1,  PCIE0, PCMSK0 },
    // pin 16
    { IOPORT_CREATE_PIN(PORTB, PORTB2), PCINT2,  PCIE0, PCMSK0 },
    // pin 17
    { IOPORT_CREATE_PIN(PORTB, PORTB3), PCINT3,  PCIE0, PCMSK0 },
    // pin 18
    { IOPORT_CREATE_PIN(PORTB, PORTB4), PCINT4,  PCIE0, PCMSK0 },
    // pin 19
    { IOPORT_CREATE_PIN(PORTB, PORTB5), PCINT5,  PCIE0, PCMSK0 },
};



IoPort328p::IoPort328p()
    :
    m_callback(0),
    m_context(0)
{
} 


IoPort328p::~IoPort328p()
{
}
void IoPort328p::init(ioportCallback callback, void* ctx)
{
    m_callback = callback;
    m_context = ctx;
}
void IoPort328p::callback()
{
    if(m_callback) m_callback(m_context);
}
void IoPort328p::configurePin(ePin pin, ioport_direction dir, bool interruptEnable)
{
    gpio_configure_pin(pinIndexToIoPort[pin].ioport, dir);
    enableInterrupt(pin, interruptEnable);
}

void IoPort328p::enableInterrupt(ePin pin, bool interruptEnable)
{
    // TODO: not handling external INT0 & INT1 (pins 4 & 5)
    const IoPortInfo & pi = pinIndexToIoPort[pin];
    // set or clear interrupt mask bit
    switch(pin)
    {
        case pin13: case pin12 : case pin11 : case pin6 :
        case pin5:  case pin4:   case pin3 :  case pin2:
        {
            if(interruptEnable)
                PCMSK2 |= _BV(pi.pcint);
            else
                PCMSK2 &= ~_BV(pi.pcint);                
            break;
        }
        case pin1:
        {
            if(interruptEnable)
                PCMSK1 |= _BV(pi.pcint);
            else
                PCMSK1 &= ~_BV(pi.pcint);
            break;
        }
        case pin10: case pin9:  case pin19: case pin18:
        case pin17: case pin16: case pin15: case pin14:
        {
            if(interruptEnable)
                PCMSK0 |= _BV(pi.pcint);
            else
                PCMSK0 &= ~_BV(pi.pcint);
            break;
        }
        default:
        {
            break;
        }
    }
    // enable all PCIEn for now. It would probably be better to track
    // which buttons are used as inputs with interrupts enabled
    // and mask this register accordingly.
    PCICR |= _BV(PCIE0) | _BV(PCIE1) | _BV(PCIE2);
}

bool IoPort328p::pinIsHigh(ePin pin)
{
    return gpio_pin_is_high(pinIndexToIoPort[pin].ioport);
}
void IoPort328p::setPin(ePin pin, bool high)
{
    uint8_t ioport = pinIndexToIoPort[pin].ioport;
    if(high)
    {
        gpio_set_pin_high(ioport);                
    }
    else
    {
        gpio_set_pin_low(ioport);                
    }
}

IoPort328p ioport;


ISR(PCINT0_vect)
{
    ioport.callback();
}

ISR(PCINT1_vect)
{
    ioport.callback();
}

ISR(PCINT2_vect)
{
    ioport.callback();
}
