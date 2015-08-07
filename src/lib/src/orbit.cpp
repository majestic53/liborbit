/**
 * liborbit
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * liborbit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liborbit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/orbit.h"
#include "../include/orbit_type.h"

namespace ORBIT {

	orbit_ptr orbit::m_instance = NULL;

	_orbit::_orbit(void) :
		m_initialized(false)
	{
		std::atexit(orbit::_delete);
	}

	_orbit::~_orbit(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_orbit::_delete(void)
	{

		if(orbit::m_instance) {
			delete orbit::m_instance;
			orbit::m_instance = NULL;
		}
	}

	orbit_ptr 
	_orbit::acquire(void)
	{

		if(!orbit::m_instance) {

			orbit::m_instance = new orbit;
			if(!orbit::m_instance) {
				THROW_ORBIT_EXCEPTION(ORBIT_EXCEPTION_ALLOCATION);
			}
		}

		return orbit::m_instance;
	}

	void 
	_orbit::initialize(void)
	{
		SERIALIZE_CALL_RECUR(m_lock);

		if(m_initialized) {
			THROW_ORBIT_EXCEPTION(ORBIT_EXCEPTION_INITIALIZE);
		}

		m_initialized = true;

		// TODO
	}

	bool 
	_orbit::is_allocated(void)
	{
		return (orbit::m_instance != NULL);
	}

	bool 
	_orbit::is_initialized(void)
	{
		SERIALIZE_CALL_RECUR(m_lock);
		return m_initialized;
	}

	std::string 
	_orbit::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		SERIALIZE_CALL_RECUR(m_lock);

		result << "[" << (m_initialized ? "INIT" : "UNINIT") << "] " 
<< ORBIT_HEADER;

		if(verbose) {
			result << " (" << VALUE_AS_HEX(uintptr_t, this) << ")";
		}

		// TODO

		return CHECK_STR(result.str());
	}

	void 
	_orbit::uninitialize(void)
	{
		SERIALIZE_CALL_RECUR(m_lock);

		if(!m_initialized) {
			THROW_ORBIT_EXCEPTION(ORBIT_EXCEPTION_UNINITIALIZE);
		}

		// TODO

		m_initialized = false;
	}

	std::string 
	_orbit::version(void)
	{
		return VERSION_STR;
	}

}
