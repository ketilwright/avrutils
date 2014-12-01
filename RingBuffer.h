/*
* RingBuffer.h
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
#pragma once#include <stddef.h>#include <stdint.h>#include <avr/interrupt.h>// Minimal ringbuffer class, using a model where there is// an empty slot to distinguish the full & empty condition.template<uint8_t size>class RingBuffer{    const uint8_t  m_size;    // keep a slot open to avoid overflow    uint8_t m_ring[size + 1];    volatile uint8_t m_get;    volatile uint8_t m_put;   public:    RingBuffer()         :         m_size(size + 1),         m_get(0),         m_put(0)    {        reset();    }    bool isFull() const    {        return ((m_put + 1) % m_size) == m_get;    }    bool isEmpty() const    {        return m_get == m_put;    }    uint8_t put(uint8_t e)    {        uint8_t rVal = 0;        if (!isFull())        {            m_ring[m_put++] = e;            m_put %= m_size;            if (m_put == m_get)            {                m_get = (m_get + 1) % m_size;            }            rVal = 1;        }        return rVal;    }    uint8_t get(volatile uint8_t *e)    {        uint8_t rVal = 0;        if (!isEmpty())        {            *e = m_ring[m_get++];            m_get %= m_size;            rVal = 1;        }        return rVal;    }     void reset()     {         for (size_t i = 0; i < m_size; i++)         {             m_ring[i] = 0;         }         m_get = m_put = 0;     }};