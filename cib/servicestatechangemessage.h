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

#ifndef SERVICESTATECHANGEMESSAGE_H
#define SERVICESTATECHANGEMESSAGE_H

namespace icinga
{

class I2_CIB_API ServiceStateChangeMessage : public MessagePart
{
public:
	ServiceStateChangeMessage(void) : MessagePart() { }
	ServiceStateChangeMessage(const MessagePart& message) : MessagePart(message) { }

	bool GetService(String *service) const;
	void SetService(const String& service);
};

}

#endif /* SERVICESTATECHANGEMESSAGE_H */
