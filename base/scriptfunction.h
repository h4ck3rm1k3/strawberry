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

#ifndef SCRIPTFUNCTION_H
#define SCRIPTFUNCTION_H

namespace icinga
{

class ScriptTask;

class I2_BASE_API ScriptFunction : public Object
{
public:
	typedef shared_ptr<ScriptFunction> Ptr;
	typedef weak_ptr<ScriptFunction> WeakPtr;

	typedef function<void (const shared_ptr<ScriptTask>&, const vector<Value>& arguments)> Callback;

	ScriptFunction(const Callback& function);

	static void Register(const String& name, const ScriptFunction::Ptr& function);
	static void Unregister(const String& name);
	static ScriptFunction::Ptr GetByName(const String& name);

	void Invoke(const shared_ptr<ScriptTask>& task, const vector<Value>& arguments);

private:
	Callback m_Callback;

	static map<String, ScriptFunction::Ptr> m_Functions;
};

}

#endif /* SCRIPTFUNCTION_H */

