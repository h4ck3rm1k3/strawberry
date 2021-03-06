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

#ifndef VIRTUALENDPOINT_H
#define VIRTUALENDPOINT_H

namespace icinga
{

/**
 * A local endpoint.
 *
 * @ingroup icinga
 */
class I2_ICINGA_API VirtualEndpoint : public Endpoint
{
public:
	typedef shared_ptr<VirtualEndpoint> Ptr;
	typedef weak_ptr<VirtualEndpoint> WeakPtr;

	void RegisterTopicHandler(String topic, function<void (const VirtualEndpoint::Ptr&, const Endpoint::Ptr, const RequestMessage&)> callback);
	void UnregisterTopicHandler(String topic, function<void (const VirtualEndpoint::Ptr&, const Endpoint::Ptr, const RequestMessage&)> callback);

	virtual String GetIdentity(void) const;
	virtual String GetAddress(void) const;

	virtual bool IsLocal(void) const;
	virtual bool IsConnected(void) const;

	virtual void ProcessRequest(Endpoint::Ptr sender, const RequestMessage& message);
	virtual void ProcessResponse(Endpoint::Ptr sender, const ResponseMessage& message);

	virtual void Stop(void);

private:
	map< String, shared_ptr<boost::signal<void (const VirtualEndpoint::Ptr&, const Endpoint::Ptr, const RequestMessage&)> > > m_TopicHandlers;
};

}

#endif /* VIRTUALENDPOINT_H */
