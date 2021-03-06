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

#include "i2-dyn.h"

using namespace icinga;

Expression::Expression(const String& key, ExpressionOperator op, const Value& value, const DebugInfo& debuginfo)
	: m_Key(key), m_Operator(op), m_Value(value), m_DebugInfo(debuginfo)
{
}

void Expression::Execute(const Dictionary::Ptr& dictionary) const
{
	Value oldValue, newValue;

	ExpressionList::Ptr valueExprl;
	Dictionary::Ptr valueDict;
	if (m_Value.IsObjectType<ExpressionList>())
		valueExprl = m_Value;

	if (m_Value.IsObjectType<Dictionary>())
		valueDict = m_Value;

	newValue = m_Value;

	Dictionary::Ptr dict;

	switch (m_Operator) {
		case OperatorExecute:
			if (!valueExprl)
				throw_exception(invalid_argument("Operand for OperatorExecute must be an ExpressionList."));

			valueExprl->Execute(dictionary);

			return;

		case OperatorSet:
			if (valueExprl) {
				dict = boost::make_shared<Dictionary>();
				valueExprl->Execute(dict);
				newValue = dict;
			}

			break;

		case OperatorPlus:
			oldValue = dictionary->Get(m_Key);

			if (oldValue.IsObjectType<Dictionary>())
				dict = oldValue;

			if (!dict) {
				if (!oldValue.IsEmpty()) {
					stringstream message;
					message << "Wrong argument types for += (non-dictionary and dictionary) (" << m_DebugInfo << ")";
					throw_exception(domain_error(message.str()));
				}

				dict = boost::make_shared<Dictionary>();
			}

			newValue = dict;

			if (valueExprl) {
				valueExprl->Execute(dict);
			} else if (valueDict) {
				String key;
				Value value;
				BOOST_FOREACH(tie(key, value), valueDict) {
					dict->Set(key, value);
				}
			} else {
				stringstream message;
				message << "+= only works for dictionaries (" << m_DebugInfo << ")";
				throw_exception(domain_error(message.str()));
			}

			break;

		default:
			throw_exception(runtime_error("Not yet implemented."));
	}

	dictionary->Set(m_Key, newValue);
}
