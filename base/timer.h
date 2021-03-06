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

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

namespace icinga {

/**
 * A timer that periodically triggers an event.
 *
 * @ingroup base
 */
class I2_BASE_API Timer : public Object
{
public:
	typedef shared_ptr<Timer> Ptr;
	typedef weak_ptr<Timer> WeakPtr;

	typedef list<Timer::WeakPtr> CollectionType;

	Timer(void);

	void SetInterval(double interval);
	double GetInterval(void) const;

	static double ProcessTimers(void);

	void Start(void);
	void Stop(void);

	void Reschedule(double next);

	boost::signal<void(const Timer::Ptr&)> OnTimerExpired;

private:
	double m_Interval; /**< The interval of the timer. */
	double m_Next; /**< When the next event should happen. */

	static Timer::CollectionType m_Timers;

	void Call(void);
};

}

#endif /* TIMER_H */
