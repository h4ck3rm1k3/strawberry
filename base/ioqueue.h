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
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef IOQUEUE_H
#define IOQUEUE_H

namespace icinga
{

/**
 * An I/O queue.
 */
class IOQueue
{
public:
	/**
	 * Retrieves the number of bytes available for reading.
	 *
	 * @returns The number of available bytes.
	 */
	virtual size_t GetAvailableBytes(void) const = 0;

	/**
	 * Reads data from the queue without advancing the read pointer. Trying
	 * to read more data than is available in the queue is a programming error.
	 * Use GetBytesAvailable() to check how much data is available.
	 *
	 * @buffer The buffer where data should be stored. May be NULL if you're
	 *		 not actually interested in the data.
	 * @param count The number of bytes to read from the queue.
	 */
	virtual void Peek(void *buffer, size_t count) = 0;

	/**
	 * Reads data from the queue. Trying to read more data than is
	 * available in the queue is a programming error. Use GetBytesAvailable()
	 * to check how much data is available.
	 *
	 * @param buffer The buffer where data should be stored. May be NULL if you're
	 *		 not actually interested in the data.
	 * @param count The number of bytes to read from the queue.
	 */
	virtual void Read(void *buffer, size_t count) = 0;

	/**
	 * Writes data to the queue.
	 *
	 * @param buffer The data that is to be written.
	 * @param count The number of bytes to write.
	 * @returns The number of bytes written
	 */
	virtual void Write(const void *buffer, size_t count) = 0;
};

}

#endif /* IOQUEUE_H */
