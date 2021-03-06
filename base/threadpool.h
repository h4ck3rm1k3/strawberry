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

#ifndef THREADPOOL_H
#define THREADPOOL_H

namespace icinga
{

struct ThreadPoolTask
{
	typedef shared_ptr<ThreadPoolTask> Ptr;
	typedef weak_ptr<ThreadPoolTask> WeakPtr;

	virtual void Execute(void) = 0;
};

class I2_BASE_API ThreadPool : public Object
{
public:
	typedef shared_ptr<ThreadPool> Ptr;
	typedef weak_ptr<ThreadPool> WeakPtr;

	ThreadPool(long maxThreads = 128);
	~ThreadPool(void);

	static ThreadPool::Ptr GetDefaultPool(void);

	void EnqueueTasks(list<ThreadPoolTask::Ptr>& tasks);
	void EnqueueTask(const ThreadPoolTask::Ptr& task);
	void WaitForTasks(void);

private:
	mutable boost::mutex m_Lock;
	condition_variable m_CV;

	list<ThreadPoolTask::Ptr> m_Tasks;

	thread_group m_Threads;
	bool m_Alive;

	ThreadPoolTask::Ptr DequeueTask(void);
	void WorkerThreadProc(void);
};

}

#endif /* THREADPOOL_H */
