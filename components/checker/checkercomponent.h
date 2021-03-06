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

#ifndef CHECKERCOMPONENT_H
#define CHECKERCOMPONENT_H

namespace icinga
{

struct ServiceNextCheckExtractor
{
	typedef double result_type;

	double operator()(const Service::Ptr& service)
	{
		return service->GetNextCheck();
	}
};

/**
 * @ingroup checker
 */
class CheckerComponent : public IComponent
{
public:
	typedef shared_ptr<CheckerComponent> Ptr;
	typedef weak_ptr<CheckerComponent> WeakPtr;

	typedef multi_index_container<
		Service::Ptr,
		indexed_by<
			ordered_unique<identity<Service::Ptr> >,
			ordered_non_unique<ServiceNextCheckExtractor>
		>
	> ServiceSet;

	virtual void Start(void);
	virtual void Stop(void);

private:
	VirtualEndpoint::Ptr m_Endpoint;

	ServiceSet m_IdleServices;
	ServiceSet m_PendingServices;

	Timer::Ptr m_CheckTimer;

	Timer::Ptr m_ResultTimer;

	void CheckTimerHandler(void);
	void ResultTimerHandler(void);

	void CheckCompletedHandler(const Service::Ptr& service, const ScriptTask::Ptr& task);

	void AdjustCheckTimer(void);

	void CheckerChangedHandler(const Service::Ptr& service);
	void ServiceRemovedHandler(const DynamicObject::Ptr& object);

	//void AssignServiceRequestHandler(const Endpoint::Ptr& sender, const RequestMessage& request);
	//void ClearServicesRequestHandler(const Endpoint::Ptr& sender, const RequestMessage& request);
};

}

#endif /* CHECKERCOMPONENT_H */
