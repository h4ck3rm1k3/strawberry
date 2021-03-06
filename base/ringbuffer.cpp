/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * aRingBuffer::SizeType with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "i2-base.h"

using namespace icinga;

RingBuffer::RingBuffer(RingBuffer::SizeType slots)
	: m_Slots(slots, 0), m_Offset(0)
{ }

RingBuffer::SizeType RingBuffer::GetLength(void) const
{
	return m_Slots.size();
}

void RingBuffer::InsertValue(RingBuffer::SizeType tv, int num)
{
	vector<int>::size_type offsetTarget = tv % m_Slots.size();

	/* walk towards the target offset, resetting slots to 0 */
	while (m_Offset != offsetTarget) {
		m_Offset++;

		if (m_Offset >= m_Slots.size())
			m_Offset = 0;

		m_Slots[m_Offset] = 0;
	}

	m_Slots[m_Offset] += num;
}

int RingBuffer::GetValues(RingBuffer::SizeType span) const
{
	if (span > m_Slots.size())
		span = m_Slots.size();

	int off = m_Offset;
	int sum = 0;
	while (span > 0) {
		sum += m_Slots[off];

		if (off == 0)
			off = m_Slots.size();

		off--;
		span--;
	}

	return sum;
}
